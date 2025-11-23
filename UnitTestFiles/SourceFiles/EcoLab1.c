/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"
#include "IdEcoList1.h"
#include "CEcoLab1Sink.h"
#include "IEcoConnectionPointContainer.h"

/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;

    /* Несколько компонентов EcoLab1 */
    IEcoLab1* pIEcoLab1_1 = 0;
    IEcoLab1* pIEcoLab1_2 = 0;
    IEcoLab1* pIEcoLab1_3 = 0;

	/* Несколько приемников событий */
    IEcoLab1Events* pIEcoLab1Sink1 = 0;
    IEcoLab1Events* pIEcoLab1Sink2 = 0;

	/* Connection Points для компонентов */
    IEcoConnectionPointContainer* pICPC1 = 0;
    IEcoConnectionPointContainer* pICPC2 = 0;
    IEcoConnectionPointContainer* pICPC3 = 0;
    IEcoConnectionPoint* pICP1 = 0;
    IEcoConnectionPoint* pICP2 = 0;
    IEcoConnectionPoint* pICP3 = 0;

    IEcoUnknown* pISinkUnk = 0;
    
	/* Cookie для подключений */
    uint32_t cAdvise1_1 = 0, cAdvise1_2 = 0;  // Два подключения к первому компоненту
    uint32_t cAdvise2_1 = 0;                   // Одно подключение ко второму компоненту
    uint32_t cAdvise3_1 = 0;                   // Одно подключение к третьему компоненту

    /* Переменные для тестирования */
    int32_t* generatedArray1 = 0;
    int32_t* generatedArray2 = 0;
    int32_t* generatedArray3 = 0;
    int32_t* sortedArray1 = 0;
    int32_t* sortedArray2 = 0;
    int32_t* sortedArray3 = 0;
    uint32_t arrayLengthSmall = 5;
    uint32_t arrayLengthMedium = 10;
    uint32_t seed = 257;
    uint32_t i;
    clock_t start, end;
    double workingTime1, workingTime2, workingTime3;
	int sortedCorrectly = 0;

    /* Проверка и создание системного интерфейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
        if (result != 0 && pISys == 0) {
            /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
	/* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoList1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_53884AFC93C448ECAA929C8D3A562281);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif

    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }

    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1_1);
    if (result != 0 || pIEcoLab1_1 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

	result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1_2);
    if (result != 0 || pIEcoLab1_2 == 0) {
        goto Release;
    }
    
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1_3);
    if (result != 0 || pIEcoLab1_3 == 0) {
        goto Release;
    }

    /* Создаем два приемника событий */
	result = createCEcoLab1Sink(pIMem, &pIEcoLab1Sink1);
    if (result != 0 || pIEcoLab1Sink1 == 0) {
        goto Release;
    }
    
    result = createCEcoLab1Sink(pIMem, &pIEcoLab1Sink2);
    if (result != 0 || pIEcoLab1Sink2 == 0) {
        goto Release;
    }

    /* Компонент 1: два приемника (Many-to-One) */
    printf("   Component 1 -> Two sinks (Many-to-One)\n");
    result = pIEcoLab1_1->pVTbl->QueryInterface(pIEcoLab1_1, &IID_IEcoConnectionPointContainer, (void **)&pICPC1);
    if (result == 0 && pICPC1 != 0) {
        result = pICPC1->pVTbl->FindConnectionPoint(pICPC1, &IID_IEcoLab1Events, &pICP1);
        if (result == 0 && pICP1 != 0) {
            /* Первый приемник к компоненту 1 */
            result = pICP1->pVTbl->Advise(pICP1, (IEcoUnknown*)pIEcoLab1Sink1, &cAdvise1_1);
            if (result == 0) printf("     - Sink 1 connected to Component 1\n");
            
            /* Второй приемник к компоненту 1 */
            result = pICP1->pVTbl->Advise(pICP1, (IEcoUnknown*)pIEcoLab1Sink2, &cAdvise1_2);
            if (result == 0) printf("     - Sink 2 connected to Component 1\n");
        }
    }

    /* Компонент 2: один приемник (One-to-One) */
    printf("   Component 2 -> One sink (One-to-One)\n");
    result = pIEcoLab1_2->pVTbl->QueryInterface(pIEcoLab1_2, &IID_IEcoConnectionPointContainer, (void **)&pICPC2);
    if (result == 0 && pICPC2 != 0) {
        result = pICPC2->pVTbl->FindConnectionPoint(pICPC2, &IID_IEcoLab1Events, &pICP2);
        if (result == 0 && pICP2 != 0) {
            result = pICP2->pVTbl->Advise(pICP2, (IEcoUnknown*)pIEcoLab1Sink1, &cAdvise2_1);
            if (result == 0) printf("     - Sink 1 connected to Component 2\n");
        }
    }

    /* Компонент 3: один приемник (One-to-One) */
    printf("   Component 3 -> One sink (One-to-One)\n");
    result = pIEcoLab1_3->pVTbl->QueryInterface(pIEcoLab1_3, &IID_IEcoConnectionPointContainer, (void **)&pICPC3);
    if (result == 0 && pICPC3 != 0) {
        result = pICPC3->pVTbl->FindConnectionPoint(pICPC3, &IID_IEcoLab1Events, &pICP3);
        if (result == 0 && pICP3 != 0) {
            result = pICP3->pVTbl->Advise(pICP3, (IEcoUnknown*)pIEcoLab1Sink1, &cAdvise3_1);
            if (result == 0) printf("     - Sink 1 connected to Component 3\n");
        }
    }

    /* Тестирование сортировки пузырьком */
	/* Тест 1: Один источник -> два приемника */
	printf("\n   Test 1: One Source -> Two Sinks\n");
    result = pIEcoLab1_1->pVTbl->ArrayGen(pIEcoLab1_1, arrayLengthSmall, seed, &generatedArray1);
    if (result == 0 && generatedArray1 != 0) {
        printf("     Generated: ");
        for (i = 0; i < arrayLengthSmall; i++) printf("%d ", generatedArray1[i]);
        printf("\n");
        
        start = clock();
        result = pIEcoLab1_1->pVTbl->BubbleSort(pIEcoLab1_1, generatedArray1, arrayLengthSmall, &sortedArray1);
        end = clock();
        
        if (result == 0 && sortedArray1 != 0) {
            workingTime1 = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("     Sorted: ");
            for (i = 0; i < arrayLengthSmall; i++) printf("%d ", sortedArray1[i]);
            printf("\n");
            printf("     Time: %f seconds\n", workingTime1);
            
            /* Проверка сортировки */
            sortedCorrectly = 1;
            for (i = 0; i < arrayLengthSmall - 1; i++) {
                if (sortedArray1[i] > sortedArray1[i + 1]) {
                    sortedCorrectly = 0;
                    break;
                }
            }
            printf("     Sort verification: %s\n", sortedCorrectly ? "PASSED" : "FAILED");
        }
    }

    /* Тест 2: Один источник -> один приемник */  
	printf("\n   Test 2: One Source -> One Sink\n");
    result = pIEcoLab1_2->pVTbl->ArrayGen(pIEcoLab1_2, arrayLengthMedium, seed + 100, &generatedArray2);
    if (result == 0 && generatedArray2 != 0) {
        start = clock();
        result = pIEcoLab1_2->pVTbl->BubbleSort(pIEcoLab1_2, generatedArray2, arrayLengthMedium, &sortedArray2);
        end = clock();
        
        if (result == 0 && sortedArray2 != 0) {
            workingTime2 = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("     Time: %f seconds\n", workingTime2);
        }
    }

    /* Тест 3: Множество источников -> один приемник */
	printf("\n   Test 3: Multiple Sources -> One Sink\n");
    result = pIEcoLab1_3->pVTbl->ArrayGen(pIEcoLab1_3, arrayLengthSmall, seed + 200, &generatedArray3);
    if (result == 0 && generatedArray3 != 0) {
        start = clock();
        result = pIEcoLab1_3->pVTbl->BubbleSort(pIEcoLab1_3, generatedArray3, arrayLengthSmall, &sortedArray3);
        end = clock();
        
        if (result == 0 && sortedArray3 != 0) {
            workingTime3 = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("     Time: %f seconds\n", workingTime3);
        }
    }

Release:
    /* Отключение всех подключений */
    if (pICP1 != 0) {
        if (cAdvise1_1 != 0) pICP1->pVTbl->Unadvise(pICP1, cAdvise1_1);
        if (cAdvise1_2 != 0) pICP1->pVTbl->Unadvise(pICP1, cAdvise1_2);
    }
    if (pICP2 != 0 && cAdvise2_1 != 0) pICP2->pVTbl->Unadvise(pICP2, cAdvise2_1);
    if (pICP3 != 0 && cAdvise3_1 != 0) pICP3->pVTbl->Unadvise(pICP3, cAdvise3_1);

    /* Освобождение памяти массивов */
    if (generatedArray1 != 0 && pIMem != 0) pIMem->pVTbl->Free(pIMem, generatedArray1);
    if (generatedArray2 != 0 && pIMem != 0) pIMem->pVTbl->Free(pIMem, generatedArray2);
    if (generatedArray3 != 0 && pIMem != 0) pIMem->pVTbl->Free(pIMem, generatedArray3);
    if (sortedArray1 != 0 && pIMem != 0) pIMem->pVTbl->Free(pIMem, sortedArray1);
    if (sortedArray2 != 0 && pIMem != 0) pIMem->pVTbl->Free(pIMem, sortedArray2);
    if (sortedArray3 != 0 && pIMem != 0) pIMem->pVTbl->Free(pIMem, sortedArray3);
    
    /* Освобождение интерфейсов */
    if (pIEcoLab1Sink1 != 0) pIEcoLab1Sink1->pVTbl->Release(pIEcoLab1Sink1);
    if (pIEcoLab1Sink2 != 0) pIEcoLab1Sink2->pVTbl->Release(pIEcoLab1Sink2);
    if (pICP1 != 0) pICP1->pVTbl->Release(pICP1);
    if (pICP2 != 0) pICP2->pVTbl->Release(pICP2);
    if (pICP3 != 0) pICP3->pVTbl->Release(pICP3);
    if (pICPC1 != 0) pICPC1->pVTbl->Release(pICPC1);
    if (pICPC2 != 0) pICPC2->pVTbl->Release(pICPC2);
    if (pICPC3 != 0) pICPC3->pVTbl->Release(pICPC3);
    if (pIEcoLab1_1 != 0) pIEcoLab1_1->pVTbl->Release(pIEcoLab1_1);
    if (pIEcoLab1_2 != 0) pIEcoLab1_2->pVTbl->Release(pIEcoLab1_2);
    if (pIEcoLab1_3 != 0) pIEcoLab1_3->pVTbl->Release(pIEcoLab1_3);
    if (pIBus != 0) pIBus->pVTbl->Release(pIBus);
    if (pIMem != 0) pIMem->pVTbl->Release(pIMem);
    if (pISys != 0) pISys->pVTbl->Release(pISys);

    return result;
}
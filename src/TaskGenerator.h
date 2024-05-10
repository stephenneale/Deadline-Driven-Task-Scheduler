/*
 * TaskGenerator.h
 *
 */

#ifndef TASKGENERATOR_H_
#define TASKGENERATOR_H_

#include "STM32_RTOS_Config.h"

// Generator task defines

void Task1(void *pvParameters);
void Task2(void *pvParameters);
void Task3(void *pvParameters);

void Task1GeneratorCallback(void *pvParameters);
void Task2GeneratorCallback(void *pvParameters);
void Task3GeneratorCallback(void *pvParameters);

#endif /* TASKGENERATOR_H_ */

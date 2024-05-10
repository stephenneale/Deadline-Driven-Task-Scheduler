/*
 * STM32_RTOS_Config.h
 *
 */

#ifndef STM32RTOSCONFIG_H_
#define STM32RTOSCONFIG_H_

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stm32f4_discovery.h"

/* Kernel includes. */
#include "stm32f4xx.h"
#include "../FreeRTOS_Source/include/FreeRTOS.h"
#include "../FreeRTOS_Source/include/queue.h"
#include "../FreeRTOS_Source/include/semphr.h"
#include "../FreeRTOS_Source/include/task.h"
#include "../FreeRTOS_Source/include/timers.h"

/* Defines */

volatile uint32_t TASK1_PERIOD;
volatile uint32_t TASK1_EXEC_TIME;
volatile uint32_t TASK2_PERIOD;
volatile uint32_t TASK2_EXEC_TIME;
volatile uint32_t TASK3_PERIOD;
volatile uint32_t TASK3_EXEC_TIME;
volatile int TASK_PREEMPTED[3];

#define MONITOR_OR_DEBUG 0 // 0 for monitor, 1 for debug statements
#define TEST_BENCH 2 // Switch between 1, 2, and 3 for different test benches

/* Queues and timers */
QueueHandle_t taskQueue, completedQueue;
TimerHandle_t task1Timer, task2Timer, task3Timer;


/* Monitor message types */
typedef enum {
	ACTIVE,
	COMPLETE,
	OVERDUE
} monitor_message;



#endif /* STM32RTOSCONFIG_H_ */

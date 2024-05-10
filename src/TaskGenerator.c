/*
 * TaskGenerator.c
 *
 */

#include "STM32_RTOS_Config.h"
#include "TaskGenerator.h"
#include "TaskScheduler.h"


void Task1(void *pvParameters) {
    TickType_t start_tick = xTaskGetTickCount(); // Get the tick count when the task starts
    TickType_t curr_tick;
    TickType_t time_elapsed = 0;
    TickType_t time_to_completion = pdMS_TO_TICKS(TASK1_EXEC_TIME);

    while (time_elapsed < time_to_completion) {
    	if (TASK_PREEMPTED[0]) {
    		// Reset start tick if preempted
    		start_tick = xTaskGetTickCount();
    		TASK_PREEMPTED[0] = 0;
    	}

        // Run task for an amount of time
        curr_tick = xTaskGetTickCount();
        time_elapsed = curr_tick - start_tick;
    }
    // Task completed
    delete_dd_task(1);
    vTaskDelete(NULL);
}

void Task2(void *pvParameters) {
    TickType_t start_tick = xTaskGetTickCount(); // Get the tick count when the task starts
    TickType_t curr_tick;
    TickType_t time_elapsed = 0;
    TickType_t time_to_completion = pdMS_TO_TICKS(TASK2_EXEC_TIME);

    while (time_elapsed < time_to_completion) {
    	if (TASK_PREEMPTED[1]) {
    		// Reset start tick if preempted
    		start_tick = xTaskGetTickCount();
    		TASK_PREEMPTED[1] = 0;
    	}

        // Run task for an amount of time
        curr_tick = xTaskGetTickCount();
        time_elapsed = curr_tick - start_tick;
    }
    // Task completed
    delete_dd_task(2);
    vTaskDelete(NULL);
}

void Task3(void *pvParameters) {
    TickType_t start_tick = xTaskGetTickCount(); // Get the tick count when the task starts
    TickType_t curr_tick;
    TickType_t time_elapsed = 0;
    TickType_t time_to_completion = pdMS_TO_TICKS(TASK3_EXEC_TIME);

    while (time_elapsed < time_to_completion) {
    	if (TASK_PREEMPTED[2]) {
    		// Reset start tick if preempted
    		start_tick = xTaskGetTickCount();
    		TASK_PREEMPTED[2] = 0;
    	}

        // Run task for an amount of time
        curr_tick = xTaskGetTickCount();
        time_elapsed = curr_tick - start_tick;
    }
    // Task completed
    delete_dd_task(3);
    vTaskDelete(NULL);
}

void Task1GeneratorCallback(TimerHandle_t pxTimer) {
	TaskHandle_t task;
	xTaskCreate(Task1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &task);

	if (task != NULL) {
		TickType_t current_tick = xTaskGetTickCount();
		create_dd_task(task, PERIODIC, 1, current_tick, current_tick + pdMS_TO_TICKS(TASK1_PERIOD));
	}
	// Restart timer
	xTimerChangePeriod(pxTimer, pdMS_TO_TICKS(TASK1_PERIOD), 0);
	xTimerStart(pxTimer, 0);
}

void Task2GeneratorCallback(TimerHandle_t pxTimer) {
	TaskHandle_t task;
	xTaskCreate(Task2, "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &task);

	if (task != NULL) {
		TickType_t current_tick = xTaskGetTickCount();
		create_dd_task(task, PERIODIC, 2, current_tick, current_tick + pdMS_TO_TICKS(TASK2_PERIOD));
	}
	// Restart timer
	xTimerChangePeriod(pxTimer, pdMS_TO_TICKS(TASK2_PERIOD), 0);
	xTimerStart(pxTimer, 0);
}

void Task3GeneratorCallback(TimerHandle_t pxTimer) {
	TaskHandle_t task;
	xTaskCreate(Task3, "Task3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &task);

	if (task != NULL) {
		TickType_t current_tick = xTaskGetTickCount();
		create_dd_task(task, PERIODIC, 3, current_tick, current_tick + pdMS_TO_TICKS(TASK3_PERIOD));
	}
	// Restart timer
	xTimerChangePeriod(pxTimer, pdMS_TO_TICKS(TASK3_PERIOD), 0);
	xTimerStart(pxTimer, 0);
}



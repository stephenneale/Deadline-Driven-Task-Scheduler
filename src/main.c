/*
 * ECE455 Project 3
 * Deadline-Driven Task Scheduler
 *
 * Stephen Neale V00892644
 * Andrew Petten V00976162
 *
 */

/* Includes. */
#include "Task.h"
#include "TaskMonitor.h"
#include "TaskScheduler.h"
#include "TaskGenerator.h"
#include "STM32_RTOS_Config.h"

#define MONITOR_PRIORITY configMAX_PRIORITIES-2
#define SCHEDULER_PRIORITY configMAX_PRIORITIES-2

void set_test_bench(int bench);

/*
 * To switch between monitor task mode and debug print mode,
 * edit the MONITOR_OR_DEBUG flag in STM32_RTOS_Config.h
 *
 * To switch between test benches, edit the TEST_BENCH flag
 * in STM32_RTOS_Config.h
 */

int main(void) {

	// Set test bench and reset preempt flags
	set_test_bench(TEST_BENCH);
	for (int i = 0; i < 3; i++) {
	    TASK_PREEMPTED[i] = 0;
	}
	printf("Setting bench to %d\n", TEST_BENCH);

	// Create queues for DDS
	taskQueue = xQueueCreate(10, sizeof(dd_task));
	completedQueue = xQueueCreate(10, sizeof(uint32_t));

	// Create DDS and Monitor Task.
	xTaskCreate(DDSchedulerTask, "DDScheduler", configMINIMAL_STACK_SIZE, NULL, SCHEDULER_PRIORITY, NULL);
	if (MONITOR_OR_DEBUG == 0) {
		xTaskCreate(MonitorTask, "MonitorTask", configMINIMAL_STACK_SIZE, NULL, MONITOR_PRIORITY, NULL);
	}

	task1Timer = xTimerCreate("Timer1", 1, pdFALSE, (void *)0, Task1GeneratorCallback);
	task2Timer = xTimerCreate("Timer2", 1, pdFALSE, (void *)0, Task2GeneratorCallback);
	task3Timer = xTimerCreate("Timer3", 1, pdFALSE, (void *)0, Task3GeneratorCallback);

	// Start generator timers
	xTimerStart(task1Timer, 0);
	xTimerStart(task2Timer, 0);
	xTimerStart(task3Timer, 0);

	// Start tasks
	vTaskStartScheduler();

	return 0;
}

void set_test_bench(int bench) {
	// Sets task parameters to reflect specific test benches
	if (bench == 1) {
		TASK1_EXEC_TIME = 95;
		TASK1_PERIOD = 500;
		TASK2_EXEC_TIME = 150;
		TASK2_PERIOD = 500;
		TASK3_EXEC_TIME = 250;
		TASK3_PERIOD = 750;
	}
	else if (bench == 2) {
		TASK1_EXEC_TIME = 95;
		TASK1_PERIOD = 250;
		TASK2_EXEC_TIME = 150;
		TASK2_PERIOD = 500;
		TASK3_EXEC_TIME = 250;
		TASK3_PERIOD = 750;
	}
	else if (bench == 3) {
		TASK1_EXEC_TIME = 100;
		TASK1_PERIOD = 500;
		TASK2_EXEC_TIME = 200;
		TASK2_PERIOD = 500;
		TASK3_EXEC_TIME = 200;
		TASK3_PERIOD = 500;
	}
}


void vApplicationMallocFailedHook( void )
{
	/* The malloc failed hook is enabled by setting
	configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

	Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected.  pxCurrentTCB can be
	inspected in the debugger if the task name passed into this function is
	corrupt. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
volatile size_t xFreeStackSpace;

	/* The idle task hook is enabled by setting configUSE_IDLE_HOOK to 1 in
	FreeRTOSConfig.h.

	This function is called on each cycle of the idle task.  In this case it
	does nothing useful, other than report the amount of FreeRTOS heap that
	remains unallocated. */
	xFreeStackSpace = xPortGetFreeHeapSize();

	if( xFreeStackSpace > 100 )
	{
		/* By now, the kernel has allocated everything it is going to, so
		if there is a lot of heap remaining unallocated then
		the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		reduced accordingly. */
	}
}
/*-----------------------------------------------------------*/


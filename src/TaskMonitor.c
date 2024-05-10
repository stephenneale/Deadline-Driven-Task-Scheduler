/*
 * TaskMonitor.c
 *
 */

#include "TaskMonitor.h"
#include "TaskScheduler.h"
#include "STM32_RTOS_Config.h"

volatile dd_task_list *active_task_list_received;
volatile dd_task_list *completed_task_list_received;
volatile dd_task_list *overdue_task_list_received;
volatile int HYPER_PERIOD = 0;

/* Test Bench 2 expected values */
volatile int TB_2_EXPECTED_COMPLETED = 0;
volatile int TB_2_EXPECTED_OVERDUE = 0;


// Monitors the deadline-driven scheduler
void MonitorTask( void *pvParameters ) {
	printf("Monitor Task\n");
	printf("--------------------\n");
	while(1) {
		int active_size, completed_size, overdue_size;

		// Get lists
		active_task_list_received = get_active_dd_task_list();
		completed_task_list_received = get_complete_dd_task_list();
		overdue_task_list_received = get_overdue_dd_task_list();

		// Calculate size of lists
		active_size = getTaskListSize(active_task_list_received);
		completed_size = getTaskListSize(completed_task_list_received);
		overdue_size = getTaskListSize(overdue_task_list_received);

		if (TEST_BENCH == 2) {
			// Print report
			printf("|Period:    %d T: %d\n"
				   "|\n"
				   "|Active:    %d\n"
				   "|       Ex: %d\n"
				   "|\n"
				   "|Completed: %d\n"
				   "|       Ex: %d\n"
				   "|\n"
				   "|Overdue:   %d\n"
				   "|       Ex: %d\n",
				   HYPER_PERIOD, xTaskGetTickCount(),
				   active_size, 3,
				   completed_size, TB_2_EXPECTED_COMPLETED,
				   overdue_size, TB_2_EXPECTED_OVERDUE);

			printf("------------------\n\n");

			TB_2_EXPECTED_COMPLETED += 10;
			TB_2_EXPECTED_OVERDUE += 1;
		} else {
			printf("|Period: %d |Active: %d |Completed: %d |Overdue: %d\n", HYPER_PERIOD, active_size, completed_size, overdue_size);
		}
		HYPER_PERIOD += 1;
		vTaskDelay(1505);
	}
}

// Function to get the size of a task list
int getTaskListSize(volatile dd_task_list *list) {
    int size = 0;
    dd_task_list *current = list;

    // Traverse the list and count the tasks
    while (current != NULL) {
        size++;
        current = current->next_task;
    }

    return size;
}

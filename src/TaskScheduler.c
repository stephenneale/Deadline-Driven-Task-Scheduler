/*
 * TaskScheduler.c
 *
 */

#include "TaskScheduler.h"
#include "TaskGenerator.h"
#include "Task.h"
#include "STM32_RTOS_Config.h"


volatile dd_task_list *active_task_list;
volatile dd_task_list *completed_task_list;
volatile dd_task_list *overdue_task_list;
volatile dd_task *current_task = NULL;
volatile uint32_t current_tick;

/* Test Bench 1 expected values */
volatile int TB_1_RELEASE_VALUES[3][13] = {
    {0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000},
    {0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000},
    {0, 750, 1500, 2250, 3000, 3750, 4500, 5250, 6000}
};
volatile int TB_1_COMPLETED_VALUES[3][13] = {
    {95, 595, 1095, 1595, 2095, 2595, 3095, 3595, 4095, 4595, 5095, 5595, 6095},
    {245, 745, 1245, 1745, 2245, 2745, 3245, 3745, 4245, 4745, 5245, 5745, 6245},
    {495, 1000, 1995, 2500, 3495, 4000, 4995, 5500, 6495}
};
volatile int TASK_RELEASE_COUNT[3] = {0, 0, 0};
volatile int TASK_COMPLETED_COUNT[3] = {0, 0, 0};
volatile int EVENT_NUMBER = 1;


// Deadline-driven task scheduler - EDF
void DDSchedulerTask( void *pvParameters ) {

	if (MONITOR_OR_DEBUG == 1) {
		printf("Begin scheduling.\n");
	}

	while(1) {
		dd_task *new_task;
		uint32_t *completed_task_id;
		current_tick = xTaskGetTickCount();

		// Check if there are completed tasks to add to completed_task_list
		if (xQueueReceive(completedQueue, &completed_task_id, 0) == pdTRUE && completed_task_id != NULL && current_task != NULL) {
			// Remove task from active task list
			if (completed_task_id == current_task->task_id) {
				dd_task *removed_task = remove_from_list((volatile dd_task_list **)&active_task_list, completed_task_id);
				removed_task->completion_time = current_tick;
				vTaskDelete(current_task->t_handle);
				// Add task to completed list
				if (MONITOR_OR_DEBUG == 1) {
					if (TEST_BENCH == 1) {
						printf("%d Task %d completed: %d Ex: %d\n", EVENT_NUMBER, completed_task_id, removed_task->completion_time, TB_1_COMPLETED_VALUES[removed_task->task_id - 1][TASK_COMPLETED_COUNT[removed_task->task_id - 1]]);
						TASK_COMPLETED_COUNT[removed_task->task_id - 1] += 1;
					} else {
						printf("%d Task %d completed: %d\n", EVENT_NUMBER, completed_task_id, removed_task->completion_time);
					}
				}
				push_to_list((volatile dd_task_list **)&completed_task_list, removed_task);
				EVENT_NUMBER += 1;
				current_task = NULL;
			}
			else {
				// should NEVER be here...
				printf("Something went wrong.\n");
			}
		}
		// Check if any new tasks to release to active_task_list
		if (xQueueReceive(taskQueue, &new_task, 0) == pdTRUE && new_task != NULL) {
			// Add task to active task list
			if (MONITOR_OR_DEBUG == 1) {
				if (TEST_BENCH == 1) {
					printf("%d Task %d released:  %d Ex: %d\n", EVENT_NUMBER, new_task->task_id, current_tick, TB_1_RELEASE_VALUES[new_task->task_id - 1][TASK_RELEASE_COUNT[new_task->task_id - 1]]);
					TASK_RELEASE_COUNT[new_task->task_id - 1] += 1;
				} else {
					printf("%d Task %d released:  %d\n", EVENT_NUMBER, new_task->task_id, current_tick);
				}
			}
			push_to_list_edf((volatile dd_task_list **)&active_task_list, new_task);
			EVENT_NUMBER += 1;
		}

        // Check if current task is overdue
        if (current_task != NULL && current_tick > current_task->absolute_deadline) {
        	// Remove task from active task list, and add to overdue list
        	if (MONITOR_OR_DEBUG == 1) {
        		printf("Task %d OVERDUE: %d DL: %d\n", current_task->task_id, current_tick, current_task->absolute_deadline);
        	}
        	dd_task *removed_task = remove_from_list((volatile dd_task_list **)&active_task_list, current_task->task_id);
        	vTaskDelete(current_task->t_handle);
        	push_to_list((volatile dd_task_list **)&overdue_task_list, removed_task);
        	EVENT_NUMBER += 1;
        	current_task = NULL;
        }

        // Check active_task_list
        dd_task *first_task = NULL;
        if (active_task_list != NULL) {
            first_task = active_task_list->task;

            // Preempt current task and begin new task
            if (first_task != NULL && current_task != NULL && first_task->absolute_deadline < current_task->absolute_deadline) {
            	vTaskPrioritySet(current_task->t_handle, tskIDLE_PRIORITY);
            	TASK_PREEMPTED[current_task->task_id-1] = 1;
            	// Swap to new task
//            	printf("Preempt %d, swap to %d\n", current_task->task_id, first_task->task_id);
            	current_task = first_task;
            	vTaskPrioritySet(first_task->t_handle, configMAX_PRIORITIES-2);
            }

            // Start task from idle state
            if (first_task != NULL && current_task == NULL) {
                current_task = first_task;
//              printf("Idle to %d\n", current_task->task_id);
                vTaskPrioritySet(current_task->t_handle, configMAX_PRIORITIES-2);
            }
        }
	}
}

void create_dd_task(TaskHandle_t handle, task_type type, uint32_t id, uint32_t release_time, uint32_t absolute_deadline) {
	// Send a new task to the task queue
	dd_task *task = (dd_task *)pvPortMalloc(sizeof(dd_task));

	if (task == NULL) {
		printf("Failed to create task.\n");
		return;
	}

	task->t_handle = handle;
	task->type = type;
	task->task_id = id;
	task->release_time = pdMS_TO_TICKS(release_time);
	task->absolute_deadline = pdMS_TO_TICKS(absolute_deadline);
	task->completion_time = 0;

	xQueueSend(taskQueue, &task, 50);

}

void delete_dd_task(uint32_t task_id) {
    // Send task ID to completed queue
    xQueueSend(completedQueue, &task_id, 50);
}

struct dd_task_list *get_active_dd_task_list(void) {
	return active_task_list;
}
struct dd_task_list *get_complete_dd_task_list(void) {
	return completed_task_list;
}
struct dd_task_list *get_overdue_dd_task_list(void) {
	return overdue_task_list;
}

void push_to_list(volatile dd_task_list **list, dd_task *new_task) {
    // Allocate memory for a new node
    dd_task_list *new_node = (dd_task_list *)malloc(sizeof(dd_task_list));
    if (new_node == NULL) {
        return;
    }

    // Populate the new node with the new_task
    new_node->task = new_task;
    new_node->next_task = NULL;

    // If the list is empty, set the new node as the head of the list
    if (*list == NULL) {
        *list = new_node;
        return;
    }

    // Traverse the list to find the last node
    dd_task_list *current = *list;
    while (current->next_task != NULL) {
        current = current->next_task;
    }

    // Attach the new node to the end of the list
    current->next_task = new_node;

}

void push_to_list_edf(volatile dd_task_list **list, dd_task *new_task) {
	// Same as push_to_list, except by ascending deadline
    dd_task_list *new_node = (dd_task_list *)malloc(sizeof(dd_task_list));
    if (new_node == NULL) {
        return;
    }

    new_node->task = new_task;
    new_node->next_task = NULL;

    if (*list == NULL || (*list)->task->absolute_deadline > new_task->absolute_deadline) {
        // If the list is empty or new_task has the earliest deadline, insert at the beginning
        new_node->next_task = *list;
        *list = new_node;
        return;
    }

    dd_task_list *current = *list;
    while (current->next_task != NULL) {
        dd_task_list *check = current->next_task;
        if (check->task->absolute_deadline > new_task->absolute_deadline) {
        	break;
        }
        else {
        	current = current->next_task;
        }
    }

    // Insert new_node after current
    new_node->next_task = current->next_task;
    current->next_task = new_node;

}

dd_task* remove_from_list(volatile dd_task_list **list, uint32_t task_id) {
    dd_task_list *current = *list;
    dd_task_list *prev = NULL;
    dd_task *removed_task = NULL;

    // Search for the task in the list
    while (current != NULL && current->task->task_id != task_id) {
        prev = current;
        current = current->next_task;
    }

    if (current != NULL) {
        // Task found in the list
        // Remove the task from the list
        if (prev != NULL) {
            prev->next_task = current->next_task;
        } else {
            *list = current->next_task;
        }

        // Save a reference to the removed task
        removed_task = current->task;

        // Free the memory allocated for the list node
        free(current);
    } else {
        // Task not found in the list
        printf("Task %d not found in the list.\n", task_id);
    }

    // Return a reference to the removed task
    return removed_task;
}

// Function to print the tasks in a given list
void print_list(const volatile dd_task_list *list) {
    const dd_task_list *current = list;
    while (current != NULL) {
        printf("Rel: %d, Comp: %d, Ded: %d\n", current->task->release_time, current->task->completion_time, current->task->absolute_deadline);
        current = current->next_task;
    }
}




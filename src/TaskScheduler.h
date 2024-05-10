/*
 * TaskScheduler.h
 *
 */

#ifndef TASKSCHEDULER_H_
#define TASKSCHEDULER_H_

#include "STM32_RTOS_Config.h"
#include "Task.h"

// Task Scheduler
void DDSchedulerTask( void *pvParameters );

// Core functions (2.1.3)
void create_dd_task(TaskHandle_t t_handle,
					task_type type,
					uint32_t task_id,
					uint32_t release_time,
					uint32_t absolute_deadline);
void delete_dd_task(uint32_t task_id);
struct dd_task_list *get_active_dd_task_list(void);
struct dd_task_list *get_complete_dd_task_list(  void  );
struct dd_task_list *get_overdue_dd_task_list(  void  );

// Helper functions
void push_to_list(volatile dd_task_list **list, dd_task *new_task);
void push_to_list_edf(volatile dd_task_list **list, dd_task *new_task);
dd_task* remove_from_list(volatile dd_task_list **list, uint32_t task_id);
void print_list(const volatile dd_task_list *list);

volatile dd_task *current_task;

#endif /* TASKSCHEDULER_H_ */

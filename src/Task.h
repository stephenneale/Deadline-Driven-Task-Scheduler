/*
 * Task.h
 *
 */

#ifndef TASK_H_
#define TASK_H_

#include "STM32_RTOS_Config.h"


// DD Task data structure

typedef enum {
	PERIODIC,
	APERIODIC
} task_type;

typedef struct {
	TaskHandle_t t_handle;
	task_type type;
	uint32_t task_id;
	uint32_t release_time;
	uint32_t absolute_deadline;
	uint32_t completion_time;
} dd_task;

typedef struct {
	dd_task *task;
	struct dd_task_list *next_task;
} dd_task_list;




#endif /* TASK_H_ */

/*
 * TaskMonitor.h
 *
 */

#ifndef TASKMONITOR_H_
#define TASKMONITOR_H_

#include "STM32_RTOS_Config.h"
#include "Task.h"

// Monitor task defines

void MonitorTask( void *pvParameters );
int getTaskListSize(volatile dd_task_list *list);

#endif /* TASKMONITOR_H_ */

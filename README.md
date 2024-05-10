# Deadline Driven Task Scheduler

This project incorporates an earliest-deadline-first task scheduler for the STM32-F Discovery microcontroller board utilizing the FreeRTOS operating system. The system has three main jobs working together to achieve this: a task generator, the scheduler itself, and a monitor task.

To observe debug prints, the SWV console must be enabled on port 0 with recording started before running the program. To switch between the monitor task output and the scheduler debug output, toggle `MONITOR_OR_DEBUG` in `STM32_RTOS_Config.h`. To switch between the provided test benches, change the `TEST_BENCH` setting found in the same file previously mentioned.
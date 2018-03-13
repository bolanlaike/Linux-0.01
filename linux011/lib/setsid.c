/*
* linux/lib/setsid.c
*
* (C) 1991 Linus Torvalds
*/

#define __LIBRARY__
#include <unistd.h>		// Linux 标准头文件。定义了各种符号常数和类型，并申明了各种函数。
// 如定义了__LIBRARY__，则还包括系统调用号和内嵌汇编_syscall0()等。

//// 创建一个会话并设置进程组号。
// 下面系统调用宏对应于函数：pid_t setsid()。
// 返回：调用进程的会话标识符(session ID)。
_syscall0 (pid_t, setsid)

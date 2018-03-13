/*
* linux/lib/wait.c
*
* (C) 1991 Linus Torvalds
*/

#define __LIBRARY__
#include <unistd.h>		// Linux 标准头文件。定义了各种符号常数和类型，并申明了各种函数。
// 如定义了__LIBRARY__，则还包括系统调用号和内嵌汇编_syscall0()等。
#include <sys/wait.h>		// 等待调用头文件。定义系统调用wait()和waitpid()及相关常数符号。

//// 等待进程终止系统调用函数。
// 该下面宏结构对应于函数：pid_t waitpid(pid_t pid, int * wait_stat, int options)
//
// 参数：pid - 等待被终止进程的进程id，或者是用于指定特殊情况的其它特定数值；
// wait_stat - 用于存放状态信息；options - WNOHANG 或WUNTRACED 或是0。
_syscall3 (pid_t, waitpid, pid_t, pid, int *, wait_stat, int, options)
//// wait()系统调用。直接调用waitpid()函数。
     pid_t wait (int *wait_stat)
{
  return waitpid (-1, wait_stat, 0);
}

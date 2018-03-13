/*
* linux/lib/write.c
*
* (C) 1991 Linus Torvalds
*/

#define __LIBRARY__
#include <unistd.h>		// Linux 标准头文件。定义了各种符号常数和类型，并申明了各种函数。
// 如定义了__LIBRARY__，则还包括系统调用号和内嵌汇编_syscall0()等。

//// 写文件系统调用函数。
// 该宏结构对应于函数：int write(int fd, const char * buf, off_t count)
// 参数：fd - 文件描述符；buf - 写缓冲区指针；count - 写字节数。
// 返回：成功时返回写入的字节数(0 表示写入0 字节)；出错时将返回-1，并且设置了出错号。
_syscall3 (int, write, int, fd, const char *, buf, off_t, count)

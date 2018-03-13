/*
* linux/mm/page.s
*
* (C) 1991 Linus Torvalds
*/

/*
* page.s contains the low-level page-exception code.
* the real work is done in mm.c
*/
/*
* page.s 程序包含底层页异常处理代码。实际的工作在memory.c 中完成。
*/

.globl _page_fault

_page_fault:
xchgl %eax,(%esp) # 取出错码到eax。
pushl %ecx
pushl %edx
push %ds
push %es
push %fs
movl $0x10,%edx # 置内核数据段选择符。
mov %dx,%ds
mov %dx,%es
mov %dx,%fs
movl %cr2,%edx # 取引起页面异常的线性地址
pushl %edx # 将该线性地址和出错码压入堆栈，作为调用函数的参数。
pushl %eax
testl $1,%eax # 测试标志P，如果不是缺页引起的异常则跳转。
10.5 page.s 程序
jne 1f
call _do_no_page # 调用缺页处理函数（mm/memory.c,365 行）。
jmp 2f
1: call _do_wp_page # 调用写保护处理函数（mm/memory.c,247 行）。
2: addl $8,%esp # 丢弃压入栈的两个参数。
pop %fs
pop %es
pop %ds
popl %edx
popl %ecx
popl %eax
iret

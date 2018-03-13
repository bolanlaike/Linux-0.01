/*
* linux/kernel/rs_io.s
*
* (C) 1991 Linus Torvalds
*/

/*
* rs_io.s
*
* This module implements the rs232 io interrupts.
*/
/*
* 该程序模块实现rs232 输入输出中断处理程序。
*/

.text
.globl _rs1_interrupt,_rs2_interrupt

// size 是读写队列缓冲区的字节长度。
size = 1024 /* must be power of two ! 必须是2 的次方并且需
and must match the value 与tty_io.c 中的值匹配!
in tty_io.c!!! */

/* these are the offsets into the read/write buffer structures */
/* 以下这些是读写缓冲结构中的偏移量 */
// 对应定义在include/linux/tty.h 文件中tty_queue 结构中各变量的偏移量。
rs_addr = 0 // 串行端口号字段偏移（端口号是0x3f8 或0x2f8）。
head = 4 // 缓冲区中头指针字段偏移。
tail = 8 // 缓冲区中尾指针字段偏移。
proc_list = 12 // 等待该缓冲的进程字段偏移。
buf = 16 // 缓冲区字段偏移。

startup = 256 /* chars left in write queue when we restart it */
/* 当写队列里还剩256 个字符空间(WAKEUP_CHARS)时，我们就可以写 */

/*
* These are the actual interrupt routines. They look where
* the interrupt is coming from, and take appropriate action.
*/
/*
* 这些是实际的中断程序。程序首先检查中断的来源，然后执行相应
* 的处理。
*/
.align 2
//// 串行端口1 中断处理程序入口点。
_rs1_interrupt:
pushl $_table_list+8 // tty 表中对应串口1 的读写缓冲指针的地址入栈(tty_io.c，99)。
jmp rs_int
.align 2
//// 串行端口2 中断处理程序入口点。
_rs2_interrupt:
pushl $_table_list+16 // tty 表中对应串口2 的读写缓冲队列指针的地址入栈。
rs_int:
pushl %edx
pushl %ecx
pushl %ebx
pushl %eax
push %es
push %ds /* as this is an interrupt, we cannot */
pushl $0x10 /* know that bs is ok. Load it */
pop %ds /* 由于这是一个中断程序，我们不知道ds 是否正确，*/
pushl $0x10 /* 所以加载它们(让ds、es 指向内核数据段 */
pop %es
movl 24(%esp),%edx // 将缓冲队列指针地址存入edx 寄存器，
// 也即35 或39 行上最先压入堆栈的地址。
movl (%edx),%edx // 取读队列指针(地址)??edx。
movl rs_addr(%edx),%edx // 取串口1 的端口号??edx。
addl $2,%edx /* interrupt ident. reg */ /* edx 指向中断标识寄存器 */
rep_int: // 中断标识寄存器端口是0x3fa（0x2fa），参见上节列表后信息。
xorl %eax,%eax // eax 清零。
inb %dx,%al // 取中断标识字节，用以判断中断来源(有4 种中断情况)。
testb $1,%al // 首先判断有无待处理的中断(位0=1 无中断；=0 有中断)。
jne end // 若无待处理中断，则跳转至退出处理处end。
cmpb $6,%al /* this shouldn't happen, but ... */ /* 这不会发生，但是…*/
ja end // al 值>6? 是则跳转至end（没有这种状态）。
movl 24(%esp),%ecx // 再取缓冲队列指针地址??ecx。
pushl %edx // 将端口号0x3fa(0x2fa)入栈。
subl $2,%edx // 0x3f8(0x2f8)。
call jmp_table(,%eax,2) /* NOTE! not *4, bit0 is 0 already */ /* 不乘4，位0 已是0*/
// 上面语句是指，当有待处理中断时，al 中位0=0，位2-1 是中断类型，因此相当于已经将中断类型
// 乘了2，这里再乘2，得到跳转表对应各中断类型地址，并跳转到那里去作相应处理。
popl %edx // 弹出中断标识寄存器端口号0x3fa（或0x2fa）。
jmp rep_int // 跳转，继续判断有无待处理中断并继续处理。
end: movb $0x20,%al // 向中断控制器发送结束中断指令EOI。
outb %al,$0x20 /* EOI */
pop %ds
pop %es
popl %eax
popl %ebx
popl %ecx
popl %edx
addl $4,%esp # jump over _table_list entry # 丢弃缓冲队列指针地址。
iret

// 各中断类型处理程序地址跳转表，共有4 种中断来源：
// modem 状态变化中断，写字符中断，读字符中断，线路状态有问题中断。
jmp_table:
.long modem_status,write_char,read_char,line_status

.align 2
modem_status:
addl $6,%edx /* clear intr by reading modem status reg */
inb %dx,%al /* 通过读modem 状态寄存器进行复位(0x3fe) */
ret

.align 2
line_status:
addl $5,%edx /* clear intr by reading line status reg. */
inb %dx,%al /* 通过读线路状态寄存器进行复位(0x3fd) */
ret

.align 2
read_char:
inb %dx,%al /* 读取字符??al。
movl %ecx,%edx /* 当前串口缓冲队列指针地址??edx。
subl $_table_list,%edx // 缓冲队列指针表首址 - 当前串口队列指针地址??edx，
shrl $3,%edx // 差值/8。对于串口1 是1，对于串口2 是2。
movl (%ecx),%ecx # read-queue # 取读缓冲队列结构地址??ecx。
movl head(%ecx),%ebx // 取读队列中缓冲头指针??ebx。
movb %al,buf(%ecx,%ebx) // 将字符放在缓冲区中头指针所指的位置。
incl %ebx // 将头指针前移一字节。
andl $size-1,%ebx // 用缓冲区大小对头指针进行模操作。指针不能超过缓冲区大小。
cmpl tail(%ecx),%ebx // 缓冲区头指针与尾指针比较。
je 1f // 若相等，表示缓冲区满，跳转到标号1 处。
movl %ebx,head(%ecx) // 保存修改过的头指针。
1: pushl %edx // 将串口号压入堆栈(1- 串口1，2 - 串口2)，作为参数，
call _do_tty_interrupt // 调用tty 中断处理C 函数（。
addl $4,%esp // 丢弃入栈参数，并返回。
ret

.align 2
write_char:
movl 4(%ecx),%ecx # write-queue # 取写缓冲队列结构地址??ecx。
movl head(%ecx),%ebx // 取写队列头指针??ebx。
subl tail(%ecx),%ebx // 头指针 - 尾指针 = 队列中字符数。
andl $size-1,%ebx # nr chars in queue # 对指针取模运算。
je write_buffer_empty // 如果头指针 = 尾指针，说明写队列无字符，跳转处理。
cmpl $startup,%ebx // 队列中字符数超过256 个？
ja 1f // 超过，则跳转处理。
movl proc_list(%ecx),%ebx # wake up sleeping process # 唤醒等待的进程。
// 取等待该队列的进程的指针，并判断是否为空。
testl %ebx,%ebx # is there any? # 有等待的进程吗？
je 1f // 是空的，则向前跳转到标号1 处。
movl $0,(%ebx) // 否则将进程置为可运行状态(唤醒进程)。。
1: movl tail(%ecx),%ebx // 取尾指针。
movb buf(%ecx,%ebx),%al // 从缓冲中尾指针处取一字符??al。
outb %al,%dx // 向端口0x3f8(0x2f8)送出到保持寄存器中。
incl %ebx // 尾指针前移。
andl $size-1,%ebx // 尾指针若到缓冲区末端，则折回。
movl %ebx,tail(%ecx) // 保存已修改过的尾指针。
cmpl head(%ecx),%ebx // 尾指针与头指针比较，
je write_buffer_empty // 若相等，表示队列已空，则跳转。
ret
.align 2
write_buffer_empty:
movl proc_list(%ecx),%ebx # wake up sleeping process # 唤醒等待的进程。
// 取等待该队列的进程的指针，并判断是否为空。
testl %ebx,%ebx # is there any? # 有等待的进程吗？
je 1f # 无，则向前跳转到标号1 处。
movl $0,(%ebx) # 否则将进程置为可运行状态(唤醒进程)。
1: incl %edx # 指向端口0x3f9(0x2f9)。
inb %dx,%al # 读取中断允许寄存器。
jmp 1f # 稍作延迟。
1: jmp 1f
1: andb $0xd,%al /* disable transmit interrupt */
/* 屏蔽发送保持寄存器空中断（位1） */
outb %al,%dx // 写入0x3f9(0x2f9)。
ret

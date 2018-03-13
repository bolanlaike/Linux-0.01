!
! setup.s (C) 1991 Linus Torvalds
!
! setup.s is responsible for getting the system data from the BIOS,
! and putting them into the appropriate places in system memory.
! both setup.s and system has been loaded by the bootblock.
!
! This code asks the bios for memory/disk/other parameters, and
! puts them in a "safe" place: 0x90000-0x901FF, ie where the
! boot-block used to be. It is then up to the protected mode
! system to read them from there before the area is overwritten
! for buffer-blocks.
!
! setup.s 负责从BIOS 中获取系统数据，并将这些数据放到系统内存的适当地方。
! 此时setup.s 和system 已经由bootsect 引导块加载到内存中。
!
! 这段代码询问bios 有关内存/磁盘/其它参数，并将这些参数放到一个
! “安全的”地方：0x90000-0x901FF，也即原来bootsect 代码块曾经在
! 的地方，然后在被缓冲块覆盖掉之前由保护模式的system 读取。
!

! NOTE! These had better be the same as in bootsect.s!
! 以下这些参数最好和bootsect.s 中的相同！

INITSEG = 0x9000 ! we move boot here - out of the way ! 原来bootsect 所处的段。
SYSSEG = 0x1000 ! system loaded at 0x10000 (65536). ! system 在0x10000(64k)处。
SETUPSEG = 0x9020 ! this is the current segment ! 本程序所在的段地址。

.globl begtext, begdata, begbss, endtext, enddata, endbss
.text
begtext:
.data
begdata:
.bss
begbss:
.text

entry start
start:

! ok, the read went well so we get current cursor position and save it for
! posterity.
! ok，整个读磁盘过程都正常，现在将光标位置保存以备今后使用。

mov ax,#INITSEG ! this is done in bootsect already, but...
! 将ds 置成#INITSEG(0x9000)。这已经在bootsect 程序中
! 设置过，但是现在是setup 程序，Linus 觉得需要再重新
! 设置一下。
mov ds,ax
mov ah,#0x03 ! read cursor pos
! BIOS 中断0x10 的读光标功能号 ah = 0x03
! 输入：bh = 页号
! 返回：ch = 扫描开始线，cl = 扫描结束线，
! dh = 行号(0x00 是顶端)，dl = 列号(0x00 是左边)。
xor bh,bh
int 0x10 ! save it in known place, con_init fetches
mov [0],dx ! it from 0x90000.
! 上两句是说将光标位置信息存放在0x90000 处，控制台
! 初始化时会来取。

! Get memory size (extended mem, kB) ! 下面3 句取扩展内存的大小值（KB）。
! 是调用中断0x15，功能号ah = 0x88
! 返回：ax = 从0x100000（1M）处开始的扩展内存大小(KB)。
! 若出错则CF 置位，ax = 出错码。

mov ah,#0x88
int 0x15
mov [2],ax ! 将扩展内存数值存在0x90002 处（1 个字）。

! Get video-card data: ! 下面这段用于取显示卡当前显示模式。
! 调用BIOS 中断0x10，功能号 ah = 0x0f
! 返回：ah = 字符列数，al = 显示模式，bh = 当前显示页。
! 0x90004(1 字)存放当前页，0x90006 显示模式，0x90007 字符列数。

mov ah,#0x0f
int 0x10
mov [4],bx ! bh = display page
mov [6],ax ! al = video mode, ah = window width

! check for EGA/VGA and some config parameters ! 检查显示方式（EGA/VGA）并取参数。
! 调用BIOS 中断0x10，附加功能选择 -取方式信息
! 功能号：ah = 0x12，bl = 0x10
! 返回：bh = 显示状态
! (0x00 - 彩色模式，I/O 端口=0x3dX)
! (0x01 - 单色模式，I/O 端口=0x3bX)
! bl = 安装的显示内存
! (0x00 - 64k, 0x01 - 128k, 0x02 - 192k, 0x03 = 256k)
! cx = 显示卡特性参数(参见程序后的说明)。

mov ah,#0x12
mov bl,#0x10
int 0x10
mov [8],ax ! 0x90008 = ??
mov [10],bx ! 0x9000A = 安装的显示内存，0x9000B = 显示状态(彩色/单色)
mov [12],cx ! 0x9000C = 显示卡特性参数。

! Get hd0 data ! 取第一个硬盘的信息（复制硬盘参数表）。
! 第1 个硬盘参数表的首地址竟然是中断向量0x41 的向量值！而第2 个硬盘
! 参数表紧接第1 个表的后面，中断向量0x46 的向量值也指向这第2 个硬盘
! 的参数表首址。表的长度是16 个字节(0x10)。
! 下面两段程序分别复制BIOS 有关两个硬盘的参数表，0x90080 处存放第1 个
! 硬盘的表，0x90090 处存放第2 个硬盘的表。

mov ax,#0x0000
mov ds,ax
lds si,[4*0x41] ! 取中断向量0x41 的值，也即hd0 参数表的地址??ds:si
mov ax,#INITSEG
mov es,ax
mov di,#0x0080 ! 传输的目的地址: 0x9000:0x0080 ?? es:di
mov cx,#0x10 ! 共传输0x10 字节。
rep
movsb

! Get hd1 data

mov ax,#0x0000
mov ds,ax
lds si,[4*0x46] ! 取中断向量0x46 的值，也即hd1 参数表的地址??ds:si
mov ax,#INITSEG
mov es,ax
mov di,#0x0090 ! 传输的目的地址: 0x9000:0x0090 ?? es:di
mov cx,#0x10
rep
movsb

! Check that there IS a hd1 :-) ! 检查系统是否存在第2 个硬盘，如果不存在则第2 个表清零。
! 利用BIOS 中断调用0x13 的取盘类型功能。
! 功能号 ah = 0x15；
! 输入：dl = 驱动器号（0x8X 是硬盘：0x80 指第1 个硬盘，0x81 第2 个硬盘）
! 输出：ah = 类型码；00 --没有这个盘，CF 置位； 01 --是软驱，没有change-line 支持；
! 02 --是软驱(或其它可移动设备)，有change-line 支持； 03 --是硬盘。

mov ax,#0x01500
mov dl,#0x81
int 0x13
jc no_disk1
cmp ah,#3 ! 是硬盘吗？(类型 = 3 ？)。
je is_disk1
no_disk1:
mov ax,#INITSEG ! 第2 个硬盘不存在，则对第2 个硬盘表清零。
mov es,ax
mov di,#0x0090
mov cx,#0x10
mov ax,#0x00
rep
stosb
is_disk1:

! now we want to move to protected mode ... ! 从这里开始我们要保护模式方面的工作了。

cli ! no interrupts allowed ! ! 此时不允许中断。

! first we move the system to it's rightful place
! 首先我们将system 模块移到正确的位置。
! bootsect 引导程序是将system 模块读入到从0x10000（64k）开始的位置。由于当时假设
! system 模块最大长度不会超过0x80000（512k），也即其末端不会超过内存地址0x90000，
! 所以bootsect 会将自己移动到0x90000 开始的地方，并把setup 加载到它的后面。
! 下面这段程序的用途是再把整个system 模块移动到0x00000 位置，即把从0x10000 到0x8ffff
! 的内存数据块(512k)，整块地向内存低端移动了0x10000（64k）的位置。

mov ax,#0x0000
cld ! 'direction'=0, movs moves forward
do_move:
mov es,ax ! destination segment ! es:di??目的地址(初始为0x0000:0x0)
add ax,#0x1000
cmp ax,#0x9000 ! 已经把从0x8000 段开始的64k 代码移动完？
jz end_move
mov ds,ax ! source segment ! ds:si??源地址(初始为0x1000:0x0)
sub di,di
sub si,si
mov cx,#0x8000 ! 移动0x8000 字（64k 字节）。
rep
movsw
jmp do_move

! then we load the segment descriptors
! 此后，我们加载段描述符。
! 从这里开始会遇到32 位保护模式的操作，因此需要Intel 32 位保护模式编程方面的知识了,
! 有关这方面的信息请查阅列表后的简单介绍或附录中的详细说明。这里仅作概要说明。
!
! lidt 指令用于加载中断描述符表(idt)寄存器，它的操作数是6 个字节，0-1 字节是描述符表的
! 长度值(字节)；2-5 字节是描述符表的32 位线性基地址（首地址），其形式参见下面
! 219-220 行和223-224 行的说明。中断描述符表中的每一个表项（8 字节）指出发生中断时
! 需要调用的代码的信息，与中断向量有些相似，但要包含更多的信息。
!
! lgdt 指令用于加载全局描述符表(gdt)寄存器，其操作数格式与lidt 指令的相同。全局描述符
! 表中的每个描述符项(8 字节)描述了保护模式下数据和代码段（块）的信息。其中包括段的
! 最大长度限制(16 位)、段的线性基址（32 位）、段的特权级、段是否在内存、读写许可以及
! 其它一些保护模式运行的标志。参见后面205-216 行。
!

end_move:
mov ax,#SETUPSEG ! right, forgot this at first. didn't work :-)
mov ds,ax ! ds 指向本程序(setup)段。
lidt idt_48 ! load idt with 0,0
! 加载中断描述符表(idt)寄存器，idt_48 是6 字节操作数的位置
! (见218 行)。前2 字节表示idt 表的限长，后4 字节表示idt 表
! 所处的基地址。
lgdt gdt_48 ! load gdt with whatever appropriate
! 加载全局描述符表(gdt)寄存器，gdt_48 是6 字节操作数的位置
! (见222 行)。

! that was painless, now we enable A20
! 以上的操作很简单，现在我们开启A20 地址线。参见程序列表后有关A20 信号线的说明。

call empty_8042 ! 等待输入缓冲器空。
! 只有当输入缓冲器为空时才可以对其进行写命令。
mov al,#0xD1 ! command write ! 0xD1 命令码-表示要写数据到
out #0x64,al ! 8042 的P2 端口。P2 端口的位1 用于A20 线的选通。
! 数据要写到0x60 口。
call empty_8042 ! 等待输入缓冲器空，看命令是否被接受。
mov al,#0xDF ! A20 on ! 选通A20 地址线的参数。
out #0x60,al
call empty_8042 ! 输入缓冲器为空，则表示A20 线已经选通。

! well, that went ok, I hope. Now we have to reprogram the interrupts :-(
! we put them right after the intel-reserved hardware interrupts, at
! int 0x20-0x2F. There they won't mess up anything. Sadly IBM really
! messed this up with the original PC, and they haven't been able to
! rectify it afterwards. Thus the bios puts interrupts at 0x08-0x0f,
! which is used for the internal hardware interrupts as well. We just
! have to reprogram the 8259's, and it isn't fun.
!! 希望以上一切正常。现在我们必须重新对中断进行编程??
!! 我们将它们放在正好处于intel 保留的硬件中断后面，在int 0x20-0x2F。
!! 在那里它们不会引起冲突。不幸的是IBM 在原PC 机中搞糟了，以后也没有纠正过来。
!! PC 机的bios 将中断放在了0x08-0x0f，这些中断也被用于内部硬件中断。
!! 所以我们就必须重新对8259 中断控制器进行编程，这一点都没劲。

mov al,#0x11 ! initialization sequence
! 0x11 表示初始化命令开始，是ICW1 命令字，表示边
! 沿触发、多片8259 级连、最后要发送ICW4 命令字。
out #0x20,al ! send it to 8259A-1 ! 发送到8259A 主芯片。
.word 0x00eb,0x00eb ! jmp $+2, jmp $+2 ! $ 表示当前指令的地址，
! 两条跳转指令，跳到下一条指令，起延时作用。
out #0xA0,al ! and to 8259A-2 ! 再发送到8259A 从芯片。
.word 0x00eb,0x00eb
mov al,#0x20 ! start of hardware int's (0x20)
out #0x21,al ! 送主芯片ICW2 命令字，起始中断号，要送奇地址。
.word 0x00eb,0x00eb
mov al,#0x28 ! start of hardware int's 2 (0x28)
out #0xA1,al ! 送从芯片ICW2 命令字，从芯片的起始中断号。
.word 0x00eb,0x00eb
mov al,#0x04 ! 8259-1 is master
out #0x21,al ! 送主芯片ICW3 命令字，主芯片的IR2 连从芯片INT。
.word 0x00eb,0x00eb ！参见代码列表后的说明。
mov al,#0x02 ! 8259-2 is slave
out #0xA1,al ! 送从芯片ICW3 命令字，表示从芯片的INT 连到主芯
! 片的IR2 引脚上。
.word 0x00eb,0x00eb
mov al,#0x01 ! 8086 mode for both
out #0x21,al ! 送主芯片ICW4 命令字。8086 模式；普通EOI 方式，
! 需发送指令来复位。初始化结束，芯片就绪。
.word 0x00eb,0x00eb
out #0xA1,al ！送从芯片ICW4 命令字，内容同上。
.word 0x00eb,0x00eb
mov al,#0xFF ! mask off all interrupts for now
out #0x21,al ! 屏蔽主芯片所有中断请求。
.word 0x00eb,0x00eb
out #0xA1,al ！屏蔽从芯片所有中断请求。

! well, that certainly wasn't fun :-(. Hopefully it works, and we don't
! need no steenking BIOS anyway (except for the initial loading :-).
! The BIOS-routine wants lots of unnecessary data, and it's less
! "interesting" anyway. This is how REAL programmers do it.
!
! Well, now's the time to actually move into protected mode. To make
! things as simple as possible, we do no register set-up or anything,
! we let the gnu-compiled 32-bit programs do that. We just jump to
! absolute address 0x00000, in 32-bit protected mode.
!! 哼，上面这段当然没劲??，希望这样能工作，而且我们也不再需要乏味的BIOS 了（除了
!! 初始的加载?。BIOS 子程序要求很多不必要的数据，而且它一点都没趣。那是“真正”的
!! 程序员所做的事。

! 这里设置进入32 位保护模式运行。首先加载机器状态字(lmsw - Load Machine Status Word)，也称
! 控制寄存器CR0，其比特位0 置1 将导致CPU 工作在保护模式。
mov ax,#0x0001 ! protected mode (PE) bit ! 保护模式比特位(PE)。
lmsw ax ! This is it! ! 就这样加载机器状态字!
jmpi 0,8 ! jmp offset 0 of segment 8 (cs) ! 跳转至cs 段8，偏移0 处。
! 我们已经将system 模块移动到0x00000 开始的地方，所以这里的偏移地址是0。这里的段
! 值的8 已经是保护模式下的段选择符了，用于选择描述符表和描述符表项以及所要求的特权级。
! 段选择符长度为16 位（2 字节）；位0-1 表示请求的特权级0-3，linux 操作系统只
! 用到两级：0 级（系统级）和3 级（用户级）；位2 用于选择全局描述符表(0)还是局部描
! 述符表(1)；位3-15 是描述符表项的索引，指出选择第几项描述符。所以段选择符
! 8(0b0000,0000,0000,1000)表示请求特权级0、使用全局描述符表中的第1 项，该项指出
! 代码的基地址是0（参见209 行），因此这里的跳转指令就会去执行system 中的代码。

! This routine checks that the keyboard command queue is empty
! No timeout is used - if this hangs there is something wrong with
! the machine, and we probably couldn't proceed anyway.
! 下面这个子程序检查键盘命令队列是否为空。这里不使用超时方法 - 如果这里死机，
! 则说明PC 机有问题，我们就没有办法再处理下去了。
! 只有当输入缓冲器为空时（状态寄存器位2 = 0）才可以对其进行写命令。
empty_8042:
.word 0x00eb,0x00eb ! 这是两个跳转指令的机器码(跳转到下一句)，相当于延时空操作。
in al,#0x64 ! 8042 status port ! 读AT 键盘控制器状态寄存器。
test al,#2 ! is input buffer full? ! 测试位2，输入缓冲器满？
jnz empty_8042 ! yes - loop
ret

gdt: ! 全局描述符表开始处。描述符表由多个8 字节长的描述符项组成。
! 这里给出了3 个描述符项。第1 项无用（206 行），但须存在。第2 项是系统代码段
! 描述符（208-211 行），第3 项是系统数据段描述符(213-216 行)。每个描述符的具体
! 含义参见列表后说明。
.word 0,0,0,0 ! dummy ! 第1 个描述符，不用。
! 这里在gdt 表中的偏移量为0x08，当加载代码段寄存器(段选择符)时，使用的是这个偏移值。
.word 0x07FF ! 8Mb - limit=2047 (2048*4096=8Mb)
.word 0x0000 ! base address=0
.word 0x9A00 ! code read/exec
.word 0x00C0 ! granularity=4096, 386
! 这里在gdt 表中的偏移量是0x10，当加载数据段寄存器(如ds 等)时，使用的是这个偏移值。
.word 0x07FF ! 8Mb - limit=2047 (2048*4096=8Mb)
.word 0x0000 ! base address=0
.word 0x9200 ! data read/write
.word 0x00C0 ! granularity=4096, 386

idt_48:
.word 0 ! idt limit=0
.word 0,0 ! idt base=0L

gdt_48:
.word 0x800 ! gdt limit=2048, 256 GDT entries
! 全局表长度为2k 字节，因为每8 字节组成一个段描述符项
! 所以表中共可有256 项。
.word 512+gdt,0x9 ! gdt base = 0X9xxxx
! 4 个字节构成的内存线性地址：0x0009<<16 + 0x0200+gdt
! 也即0x90200 + gdt(即在本程序段中的偏移地址，205 行)。

.text
endtext:
.data
enddata:
.bss
endbss:

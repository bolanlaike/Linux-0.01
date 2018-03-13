#
# if you want the ram-disk device, define this to be the # 如果你要使用RAM 盘设备的话，就
# size in blocks. # 定义块的大小。
#
RAMDISK = #-DRAMDISK=512

AS86 =as86 -0 -a # 8086 汇编编译器和连接器，见列表后的介绍。后带的参数含义分别
LD86 =ld86 -0 # 是：-0 生成8086 目标程序；-a 生成与gas 和gld 部分兼容的代码。

AS =gas # GNU 汇编编译器和连接器，见列表后的介绍。
LD =gld
LDFLAGS =-s -x -M # GNU 连接器gld 运行时用到的选项。含义是：-s 输出文件中省略所
# 有的符号信息；-x 删除所有局部符号；-M 表示需要在标准输出设备
# (显示器)上打印连接映象(link map)，是指由连接程序产生的一种
# 内存地址映象，其中列出了程序段装入到内存中的位置信息。具体
# 来讲有如下信息：
head main kernel mm fs lib
bootsect setup system
Build 工具
内核映象文件
Image
2.8 linux/Makefile 文件
# ? 目标文件及符号信息映射到内存中的位置；
# ? 公共符号如何放置；
# ? 连接中包含的所有文件成员及其引用的符号。
CC =gcc $(RAMDISK) # gcc 是GNU C 程序编译器。对于UNIX 类的脚本(script)程序而言，
# 在引用定义的标识符时，需在前面加上$符号并用括号括住标识符。
CFLAGS =-Wall -O -fstrength-reduce -fomit-frame-pointer \
-fcombine-regs -mstring-insns # gcc 的选项。前一行最后的'\'符号表示下一行是续行。
# 选项含义为：-Wall 打印所有警告信息；-O 对代码进行优化；
# -fstrength-reduce 优化循环语句；-mstring-insns 是
# Linus 自己为gcc 增加的选项，可以去掉。
CPP =cpp -nostdinc -Iinclude # cpp 是gcc 的前(预)处理程序。-nostdinc -Iinclude 的含
# 义是不要搜索标准的头文件目录中的文件，而是使用-I
# 选项指定的目录或者是在当前目录里搜索头文件。

#
# ROOT_DEV specifies the default root-device when making the image.
# This can be either FLOPPY, /dev/xxxx or empty, in which case the
# default of /dev/hd6 is used by 'build'.
#
ROOT_DEV=/dev/hd6 # ROOT_DEV 指定在创建内核映像(image)文件时所使用的默认根文件系统所
# 在的设备，这可以是软盘(FLOPPY)、/dev/xxxx 或者干脆空着，空着时
# build 程序（在tools/目录中）就使用默认值/dev/hd6。

ARCHIVES=kernel/kernel.o mm/mm.o fs/fs.o # kernel 目录、mm 目录和fs 目录所产生的目标代
# 码文件。为了方便引用在这里将它们用
# ARCHIVES（归档文件）标识符表示。
DRIVERS =kernel/blk_drv/blk_drv.a kernel/chr_drv/chr_drv.a # 块和字符设备库文件。.a 表
# 示该文件是个归档文件，也即包含有许多可执行二进制代码子程
# 序集合的库文件，通常是用GNU 的ar 程序生成。ar 是GNU 的二进制
# 文件处理程序，用于创建、修改以及从归档文件中抽取文件。
MATH =kernel/math/math.a # 数学运算库文件。
LIBS =lib/lib.a # 由lib/目录中的文件所编译生成的通用库文件。

.c.s: # make 老式的隐式后缀规则。该行指示make 利用下面的命令将所有的
# .c 文件编译生成.s 汇编程序。':'表示下面是该规则的命令。
$(CC) $(CFLAGS) \
-nostdinc -Iinclude -S -o $*.s $< # 指使gcc 采用前面CFLAGS 所指定的选项以及
# 仅使用include/目录中的头文件，在适当地编译后不进行汇编就
# 停止（-S），从而产生与输入的各个C 文件对应的汇编语言形式的
# 代码文件。默认情况下所产生的汇编程序文件是原C 文件名去掉.c
# 而加上.s 后缀。-o 表示其后是输出文件的形式。其中$*.s（或$@）
# 是自动目标变量，$<代表第一个先决条件，这里即是符合条件
# *.c 的文件。
.s.o: # 表示将所有.s 汇编程序文件编译成.o 目标文件。下一条是实
# 现该操作的具体命令。
$(AS) -c -o $*.o $< # 使用gas 编译器将汇编程序编译成.o 目标文件。-c 表示只编译
# 或汇编，但不进行连接操作。
.c.o: # 类似上面，*.c 文件-??*.o 目标文件。
$(CC) $(CFLAGS) \
-nostdinc -Iinclude -c -o $*.o $< # 使用gcc 将C 语言文件编译成目标文件但不连接。

all: Image # all 表示创建Makefile 所知的最顶层的目标。这里即是image 文件。

2.8 linux/Makefile 文件
Image: boot/bootsect boot/setup tools/system tools/build # 说明目标（Image 文件）是由
# 分号后面的4 个元素产生，分别是boot/目录中的bootsect 和
# setup 文件、tools/目录中的system 和build 文件。
tools/build boot/bootsect boot/setup tools/system $(ROOT_DEV) > Image
sync # 这两行是执行的命令。第一行表示使用tools 目录下的build 工具
# 程序（下面会说明如何生成）将bootsect、setup 和system 文件
# 以$(ROOT_DEV)为根文件系统设备组装成内核映像文件Image。
# 第二行的sync 同步命令是迫使缓冲块数据立即写盘并更新超级块。

disk: Image # 表示disk 这个目标要由Image 产生。
dd bs=8192 if=Image of=/dev/PS0 # dd 为UNIX 标准命令：复制一个文件，根据选项
# 进行转换和格式化。bs=表示一次读/写的字节数。
# if=表示输入的文件，of=表示输出到的文件。
# 这里/dev/PS0 是指第一个软盘驱动器(设备文件)。

tools/build: tools/build.c # 由tools 目录下的build.c 程序生成执行程序build。
$(CC) $(CFLAGS) \
-o tools/build tools/build.c # 编译生成执行程序build 的命令。

boot/head.o: boot/head.s # 利用上面给出的.s.o 规则生成head.o 目标文件。

tools/system: boot/head.o init/main.o \
$(ARCHIVES) $(DRIVERS) $(MATH) $(LIBS) # 表示tools 目录中的system 文件
# 要由分号右边所列的元素生成。
$(LD) $(LDFLAGS) boot/head.o init/main.o \
$(ARCHIVES) \
$(DRIVERS) \
$(MATH) \
$(LIBS) \
-o tools/system > System.map # 生成system 的命令。最后的 > System.map 表示
# gld 需要将连接映象重定向存放在System.map 文件中。
# 关于System.map 文件的用途参见注释后的说明。

kernel/math/math.a: # 数学协处理函数文件math.a 由下一行上的命令实现。
(cd kernel/math; make) # 进入kernel/math/目录；运行make 工具程序。
# 下面从66--82 行的含义与此处的类似。

kernel/blk_drv/blk_drv.a: # 块设备函数文件blk_drv.a
(cd kernel/blk_drv; make)

kernel/chr_drv/chr_drv.a: # 字符设备函数文件chr_drv.a
(cd kernel/chr_drv; make)

kernel/kernel.o: # 内核目标模块kernel.o
(cd kernel; make)

mm/mm.o: # 内存管理模块mm.o
(cd mm; make)

fs/fs.o: # 文件系统目标模块fs.o
(cd fs; make)

lib/lib.a: # 库函数lib.a
2.8 linux/Makefile 文件
(cd lib; make)

boot/setup: boot/setup.s # 这里开始的三行是使用8086 汇编和连接器
$(AS86) -o boot/setup.o boot/setup.s # 对setup.s 文件进行编译生成setup 文件。
$(LD86) -s -o boot/setup boot/setup.o # -s 选项表示要去除目标文件中的符号信息。

boot/bootsect: boot/bootsect.s # 同上。生成bootsect.o 磁盘引导块。
$(AS86) -o boot/bootsect.o boot/bootsect.s
$(LD86) -s -o boot/bootsect boot/bootsect.o

tmp.s: boot/bootsect.s tools/system # 从92--95 这四行的作用是在bootsect.s 程序开头添加
# 一行有关system 文件长度信息。方法是首先生成含有“SYSSIZE = system 文件实际长度”
# 一行信息的tmp.s 文件，然后将bootsect.s 文件添加在其后。取得system 长度的方法是：
# 首先利用命令ls 对system 文件进行长列表显示，用grep 命令取得列表行上文件字节数字段
# 信息，并定向保存在tmp.s 临时文件中。cut 命令用于剪切字符串，tr 用于去除行尾的回车符。
# 其中：(实际长度 + 15)/16 用于获得用‘节’表示的长度信息。1 节 = 16 字节。
(echo -n "SYSSIZE = (";ls -l tools/system | grep system \
| cut -c25-31 | tr '\012' ' '; echo "+ 15 ) / 16") > tmp.s
cat boot/bootsect.s >> tmp.s

clean: # 当执行'make clean'时，就会执行98--103 行上的命令，去除所有编译连接生成的文件。
# 'rm'是文件删除命令，选项-f 含义是忽略不存在的文件，并且不显示删除信息。
rm -f Image System.map tmp_make core boot/bootsect boot/setup
rm -f init/*.o tools/system tools/build boot/*.o
(cd mm;make clean) # 进入mm/目录；执行该目录Makefile 文件中的clean 规则。
(cd fs;make clean)
(cd kernel;make clean)
(cd lib;make clean)

backup: clean # 该规则将首先执行上面的clean 规则，然后对linux/目录进行压缩，生成
# backup.Z 压缩文件。'cd .. '表示退到linux/的上一级（父）目录；
# 'tar cf - linux'表示对linux/目录执行tar 归档程序。-cf 表示需要创建
# 新的归档文件 '| compress -'表示将tar 程序的执行通过管道操作('|')
# 传递给压缩程序compress，并将压缩程序的输出存成backup.Z 文件。
(cd .. ; tar cf - linux | compress - > backup.Z)
sync # 迫使缓冲块数据立即写盘并更新磁盘超级块。

dep:
# 该目标或规则用于各文件之间的依赖关系。创建的这些依赖关系是为了给make 用来确定是否需要要
# 重建一个目标对象的。比如当某个头文件被改动过后，make 就通过生成的依赖关系，重新编译与该
# 头文件有关的所有*.c 文件。具体方法如下：
# 使用字符串编辑程序sed 对Makefile 文件（这里即是自己）进行处理，输出为删除Makefile
# 文件中'### Dependencies'行后面的所有行（下面从118 开始的行），并生成tmp_make
# 临时文件（也即110 行的作用）。然后对init/目录下的每一个C 文件（其实只有一个文件
# main.c）执行gcc 预处理操作，-M 标志告诉预处理程序输出描述每个目标文件相关性的规则，
# 并且这些规则符合make 语法。对于每一个源文件，预处理程序输出一个make 规则，其结果
# 形式是相应源程序文件的目标文件名加上其依赖关系--该源文件中包含的所有头文件列表。
# 111 行中的$$i 实际上是$($i)的意思。这里$i 是这句前面的shell 变量的值。
# 然后把预处理结果都添加到临时文件tmp_make 中，然后将该临时文件复制成新的Makefile 文件。
sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
(for i in init/*.c;do echo -n "init/";$(CPP) -M $$i;done) >> tmp_make
cp tmp_make Makefile
(cd fs; make dep) # 对fs/目录下的Makefile 文件也作同样的处理。
2.8 linux/Makefile 文件
- 30 -
(cd kernel; make dep)
(cd mm; make dep)

### Dependencies:
init/main.o : init/main.c include/unistd.h include/sys/stat.h \
include/sys/types.h include/sys/times.h include/sys/utsname.h \
include/utime.h include/time.h include/linux/tty.h include/termios.h \
include/linux/sched.h include/linux/head.h include/linux/fs.h \
include/linux/mm.h include/signal.h include/asm/system.h include/asm/io.h \
include/stddef.h include/stdarg.h include/fcntl.h

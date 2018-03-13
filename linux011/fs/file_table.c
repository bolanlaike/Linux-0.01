/*
* linux/fs/file_table.c
*
* (C) 1991 Linus Torvalds
*/

#include <linux/fs.h>		// 文件系统头文件。定义文件表结构（file,buffer_head,m_inode 等）。

struct file file_table[NR_FILE];	// 文件表数组(64 项)。
10

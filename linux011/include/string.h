#ifndef _STRING_H_
#define _STRING_H_

#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

extern char *strerror (int errno);

/*
* This string-include defines all string functions as inline
* functions. Use gcc. It also assumes ds=es=data space, this should be
* normal. Most of the string-functions are rather heavily hand-optimized,
* see especially strtok,strstr,str[c]spn. They should work, but are not
* very easy to understand. Everything is done entirely within the register
* set, making the functions fast and clean. String instructions have been
* used through-out, making for "slightly" unclear code :-)
*
* (C) 1991 Linus Torvalds
*/
/*
* 这个字符串头文件以内嵌函数的形式定义了所有字符串操作函数。使用gcc 时，同时
* 假定了ds=es=数据空间，这应该是常规的。绝大多数字符串函数都是经手工进行大量
* 优化的，尤其是函数strtok、strstr、str[c]spn。它们应该能正常工作，但却不是那
* 么容易理解。所有的操作基本上都是使用寄存器集来完成的，这使得函数即快有整洁。
* 所有地方都使用了字符串指令，这又使得代码“稍微”难以理解?
*
* (C) 1991 Linus Torvalds
*/

//// 将一个字符串(src)拷贝到另一个字符串(dest)，直到遇到NULL 字符后停止。
// 参数：dest - 目的字符串指针，src - 源字符串指针。
// %0 - esi(src)，%1 - edi(dest)。
extern inline char *
strcpy (char *dest, const char *src)
{
  __asm__ ("cld\n"		// 清方向位。
	   "1:\tlodsb\n\t"	// 加载DS:[esi]处1 字节??al，并更新esi。
	   "stosb\n\t"		// 存储字节al??ES:[edi]，并更新edi。
	   "testb %%al,%%al\n\t"	// 刚存储的字节是0？
	   "jne 1b"		// 不是则向后跳转到标号1 处，否则结束。
::"S" (src), "D" (dest):"si", "di", "ax");
  return dest;			// 返回目的字符串指针。
}

//// 拷贝源字符串count 个字节到目的字符串。
// 如果源串长度小于count 个字节，就附加空字符(NULL)到目的字符串。
// 参数：dest - 目的字符串指针，src - 源字符串指针，count - 拷贝字节数。
// %0 - esi(src)，%1 - edi(dest)，%2 - ecx(count)。
extern inline char *
strncpy (char *dest, const char *src, int count)
{
  __asm__ ("cld\n"		// 清方向位。
	   "1:\tdecl %2\n\t"	// 寄存器ecx--（count--）。
	   "js 2f\n\t"		// 如果count<0 则向前跳转到标号2，结束。
	   "lodsb\n\t"		// 取ds:[esi]处1 字节??al，并且esi++。
	   "stosb\n\t"		// 存储该字节??es:[edi]，并且edi++。
	   "testb %%al,%%al\n\t"	// 该字节是0？
	   "jne 1b\n\t"		// 不是，则向前跳转到标号1 处继续拷贝。
	   "rep\n\t"		// 否则，在目的串中存放剩余个数的空字符。
"stosb\n" "2:"::"S" (src), "D" (dest), "c" (count):"si", "di", "ax",
	   "cx");
  return dest;			// 返回目的字符串指针。
}

//// 将源字符串拷贝到目的字符串的末尾处。
// 参数：dest - 目的字符串指针，src - 源字符串指针。
// %0 - esi(src)，%1 - edi(dest)，%2 - eax(0)，%3 - ecx(-1)。
extern inline char *
strcat (char *dest, const char *src)
{
  __asm__ ("cld\n\t"		// 清方向位。
	   "repne\n\t"		// 比较al 与es:[edi]字节，并更新edi++，
	   "scasb\n\t"		// 直到找到目的串中是0 的字节，此时edi 已经指向后1 字节。
	   "decl %1\n"		// 让es:[edi]指向0 值字节。
	   "1:\tlodsb\n\t"	// 取源字符串字节ds:[esi]??al，并esi++。
	   "stosb\n\t"		// 将该字节存到es:[edi]，并edi++。
	   "testb %%al,%%al\n\t"	// 该字节是0？
	   "jne 1b"		// 不是，则向后跳转到标号1 处继续拷贝，否则结束。
::"S" (src), "D" (dest), "a" (0), "c" (0xffffffff):"si", "di", "ax",
	   "cx");
  return dest;			// 返回目的字符串指针。
}

//// 将源字符串的count 个字节复制到目的字符串的末尾处，最后添一空字符。
// 参数：dest - 目的字符串，src - 源字符串，count - 欲复制的字节数。
// %0 - esi(src)，%1 - edi(dest)，%2 - eax(0)，%3 - ecx(-1)，%4 - (count)。
extern inline char *
strncat (char *dest, const char *src, int count)
{
  __asm__ ("cld\n\t"		// 清方向位。
	   "repne\n\t"		// 比较al 与es:[edi]字节，edi++。
	   "scasb\n\t"		// 直到找到目的串的末端0 值字节。
	   "decl %1\n\t"	// edi 指向该0 值字节。
	   "movl %4,%3\n"	// 欲复制字节数??ecx。
	   "1:\tdecl %3\n\t"	// ecx--（从0 开始计数）。
	   "js 2f\n\t"		// ecx <0 ?，是则向前跳转到标号2 处。
	   "lodsb\n\t"		// 否则取ds:[esi]处的字节??al，esi++。
	   "stosb\n\t"		// 存储到es:[edi]处，edi++。
	   "testb %%al,%%al\n\t"	// 该字节值为0？
	   "jne 1b\n"		// 不是则向后跳转到标号1 处，继续复制。
	   "2:\txorl %2,%2\n\t"	// 将al 清零。
	   "stosb"		// 存到es:[edi]处。
::"S" (src), "D" (dest), "a" (0), "c" (0xffffffff), "g" (count):"si", "di", "ax",
	   "cx");
  return dest;			// 返回目的字符串指针。
}

//// 将一个字符串与另一个字符串进行比较。
// 参数：cs - 字符串1，ct - 字符串2。
// %0 - eax(__res)返回值，%1 - edi(cs)字符串1 指针，%2 - esi(ct)字符串2 指针。
// 返回：如果串1 > 串2，则返回1；串1 = 串2，则返回0；串1 < 串2，则返回-1。
extern inline int
strcmp (const char *cs, const char *ct)
{
  register int __res __asm__ ("ax");	// __res 是寄存器变量(eax)。
  __asm__ ("cld\n"		// 清方向位。
	   "1:\tlodsb\n\t"	// 取字符串2 的字节ds:[esi]??al，并且esi++。
	   "scasb\n\t"		// al 与字符串1 的字节es:[edi]作比较，并且edi++。
	   "jne 2f\n\t"		// 如果不相等，则向前跳转到标号2。
	   "testb %%al,%%al\n\t"	// 该字节是0 值字节吗（字符串结尾）？
	   "jne 1b\n\t"		// 不是，则向后跳转到标号1，继续比较。
	   "xorl %%eax,%%eax\n\t"	// 是，则返回值eax 清零，
	   "jmp 3f\n"		// 向前跳转到标号3，结束。
	   "2:\tmovl $1,%%eax\n\t"	// eax 中置1。
	   "jl 3f\n\t"		// 若前面比较中串2 字符<串1 字符，则返回正值，结束。
	   "negl %%eax\n"	// 否则eax = -eax，返回负值，结束。
"3:": "=a" (__res): "D" (cs), "S" (ct):"si", "di");
  return __res;			// 返回比较结果。
}

//// 字符串1 与字符串2 的前count 个字符进行比较。
// 参数：cs - 字符串1，ct - 字符串2，count - 比较的字符数。
// %0 - eax(__res)返回值，%1 - edi(cs)串1 指针，%2 - esi(ct)串2 指针，%3 - ecx(count)。
// 返回：如果串1 > 串2，则返回1；串1 = 串2，则返回0；串1 < 串2，则返回-1。
extern inline int
strncmp (const char *cs, const char *ct, int count)
{
  register int __res __asm__ ("ax");	// __res 是寄存器变量(eax)。
  __asm__ ("cld\n"		// 清方向位。
	   "1:\tdecl %3\n\t"	// count--。
	   "js 2f\n\t"		// 如果count<0，则向前跳转到标号2。
	   "lodsb\n\t"		// 取串2 的字符ds:[esi]??al，并且esi++。
	   "scasb\n\t"		// 比较al 与串1 的字符es:[edi]，并且edi++。
	   "jne 3f\n\t"		// 如果不相等，则向前跳转到标号3。
	   "testb %%al,%%al\n\t"	// 该字符是NULL 字符吗？
	   "jne 1b\n"		// 不是，则向后跳转到标号1，继续比较。
	   "2:\txorl %%eax,%%eax\n\t"	// 是NULL 字符，则eax 清零（返回值）。
	   "jmp 4f\n"		// 向前跳转到标号4，结束。
	   "3:\tmovl $1,%%eax\n\t"	// eax 中置1。
	   "jl 4f\n\t"		// 如果前面比较中串2 字符<串2 字符，则返回1，结束。
	   "negl %%eax\n"	// 否则eax = -eax，返回负值，结束。
"4:": "=a" (__res): "D" (cs), "S" (ct), "c" (count):"si", "di",
	   "cx");
  return __res;			// 返回比较结果。
}

//// 在字符串中寻找第一个匹配的字符。
// 参数：s - 字符串，c - 欲寻找的字符。
// %0 - eax(__res)，%1 - esi(字符串指针s)，%2 - eax(字符c)。
// 返回：返回字符串中第一次出现匹配字符的指针。若没有找到匹配的字符，则返回空指针。
extern inline char *
strchr (const char *s, char c)
{
  register char *__res __asm__ ("ax");	// __res 是寄存器变量(eax)。
  __asm__ ("cld\n\t"		// 清方向位。
	   "movb %%al,%%ah\n"	// 将欲比较字符移到ah。
	   "1:\tlodsb\n\t"	// 取字符串中字符ds:[esi]??al，并且esi++。
	   "cmpb %%ah,%%al\n\t"	// 字符串中字符al 与指定字符ah 相比较。
	   "je 2f\n\t"		// 若相等，则向前跳转到标号2 处。
	   "testb %%al,%%al\n\t"	// al 中字符是NULL 字符吗？（字符串结尾？）
	   "jne 1b\n\t"		// 若不是，则向后跳转到标号1，继续比较。
	   "movl $1,%1\n"	// 是，则说明没有找到匹配字符，esi 置1。
	   "2:\tmovl %1,%0\n\t"	// 将指向匹配字符后一个字节处的指针值放入eax
	   "decl %0"		// 将指针调整为指向匹配的字符。
: "=a" (__res): "S" (s), "" (c):"si");
  return __res;			// 返回指针。
}

//// 寻找字符串中指定字符最后一次出现的地方。（反向搜索字符串）
// 参数：s - 字符串，c - 欲寻找的字符。
// %0 - edx(__res)，%1 - edx(0)，%2 - esi(字符串指针s)，%3 - eax(字符c)。
// 返回：返回字符串中最后一次出现匹配字符的指针。若没有找到匹配的字符，则返回空指针。
extern inline char *
strrchr (const char *s, char c)
{
  register char *__res __asm__ ("dx");	// __res 是寄存器变量(edx)。
  __asm__ ("cld\n\t"		// 清方向位。
	   "movb %%al,%%ah\n"	// 将欲寻找的字符移到ah。
	   "1:\tlodsb\n\t"	// 取字符串中字符ds:[esi]??al，并且esi++。
	   "cmpb %%ah,%%al\n\t"	// 字符串中字符al 与指定字符ah 作比较。
	   "jne 2f\n\t"		// 若不相等，则向前跳转到标号2 处。
	   "movl %%esi,%0\n\t"	// 将字符指针保存到edx 中。
	   "decl %0\n"		// 指针后退一位，指向字符串中匹配字符处。
	   "2:\ttestb %%al,%%al\n\t"	// 比较的字符是0 吗（到字符串尾）？
	   "jne 1b"		// 不是则向后跳转到标号1 处，继续比较。
: "=d" (__res): "" (0), "S" (s), "a" (c):"ax", "si");
  return __res;			// 返回指针。
}

//// 在字符串1 中寻找第1 个字符序列，该字符序列中的任何字符都包含在字符串2 中。
// 参数：cs - 字符串1 指针，ct - 字符串2 指针。
// %0 - esi(__res)，%1 - eax(0)，%2 - ecx(-1)，%3 - esi(串1 指针cs)，%4 - (串2 指针ct)。
// 返回字符串1 中包含字符串2 中任何字符的首个字符序列的长度值。
extern inline int
strspn (const char *cs, const char *ct)
{
  register char *__res __asm__ ("si");	// __res 是寄存器变量(esi)。
  __asm__ ("cld\n\t"		// 清方向位。
	   "movl %4,%%edi\n\t"	// 首先计算串2 的长度。串2 指针放入edi 中。
	   "repne\n\t"		// 比较al(0)与串2 中的字符（es:[edi]），并edi++。
	   "scasb\n\t"		// 如果不相等就继续比较(ecx 逐步递减)。
	   "notl %%ecx\n\t"	// ecx 中每位取反。
	   "decl %%ecx\n\t"	// ecx--，得串2 的长度值。
	   "movl %%ecx,%%edx\n"	// 将串2 的长度值暂放入edx 中。
	   "1:\tlodsb\n\t"	// 取串1 字符ds:[esi]??al，并且esi++。
	   "testb %%al,%%al\n\t"	// 该字符等于0 值吗（串1 结尾）？
	   "je 2f\n\t"		// 如果是，则向前跳转到标号2 处。
	   "movl %4,%%edi\n\t"	// 取串2 头指针放入edi 中。
	   "movl %%edx,%%ecx\n\t"	// 再将串2 的长度值放入ecx 中。
	   "repne\n\t"		// 比较al 与串2 中字符es:[edi]，并且edi++。
	   "scasb\n\t"		// 如果不相等就继续比较。
	   "je 1b\n"		// 如果相等，则向后跳转到标号1 处。
	   "2:\tdecl %0"	// esi--，指向最后一个包含在串2 中的字符。
: "=S" (__res): "a" (0), "c" (0xffffffff), "" (cs), "g" (ct):"ax", "cx", "dx",
	   "di");
  return __res - cs;		// 返回字符序列的长度值。
}

//// 寻找字符串1 中不包含字符串2 中任何字符的首个字符序列。
// 参数：cs - 字符串1 指针，ct - 字符串2 指针。
// %0 - esi(__res)，%1 - eax(0)，%2 - ecx(-1)，%3 - esi(串1 指针cs)，%4 - (串2 指针ct)。
// 返回字符串1 中不包含字符串2 中任何字符的首个字符序列的长度值。
extern inline int
strcspn (const char *cs, const char *ct)
{
  register char *__res __asm__ ("si");	// __res 是寄存器变量(esi)。
  __asm__ ("cld\n\t"		// 清方向位。
	   "movl %4,%%edi\n\t"	// 首先计算串2 的长度。串2 指针放入edi 中。
	   "repne\n\t"		// 比较al(0)与串2 中的字符（es:[edi]），并edi++。
	   "scasb\n\t"		// 如果不相等就继续比较(ecx 逐步递减)。
	   "notl %%ecx\n\t"	// ecx 中每位取反。
	   "decl %%ecx\n\t"	// ecx--，得串2 的长度值。
	   "movl %%ecx,%%edx\n"	// 将串2 的长度值暂放入edx 中。
	   "1:\tlodsb\n\t"	// 取串1 字符ds:[esi]??al，并且esi++。
	   "testb %%al,%%al\n\t"	// 该字符等于0 值吗（串1 结尾）？
	   "je 2f\n\t"		// 如果是，则向前跳转到标号2 处。
	   "movl %4,%%edi\n\t"	// 取串2 头指针放入edi 中。
	   "movl %%edx,%%ecx\n\t"	// 再将串2 的长度值放入ecx 中。
	   "repne\n\t"		// 比较al 与串2 中字符es:[edi]，并且edi++。
	   "scasb\n\t"		// 如果不相等就继续比较。
	   "jne 1b\n"		// 如果不相等，则向后跳转到标号1 处。
	   "2:\tdecl %0"	// esi--，指向最后一个包含在串2 中的字符。
: "=S" (__res): "a" (0), "c" (0xffffffff), "" (cs), "g" (ct):"ax", "cx", "dx",
	   "di");
  return __res - cs;		// 返回字符序列的长度值。
}

//// 在字符串1 中寻找首个包含在字符串2 中的任何字符。
// 参数：cs - 字符串1 的指针，ct - 字符串2 的指针。
// %0 -esi(__res)，%1 -eax(0)，%2 -ecx(0xffffffff)，%3 -esi(串1 指针cs)，%4 -(串2 指针ct)。
// 返回字符串1 中首个包含字符串2 中字符的指针。
extern inline char *
strpbrk (const char *cs, const char *ct)
{
  register char *__res __asm__ ("si");	// __res 是寄存器变量(esi)。
  __asm__ ("cld\n\t"		// 清方向位。
	   "movl %4,%%edi\n\t"	// 首先计算串2 的长度。串2 指针放入edi 中。
	   "repne\n\t"		// 比较al(0)与串2 中的字符（es:[edi]），并edi++。
	   "scasb\n\t"		// 如果不相等就继续比较(ecx 逐步递减)。
	   "notl %%ecx\n\t"	// ecx 中每位取反。
	   "decl %%ecx\n\t"	// ecx--，得串2 的长度值。
	   "movl %%ecx,%%edx\n"	// 将串2 的长度值暂放入edx 中。
	   "1:\tlodsb\n\t"	// 取串1 字符ds:[esi]??al，并且esi++。
	   "testb %%al,%%al\n\t"	// 该字符等于0 值吗（串1 结尾）？
	   "je 2f\n\t"		// 如果是，则向前跳转到标号2 处。
	   "movl %4,%%edi\n\t"	// 取串2 头指针放入edi 中。
	   "movl %%edx,%%ecx\n\t"	// 再将串2 的长度值放入ecx 中。
	   "repne\n\t"		// 比较al 与串2 中字符es:[edi]，并且edi++。
	   "scasb\n\t"		// 如果不相等就继续比较。
	   "jne 1b\n\t"		// 如果不相等，则向后跳转到标号1 处。
	   "decl %0\n\t"	// esi--，指向一个包含在串2 中的字符。
	   "jmp 3f\n"		// 向前跳转到标号3 处。
	   "2:\txorl %0,%0\n"	// 没有找到符合条件的，将返回值为NULL。
"3:": "=S" (__res): "a" (0), "c" (0xffffffff), "" (cs), "g" (ct):"ax", "cx", "dx",
	   "di");
  return __res;			// 返回指针值。
}

//// 在字符串1 中寻找首个匹配整个字符串2 的字符串。
// 参数：cs - 字符串1 的指针，ct - 字符串2 的指针。
// %0 -eax(__res)，%1 -eax(0)，%2 -ecx(0xffffffff)，%3 -esi(串1 指针cs)，%4 -(串2 指针ct)。
// 返回：返回字符串1 中首个匹配字符串2 的字符串指针。
extern inline char *
strstr (const char *cs, const char *ct)
{
  register char *__res __asm__ ("ax");	// __res 是寄存器变量(eax)。
  __asm__ ("cld\n\t" \		// 清方向位。
	   "movl %4,%%edi\n\t"	// 首先计算串2 的长度。串2 指针放入edi 中。
	   "repne\n\t"		// 比较al(0)与串2 中的字符（es:[edi]），并edi++。
	   "scasb\n\t"		// 如果不相等就继续比较(ecx 逐步递减)。
	   "notl %%ecx\n\t"	// ecx 中每位取反。
	   "decl %%ecx\n\t"	/* NOTE! This also sets Z if searchstring='' */
/* 注意！如果搜索串为空，将设置Z 标志 */// 得串2 的长度值。
	   "movl %%ecx,%%edx\n"	// 将串2 的长度值暂放入edx 中。
	   "1:\tmovl %4,%%edi\n\t"	// 取串2 头指针放入edi 中。
	   "movl %%esi,%%eax\n\t"	// 将串1 的指针复制到eax 中。
	   "movl %%edx,%%ecx\n\t"	// 再将串2 的长度值放入ecx 中。
	   "repe\n\t"		// 比较串1 和串2 字符(ds:[esi],es:[edi])，esi++,edi++。
	   "cmpsb\n\t"		// 若对应字符相等就一直比较下去。
	   "je 2f\n\t"		/* also works for empty string, see above */
/* 对空串同样有效，见上面 */// 若全相等，则转到标号2。
	   "xchgl %%eax,%%esi\n\t"	// 串1 头指针??esi，比较结果的串1 指针??eax。
	   "incl %%esi\n\t"	// 串1 头指针指向下一个字符。
	   "cmpb $0,-1(%%eax)\n\t"	// 串1 指针(eax-1)所指字节是0 吗？
	   "jne 1b\n\t"		// 不是则跳转到标号1，继续从串1 的第2 个字符开始比较。
	   "xorl %%eax,%%eax\n\t"	// 清eax，表示没有找到匹配。
"2:": "=a" (__res): "" (0), "c" (0xffffffff), "S" (cs), "g" (ct):"cx", "dx", "di",
	   "si");
  return __res;			// 返回比较结果。
}

//// 计算字符串长度。
// 参数：s - 字符串。
// %0 - ecx(__res)，%1 - edi(字符串指针s)，%2 - eax(0)，%3 - ecx(0xffffffff)。
// 返回：返回字符串的长度。
extern inline int
strlen (const char *s)
{
  register int __res __asm__ ("cx");	// __res 是寄存器变量(ecx)。
  __asm__ ("cld\n\t"		// 清方向位。
	   "repne\n\t"		// al(0)与字符串中字符es:[edi]比较，
	   "scasb\n\t"		// 若不相等就一直比较。
	   "notl %0\n\t"	// ecx 取反。
	   "decl %0"		// ecx--，得字符串得长度值。
: "=c" (__res): "D" (s), "a" (0), "" (0xffffffff):"di");
  return __res;			// 返回字符串长度值。
}

extern char *___strtok;		// 用于临时存放指向下面被分析字符串1(s)的指针。

//// 利用字符串2 中的字符将字符串1 分割成标记(tokern)序列。
// 将串1 看作是包含零个或多个单词(token)的序列，并由分割符字符串2 中的一个或多个字符分开。
// 第一次调用strtok()时，将返回指向字符串1 中第1 个token 首字符的指针，并在返回token 时将
// 一null 字符写到分割符处。后续使用null 作为字符串1 的调用，将用这种方法继续扫描字符串1，
// 直到没有token 为止。在不同的调用过程中，分割符串2 可以不同。
// 参数：s - 待处理的字符串1，ct - 包含各个分割符的字符串2。
// 汇编输出：%0 - ebx(__res)，%1 - esi(__strtok)；
// 汇编输入：%2 - ebx(__strtok)，%3 - esi(字符串1 指针s)，%4 - （字符串2 指针ct）。
// 返回：返回字符串s 中第1 个token，如果没有找到token，则返回一个null 指针。
// 后续使用字符串s 指针为null 的调用，将在原字符串s 中搜索下一个token。
extern inline char *
strtok (char *s, const char *ct)
{
  register char *__res __asm__ ("si");
  __asm__ ("testl %1,%1\n\t"	// 首先测试esi(字符串1 指针s)是否是NULL。
	   "jne 1f\n\t"		// 如果不是，则表明是首次调用本函数，跳转标号1。
	   "testl %0,%0\n\t"	// 如果是NULL，则表示此次是后续调用，测ebx(__strtok)。
	   "je 8f\n\t"		// 如果ebx 指针是NULL，则不能处理，跳转结束。
	   "movl %0,%1\n"	// 将ebx 指针复制到esi。
	   "1:\txorl %0,%0\n\t"	// 清ebx 指针。
	   "movl $-1,%%ecx\n\t"	// 置ecx = 0xffffffff。
	   "xorl %%eax,%%eax\n\t"	// 清零eax。
	   "cld\n\t"		// 清方向位。
	   "movl %4,%%edi\n\t"	// 下面求字符串2 的长度。edi 指向字符串2。
	   "repne\n\t"		// 将al(0)与es:[edi]比较，并且edi++。
	   "scasb\n\t"		// 直到找到字符串2 的结束null 字符，或计数ecx==0。
	   "notl %%ecx\n\t"	// 将ecx 取反，
	   "decl %%ecx\n\t"	// ecx--，得到字符串2 的长度值。
	   "je 7f\n\t"		/* empty delimeter-string */
/* 分割符字符串空 */// 若串2 长度为0，则转标号7。
	   "movl %%ecx,%%edx\n"	// 将串2 长度暂存入edx。
	   "2:\tlodsb\n\t"	// 取串1 的字符ds:[esi]??al，并且esi++。
	   "testb %%al,%%al\n\t"	// 该字符为0 值吗(串1 结束)？
	   "je 7f\n\t"		// 如果是，则跳转标号7。
	   "movl %4,%%edi\n\t"	// edi 再次指向串2 首。
	   "movl %%edx,%%ecx\n\t"	// 取串2 的长度值置入计数器ecx。
	   "repne\n\t"		// 将al 中串1 的字符与串2 中所有字符比较，
	   "scasb\n\t"		// 判断该字符是否为分割符。
	   "je 2b\n\t"		// 若能在串2 中找到相同字符（分割符），则跳转标号2。
	   "decl %1\n\t"	// 若不是分割符，则串1 指针esi 指向此时的该字符。
	   "cmpb $0,(%1)\n\t"	// 该字符是NULL 字符吗？
	   "je 7f\n\t"		// 若是，则跳转标号7 处。
	   "movl %1,%0\n"	// 将该字符的指针esi 存放在ebx。
	   "3:\tlodsb\n\t"	// 取串1 下一个字符ds:[esi]??al，并且esi++。
	   "testb %%al,%%al\n\t"	// 该字符是NULL 字符吗？
	   "je 5f\n\t"		// 若是，表示串1 结束，跳转到标号5。
	   "movl %4,%%edi\n\t"	// edi 再次指向串2 首。
	   "movl %%edx,%%ecx\n\t"	// 串2 长度值置入计数器ecx。
	   "repne\n\t"		// 将al 中串1 的字符与串2 中每个字符比较，
	   "scasb\n\t"		// 测试al 字符是否是分割符。
	   "jne 3b\n\t"		// 若不是分割符则跳转标号3，检测串1 中下一个字符。
	   "decl %1\n\t"	// 若是分割符，则esi--，指向该分割符字符。
	   "cmpb $0,(%1)\n\t"	// 该分割符是NULL 字符吗？
	   "je 5f\n\t"		// 若是，则跳转到标号5。
	   "movb $0,(%1)\n\t"	// 若不是，则将该分割符用NULL 字符替换掉。
	   "incl %1\n\t"	// esi 指向串1 中下一个字符，也即剩余串首。
	   "jmp 6f\n"		// 跳转标号6 处。
	   "5:\txorl %1,%1\n"	// esi 清零。
	   "6:\tcmpb $0,(%0)\n\t"	// ebx 指针指向NULL 字符吗？
	   "jne 7f\n\t"		// 若不是，则跳转标号7。
	   "xorl %0,%0\n"	// 若是，则让ebx=NULL。
	   "7:\ttestl %0,%0\n\t"	// ebx 指针为NULL 吗？
	   "jne 8f\n\t"		// 若不是则跳转8，结束汇编代码。
	   "movl %0,%1\n"	// 将esi 置为NULL。
"8:": "=b" (__res), "=S" (___strtok): "" (___strtok), "1" (s), "g" (ct):"ax", "cx", "dx",
	   "di");
  return __res;			// 返回指向新token 的指针。
}

//// 内存块复制。从源地址src 处开始复制n 个字节到目的地址dest 处。
// 参数：dest - 复制的目的地址，src - 复制的源地址，n - 复制字节数。
// %0 - ecx(n)，%1 - esi(src)，%2 - edi(dest)。
extern inline void *
memcpy (void *dest, const void *src, int n)
{
  __asm__ ("cld\n\t"		// 清方向位。
	   "rep\n\t"		// 重复执行复制ecx 个字节，
	   "movsb"		// 从ds:[esi]到es:[edi]，esi++，edi++。
::"c" (n), "S" (src), "D" (dest):"cx", "si", "di");
  return dest;			// 返回目的地址。
}

//// 内存块移动。同内存块复制，但考虑移动的方向。
// 参数：dest - 复制的目的地址，src - 复制的源地址，n - 复制字节数。
// 若dest<src 则：%0 - ecx(n)，%1 - esi(src)，%2 - edi(dest)。
// 否则：%0 - ecx(n)，%1 - esi(src+n-1)，%2 - edi(dest+n-1)。
// 这样操作是为了防止在复制时错误地重叠覆盖。
extern inline void *
memmove (void *dest, const void *src, int n)
{
  if (dest < src)
    __asm__ ("cld\n\t"		// 清方向位。
	     "rep\n\t"		// 从ds:[esi]到es:[edi]，并且esi++，edi++，
	     "movsb"		// 重复执行复制ecx 字节。
  ::"c" (n), "S" (src), "D" (dest):"cx", "si", "di");
  else
    __asm__ ("std\n\t"		// 置方向位，从末端开始复制。
	     "rep\n\t"		// 从ds:[esi]到es:[edi]，并且esi--，edi--，
	     "movsb"		// 复制ecx 个字节。
  ::"c" (n), "S" (src + n - 1), "D" (dest + n - 1):"cx", "si",
	     "di");
  return dest;
}

//// 比较n 个字节的两块内存（两个字符串），即使遇上NULL 字节也不停止比较。
// 参数：cs - 内存块1 地址，ct - 内存块2 地址，count - 比较的字节数。
// %0 - eax(__res)，%1 - eax(0)，%2 - edi(内存块1)，%3 - esi(内存块2)，%4 - ecx(count)。
// 返回：若块1>块2 返回1；块1<块2，返回-1；块1==块2，则返回0。
extern inline int
memcmp (const void *cs, const void *ct, int count)
{
  register int __res __asm__ ("ax");	// __res 是寄存器变量。
  __asm__ ("cld\n\t"		// 清方向位。
	   "repe\n\t"		// 如果相等则重复，
	   "cmpsb\n\t"		// 比较ds:[esi]与es:[edi]的内容，并且esi++，edi++。
	   "je 1f\n\t"		// 如果都相同，则跳转到标号1，返回0(eax)值
	   "movl $1,%%eax\n\t"	// 否则eax 置1，
	   "jl 1f\n\t"		// 若内存块2 内容的值<内存块1，则跳转标号1。
	   "negl %%eax\n"	// 否则eax = -eax。
"1:": "=a" (__res): "" (0), "D" (cs), "S" (ct), "c" (count):"si", "di",
	   "cx");
  return __res;			// 返回比较结果。
}

//// 在n 字节大小的内存块(字符串)中寻找指定字符。
// 参数：cs - 指定内存块地址，c - 指定的字符，count - 内存块长度。
// %0 - edi(__res)，%1 - eax(字符c)，%2 - edi(内存块地址cs)，%3 - ecx(字节数count)。
// 返回第一个匹配字符的指针，如果没有找到，则返回NULL 字符。
extern inline void *
memchr (const void *cs, char c, int count)
{
  register void *__res __asm__ ("di");	// __res 是寄存器变量。
  if (!count)			// 如果内存块长度==0，则返回NULL，没有找到。
    return NULL;
  __asm__ ("cld\n\t"		// 清方向位。
	   "repne\n\t"		// 如果不相等则重复执行下面语句，
	   "scasb\n\t"		// al 中字符与es:[edi]字符作比较，并且edi++，
	   "je 1f\n\t"		// 如果相等则向前跳转到标号1 处。
	   "movl $1,%0\n"	// 否则edi 中置1。
	   "1:\tdecl %0"	// 让edi 指向找到的字符（或是NULL）。
: "=D" (__res): "a" (c), "D" (cs), "c" (count):"cx");
  return __res;			// 返回字符指针。
}

//// 用字符填写指定长度内存块。
// 用字符c 填写s 指向的内存区域，共填count 字节。
// %0 - eax(字符c)，%1 - edi(内存地址)，%2 - ecx(字节数count)。
extern inline void *
memset (void *s, char c, int count)
{
  __asm__ ("cld\n\t"		// 清方向位。
	   "rep\n\t"		// 重复ecx 指定的次数，执行
	   "stosb"		// 将al 中字符存入es:[edi]中，并且edi++。
::"a" (c), "D" (s), "c" (count):"cx", "di");
  return s;
}

#endif

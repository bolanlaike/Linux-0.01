/*
* linux/mm/memory.c
*
* (C) 1991 Linus Torvalds
*/

/*
* demand-loading started 01.12.91 - seems it is high on the list of
* things wanted, and it should be easy to implement. - Linus
*/
/*
* ĞèÇó¼ÓÔØÊÇ´Ó01.12.91 ¿ªÊ¼±àĞ´µÄ - ÔÚ³ÌĞò±àÖÆ±íÖĞÊÇºôÊÇ×îÖØÒªµÄ³ÌĞò£¬
* ²¢ÇÒÓ¦¸ÃÊÇºÜÈİÒ×±àÖÆµÄ - linus
*/

/*
* Ok, demand-loading was easy, shared pages a little bit tricker. Shared
* pages started 02.12.91, seems to work. - Linus.
*
* Tested sharing by executing about 30 /bin/sh: under the old kernel it
* would have taken more than the 6M I have free, but it worked well as
* far as I could see.
*
* Also corrected some "invalidate()"s - I wasn't doing enough of them.
*/
/*
* OK£¬ĞèÇó¼ÓÔØÊÇ±È½ÏÈİÒ×±àĞ´µÄ£¬¶ø¹²ÏíÒ³ÃæÈ´ĞèÒªÓĞµã¼¼ÇÉ¡£¹²ÏíÒ³Ãæ³ÌĞòÊÇ
* 02.12.91 ¿ªÊ¼±àĞ´µÄ£¬ºÃÏóÄÜ¹»¹¤×÷ - Linus¡£
*
* Í¨¹ıÖ´ĞĞ´óÔ¼30 ¸ö/bin/sh ¶Ô¹²Ïí²Ù×÷½øĞĞÁË²âÊÔ£ºÔÚÀÏÄÚºËµ±ÖĞĞèÒªÕ¼ÓÃ¶àÓÚ
* 6M µÄÄÚ´æ£¬¶øÄ¿Ç°È´²»ÓÃ¡£ÏÖÔÚ¿´À´¹¤×÷µÃºÜºÃ¡£
*
* ¶Ô"invalidate()"º¯ÊıÒ²½øĞĞÁËĞŞÕı - ÔÚÕâ·½ÃæÎÒ»¹×öµÄ²»¹»¡£
*/

#include <signal.h>		// ĞÅºÅÍ·ÎÄ¼ş¡£¶¨ÒåĞÅºÅ·ûºÅ³£Á¿£¬ĞÅºÅ½á¹¹ÒÔ¼°ĞÅºÅ²Ù×÷º¯ÊıÔ­ĞÍ¡£

#include <asm/system.h>		// ÏµÍ³Í·ÎÄ¼ş¡£¶¨ÒåÁËÉèÖÃ»òĞŞ¸ÄÃèÊö·û/ÖĞ¶ÏÃÅµÈµÄÇ¶ÈëÊ½»ã±àºê¡£

#include <linux/sched.h>	// µ÷¶È³ÌĞòÍ·ÎÄ¼ş£¬¶¨ÒåÁËÈÎÎñ½á¹¹task_struct¡¢³õÊ¼ÈÎÎñ0 µÄÊı¾İ£¬
// »¹ÓĞÒ»Ğ©ÓĞ¹ØÃèÊö·û²ÎÊıÉèÖÃºÍ»ñÈ¡µÄÇ¶ÈëÊ½»ã±àº¯ÊıºêÓï¾ä¡£
#include <linux/head.h>		// head Í·ÎÄ¼ş£¬¶¨ÒåÁË¶ÎÃèÊö·ûµÄ¼òµ¥½á¹¹£¬ºÍ¼¸¸öÑ¡Ôñ·û³£Á¿¡£
#include <linux/kernel.h>	// ÄÚºËÍ·ÎÄ¼ş¡£º¬ÓĞÒ»Ğ©ÄÚºË³£ÓÃº¯ÊıµÄÔ­ĞÎ¶¨Òå¡£

volatile void do_exit (long code);	// ½ø³ÌÍË³ö´¦Àíº¯Êı£¬ÔÚkernel/exit.c£¬102 ĞĞ¡£

//// ÏÔÊ¾ÄÚ´æÒÑÓÃÍê³ö´íĞÅÏ¢£¬²¢ÍË³ö¡£
static inline volatile void
oom (void)
{
  printk ("out of memory\n\r");
  do_exit (SIGSEGV);		// do_exit()Ó¦¸ÃÊ¹ÓÃÍË³ö´úÂë£¬ÕâÀïÓÃÁËĞÅºÅÖµSIGSEGV(11)
}				// ÏàÍ¬ÖµµÄ³ö´íÂëº¬ÒåÊÇ¡°×ÊÔ´ÔİÊ±²»¿ÉÓÃ¡±£¬ÕıºÃÍ¬Òå¡£

// Ë¢ĞÂÒ³±ä»»¸ßËÙ»º³åºêº¯Êı¡£
// ÎªÁËÌá¸ßµØÖ·×ª»»µÄĞ§ÂÊ£¬CPU ½«×î½üÊ¹ÓÃµÄÒ³±íÊı¾İ´æ·ÅÔÚĞ¾Æ¬ÖĞ¸ßËÙ»º³åÖĞ¡£ÔÚĞŞ¸Ä¹ıÒ³±í
// ĞÅÏ¢Ö®ºó£¬¾ÍĞèÒªË¢ĞÂ¸Ã»º³åÇø¡£ÕâÀïÊ¹ÓÃÖØĞÂ¼ÓÔØÒ³Ä¿Â¼»ùÖ·¼Ä´æÆ÷cr3 µÄ·½·¨À´½øĞĞË¢ĞÂ¡£
// ÏÂÃæeax = 0£¬ÊÇÒ³Ä¿Â¼µÄ»ùÖ·¡£
#define invalidate() \
__asm__( "movl %%eax,%%cr3":: "a" (0))

/* these are not to be changed without changing head.s etc */
/* ÏÂÃæ¶¨ÒåÈôĞèÒª¸Ä¶¯£¬ÔòĞèÒªÓëhead.s µÈÎÄ¼şÖĞµÄÏà¹ØĞÅÏ¢Ò»Æğ¸Ä±ä */
// linux 0.11 ÄÚºËÄ¬ÈÏÖ§³ÖµÄ×î´óÄÚ´æÈİÁ¿ÊÇ16M£¬¿ÉÒÔĞŞ¸ÄÕâĞ©¶¨ÒåÒÔÊÊºÏ¸ü¶àµÄÄÚ´æ¡£
#define LOW_MEM 0x100000	// ÄÚ´æµÍ¶Ë£¨1MB£©¡£
#define PAGING_MEMORY (15*1024*1024)	// ·ÖÒ³ÄÚ´æ15MB¡£Ö÷ÄÚ´æÇø×î¶à15M¡£
#define PAGING_PAGES (PAGING_MEMORY>>12)	// ·ÖÒ³ºóµÄÎïÀíÄÚ´æÒ³Êı¡£
#define MAP_NR(addr) (((addr)-LOW_MEM)>>12)	// Ö¸¶¨ÄÚ´æµØÖ·Ó³ÉäÎªÒ³ºÅ¡£
#define USED 100		// Ò³Ãæ±»Õ¼ÓÃ±êÖ¾£¬²Î¼û405 ĞĞ¡£

// CODE_SPACE(addr) ((((addr)+0xfff)&~0xfff) < current->start_code + current->end_code)¡£
// ¸ÃºêÓÃÓÚÅĞ¶Ï¸ø¶¨µØÖ·ÊÇ·ñÎ»ÓÚµ±Ç°½ø³ÌµÄ´úÂë¶ÎÖĞ£¬²Î¼û252 ĞĞ¡£
#define CODE_SPACE(addr) ((((addr)+4095)&~4095) < \
current->start_code + current->end_code)

static long HIGH_MEMORY = 0;	// È«¾Ö±äÁ¿£¬´æ·ÅÊµ¼ÊÎïÀíÄÚ´æ×î¸ß¶ËµØÖ·¡£

// ¸´ÖÆ1 Ò³ÄÚ´æ£¨4K ×Ö½Ú£©¡£
#define copy_page(from,to) \
__asm__( "cld ; rep ; movsl":: "S" (from), "D" (to), "c" (1024): "cx", "di", "si")

// ÄÚ´æÓ³Éä×Ö½ÚÍ¼(1 ×Ö½Ú´ú±í1 Ò³ÄÚ´æ)£¬Ã¿¸öÒ³Ãæ¶ÔÓ¦µÄ×Ö½ÚÓÃÓÚ±êÖ¾Ò³Ãæµ±Ç°±»ÒıÓÃ£¨Õ¼ÓÃ£©´ÎÊı¡£
static unsigned char mem_map[PAGING_PAGES] = { 0, };

/*
* Get physical address of first (actually last :-) free page, and mark it
* used. If no free pages left, return 0.
*/
/*
* »ñÈ¡Ê×¸ö(Êµ¼ÊÉÏÊÇ×îºó1 ¸ö:-)¿ÕÏĞÒ³Ãæ£¬²¢±ê¼ÇÎªÒÑÊ¹ÓÃ¡£Èç¹ûÃ»ÓĞ¿ÕÏĞÒ³Ãæ£¬
* ¾Í·µ»Ø0¡£
*/
//// È¡¿ÕÏĞÒ³Ãæ¡£Èç¹ûÒÑ¾­Ã»ÓĞ¿ÉÓÃÄÚ´æÁË£¬Ôò·µ»Ø0¡£
// ÊäÈë£º%1(ax=0) - 0£»%2(LOW_MEM)£»%3(cx=PAGING PAGES)£»%4(edi=mem_map+PAGING_PAGES-1)¡£
// Êä³ö£º·µ»Ø%0(ax=Ò³ÃæÆğÊ¼µØÖ·)¡£
// ÉÏÃæ%4 ¼Ä´æÆ÷Êµ¼ÊÖ¸Ïòmem_map[]ÄÚ´æ×Ö½ÚÍ¼µÄ×îºóÒ»¸ö×Ö½Ú¡£±¾º¯Êı´Ó×Ö½ÚÍ¼Ä©¶Ë¿ªÊ¼ÏòÇ°É¨Ãè
// ËùÓĞÒ³Ãæ±êÖ¾£¨Ò³Ãæ×ÜÊıÎªPAGING_PAGES£©£¬ÈôÓĞÒ³Ãæ¿ÕÏĞ£¨ÆäÄÚ´æÓ³Ïñ×Ö½ÚÎª0£©Ôò·µ»ØÒ³ÃæµØÖ·¡£
// ×¢Òâ£¡±¾º¯ÊıÖ»ÊÇÖ¸³öÔÚÖ÷ÄÚ´æÇøµÄÒ»Ò³¿ÕÏĞÒ³Ãæ£¬µ«²¢Ã»ÓĞÓ³Éäµ½Ä³¸ö½ø³ÌµÄÏßĞÔµØÖ·È¥¡£ºóÃæ
// µÄput_page()º¯Êı¾ÍÊÇÓÃÀ´×÷Ó³ÉäµÄ¡£
unsigned long
get_free_page (void)
{
  register unsigned long __res asm ("ax");

  __asm__ ("std ; repne ; scasb\n\t"	// ·½ÏòÎ»ÖÃÎ»£¬½«al(0)Óë¶ÔÓ¦Ã¿¸öÒ³ÃæµÄ(di)ÄÚÈİ±È½Ï£¬
	   "jne 1f\n\t"		// Èç¹ûÃ»ÓĞµÈÓÚ0 µÄ×Ö½Ú£¬ÔòÌø×ª½áÊø£¨·µ»Ø0£©¡£
	   "movb $1,1(%%edi)\n\t"	// ½«¶ÔÓ¦Ò³ÃæµÄÄÚ´æÓ³ÏñÎ»ÖÃ1¡£
	   "sall $12,%%ecx\n\t"	// Ò³ÃæÊı*4K = Ïà¶ÔÒ³ÃæÆğÊ¼µØÖ·¡£
	   "addl %2,%%ecx\n\t"	// ÔÙ¼ÓÉÏµÍ¶ËÄÚ´æµØÖ·£¬¼´»ñµÃÒ³ÃæÊµ¼ÊÎïÀíÆğÊ¼µØÖ·¡£
	   "movl %%ecx,%%edx\n\t"	// ½«Ò³ÃæÊµ¼ÊÆğÊ¼µØÖ·??edx ¼Ä´æÆ÷¡£
	   "movl $1024,%%ecx\n\t"	// ¼Ä´æÆ÷ecx ÖÃ¼ÆÊıÖµ1024¡£
	   "leal 4092(%%edx),%%edi\n\t"	// ½«4092+edx µÄÎ»ÖÃ??edi(¸ÃÒ³ÃæµÄÄ©¶Ë)¡£
	   "rep ; stosl\n\t"	// ½«edi ËùÖ¸ÄÚ´æÇåÁã£¨·´·½Ïò£¬Ò²¼´½«¸ÃÒ³ÃæÇåÁã£©¡£
	   "movl %%edx,%%eax\n"	// ½«Ò³ÃæÆğÊ¼µØÖ·??eax£¨·µ»ØÖµ£©¡£
"1:": "=a" (__res): "" (0), "i" (LOW_MEM), "c" (PAGING_PAGES), "D" (mem_map + PAGING_PAGES - 1):"di", "cx",
	   "dx");
  return __res;			// ·µ»Ø¿ÕÏĞÒ³ÃæµØÖ·£¨Èç¹ûÎŞ¿ÕÏĞÒ²Ôò·µ»Ø0£©¡£
}

/*
* Free a page of memory at physical address 'addr'. Used by
* 'free_page_tables()'
*/
/*
* ÊÍ·ÅÎïÀíµØÖ·'addr'¿ªÊ¼µÄÒ»Ò³ÄÚ´æ¡£ÓÃÓÚº¯Êı'free_page_tables()'¡£
*/
//// ÊÍ·ÅÎïÀíµØÖ·addr ¿ªÊ¼µÄÒ»Ò³ÃæÄÚ´æ¡£
// 1MB ÒÔÏÂµÄÄÚ´æ¿Õ¼äÓÃÓÚÄÚºË³ÌĞòºÍ»º³å£¬²»×÷Îª·ÖÅäÒ³ÃæµÄÄÚ´æ¿Õ¼ä¡£
void
free_page (unsigned long addr)
{
  if (addr < LOW_MEM)
    return;			// Èç¹ûÎïÀíµØÖ·addr Ğ¡ÓÚÄÚ´æµÍ¶Ë£¨1MB£©£¬Ôò·µ»Ø¡£
  if (addr >= HIGH_MEMORY)	// Èç¹ûÎïÀíµØÖ·addr>=ÄÚ´æ×î¸ß¶Ë£¬ÔòÏÔÊ¾³ö´íĞÅÏ¢¡£
    panic ("trying to free nonexistent page");
  addr -= LOW_MEM;		// ÎïÀíµØÖ·¼õÈ¥µÍ¶ËÄÚ´æÎ»ÖÃ£¬ÔÙ³ıÒÔ4KB£¬µÃÒ³ÃæºÅ¡£
  addr >>= 12;
  if (mem_map[addr]--)
    return;			// Èç¹û¶ÔÓ¦ÄÚ´æÒ³ÃæÓ³Éä×Ö½Ú²»µÈÓÚ0£¬Ôò¼õ1 ·µ»Ø¡£
  mem_map[addr] = 0;		// ·ñÔòÖÃ¶ÔÓ¦Ò³ÃæÓ³Éä×Ö½ÚÎª0£¬²¢ÏÔÊ¾³ö´íĞÅÏ¢£¬ËÀ»ú¡£
  panic ("trying to free free page");
}

/*
* This function frees a continuos block of page tables, as needed
* by 'exit()'. As does copy_page_tables(), this handles only 4Mb blocks.
*/
/*
* ÏÂÃæº¯ÊıÊÍ·ÅÒ³±íÁ¬ĞøµÄÄÚ´æ¿é£¬'exit()'ĞèÒª¸Ãº¯Êı¡£Óëcopy_page_tables()
* ÀàËÆ£¬¸Ãº¯Êı½ö´¦Àí4Mb µÄÄÚ´æ¿é¡£
*/
//// ¸ù¾İÖ¸¶¨µÄÏßĞÔµØÖ·ºÍÏŞ³¤£¨Ò³±í¸öÊı£©£¬ÊÍ·Å¶ÔÓ¦ÄÚ´æÒ³±íËùÖ¸¶¨µÄÄÚ´æ¿é²¢ÖÃ±íÏî¿ÕÏĞ¡£
// Ò³Ä¿Â¼Î»ÓÚÎïÀíµØÖ·0 ¿ªÊ¼´¦£¬¹²1024 Ïî£¬Õ¼4K ×Ö½Ú¡£Ã¿¸öÄ¿Â¼ÏîÖ¸¶¨Ò»¸öÒ³±í¡£
// Ò³±í´ÓÎïÀíµØÖ·0x1000 ´¦¿ªÊ¼£¨½ô½Ó×ÅÄ¿Â¼¿Õ¼ä£©£¬Ã¿¸öÒ³±íÓĞ1024 Ïî£¬Ò²Õ¼4K ÄÚ´æ¡£
// Ã¿¸öÒ³±íÏî¶ÔÓ¦Ò»Ò³ÎïÀíÄÚ´æ£¨4K£©¡£Ä¿Â¼ÏîºÍÒ³±íÏîµÄ´óĞ¡¾ùÎª4 ¸ö×Ö½Ú¡£
// ²ÎÊı£ºfrom - ÆğÊ¼»ùµØÖ·£»size - ÊÍ·ÅµÄ³¤¶È¡£
int
free_page_tables (unsigned long from, unsigned long size)
{
  unsigned long *pg_table;
  unsigned long *dir, nr;

  if (from & 0x3fffff)		// ÒªÊÍ·ÅÄÚ´æ¿éµÄµØÖ·ĞèÒÔ4M Îª±ß½ç¡£
    panic ("free_page_tables called with wrong alignment");
  if (!from)			// ³ö´í£¬ÊÔÍ¼ÊÍ·ÅÄÚºËºÍ»º³åËùÕ¼¿Õ¼ä¡£
    panic ("Trying to free up swapper memory space");
// ¼ÆËãËùÕ¼Ò³Ä¿Â¼ÏîÊı(4M µÄ½øÎ»ÕûÊı±¶)£¬Ò²¼´ËùÕ¼Ò³±íÊı¡£
  size = (size + 0x3fffff) >> 22;
// ÏÂÃæÒ»¾ä¼ÆËãÆğÊ¼Ä¿Â¼Ïî¡£¶ÔÓ¦µÄÄ¿Â¼ÏîºÅ=from>>22£¬ÒòÃ¿ÏîÕ¼4 ×Ö½Ú£¬²¢ÇÒÓÉÓÚÒ³Ä¿Â¼ÊÇ´Ó
// ÎïÀíµØÖ·0 ¿ªÊ¼£¬Òò´ËÊµ¼ÊµÄÄ¿Â¼ÏîÖ¸Õë=Ä¿Â¼ÏîºÅ<<2£¬Ò²¼´(from>>20)¡£ÓëÉÏ0xffc È·±£
// Ä¿Â¼ÏîÖ¸Õë·¶Î§ÓĞĞ§¡£
  dir = (unsigned long *) ((from >> 20) & 0xffc);	/* _pg_dir = 0 */
  for (; size-- > 0; dir++)
    {				// size ÏÖÔÚÊÇĞèÒª±»ÊÍ·ÅÄÚ´æµÄÄ¿Â¼ÏîÊı¡£
      if (!(1 & *dir))		// Èç¹û¸ÃÄ¿Â¼ÏîÎŞĞ§(P Î»=0)£¬Ôò¼ÌĞø¡£
	continue;		// Ä¿Â¼ÏîµÄÎ»0(P Î»)±íÊ¾¶ÔÓ¦Ò³±íÊÇ·ñ´æÔÚ¡£
      pg_table = (unsigned long *) (0xfffff000 & *dir);	// È¡Ä¿Â¼ÏîÖĞÒ³±íµØÖ·¡£
      for (nr = 0; nr < 1024; nr++)
	{			// Ã¿¸öÒ³±íÓĞ1024 ¸öÒ³Ïî¡£
	  if (1 & *pg_table)	// Èô¸ÃÒ³±íÏîÓĞĞ§(P Î»=1)£¬ÔòÊÍ·Å¶ÔÓ¦ÄÚ´æÒ³¡£
	    free_page (0xfffff000 & *pg_table);
	  *pg_table = 0;	// ¸ÃÒ³±íÏîÄÚÈİÇåÁã¡£
	  pg_table++;		// Ö¸ÏòÒ³±íÖĞÏÂÒ»Ïî¡£
	}
      free_page (0xfffff000 & *dir);	// ÊÍ·Å¸ÃÒ³±íËùÕ¼ÄÚ´æÒ³Ãæ¡£µ«ÓÉÓÚÒ³±íÔÚ
// ÎïÀíµØÖ·1M ÒÔÄÚ£¬ËùÒÔÕâ¾äÊ²Ã´¶¼²»×ö¡£
      *dir = 0;			// ¶ÔÏàÓ¦Ò³±íµÄÄ¿Â¼ÏîÇåÁã¡£
    }
  invalidate ();		// Ë¢ĞÂÒ³±ä»»¸ßËÙ»º³å¡£
  return 0;
}

/*
* Well, here is one of the most complicated functions in mm. It
* copies a range of linerar addresses by copying only the pages.
* Let's hope this is bug-free, 'cause this one I don't want to debug :-)
*
* Note! We don't copy just any chunks of memory - addresses have to
* be divisible by 4Mb (one page-directory entry), as this makes the
* function easier. It's used only by fork anyway.
*
* NOTE 2!! When from==0 we are copying kernel space for the first
* fork(). Then we DONT want to copy a full page-directory entry, as
* that would lead to some serious memory waste - we just copy the
* first 160 pages - 640kB. Even that is more than we need, but it
* doesn't take any more memory - we don't copy-on-write in the low
* 1 Mb-range, so the pages can be shared with the kernel. Thus the
* special case for nr=xxxx.
*/
/*
* ºÃÁË£¬ÏÂÃæÊÇÄÚ´æ¹ÜÀímm ÖĞ×îÎª¸´ÔÓµÄ³ÌĞòÖ®Ò»¡£ËüÍ¨¹ıÖ»¸´ÖÆÄÚ´æÒ³Ãæ
* À´¿½±´Ò»¶¨·¶Î§ÄÚÏßĞÔµØÖ·ÖĞµÄÄÚÈİ¡£Ï£Íû´úÂëÖĞÃ»ÓĞ´íÎó£¬ÒòÎªÎÒ²»Ïë
* ÔÙµ÷ÊÔÕâ¿é´úÂëÁË?¡£
*
* ×¢Òâ£¡ÎÒÃÇ²¢²»ÊÇ½ö¸´ÖÆÈÎºÎÄÚ´æ¿é - ÄÚ´æ¿éµÄµØÖ·ĞèÒªÊÇ4Mb µÄ±¶Êı£¨ÕıºÃ
* Ò»¸öÒ³Ä¿Â¼Ïî¶ÔÓ¦µÄÄÚ´æ´óĞ¡£©£¬ÒòÎªÕâÑù´¦Àí¿ÉÊ¹º¯ÊıºÜ¼òµ¥¡£²»¹ÜÔõÑù£¬
* Ëü½ö±»fork()Ê¹ÓÃ£¨fork.c µÚ56 ĞĞ£©¡£
*
* ×¢Òâ2£¡£¡µ±from==0 Ê±£¬ÊÇÔÚÎªµÚÒ»´Îfork()µ÷ÓÃ¸´ÖÆÄÚºË¿Õ¼ä¡£´ËÊ±ÎÒÃÇ
* ²»Ïë¸´ÖÆÕû¸öÒ³Ä¿Â¼Ïî¶ÔÓ¦µÄÄÚ´æ£¬ÒòÎªÕâÑù×ö»áµ¼ÖÂÄÚ´æÑÏÖØµÄÀË·Ñ - ÎÒÃÇ
* Ö»¸´ÖÆÍ·160 ¸öÒ³Ãæ - ¶ÔÓ¦640kB¡£¼´Ê¹ÊÇ¸´ÖÆÕâĞ©Ò³ÃæÒ²ÒÑ¾­³¬³öÎÒÃÇµÄĞèÇó£¬
* µ«Õâ²»»áÕ¼ÓÃ¸ü¶àµÄÄÚ´æ - ÔÚµÍ1Mb ÄÚ´æ·¶Î§ÄÚÎÒÃÇ²»Ö´ĞĞĞ´Ê±¸´ÖÆ²Ù×÷£¬ËùÒÔ
* ÕâĞ©Ò³Ãæ¿ÉÒÔÓëÄÚºË¹²Ïí¡£Òò´ËÕâÊÇnr=xxxx µÄÌØÊâÇé¿ö£¨nr ÔÚ³ÌĞòÖĞÖ¸Ò³ÃæÊı£©¡£
*/
//// ¸´ÖÆÖ¸¶¨ÏßĞÔµØÖ·ºÍ³¤¶È£¨Ò³±í¸öÊı£©ÄÚ´æ¶ÔÓ¦µÄÒ³Ä¿Â¼ÏîºÍÒ³±í£¬´Ó¶ø±»¸´ÖÆµÄÒ³Ä¿Â¼ºÍ
//// Ò³±í¶ÔÓ¦µÄÔ­ÎïÀíÄÚ´æÇø±»¹²ÏíÊ¹ÓÃ¡£
// ¸´ÖÆÖ¸¶¨µØÖ·ºÍ³¤¶ÈµÄÄÚ´æ¶ÔÓ¦µÄÒ³Ä¿Â¼ÏîºÍÒ³±íÏî¡£ĞèÉêÇëÒ³ÃæÀ´´æ·ÅĞÂÒ³±í£¬Ô­ÄÚ´æÇø±»¹²Ïí£»
// ´ËºóÁ½¸ö½ø³Ì½«¹²ÏíÄÚ´æÇø£¬Ö±µ½ÓĞÒ»¸ö½ø³ÌÖ´ĞĞĞ´²Ù×÷Ê±£¬²Å·ÖÅäĞÂµÄÄÚ´æÒ³£¨Ğ´Ê±¸´ÖÆ»úÖÆ£©¡£
int
copy_page_tables (unsigned long from, unsigned long to, long size)
{
  unsigned long *from_page_table;
  unsigned long *to_page_table;
  unsigned long this_page;
  unsigned long *from_dir, *to_dir;
  unsigned long nr;

// Ô´µØÖ·ºÍÄ¿µÄµØÖ·¶¼ĞèÒªÊÇÔÚ4Mb µÄÄÚ´æ±ß½çµØÖ·ÉÏ¡£·ñÔò³ö´í£¬ËÀ»ú¡£
  if ((from & 0x3fffff) || (to & 0x3fffff))
    panic ("copy_page_tables called with wrong alignment");
// È¡µÃÔ´µØÖ·ºÍÄ¿µÄµØÖ·µÄÄ¿Â¼Ïî(from_dir ºÍto_dir)¡£²Î¼û¶Ô115 ¾äµÄ×¢ÊÍ¡£
  from_dir = (unsigned long *) ((from >> 20) & 0xffc);	/* _pg_dir = 0 */
  to_dir = (unsigned long *) ((to >> 20) & 0xffc);
// ¼ÆËãÒª¸´ÖÆµÄÄÚ´æ¿éÕ¼ÓÃµÄÒ³±íÊı£¨Ò²¼´Ä¿Â¼ÏîÊı£©¡£
  size = ((unsigned) (size + 0x3fffff)) >> 22;
// ÏÂÃæ¿ªÊ¼¶ÔÃ¿¸öÕ¼ÓÃµÄÒ³±íÒÀ´Î½øĞĞ¸´ÖÆ²Ù×÷¡£
  for (; size-- > 0; from_dir++, to_dir++)
    {
// Èç¹ûÄ¿µÄÄ¿Â¼ÏîÖ¸¶¨µÄÒ³±íÒÑ¾­´æÔÚ(P=1)£¬Ôò³ö´í£¬ËÀ»ú¡£
      if (1 & *to_dir)
	panic ("copy_page_tables: already exist");
// Èç¹û´ËÔ´Ä¿Â¼ÏîÎ´±»Ê¹ÓÃ£¬Ôò²»ÓÃ¸´ÖÆ¶ÔÓ¦Ò³±í£¬Ìø¹ı¡£
      if (!(1 & *from_dir))
	continue;
// È¡µ±Ç°Ô´Ä¿Â¼ÏîÖĞÒ³±íµÄµØÖ·??from_page_table¡£
      from_page_table = (unsigned long *) (0xfffff000 & *from_dir);
// ÎªÄ¿µÄÒ³±íÈ¡Ò»Ò³¿ÕÏĞÄÚ´æ£¬Èç¹û·µ»ØÊÇ0 ÔòËµÃ÷Ã»ÓĞÉêÇëµ½¿ÕÏĞÄÚ´æÒ³Ãæ¡£·µ»ØÖµ=-1£¬ÍË³ö¡£
      if (!(to_page_table = (unsigned long *) get_free_page ()))
	return -1;		/* Out of memory, see freeing */
// ÉèÖÃÄ¿µÄÄ¿Â¼ÏîĞÅÏ¢¡£7 ÊÇ±êÖ¾ĞÅÏ¢£¬±íÊ¾(Usr, R/W, Present)¡£
      *to_dir = ((unsigned long) to_page_table) | 7;
// Õë¶Ôµ±Ç°´¦ÀíµÄÒ³±í£¬ÉèÖÃĞè¸´ÖÆµÄÒ³ÃæÊı¡£Èç¹ûÊÇÔÚÄÚºË¿Õ¼ä£¬Ôò½öĞè¸´ÖÆÍ·160 Ò³£¬·ñÔòĞèÒª
// ¸´ÖÆ1 ¸öÒ³±íÖĞµÄËùÓĞ1024 Ò³Ãæ¡£
      nr = (from == 0) ? 0xA0 : 1024;
// ¶ÔÓÚµ±Ç°Ò³±í£¬¿ªÊ¼¸´ÖÆÖ¸¶¨ÊıÄ¿nr ¸öÄÚ´æÒ³Ãæ¡£
      for (; nr-- > 0; from_page_table++, to_page_table++)
	{
	  this_page = *from_page_table;	// È¡Ô´Ò³±íÏîÄÚÈİ¡£
	  if (!(1 & this_page))	// Èç¹ûµ±Ç°Ô´Ò³ÃæÃ»ÓĞÊ¹ÓÃ£¬Ôò²»ÓÃ¸´ÖÆ¡£
	    continue;
// ¸´Î»Ò³±íÏîÖĞR/W ±êÖ¾(ÖÃ0)¡£(Èç¹ûU/S Î»ÊÇ0£¬ÔòR/W ¾ÍÃ»ÓĞ×÷ÓÃ¡£Èç¹ûU/S ÊÇ1£¬¶øR/W ÊÇ0£¬
// ÄÇÃ´ÔËĞĞÔÚÓÃ»§²ãµÄ´úÂë¾ÍÖ»ÄÜ¶ÁÒ³Ãæ¡£Èç¹ûU/S ºÍR/W ¶¼ÖÃÎ»£¬Ôò¾ÍÓĞĞ´µÄÈ¨ÏŞ¡£)
	  this_page &= ~2;
	  *to_page_table = this_page;	// ½«¸ÃÒ³±íÏî¸´ÖÆµ½Ä¿µÄÒ³±íÖĞ¡£
// Èç¹û¸ÃÒ³±íÏîËùÖ¸Ò³ÃæµÄµØÖ·ÔÚ1M ÒÔÉÏ£¬ÔòĞèÒªÉèÖÃÄÚ´æÒ³ÃæÓ³ÉäÊı×émem_map[]£¬ÓÚÊÇ¼ÆËã
// Ò³ÃæºÅ£¬²¢ÒÔËüÎªË÷ÒıÔÚÒ³ÃæÓ³ÉäÊı×éÏàÓ¦ÏîÖĞÔö¼ÓÒıÓÃ´ÎÊı¡£
	  if (this_page > LOW_MEM)
	    {
// ÏÂÃæÕâ¾äµÄº¬ÒåÊÇÁîÔ´Ò³±íÏîËùÖ¸ÄÚ´æÒ³Ò²ÎªÖ»¶Á¡£ÒòÎªÏÖÔÚ¿ªÊ¼ÓĞÁ½¸ö½ø³Ì¹²ÓÃÄÚ´æÇøÁË¡£
// ÈôÆäÖĞÒ»¸öÄÚ´æĞèÒª½øĞĞĞ´²Ù×÷£¬Ôò¿ÉÒÔÍ¨¹ıÒ³Òì³£µÄĞ´±£»¤´¦Àí£¬ÎªÖ´ĞĞĞ´²Ù×÷µÄ½ø³Ì·ÖÅä
// Ò»Ò³ĞÂµÄ¿ÕÏĞÒ³Ãæ£¬Ò²¼´½øĞĞĞ´Ê±¸´ÖÆµÄ²Ù×÷¡£
	      *from_page_table = this_page;	// ÁîÔ´Ò³±íÏîÒ²Ö»¶Á¡£
	      this_page -= LOW_MEM;
	      this_page >>= 12;
	      mem_map[this_page]++;
	    }
	}
    }
  invalidate ();		// Ë¢ĞÂÒ³±ä»»¸ßËÙ»º³å¡£
  return 0;
}

/*
* This function puts a page in memory at the wanted address.
* It returns the physical address of the page gotten, 0 if
* out of memory (either when trying to access page-table or
* page.)
*/
/*
* ÏÂÃæº¯Êı½«Ò»ÄÚ´æÒ³Ãæ·ÅÖÃÔÚÖ¸¶¨µØÖ·´¦¡£Ëü·µ»ØÒ³ÃæµÄÎïÀíµØÖ·£¬Èç¹û
* ÄÚ´æ²»¹»(ÔÚ·ÃÎÊÒ³±í»òÒ³ÃæÊ±)£¬Ôò·µ»Ø0¡£
*/
//// °ÑÒ»ÎïÀíÄÚ´æÒ³ÃæÓ³Éäµ½Ö¸¶¨µÄÏßĞÔµØÖ·´¦¡£
// Ö÷Òª¹¤×÷ÊÇÔÚÒ³Ä¿Â¼ºÍÒ³±íÖĞÉèÖÃÖ¸¶¨Ò³ÃæµÄĞÅÏ¢¡£Èô³É¹¦Ôò·µ»ØÒ³ÃæµØÖ·¡£
unsigned long
put_page (unsigned long page, unsigned long address)
{
  unsigned long tmp, *page_table;

/* NOTE !!! This uses the fact that _pg_dir=0 */
/* ×¢Òâ!!!ÕâÀïÊ¹ÓÃÁËÒ³Ä¿Â¼»ùÖ·_pg_dir=0 µÄÌõ¼ş */

// Èç¹ûÉêÇëµÄÒ³ÃæÎ»ÖÃµÍÓÚLOW_MEM(1Mb)»ò³¬³öÏµÍ³Êµ¼Êº¬ÓĞÄÚ´æ¸ß¶ËHIGH_MEMORY£¬Ôò·¢³ö¾¯¸æ¡£
  if (page < LOW_MEM || page >= HIGH_MEMORY)
    printk ("Trying to put page %p at %p\n", page, address);
// Èç¹ûÉêÇëµÄÒ³ÃæÔÚÄÚ´æÒ³ÃæÓ³Éä×Ö½ÚÍ¼ÖĞÃ»ÓĞÖÃÎ»£¬ÔòÏÔÊ¾¾¯¸æĞÅÏ¢¡£
  if (mem_map[(page - LOW_MEM) >> 12] != 1)
    printk ("mem_map disagrees with %p at %p\n", page, address);
// ¼ÆËãÖ¸¶¨µØÖ·ÔÚÒ³Ä¿Â¼±íÖĞ¶ÔÓ¦µÄÄ¿Â¼ÏîÖ¸Õë¡£
  page_table = (unsigned long *) ((address >> 20) & 0xffc);
// Èç¹û¸ÃÄ¿Â¼ÏîÓĞĞ§(P=1)(Ò²¼´Ö¸¶¨µÄÒ³±íÔÚÄÚ´æÖĞ)£¬Ôò´ÓÖĞÈ¡µÃÖ¸¶¨Ò³±íµÄµØÖ·??page_table¡£
  if ((*page_table) & 1)
    page_table = (unsigned long *) (0xfffff000 & *page_table);
  else
    {
// ·ñÔò£¬ÉêÇë¿ÕÏĞÒ³Ãæ¸øÒ³±íÊ¹ÓÃ£¬²¢ÔÚ¶ÔÓ¦Ä¿Â¼ÏîÖĞÖÃÏàÓ¦±êÖ¾7£¨User, U/S, R/W£©¡£È»ºó½«
// ¸ÃÒ³±íµÄµØÖ·??page_table¡£
      if (!(tmp = get_free_page ()))
	return 0;
      *page_table = tmp | 7;
      page_table = (unsigned long *) tmp;
    }
// ÔÚÒ³±íÖĞÉèÖÃÖ¸¶¨µØÖ·µÄÎïÀíÄÚ´æÒ³ÃæµÄÒ³±íÏîÄÚÈİ¡£Ã¿¸öÒ³±í¹²¿ÉÓĞ1024 Ïî(0x3ff)¡£
  page_table[(address >> 12) & 0x3ff] = page | 7;
/* no need for invalidate */
/* ²»ĞèÒªË¢ĞÂÒ³±ä»»¸ßËÙ»º³å */
  return page;			// ·µ»ØÒ³ÃæµØÖ·¡£
}

//// È¡ÏûĞ´±£»¤Ò³Ãæº¯Êı¡£ÓÃÓÚÒ³Òì³£ÖĞ¶Ï¹ı³ÌÖĞĞ´±£»¤Òì³£µÄ´¦Àí£¨Ğ´Ê±¸´ÖÆ£©¡£
// ÊäÈë²ÎÊıÎªÒ³±íÏîÖ¸Õë¡£
// [ un_wp_page ÒâË¼ÊÇÈ¡ÏûÒ³ÃæµÄĞ´±£»¤£ºUn-Write Protected¡£]
void
un_wp_page (unsigned long *table_entry)
{
  unsigned long old_page, new_page;

  old_page = 0xfffff000 & *table_entry;	// È¡Ô­Ò³Ãæ¶ÔÓ¦µÄÄ¿Â¼ÏîºÅ¡£
// Èç¹ûÔ­Ò³ÃæµØÖ·´óÓÚÄÚ´æµÍ¶ËLOW_MEM(1Mb)£¬²¢ÇÒÆäÔÚÒ³ÃæÓ³Éä×Ö½ÚÍ¼Êı×éÖĞÖµÎª1£¨±íÊ¾½ö
// ±»ÒıÓÃ1 ´Î£¬Ò³ÃæÃ»ÓĞ±»¹²Ïí£©£¬ÔòÔÚ¸ÃÒ³ÃæµÄÒ³±íÏîÖĞÖÃR/W ±êÖ¾£¨¿ÉĞ´£©£¬²¢Ë¢ĞÂÒ³±ä»»
// ¸ßËÙ»º³å£¬È»ºó·µ»Ø¡£
  if (old_page >= LOW_MEM && mem_map[MAP_NR (old_page)] == 1)
    {
      *table_entry |= 2;
      invalidate ();
      return;
    }
// ·ñÔò£¬ÔÚÖ÷ÄÚ´æÇøÄÚÉêÇëÒ»Ò³¿ÕÏĞÒ³Ãæ¡£
  if (!(new_page = get_free_page ()))
    oom ();			// Out of Memory¡£ÄÚ´æ²»¹»´¦Àí¡£
// Èç¹ûÔ­Ò³Ãæ´óÓÚÄÚ´æµÍ¶Ë£¨ÔòÒâÎ¶×Åmem_map[]>1£¬Ò³ÃæÊÇ¹²ÏíµÄ£©£¬Ôò½«Ô­Ò³ÃæµÄÒ³ÃæÓ³Éä
// Êı×éÖµµİ¼õ1¡£È»ºó½«Ö¸¶¨Ò³±íÏîÄÚÈİ¸üĞÂÎªĞÂÒ³ÃæµÄµØÖ·£¬²¢ÖÃ¿É¶ÁĞ´µÈ±êÖ¾(U/S, R/W, P)¡£
// Ë¢ĞÂÒ³±ä»»¸ßËÙ»º³å¡£×îºó½«Ô­Ò³ÃæÄÚÈİ¸´ÖÆµ½ĞÂÒ³Ãæ¡£
  if (old_page >= LOW_MEM)
    mem_map[MAP_NR (old_page)]--;
  *table_entry = new_page | 7;
  invalidate ();
  copy_page (old_page, new_page);
}

/*
* This routine handles present pages, when users try to write
* to a shared page. It is done by copying the page to a new address
* and decrementing the shared-page counter for the old page.
*
* If it's in code space we exit with a segment error.
*/
/*
* µ±ÓÃ»§ÊÔÍ¼ÍùÒ»¸ö¹²ÏíÒ³ÃæÉÏĞ´Ê±£¬¸Ãº¯Êı´¦ÀíÒÑ´æÔÚµÄÄÚ´æÒ³Ãæ£¬£¨Ğ´Ê±¸´ÖÆ£©
* ËüÊÇÍ¨¹ı½«Ò³Ãæ¸´ÖÆµ½Ò»¸öĞÂµØÖ·ÉÏ²¢µİ¼õÔ­Ò³ÃæµÄ¹²ÏíÒ³Ãæ¼ÆÊıÖµÊµÏÖµÄ¡£
*
* Èç¹ûËüÔÚ´úÂë¿Õ¼ä£¬ÎÒÃÇ¾ÍÒÔ¶Î´íÎóĞÅÏ¢ÍË³ö¡£
*/
//// Ò³Òì³£ÖĞ¶Ï´¦Àíµ÷ÓÃµÄC º¯Êı¡£Ğ´¹²ÏíÒ³Ãæ´¦Àíº¯Êı¡£ÔÚpage.s ³ÌĞòÖĞ±»µ÷ÓÃ¡£
// ²ÎÊıerror_code ÊÇÓÉCPU ×Ô¶¯²úÉú£¬address ÊÇÒ³ÃæÏßĞÔµØÖ·¡£
// Ğ´¹²ÏíÒ³ÃæÊ±£¬Ğè¸´ÖÆÒ³Ãæ£¨Ğ´Ê±¸´ÖÆ£©¡£
void
do_wp_page (unsigned long error_code, unsigned long address)
{
#if 0
/* we cannot do this yet: the estdio library writes to code space */
/* stupid, stupid. I really want the libc.a from GNU */
/* ÎÒÃÇÏÖÔÚ»¹²»ÄÜÕâÑù×ö£ºÒòÎªestdio ¿â»áÔÚ´úÂë¿Õ¼äÖ´ĞĞĞ´²Ù×÷ */
/* ÕæÊÇÌ«ÓŞ´ÀÁË¡£ÎÒÕæÏë´ÓGNU µÃµ½libc.a ¿â¡£*/
  if (CODE_SPACE (address))	// Èç¹ûµØÖ·Î»ÓÚ´úÂë¿Õ¼ä£¬ÔòÖÕÖ¹Ö´ĞĞ³ÌĞò¡£
    do_exit (SIGSEGV);
#endif
// ´¦ÀíÈ¡ÏûÒ³Ãæ±£»¤¡£²ÎÊıÖ¸¶¨Ò³ÃæÔÚÒ³±íÖĞµÄÒ³±íÏîÖ¸Õë£¬Æä¼ÆËã·½·¨ÊÇ£º
// ((address>>10) & 0xffc)£º¼ÆËãÖ¸¶¨µØÖ·µÄÒ³ÃæÔÚÒ³±íÖĞµÄÆ«ÒÆµØÖ·£»
// (0xfffff000 &((address>>20) &0xffc))£ºÈ¡Ä¿Â¼ÏîÖĞÒ³±íµÄµØÖ·Öµ£¬
// ÆäÖĞ((address>>20) &0xffc)¼ÆËãÒ³ÃæËùÔÚÒ³±íµÄÄ¿Â¼ÏîÖ¸Õë£»
// Á½ÕßÏà¼Ó¼´µÃÖ¸¶¨µØÖ·¶ÔÓ¦Ò³ÃæµÄÒ³±íÏîÖ¸Õë¡£ÕâÀï¶Ô¹²ÏíµÄÒ³Ãæ½øĞĞ¸´ÖÆ¡£
  un_wp_page ((unsigned long *)
	      (((address >> 10) & 0xffc) + (0xfffff000 &
					    *((unsigned long
					       *) ((address >> 20) &
						   0xffc)))));

}

//// Ğ´Ò³ÃæÑéÖ¤¡£
// ÈôÒ³Ãæ²»¿ÉĞ´£¬Ôò¸´ÖÆÒ³Ãæ¡£ÔÚfork.c µÚ34 ĞĞ±»µ÷ÓÃ¡£
void
write_verify (unsigned long address)
{
  unsigned long page;

// ÅĞ¶ÏÖ¸¶¨µØÖ·Ëù¶ÔÓ¦Ò³Ä¿Â¼ÏîµÄÒ³±íÊÇ·ñ´æÔÚ(P)£¬Èô²»´æÔÚ(P=0)Ôò·µ»Ø¡£
  if (!((page = *((unsigned long *) ((address >> 20) & 0xffc))) & 1))
    return;
// È¡Ò³±íµÄµØÖ·£¬¼ÓÉÏÖ¸¶¨µØÖ·µÄÒ³ÃæÔÚÒ³±íÖĞµÄÒ³±íÏîÆ«ÒÆÖµ£¬µÃ¶ÔÓ¦ÎïÀíÒ³ÃæµÄÒ³±íÏîÖ¸Õë¡£
  page &= 0xfffff000;
  page += ((address >> 10) & 0xffc);
// Èç¹û¸ÃÒ³Ãæ²»¿ÉĞ´(±êÖ¾R/W Ã»ÓĞÖÃÎ»)£¬ÔòÖ´ĞĞ¹²Ïí¼ìÑéºÍ¸´ÖÆÒ³Ãæ²Ù×÷£¨Ğ´Ê±¸´ÖÆ£©¡£
  if ((3 & *(unsigned long *) page) == 1)	/* non-writeable, present */
    un_wp_page ((unsigned long *) page);
  return;
}

//// È¡µÃÒ»Ò³¿ÕÏĞÄÚ´æ²¢Ó³Éäµ½Ö¸¶¨ÏßĞÔµØÖ·´¦¡£
// Óëget_free_page()²»Í¬¡£get_free_page()½öÊÇÉêÇëÈ¡µÃÁËÖ÷ÄÚ´æÇøµÄÒ»Ò³ÎïÀíÄÚ´æ¡£¶ø¸Ãº¯Êı
// ²»½öÊÇ»ñÈ¡µ½Ò»Ò³ÎïÀíÄÚ´æÒ³Ãæ£¬»¹½øÒ»²½µ÷ÓÃput_page()£¬½«ÎïÀíÒ³ÃæÓ³Éäµ½Ö¸¶¨µÄÏßĞÔµØÖ·
// ´¦¡£
void
get_empty_page (unsigned long address)
{
  unsigned long tmp;

// Èô²»ÄÜÈ¡µÃÒ»¿ÕÏĞÒ³Ãæ£¬»òÕß²»ÄÜ½«Ò³Ãæ·ÅÖÃµ½Ö¸¶¨µØÖ·´¦£¬ÔòÏÔÊ¾ÄÚ´æ²»¹»µÄĞÅÏ¢¡£
// 279 ĞĞÉÏÓ¢ÎÄ×¢ÊÍµÄº¬ÒåÊÇ£º¼´Ê¹Ö´ĞĞget_free_page()·µ»Ø0 Ò²ÎŞËùÎ½£¬ÒòÎªput_page()
// ÖĞ»¹»á¶Ô´ËÇé¿öÔÙ´ÎÉêÇë¿ÕÏĞÎïÀíÒ³ÃæµÄ£¬¼û210 ĞĞ¡£
  if (!(tmp = get_free_page ()) || !put_page (tmp, address))
    {
      free_page (tmp);		/* 0 is ok - ignored */
      oom ();
    }
}

/*
* try_to_share() checks the page at address "address" in the task "p",
* to see if it exists, and if it is clean. If so, share it with the current
* task.
*
* NOTE! This assumes we have checked that p != current, and that they
* share the same executable.
*/
/*
* try_to_share()ÔÚÈÎÎñ"p"ÖĞ¼ì²éÎ»ÓÚµØÖ·"address"´¦µÄÒ³Ãæ£¬¿´Ò³ÃæÊÇ·ñ´æÔÚ£¬ÊÇ·ñ¸É¾»¡£
* Èç¹ûÊÇ¸É¾»µÄ»°£¬¾ÍÓëµ±Ç°ÈÎÎñ¹²Ïí¡£
*
* ×¢Òâ£¡ÕâÀïÎÒÃÇÒÑ¼Ù¶¨p !=µ±Ç°ÈÎÎñ£¬²¢ÇÒËüÃÇ¹²ÏíÍ¬Ò»¸öÖ´ĞĞ³ÌĞò¡£
*/
//// ³¢ÊÔ¶Ô½ø³ÌÖ¸¶¨µØÖ·´¦µÄÒ³Ãæ½øĞĞ¹²Ïí²Ù×÷¡£
// Í¬Ê±»¹ÑéÖ¤Ö¸¶¨µÄµØÖ·´¦ÊÇ·ñÒÑ¾­ÉêÇëÁËÒ³Ãæ£¬ÈôÊÇÔò³ö´í£¬ËÀ»ú¡£
// ·µ»Ø1-³É¹¦£¬0-Ê§°Ü¡£
static int
try_to_share (unsigned long address, struct task_struct *p)
{
  unsigned long from;
  unsigned long to;
  unsigned long from_page;
  unsigned long to_page;
  unsigned long phys_addr;

// ÇóÖ¸¶¨ÄÚ´æµØÖ·µÄÒ³Ä¿Â¼Ïî¡£
  from_page = to_page = ((address >> 20) & 0xffc);
// ¼ÆËã½ø³Ìp µÄ´úÂëÆğÊ¼µØÖ·Ëù¶ÔÓ¦µÄÒ³Ä¿Â¼Ïî¡£
  from_page += ((p->start_code >> 20) & 0xffc);
// ¼ÆËãµ±Ç°½ø³ÌÖĞ´úÂëÆğÊ¼µØÖ·Ëù¶ÔÓ¦µÄÒ³Ä¿Â¼Ïî¡£
  to_page += ((current->start_code >> 20) & 0xffc);
/* is there a page-directory at from? */
/* ÔÚfrom ´¦ÊÇ·ñ´æÔÚÒ³Ä¿Â¼£¿*/
// *** ¶Ôp ½ø³ÌÒ³Ãæ½øĞĞ²Ù×÷¡£
// È¡Ò³Ä¿Â¼ÏîÄÚÈİ¡£Èç¹û¸ÃÄ¿Â¼ÏîÎŞĞ§(P=0)£¬Ôò·µ»Ø¡£·ñÔòÈ¡¸ÃÄ¿Â¼Ïî¶ÔÓ¦Ò³±íµØÖ·??from¡£
  from = *(unsigned long *) from_page;
  if (!(from & 1))
    return 0;
  from &= 0xfffff000;
// ¼ÆËãµØÖ·¶ÔÓ¦µÄÒ³±íÏîÖ¸ÕëÖµ£¬²¢È¡³ö¸ÃÒ³±íÏîÄÚÈİ??phys_addr¡£
  from_page = from + ((address >> 10) & 0xffc);
  phys_addr = *(unsigned long *) from_page;
/* is the page clean and present? */
/* Ò³Ãæ¸É¾»²¢ÇÒ´æÔÚÂğ£¿*/
// 0x41 ¶ÔÓ¦Ò³±íÏîÖĞµÄDirty ºÍPresent ±êÖ¾¡£Èç¹ûÒ³Ãæ²»¸É¾»»òÎŞĞ§Ôò·µ»Ø¡£
  if ((phys_addr & 0x41) != 0x01)
    return 0;
// È¡Ò³ÃæµÄµØÖ·??phys_addr¡£Èç¹û¸ÃÒ³ÃæµØÖ·²»´æÔÚ»òĞ¡ÓÚÄÚ´æµÍ¶Ë(1M)Ò²·µ»ØÍË³ö¡£
  phys_addr &= 0xfffff000;
  if (phys_addr >= HIGH_MEMORY || phys_addr < LOW_MEM)
    return 0;
// *** ¶Ôµ±Ç°½ø³ÌÒ³Ãæ½øĞĞ²Ù×÷¡£
// È¡Ò³Ä¿Â¼ÏîÄÚÈİ??to¡£Èç¹û¸ÃÄ¿Â¼ÏîÎŞĞ§(P=0)£¬ÔòÈ¡¿ÕÏĞÒ³Ãæ£¬²¢¸üĞÂto_page ËùÖ¸µÄÄ¿Â¼Ïî¡£
  to = *(unsigned long *) to_page;
  if (!(to & 1))
    if (to = get_free_page ())
      *(unsigned long *) to_page = to | 7;
    else
      oom ();
// È¡¶ÔÓ¦Ò³±íµØÖ·??to£¬Ò³±íÏîµØÖ·??to_page¡£Èç¹û¶ÔÓ¦µÄÒ³ÃæÒÑ¾­´æÔÚ£¬Ôò³ö´í£¬ËÀ»ú¡£
  to &= 0xfffff000;
  to_page = to + ((address >> 10) & 0xffc);
  if (1 & *(unsigned long *) to_page)
    panic ("try_to_share: to_page already exists");
/* share them: write-protect */
/* ¶ÔËüÃÇ½øĞĞ¹²Ïí´¦Àí£ºĞ´±£»¤ */
// ¶Ôp ½ø³ÌÖĞÒ³ÃæÖÃĞ´±£»¤±êÖ¾(ÖÃR/W=0 Ö»¶Á)¡£²¢ÇÒµ±Ç°½ø³ÌÖĞµÄ¶ÔÓ¦Ò³±íÏîÖ¸ÏòËü¡£
  *(unsigned long *) from_page &= ~2;
  *(unsigned long *) to_page = *(unsigned long *) from_page;
// Ë¢ĞÂÒ³±ä»»¸ßËÙ»º³å¡£
  invalidate ();
// ¼ÆËãËù²Ù×÷Ò³ÃæµÄÒ³ÃæºÅ£¬²¢½«¶ÔÓ¦Ò³ÃæÓ³ÉäÊı×éÏîÖĞµÄÒıÓÃµİÔö1¡£
  phys_addr -= LOW_MEM;
  phys_addr >>= 12;
  mem_map[phys_addr]++;
  return 1;
}

/*
* share_page() tries to find a process that could share a page with
* the current one. Address is the address of the wanted page relative
* to the current data space.
*
* We first check if it is at all feasible by checking executable->i_count.
* It should be >1 if there are other tasks sharing this inode.
*/
/*
* share_page()ÊÔÍ¼ÕÒµ½Ò»¸ö½ø³Ì£¬Ëü¿ÉÒÔÓëµ±Ç°½ø³Ì¹²ÏíÒ³Ãæ¡£²ÎÊıaddress ÊÇ
* µ±Ç°Êı¾İ¿Õ¼äÖĞÆÚÍû¹²ÏíµÄÄ³Ò³ÃæµØÖ·¡£
*
* Ê×ÏÈÎÒÃÇÍ¨¹ı¼ì²âexecutable->i_count À´²éÖ¤ÊÇ·ñ¿ÉĞĞ¡£Èç¹ûÓĞÆäËüÈÎÎñÒÑ¹²Ïí
* ¸Ãinode£¬ÔòËüÓ¦¸Ã´óÓÚ1¡£
*/
//// ¹²ÏíÒ³Ãæ¡£ÔÚÈ±Ò³´¦ÀíÊ±¿´¿´ÄÜ·ñ¹²ÏíÒ³Ãæ¡£
// ·µ»Ø1 - ³É¹¦£¬0 - Ê§°Ü¡£
static int
share_page (unsigned long address)
{
  struct task_struct **p;

// Èç¹ûÊÇ²»¿ÉÖ´ĞĞµÄ£¬Ôò·µ»Ø¡£excutable ÊÇÖ´ĞĞ½ø³ÌµÄÄÚ´æi ½Úµã½á¹¹¡£
  if (!current->executable)
    return 0;
// Èç¹ûÖ»ÄÜµ¥¶ÀÖ´ĞĞ(executable->i_count=1)£¬Ò²ÍË³ö¡£
  if (current->executable->i_count < 2)
    return 0;
// ËÑË÷ÈÎÎñÊı×éÖĞËùÓĞÈÎÎñ¡£Ñ°ÕÒÓëµ±Ç°½ø³Ì¿É¹²ÏíÒ³ÃæµÄ½ø³Ì£¬²¢³¢ÊÔ¶ÔÖ¸¶¨µØÖ·µÄÒ³Ãæ½øĞĞ¹²Ïí¡£
  for (p = &LAST_TASK; p > &FIRST_TASK; --p)
    {
      if (!*p)			// Èç¹û¸ÃÈÎÎñÏî¿ÕÏĞ£¬Ôò¼ÌĞøÑ°ÕÒ¡£
	continue;
      if (current == *p)	// Èç¹û¾ÍÊÇµ±Ç°ÈÎÎñ£¬Ò²¼ÌĞøÑ°ÕÒ¡£
	continue;
      if ((*p)->executable != current->executable)	// Èç¹ûexecutable ²»µÈ£¬Ò²¼ÌĞø¡£
	continue;
      if (try_to_share (address, *p))	// ³¢ÊÔ¹²ÏíÒ³Ãæ¡£
	return 1;
    }
  return 0;
}

//// Ò³Òì³£ÖĞ¶Ï´¦Àíµ÷ÓÃµÄº¯Êı¡£´¦ÀíÈ±Ò³Òì³£Çé¿ö¡£ÔÚpage.s ³ÌĞòÖĞ±»µ÷ÓÃ¡£
// ²ÎÊıerror_code ÊÇÓÉCPU ×Ô¶¯²úÉú£¬address ÊÇÒ³ÃæÏßĞÔµØÖ·¡£
void
do_no_page (unsigned long error_code, unsigned long address)
{
  int nr[4];
  unsigned long tmp;
  unsigned long page;
  int block, i;

  address &= 0xfffff000;	// Ò³ÃæµØÖ·¡£
// Ê×ÏÈËã³öÖ¸¶¨ÏßĞÔµØÖ·ÔÚ½ø³Ì¿Õ¼äÖĞÏà¶ÔÓÚ½ø³Ì»ùÖ·µÄÆ«ÒÆ³¤¶ÈÖµ¡£
  tmp = address - current->start_code;
// Èôµ±Ç°½ø³ÌµÄexecutable ¿Õ£¬»òÕßÖ¸¶¨µØÖ·³¬³ö´úÂë+Êı¾İ³¤¶È£¬ÔòÉêÇëÒ»Ò³ÎïÀíÄÚ´æ£¬²¢Ó³Éä
// Ó°Éäµ½Ö¸¶¨µÄÏßĞÔµØÖ·´¦¡£executable ÊÇ½ø³ÌµÄi ½Úµã½á¹¹¡£¸ÃÖµÎª0£¬±íÃ÷½ø³Ì¸Õ¿ªÊ¼ÉèÖÃ£¬
// ĞèÒªÄÚ´æ£»¶øÖ¸¶¨µÄÏßĞÔµØÖ·³¬³ö´úÂë¼ÓÊı¾İ³¤¶È£¬±íÃ÷½ø³ÌÔÚÉêÇëĞÂµÄÄÚ´æ¿Õ¼ä£¬Ò²ĞèÒª¸øÓè¡£
// Òò´Ë¾ÍÖ±½Óµ÷ÓÃget_empty_page()º¯Êı£¬ÉêÇëÒ»Ò³ÎïÀíÄÚ´æ²¢Ó³Éäµ½Ö¸¶¨ÏßĞÔµØÖ·´¦¼´¿É¡£
// start_code ÊÇ½ø³Ì´úÂë¶ÎµØÖ·£¬end_data ÊÇ´úÂë¼ÓÊı¾İ³¤¶È¡£¶ÔÓÚlinux ÄÚºË£¬ËüµÄ´úÂë¶ÎºÍ
// Êı¾İ¶ÎÊÇÆğÊ¼»ùÖ·ÊÇÏàÍ¬µÄ¡£
  if (!current->executable || tmp >= current->end_data)
    {
      get_empty_page (address);
      return;
    }
// Èç¹û³¢ÊÔ¹²ÏíÒ³Ãæ³É¹¦£¬ÔòÍË³ö¡£
  if (share_page (tmp))
    return;
// È¡¿ÕÏĞÒ³Ãæ£¬Èç¹ûÄÚ´æ²»¹»ÁË£¬ÔòÏÔÊ¾ÄÚ´æ²»¹»£¬ÖÕÖ¹½ø³Ì¡£
  if (!(page = get_free_page ()))
    oom ();
/* remember that 1 block is used for header */
/* ¼Ç×¡£¬£¨³ÌĞò£©Í·ÒªÊ¹ÓÃ1 ¸öÊı¾İ¿é */
// Ê×ÏÈ¼ÆËãÈ±Ò³ËùÔÚµÄÊı¾İ¿éÏî¡£BLOCK_SIZE = 1024 ×Ö½Ú£¬Òò´ËÒ»Ò³ÄÚ´æĞèÒª4 ¸öÊı¾İ¿é¡£
  block = 1 + tmp / BLOCK_SIZE;
// ¸ù¾İi ½ÚµãĞÅÏ¢£¬È¡Êı¾İ¿éÔÚÉè±¸ÉÏµÄ¶ÔÓ¦µÄÂß¼­¿éºÅ¡£
  for (i = 0; i < 4; block++, i++)
    nr[i] = bmap (current->executable, block);
// ¶ÁÉè±¸ÉÏÒ»¸öÒ³ÃæµÄÊı¾İ£¨4 ¸öÂß¼­¿é£©µ½Ö¸¶¨ÎïÀíµØÖ·page ´¦¡£
  bread_page (page, current->executable->i_dev, nr);
// ÔÚÔö¼ÓÁËÒ»Ò³ÄÚ´æºó£¬¸ÃÒ³ÄÚ´æµÄ²¿·Ö¿ÉÄÜ»á³¬¹ı½ø³ÌµÄend_data Î»ÖÃ¡£ÏÂÃæµÄÑ­»·¼´ÊÇ¶ÔÎïÀí
// Ò³Ãæ³¬³öµÄ²¿·Ö½øĞĞÇåÁã´¦Àí¡£
  i = tmp + 4096 - current->end_data;
  tmp = page + 4096;
  while (i-- > 0)
    {
      tmp--;
      *(char *) tmp = 0;
    }
// Èç¹û°ÑÎïÀíÒ³ÃæÓ³Éäµ½Ö¸¶¨ÏßĞÔµØÖ·µÄ²Ù×÷³É¹¦£¬¾Í·µ»Ø¡£·ñÔò¾ÍÊÍ·ÅÄÚ´æÒ³£¬ÏÔÊ¾ÄÚ´æ²»¹»¡£
  if (put_page (page, address))
    return;
  free_page (page);
  oom ();
}

//// ÎïÀíÄÚ´æ³õÊ¼»¯¡£
// ²ÎÊı£ºstart_mem - ¿ÉÓÃ×÷·ÖÒ³´¦ÀíµÄÎïÀíÄÚ´æÆğÊ¼Î»ÖÃ£¨ÒÑÈ¥³ıRAMDISK ËùÕ¼ÄÚ´æ¿Õ¼äµÈ£©¡£
// end_mem - Êµ¼ÊÎïÀíÄÚ´æ×î´óµØÖ·¡£
// ÔÚ¸Ã°æµÄlinux ÄÚºËÖĞ£¬×î¶àÄÜÊ¹ÓÃ16Mb µÄÄÚ´æ£¬´óÓÚ16Mb µÄÄÚ´æ½«²»ÓÚ¿¼ÂÇ£¬ÆúÖÃ²»ÓÃ¡£
// 0 - 1Mb ÄÚ´æ¿Õ¼äÓÃÓÚÄÚºËÏµÍ³£¨ÆäÊµÊÇ0-640Kb£©¡£
void
mem_init (long start_mem, long end_mem)
{
  int i;

  HIGH_MEMORY = end_mem;	// ÉèÖÃÄÚ´æ×î¸ß¶Ë¡£
  for (i = 0; i < PAGING_PAGES; i++)	// Ê×ÏÈÖÃËùÓĞÒ³ÃæÎªÒÑÕ¼ÓÃ(USED=100)×´Ì¬£¬
    mem_map[i] = USED;		// ¼´½«Ò³ÃæÓ³ÉäÊı×éÈ«ÖÃ³ÉUSED¡£
  i = MAP_NR (start_mem);	// È»ºó¼ÆËã¿ÉÊ¹ÓÃÆğÊ¼ÄÚ´æµÄÒ³ÃæºÅ¡£
  end_mem -= start_mem;		// ÔÙ¼ÆËã¿É·ÖÒ³´¦ÀíµÄÄÚ´æ¿é´óĞ¡¡£
  end_mem >>= 12;		// ´Ó¶ø¼ÆËã³ö¿ÉÓÃÓÚ·ÖÒ³´¦ÀíµÄÒ³ÃæÊı¡£
  while (end_mem-- > 0)		// ×îºó½«ÕâĞ©¿ÉÓÃÒ³Ãæ¶ÔÓ¦µÄÒ³ÃæÓ³ÉäÊı×éÇåÁã¡£
    mem_map[i++] = 0;
}

// ¼ÆËãÄÚ´æ¿ÕÏĞÒ³ÃæÊı²¢ÏÔÊ¾¡£
void
calc_mem (void)
{
  int i, j, k, free = 0;
  long *pg_tbl;

// É¨ÃèÄÚ´æÒ³ÃæÓ³ÉäÊı×émem_map[]£¬»ñÈ¡¿ÕÏĞÒ³ÃæÊı²¢ÏÔÊ¾¡£
  for (i = 0; i < PAGING_PAGES; i++)
    if (!mem_map[i])
      free++;
  printk ("%d pages free (of %d)\n\r", free, PAGING_PAGES);
// É¨ÃèËùÓĞÒ³Ä¿Â¼Ïî£¨³ı0£¬1 Ïî£©£¬Èç¹ûÒ³Ä¿Â¼ÏîÓĞĞ§£¬ÔòÍ³¼Æ¶ÔÓ¦Ò³±íÖĞÓĞĞ§Ò³ÃæÊı£¬²¢ÏÔÊ¾¡£
  for (i = 2; i < 1024; i++)
    {
      if (1 & pg_dir[i])
	{
	  pg_tbl = (long *) (0xfffff000 & pg_dir[i]);
	  10.5 page.s ³ ÌĞòfor (j = k = 0; j < 1024; j++)
	    if (pg_tbl[j] & 1)
	      k++;
	  printk ("Pg-dir[%d] uses %d pages\n", i, k);
	}
    }
}

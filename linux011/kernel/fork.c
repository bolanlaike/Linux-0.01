/*
* linux/kernel/fork.c
*
* (C) 1991 Linus Torvalds
*/

/*
* 'fork.c' contains the help-routines for the 'fork' system call
* (see also system_call.s), and some misc functions ('verify_area').
* Fork is rather simple, once you get the hang of it, but the memory
* management can be a bitch. See 'mm/mm.c': 'copy_page_tables()'
*/
/*
* 'fork.c'ÖĞº¬ÓĞÏµÍ³µ÷ÓÃ'fork'µÄ¸¨Öú×Ó³ÌĞò£¨²Î¼ûsystem_call.s£©£¬ÒÔ¼°Ò»Ğ©ÆäËüº¯Êı
* ('verify_area')¡£Ò»µ©ÄãÁË½âÁËfork£¬¾Í»á·¢ÏÖËüÊÇ·Ç³£¼òµ¥µÄ£¬µ«ÄÚ´æ¹ÜÀíÈ´ÓĞĞ©ÄÑ¶È¡£
* ²Î¼û'mm/mm.c'ÖĞµÄ'copy_page_tables()'¡£
*/
#include <errno.h>		// ´íÎóºÅÍ·ÎÄ¼ş¡£°üº¬ÏµÍ³ÖĞ¸÷ÖÖ³ö´íºÅ¡£(Linus ´Óminix ÖĞÒı½øµÄ)¡£

#include <linux/sched.h>	// µ÷¶È³ÌĞòÍ·ÎÄ¼ş£¬¶¨ÒåÁËÈÎÎñ½á¹¹task_struct¡¢³õÊ¼ÈÎÎñ0 µÄÊı¾İ£¬
// »¹ÓĞÒ»Ğ©ÓĞ¹ØÃèÊö·û²ÎÊıÉèÖÃºÍ»ñÈ¡µÄÇ¶ÈëÊ½»ã±àº¯ÊıºêÓï¾ä¡£
#include <linux/kernel.h>	// ÄÚºËÍ·ÎÄ¼ş¡£º¬ÓĞÒ»Ğ©ÄÚºË³£ÓÃº¯ÊıµÄÔ­ĞÎ¶¨Òå¡£
#include <asm/segment.h>	// ¶Î²Ù×÷Í·ÎÄ¼ş¡£¶¨ÒåÁËÓĞ¹Ø¶Î¼Ä´æÆ÷²Ù×÷µÄÇ¶ÈëÊ½»ã±àº¯Êı¡£
#include <asm/system.h>		// ÏµÍ³Í·ÎÄ¼ş¡£¶¨ÒåÁËÉèÖÃ»òĞŞ¸ÄÃèÊö·û/ÖĞ¶ÏÃÅµÈµÄÇ¶ÈëÊ½»ã±àºê¡£

extern void write_verify (unsigned long address);

long last_pid = 0;

//// ½ø³Ì¿Õ¼äÇøÓòĞ´Ç°ÑéÖ¤º¯Êı¡£
// ¶Ôµ±Ç°½ø³ÌµÄµØÖ·addr µ½addr+size ÕâÒ»¶Î½ø³Ì¿Õ¼äÒÔÒ³Îªµ¥Î»Ö´ĞĞĞ´²Ù×÷Ç°µÄ¼ì²â²Ù×÷¡£
// ÈôÒ³ÃæÊÇÖ»¶ÁµÄ£¬ÔòÖ´ĞĞ¹²Ïí¼ìÑéºÍ¸´ÖÆÒ³Ãæ²Ù×÷£¨Ğ´Ê±¸´ÖÆ£©¡£
void
verify_area (void *addr, int size)
{
  unsigned long start;

  start = (unsigned long) addr;
// ½«ÆğÊ¼µØÖ·start µ÷ÕûÎªÆäËùÔÚÒ³µÄ×ó±ß½ç¿ªÊ¼Î»ÖÃ£¬Í¬Ê±ÏàÓ¦µØµ÷ÕûÑéÖ¤ÇøÓò´óĞ¡¡£
// ´ËÊ±start ÊÇµ±Ç°½ø³Ì¿Õ¼äÖĞµÄÏßĞÔµØÖ·¡£
  size += start & 0xfff;
  start &= 0xfffff000;
  start += get_base (current->ldt[2]);	// ´ËÊ±start ±ä³ÉÏµÍ³Õû¸öÏßĞÔ¿Õ¼äÖĞµÄµØÖ·Î»ÖÃ¡£
  while (size > 0)
    {
      size -= 4096;
// Ğ´Ò³ÃæÑéÖ¤¡£ÈôÒ³Ãæ²»¿ÉĞ´£¬Ôò¸´ÖÆÒ³Ãæ¡££¨mm/memory.c£¬261 ĞĞ£©
      write_verify (start);
      start += 4096;
    }
}

// ÉèÖÃĞÂÈÎÎñµÄ´úÂëºÍÊı¾İ¶Î»ùÖ·¡¢ÏŞ³¤²¢¸´ÖÆÒ³±í¡£
// nr ÎªĞÂÈÎÎñºÅ£»p ÊÇĞÂÈÎÎñÊı¾İ½á¹¹µÄÖ¸Õë¡£
int
copy_mem (int nr, struct task_struct *p)
{
  unsigned long old_data_base, new_data_base, data_limit;
  unsigned long old_code_base, new_code_base, code_limit;

  code_limit = get_limit (0x0f);	// È¡¾Ö²¿ÃèÊö·û±íÖĞ´úÂë¶ÎÃèÊö·ûÏîÖĞ¶ÎÏŞ³¤¡£
  data_limit = get_limit (0x17);	// È¡¾Ö²¿ÃèÊö·û±íÖĞÊı¾İ¶ÎÃèÊö·ûÏîÖĞ¶ÎÏŞ³¤¡£
  old_code_base = get_base (current->ldt[1]);	// È¡Ô­´úÂë¶Î»ùÖ·¡£
  old_data_base = get_base (current->ldt[2]);	// È¡Ô­Êı¾İ¶Î»ùÖ·¡£
  if (old_data_base != old_code_base)	// 0.11 °æ²»Ö§³Ö´úÂëºÍÊı¾İ¶Î·ÖÁ¢µÄÇé¿ö¡£
    panic ("We don't support separate I&D");
  if (data_limit < code_limit)	// Èç¹ûÊı¾İ¶Î³¤¶È < ´úÂë¶Î³¤¶ÈÒ²²»¶Ô¡£
    panic ("Bad data_limit");
  new_data_base = new_code_base = nr * 0x4000000;	// ĞÂ»ùÖ·=ÈÎÎñºÅ*64Mb(ÈÎÎñ´óĞ¡)¡£
  p->start_code = new_code_base;
  set_base (p->ldt[1], new_code_base);	// ÉèÖÃ´úÂë¶ÎÃèÊö·ûÖĞ»ùÖ·Óò¡£
  set_base (p->ldt[2], new_data_base);	// ÉèÖÃÊı¾İ¶ÎÃèÊö·ûÖĞ»ùÖ·Óò¡£
  if (copy_page_tables (old_data_base, new_data_base, data_limit))
    {				// ¸´ÖÆ´úÂëºÍÊı¾İ¶Î¡£
      free_page_tables (new_data_base, data_limit);	// Èç¹û³ö´íÔòÊÍ·ÅÉêÇëµÄÄÚ´æ¡£
      return -ENOMEM;
    }
  return 0;
}

/*
* Ok, this is the main fork-routine. It copies the system process
* information (task[nr]) and sets up the necessary registers. It
* also copies the data segment in it's entirety.
*/
/*
* OK£¬ÏÂÃæÊÇÖ÷ÒªµÄfork ×Ó³ÌĞò¡£Ëü¸´ÖÆÏµÍ³½ø³ÌĞÅÏ¢(task[n])²¢ÇÒÉèÖÃ±ØÒªµÄ¼Ä´æÆ÷¡£
* Ëü»¹Õû¸öµØ¸´ÖÆÊı¾İ¶Î¡£
*/
// ¸´ÖÆ½ø³Ì¡£
int
copy_process (int nr, long ebp, long edi, long esi, long gs, long none,
	      long ebx, long ecx, long edx,
	      long fs, long es, long ds,
	      long eip, long cs, long eflags, long esp, long ss)
{
  struct task_struct *p;
  int i;
  struct file *f;

  p = (struct task_struct *) get_free_page ();	// ÎªĞÂÈÎÎñÊı¾İ½á¹¹·ÖÅäÄÚ´æ¡£
  if (!p)			// Èç¹ûÄÚ´æ·ÖÅä³ö´í£¬Ôò·µ»Ø³ö´íÂë²¢ÍË³ö¡£
    return -EAGAIN;
  task[nr] = p;			// ½«ĞÂÈÎÎñ½á¹¹Ö¸Õë·ÅÈëÈÎÎñÊı×éÖĞ¡£
// ÆäÖĞnr ÎªÈÎÎñºÅ£¬ÓÉÇ°Ãæfind_empty_process()·µ»Ø¡£
  *p = *current;		/* NOTE! this doesn't copy the supervisor stack */
/* ×¢Òâ£¡ÕâÑù×ö²»»á¸´ÖÆ³¬¼¶ÓÃ»§µÄ¶ÑÕ» */ £¨Ö»¸´ÖÆµ±Ç°½ø³ÌÄÚÈİ£©¡£
    p->state = TASK_UNINTERRUPTIBLE;	// ½«ĞÂ½ø³ÌµÄ×´Ì¬ÏÈÖÃÎª²»¿ÉÖĞ¶ÏµÈ´ı×´Ì¬¡£
  p->pid = last_pid;		// ĞÂ½ø³ÌºÅ¡£ÓÉÇ°Ãæµ÷ÓÃfind_empty_process()µÃµ½¡£
  p->father = current->pid;	// ÉèÖÃ¸¸½ø³ÌºÅ¡£
  p->counter = p->priority;
  p->signal = 0;		// ĞÅºÅÎ»Í¼ÖÃ0¡£
  p->alarm = 0;
  p->leader = 0;		/* process leadership doesn't inherit */
/* ½ø³ÌµÄÁìµ¼È¨ÊÇ²»ÄÜ¼Ì³ĞµÄ */
  p->utime = p->stime = 0;	// ³õÊ¼»¯ÓÃ»§Ì¬Ê±¼äºÍºËĞÄÌ¬Ê±¼ä¡£
  p->cutime = p->cstime = 0;	// ³õÊ¼»¯×Ó½ø³ÌÓÃ»§Ì¬ºÍºËĞÄÌ¬Ê±¼ä¡£
  p->start_time = jiffies;	// µ±Ç°µÎ´ğÊıÊ±¼ä¡£
// ÒÔÏÂÉèÖÃÈÎÎñ×´Ì¬¶ÎTSS ËùĞèµÄÊı¾İ£¨²Î¼ûÁĞ±íºóËµÃ÷£©¡£
  p->tss.back_link = 0;
  p->tss.esp0 = PAGE_SIZE + (long) p;	// ¶ÑÕ»Ö¸Õë£¨ÓÉÓÚÊÇ¸øÈÎÎñ½á¹¹p ·ÖÅäÁË1 Ò³
// ĞÂÄÚ´æ£¬ËùÒÔ´ËÊ±esp0 ÕıºÃÖ¸Ïò¸ÃÒ³¶¥¶Ë£©¡£
  p->tss.ss0 = 0x10;		// ¶ÑÕ»¶ÎÑ¡Ôñ·û£¨ÄÚºËÊı¾İ¶Î£©[??]¡£
  p->tss.eip = eip;		// Ö¸Áî´úÂëÖ¸Õë¡£
  p->tss.eflags = eflags;	// ±êÖ¾¼Ä´æÆ÷¡£
  p->tss.eax = 0;
  p->tss.ecx = ecx;
  p->tss.edx = edx;
  p->tss.ebx = ebx;
  p->tss.esp = esp;
  p->tss.ebp = ebp;
  p->tss.esi = esi;
  p->tss.edi = edi;
  p->tss.es = es & 0xffff;	// ¶Î¼Ä´æÆ÷½ö16 Î»ÓĞĞ§¡£
  p->tss.cs = cs & 0xffff;
  p->tss.ss = ss & 0xffff;
  p->tss.ds = ds & 0xffff;
  p->tss.fs = fs & 0xffff;
  p->tss.gs = gs & 0xffff;
  p->tss.ldt = _LDT (nr);	// ¸ÃĞÂÈÎÎñnr µÄ¾Ö²¿ÃèÊö·û±íÑ¡Ôñ·û£¨LDT µÄÃèÊö·ûÔÚGDT ÖĞ£©¡£
  p->tss.trace_bitmap = 0x80000000;
  £¨¸ß16 Î »ÓĞĞ§£©¡£
// Èç¹ûµ±Ç°ÈÎÎñÊ¹ÓÃÁËĞ­´¦ÀíÆ÷£¬¾Í±£´æÆäÉÏÏÂÎÄ¡£
    if (last_task_used_math == current)
    __asm__ ("clts ; fnsave %0"::"m" (p->tss.i387));
// ÉèÖÃĞÂÈÎÎñµÄ´úÂëºÍÊı¾İ¶Î»ùÖ·¡¢ÏŞ³¤²¢¸´ÖÆÒ³±í¡£Èç¹û³ö´í£¨·µ»ØÖµ²»ÊÇ0£©£¬Ôò¸´Î»ÈÎÎñÊı×éÖĞ
// ÏàÓ¦Ïî²¢ÊÍ·ÅÎª¸ÃĞÂÈÎÎñ·ÖÅäµÄÄÚ´æÒ³¡£
  if (copy_mem (nr, p))
    {				// ·µ»Ø²»Îª0 ±íÊ¾³ö´í¡£
      task[nr] = NULL;
      free_page ((long) p);
      return -EAGAIN;
    }
// Èç¹û¸¸½ø³ÌÖĞÓĞÎÄ¼şÊÇ´ò¿ªµÄ£¬Ôò½«¶ÔÓ¦ÎÄ¼şµÄ´ò¿ª´ÎÊıÔö1¡£
  for (i = 0; i < NR_OPEN; i++)
    if (f = p->filp[i])
      f->f_count++;
// ½«µ±Ç°½ø³Ì£¨¸¸½ø³Ì£©µÄpwd, root ºÍexecutable ÒıÓÃ´ÎÊı¾ùÔö1¡£
  if (current->pwd)
    current->pwd->i_count++;
  if (current->root)
    current->root->i_count++;
  if (current->executable)
    current->executable->i_count++;
// ÔÚGDT ÖĞÉèÖÃĞÂÈÎÎñµÄTSS ºÍLDT ÃèÊö·ûÏî£¬Êı¾İ´Ótask ½á¹¹ÖĞÈ¡¡£
// ÔÚÈÎÎñÇĞ»»Ê±£¬ÈÎÎñ¼Ä´æÆ÷tr ÓÉCPU ×Ô¶¯¼ÓÔØ¡£
  set_tss_desc (gdt + (nr << 1) + FIRST_TSS_ENTRY, &(p->tss));
  set_ldt_desc (gdt + (nr << 1) + FIRST_LDT_ENTRY, &(p->ldt));
  p->state = TASK_RUNNING;	/* do this last, just in case */
/* ×îºóÔÙ½«ĞÂÈÎÎñÉèÖÃ³É¿ÉÔËĞĞ×´Ì¬£¬ÒÔ·ÀÍòÒ» */
  return last_pid;		// ·µ»ØĞÂ½ø³ÌºÅ£¨ÓëÈÎÎñºÅÊÇ²»Í¬µÄ£©¡£
}

// ÎªĞÂ½ø³ÌÈ¡µÃ²»ÖØ¸´µÄ½ø³ÌºÅlast_pid£¬²¢·µ»ØÔÚÈÎÎñÊı×éÖĞµÄÈÎÎñºÅ(Êı×éindex)¡£
int
find_empty_process (void)
{
  int i;

repeat:
  if ((++last_pid) < 0)
    last_pid = 1;
  for (i = 0; i < NR_TASKS; i++)
    if (task[i] && task[i]->pid == last_pid)
      goto repeat;
  for (i = 1; i < NR_TASKS; i++)	// ÈÎÎñ0 ÅÅ³ıÔÚÍâ¡£
    if (!task[i])
      return i;
  return -EAGAIN;
}

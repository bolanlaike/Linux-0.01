/*
* linux/fs/pipe.c
*
* (C) 1991 Linus Torvalds
*/

#include <signal.h>		// ĞÅºÅÍ·ÎÄ¼ş¡£¶¨ÒåĞÅºÅ·ûºÅ³£Á¿£¬ĞÅºÅ½á¹¹ÒÔ¼°ĞÅºÅ²Ù×÷º¯ÊıÔ­ĞÍ¡£

#include <linux/sched.h>	// µ÷¶È³ÌĞòÍ·ÎÄ¼ş£¬¶¨ÒåÁËÈÎÎñ½á¹¹task_struct¡¢³õÊ¼ÈÎÎñ0 µÄÊı¾İ£¬
// »¹ÓĞÒ»Ğ©ÓĞ¹ØÃèÊö·û²ÎÊıÉèÖÃºÍ»ñÈ¡µÄÇ¶ÈëÊ½»ã±àº¯ÊıºêÓï¾ä¡£
#include <linux/mm.h> /* for get_free_page */	/* Ê¹ÓÃÆäÖĞµÄget_free_page */
// ÄÚ´æ¹ÜÀíÍ·ÎÄ¼ş¡£º¬ÓĞÒ³Ãæ´óĞ¡¶¨ÒåºÍÒ»Ğ©Ò³ÃæÊÍ·Åº¯ÊıÔ­ĞÍ¡£
#include <asm/segment.h>	// ¶Î²Ù×÷Í·ÎÄ¼ş¡£¶¨ÒåÁËÓĞ¹Ø¶Î¼Ä´æÆ÷²Ù×÷µÄÇ¶ÈëÊ½»ã±àº¯Êı¡£

//// ¹ÜµÀ¶Á²Ù×÷º¯Êı¡£
// ²ÎÊıinode ÊÇ¹ÜµÀ¶ÔÓ¦µÄi ½Úµã£¬buf ÊÇÊı¾İ»º³åÇøÖ¸Õë£¬count ÊÇ¶ÁÈ¡µÄ×Ö½ÚÊı¡£
int
read_pipe (struct m_inode *inode, char *buf, int count)
{
  int chars, size, read = 0;

// ÈôÓû¶ÁÈ¡µÄ×Ö½Ú¼ÆÊıÖµcount ´óÓÚ0£¬ÔòÑ­»·Ö´ĞĞÒÔÏÂ²Ù×÷¡£
  while (count > 0)
    {
// Èôµ±Ç°¹ÜµÀÖĞÃ»ÓĞÊı¾İ(size=0)£¬Ôò»½ĞÑµÈ´ı¸Ã½ÚµãµÄ½ø³Ì£¬Èç¹ûÒÑÃ»ÓĞĞ´¹ÜµÀÕß£¬Ôò·µ»ØÒÑ¶Á
// ×Ö½ÚÊı£¬ÍË³ö¡£·ñÔòÔÚ¸Ãi ½ÚµãÉÏË¯Ãß£¬µÈ´ıĞÅÏ¢¡£
      while (!(size = PIPE_SIZE (*inode)))
	{
	  wake_up (&inode->i_wait);
	  ¹ÜµÀ»º³åÇø³¤¶È(PAGE_SIZE) ¹ ÜµÀÊı¾İ³¤¶È(PIPE_SIZE) Î ²Ö¸Õëtail (i_zone[1]) Í ·Ö¸Õëhead (i_zone[0]) » º³åÇøÖ¸Õë(i_size) if (inode->i_count != 2)	/* are there any writers? */
	    return read;
	  sleep_on (&inode->i_wait);
	}
// È¡¹ÜµÀÎ²µ½»º³åÇøÄ©¶ËµÄ×Ö½ÚÊıchars¡£Èç¹ûÆä´óÓÚ»¹ĞèÒª¶ÁÈ¡µÄ×Ö½ÚÊıcount£¬ÔòÁîÆäµÈÓÚcount¡£
// Èç¹ûchars ´óÓÚµ±Ç°¹ÜµÀÖĞº¬ÓĞÊı¾İµÄ³¤¶Èsize£¬ÔòÁîÆäµÈÓÚsize¡£
      chars = PAGE_SIZE - PIPE_TAIL (*inode);
      if (chars > count)
	chars = count;
      if (chars > size)
	chars = size;
// ¶Á×Ö½Ú¼ÆÊı¼õÈ¥´Ë´Î¿É¶ÁµÄ×Ö½ÚÊıchars£¬²¢ÀÛ¼ÓÒÑ¶Á×Ö½ÚÊı¡£
      count -= chars;
      read += chars;
// Áîsize Ö¸Ïò¹ÜµÀÎ²²¿£¬µ÷Õûµ±Ç°¹ÜµÀÎ²Ö¸Õë£¨Ç°ÒÆchars ×Ö½Ú£©¡£
      size = PIPE_TAIL (*inode);
      PIPE_TAIL (*inode) += chars;
      PIPE_TAIL (*inode) &= (PAGE_SIZE - 1);
// ½«¹ÜµÀÖĞµÄÊı¾İ¸´ÖÆµ½ÓÃ»§»º³åÇøÖĞ¡£¶ÔÓÚ¹ÜµÀi ½Úµã£¬Æäi_size ×Ö¶ÎÖĞÊÇ¹ÜµÀ»º³å¿éÖ¸Õë¡£
      while (chars-- > 0)
	put_fs_byte (((char *) inode->i_size)[size++], buf++);
    }
// »½ĞÑµÈ´ı¸Ã¹ÜµÀi ½ÚµãµÄ½ø³Ì£¬²¢·µ»Ø¶ÁÈ¡µÄ×Ö½ÚÊı¡£
  wake_up (&inode->i_wait);
  return read;
}

//// ¹ÜµÀĞ´²Ù×÷º¯Êı¡£
// ²ÎÊıinode ÊÇ¹ÜµÀ¶ÔÓ¦µÄi ½Úµã£¬buf ÊÇÊı¾İ»º³åÇøÖ¸Õë£¬count ÊÇ½«Ğ´Èë¹ÜµÀµÄ×Ö½ÚÊı¡£
int
write_pipe (struct m_inode *inode, char *buf, int count)
{
  int chars, size, written = 0;

// Èô½«Ğ´ÈëµÄ×Ö½Ú¼ÆÊıÖµcount »¹´óÓÚ0£¬ÔòÑ­»·Ö´ĞĞÒÔÏÂ²Ù×÷¡£
  while (count > 0)
    {
// Èôµ±Ç°¹ÜµÀÖĞÃ»ÓĞÒÑ¾­ÂúÁË(size=0)£¬Ôò»½ĞÑµÈ´ı¸Ã½ÚµãµÄ½ø³Ì£¬Èç¹ûÒÑÃ»ÓĞ¶Á¹ÜµÀÕß£¬ÔòÏò½ø³Ì
// ·¢ËÍSIGPIPE ĞÅºÅ£¬²¢·µ»ØÒÑĞ´ÈëµÄ×Ö½ÚÊı²¢ÍË³ö¡£ÈôĞ´Èë0 ×Ö½Ú£¬Ôò·µ»Ø-1¡£·ñÔòÔÚ¸Ãi ½ÚµãÉÏ
// Ë¯Ãß£¬µÈ´ı¹ÜµÀÌÚ³ö¿Õ¼ä¡£
      while (!(size = (PAGE_SIZE - 1) - PIPE_SIZE (*inode)))
	{
	  wake_up (&inode->i_wait);
	  if (inode->i_count != 2)
	    {			/* no readers */
	      current->signal |= (1 << (SIGPIPE - 1));
	      return written ? written : -1;
	    }
	  sleep_on (&inode->i_wait);
	}
// È¡¹ÜµÀÍ·²¿µ½»º³åÇøÄ©¶Ë¿Õ¼ä×Ö½ÚÊıchars¡£Èç¹ûÆä´óÓÚ»¹ĞèÒªĞ´ÈëµÄ×Ö½ÚÊıcount£¬ÔòÁîÆäµÈÓÚ
// count¡£Èç¹ûchars ´óÓÚµ±Ç°¹ÜµÀÖĞ¿ÕÏĞ¿Õ¼ä³¤¶Èsize£¬ÔòÁîÆäµÈÓÚsize¡£
      chars = PAGE_SIZE - PIPE_HEAD (*inode);
      if (chars > count)
	chars = count;
      if (chars > size)
	chars = size;
// Ğ´Èë×Ö½Ú¼ÆÊı¼õÈ¥´Ë´Î¿ÉĞ´ÈëµÄ×Ö½ÚÊıchars£¬²¢ÀÛ¼ÓÒÑĞ´×Ö½ÚÊıµ½written¡£
      count -= chars;
      written += chars;
// Áîsize Ö¸Ïò¹ÜµÀÊı¾İÍ·²¿£¬µ÷Õûµ±Ç°¹ÜµÀÊı¾İÍ·²¿Ö¸Õë£¨Ç°ÒÆchars ×Ö½Ú£©¡£
      size = PIPE_HEAD (*inode);
      PIPE_HEAD (*inode) += chars;
      PIPE_HEAD (*inode) &= (PAGE_SIZE - 1);
// ´ÓÓÃ»§»º³åÇø¸´ÖÆchars ¸ö×Ö½Úµ½¹ÜµÀÖĞ¡£¶ÔÓÚ¹ÜµÀi ½Úµã£¬Æäi_size ×Ö¶ÎÖĞÊÇ¹ÜµÀ»º³å¿éÖ¸Õë¡£
      while (chars-- > 0)
	((char *) inode->i_size)[size++] = get_fs_byte (buf++);
    }
// »½ĞÑµÈ´ı¸Ãi ½ÚµãµÄ½ø³Ì£¬·µ»ØÒÑĞ´ÈëµÄ×Ö½ÚÊı£¬ÍË³ö¡£
  wake_up (&inode->i_wait);
  return written;
}

//// ´´½¨¹ÜµÀÏµÍ³µ÷ÓÃº¯Êı¡£
// ÔÚfildes ËùÖ¸µÄÊı×éÖĞ´´½¨Ò»¶ÔÎÄ¼ş¾ä±ú(ÃèÊö·û)¡£Õâ¶ÔÎÄ¼ş¾ä±úÖ¸ÏòÒ»¹ÜµÀi ½Úµã¡£fildes[0]
// ÓÃÓÚ¶Á¹ÜµÀÖĞÊı¾İ£¬fildes[1]ÓÃÓÚÏò¹ÜµÀÖĞĞ´ÈëÊı¾İ¡£
// ³É¹¦Ê±·µ»Ø0£¬³ö´íÊ±·µ»Ø-1¡£
int
sys_pipe (unsigned long *fildes)
{
  struct m_inode *inode;
  struct file *f[2];
  int fd[2];
  int i, j;

// ´ÓÏµÍ³ÎÄ¼ş±íÖĞÈ¡Á½¸ö¿ÕÏĞÏî£¨ÒıÓÃ¼ÆÊı×Ö¶ÎÎª0 µÄÏî£©£¬²¢·Ö±ğÉèÖÃÒıÓÃ¼ÆÊıÎª1¡£
  j = 0;
  for (i = 0; j < 2 && i < NR_FILE; i++)
    if (!file_table[i].f_count)
      (f[j++] = i + file_table)->f_count++;
// Èç¹ûÖ»ÓĞÒ»¸ö¿ÕÏĞÏî£¬ÔòÊÍ·Å¸ÃÏî(ÒıÓÃ¼ÆÊı¸´Î»)¡£
  if (j == 1)
    f[0]->f_count = 0;
// Èç¹ûÃ»ÓĞÕÒµ½Á½¸ö¿ÕÏĞÏî£¬Ôò·µ»Ø-1¡£
  if (j < 2)
    return -1;
// Õë¶ÔÉÏÃæÈ¡µÃµÄÁ½¸öÎÄ¼ş½á¹¹Ïî£¬·Ö±ğ·ÖÅäÒ»ÎÄ¼ş¾ä±ú£¬²¢Ê¹½ø³ÌµÄÎÄ¼ş½á¹¹Ö¸Õë·Ö±ğÖ¸ÏòÕâÁ½¸ö
// ÎÄ¼ş½á¹¹¡£
  j = 0;
  for (i = 0; j < 2 && i < NR_OPEN; i++)
    if (!current->filp[i])
      {
	current->filp[fd[j] = i] = f[j];
	j++;
      }
// Èç¹ûÖ»ÓĞÒ»¸ö¿ÕÏĞÎÄ¼ş¾ä±ú£¬ÔòÊÍ·Å¸Ã¾ä±ú¡£
  if (j == 1)
    current->filp[fd[0]] = NULL;
// Èç¹ûÃ»ÓĞÕÒµ½Á½¸ö¿ÕÏĞ¾ä±ú£¬ÔòÊÍ·ÅÉÏÃæ»ñÈ¡µÄÁ½¸öÎÄ¼ş½á¹¹Ïî£¨¸´Î»ÒıÓÃ¼ÆÊıÖµ£©£¬²¢·µ»Ø-1¡£
  if (j < 2)
    {
      f[0]->f_count = f[1]->f_count = 0;
      return -1;
    }
// ÉêÇë¹ÜµÀi ½Úµã£¬²¢Îª¹ÜµÀ·ÖÅä»º³åÇø£¨1 Ò³ÄÚ´æ£©¡£Èç¹û²»³É¹¦£¬ÔòÏàÓ¦ÊÍ·ÅÁ½¸öÎÄ¼ş¾ä±úºÍÎÄ
// ¼ş½á¹¹Ïî£¬²¢·µ»Ø-1¡£
  if (!(inode = get_pipe_inode ()))
    {
      current->filp[fd[0]] = current->filp[fd[1]] = NULL;
      f[0]->f_count = f[1]->f_count = 0;
      return -1;
    }
// ³õÊ¼»¯Á½¸öÎÄ¼ş½á¹¹£¬¶¼Ö¸ÏòÍ¬Ò»¸öi ½Úµã£¬¶ÁĞ´Ö¸Õë¶¼ÖÃÁã¡£µÚ1 ¸öÎÄ¼ş½á¹¹µÄÎÄ¼şÄ£Ê½ÖÃÎª¶Á£¬
// µÚ2 ¸öÎÄ¼ş½á¹¹µÄÎÄ¼şÄ£Ê½ÖÃÎªĞ´¡£
  f[0]->f_inode = f[1]->f_inode = inode;
  f[0]->f_pos = f[1]->f_pos = 0;
  f[0]->f_mode = 1;		/* read */
  f[1]->f_mode = 2;		/* write */
// ½«ÎÄ¼ş¾ä±úÊı×é¸´ÖÆµ½¶ÔÓ¦µÄÓÃ»§Êı×éÖĞ£¬²¢·µ»Ø0£¬ÍË³ö¡£
  put_fs_long (fd[0], 0 + fildes);
  put_fs_long (fd[1], 1 + fildes);
  return 0;
}

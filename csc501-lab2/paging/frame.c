/* frame.c - manage physical frames */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

#define SETZERO 0
#define SETONE  1
#define TWOTEN  1024
extern int page_replace_policy;


/*-------------------------------------------------------------------------
 * init_frm - initialize frm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL init_frm()
{
  //kprintf("To be implemented!\n");

  STATWORD ps;
  disable(ps);

  int index = SETZERO;
/*typedef struct{
  int fr_status;			/* MAPPED or UNMAPPED		*
  int fr_pid;				/* process id using this frame  *
  int fr_vpno;				/* corresponding virtual page no*
  int fr_refcnt;			/* reference count		*
  int fr_type;				/* FR_DIR, FR_TBL, FR_PAGE	*
  int fr_dirty;
}fr_map_t;
*/
  while (index < TWOTEN) {
    /* code */
    frm_tab[index].fr_status  = SETZERO;
    frm_tab[index].fr_pid     = -SETONE;
    frm_tab[index].fr_vpno    = SETZERO;
    frm_tab[index].fr_refcnt  = SETZERO;
    frm_tab[index].fr_type    = SETZERO;
    frm_tab[index].fr_dirty   = SETZERO;

    scA[index]  = SETZERO;
    scPointer   = SETZERO;
    index = index + 1;
  }
  restore(ps);
  return OK;
}

/*-------------------------------------------------------------------------
 * get_frm - get a free frame according page replacement policy
 *-------------------------------------------------------------------------
 */
SYSCALL get_frm(int* avail)
{
  kprintf("To be implemented!\n");
  return OK;
}

/*-------------------------------------------------------------------------
 * free_frm - free a frame
 *-------------------------------------------------------------------------
 */
SYSCALL free_frm(int i)
{

  kprintf("To be implemented!\n");
  return OK;
}

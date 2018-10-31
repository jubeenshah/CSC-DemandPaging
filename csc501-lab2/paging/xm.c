/* xm.c = xmmap xmunmap */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

#define SETZERO 0
#define SETONE  1
#define TWOTEN  1024

/*-------------------------------------------------------------------------
 * xmmap - xmmap
 *-------------------------------------------------------------------------
 */
SYSCALL xmmap(int virtpage, bsd_t source, int npages)
{
  //kprintf("xmmap - to be implemented!\n");
  STATWORD ps;
  disable(ps);

  int c = currpid;
  int v = virtpage;
  int s = source;
  int n = npages;
  if (virtpage < (4*TWOTEN) || source < SETZERO || source >= (TWOTEN/128) || npages < SETONE || npages > (TWOTEN / 4)) {
    /* code */
    restore(ps);
    return -SETONE;
  }

  int checkPrivateValue = bsm_tab[source].bs_private;

  if (checkPrivateValue == SETONE) {
    /* code */
    restore(ps);
    return -SETONE;
  }
  int checkMapVal = bsm_tab[source].bs_mapping;
  int checkNpages = bsm_tab[source].bs_npages;
  if (checkMapVal > SETZERO && npages > checkNpages) {
    /* code */
    restore(ps);
    return -SETONE;
  }

  bsm_map(c,v,s,n);

  restore(ps);
  return SETONE;
}



/*-------------------------------------------------------------------------
 * xmunmap - xmunmap
 *-------------------------------------------------------------------------
 */
SYSCALL xmunmap(int virtpage)
{
  //kprintf("To be implemented!");

  STATWORD ps;
  disable(ps);

  if (virtpage < (TWOTEN * 4)) {
    restore(ps);
    return -SETONE;
  }

  bsm_unmap(currpid, virtpage);
  restore(ps);
  return SETONE;
}

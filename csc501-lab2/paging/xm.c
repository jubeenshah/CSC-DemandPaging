/* xm.c = xmmap xmunmap */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

#define TWOTEN 1024
/*-------------------------------------------------------------------------
 * xmmap - xmmap
 *-------------------------------------------------------------------------
 */
SYSCALL xmmap(int virtpage, bsd_t source, int npages)
{
 // kprintf("xmmap - to be implemented!\n");
  STATWORD ps;
  disable(ps);

  if(virtpage<(TWOTEN * 4) || source<(TWOTEN - TWOTEN) || source>=(TWOTEN / 128) || npages<(TWOTEN / TWOTEN) || npages>(TWOTEN / 4)){
  	restore(ps);
	return SYSERR;
  }
  if(bsm_tab[source].bs_private==1){
  	restore(ps);
	return SYSERR;
  }
  if(bsm_tab[source].bs_mapn>0 && npages> bsm_tab[source].bs_npages){
  	restore(ps);
	return SYSERR;
  }

  bsm_map(currpid,virtpage,source,npages);

  restore(ps);
  return OK;
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

  if(virtpage<4096){
  	restore(ps);
	return SYSERR;
  }

  bsm_unmap(currpid,virtpage);
  restore(ps);
  return OK;
}

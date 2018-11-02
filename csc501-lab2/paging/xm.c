// /* xm.c = xmmap xmunmap */
//
// #include <conf.h>
// #include <kernel.h>
// #include <proc.h>
// #include <paging.h>
//
// #define SETZERO 0
// #define SETONE  1
// #define TWOTEN  1024
//
// /*-------------------------------------------------------------------------
//  * xmmap - xmmap
//  *-------------------------------------------------------------------------
//  */
// SYSCALL xmmap(int virtpage, bsd_t source, int npages)
// {
//   kprintf("xmmap - to be implemented!\n");
//   STATWORD ps;
//   disable(ps);
//
//
//   if (virtpage < (4*TWOTEN) || source < SETZERO || source >= (TWOTEN/128) || npages < SETONE || npages > (TWOTEN / 4)) {
//     /* code */
//     restore(ps);
//     return SYSERR;
//   }
//
//   int checkPrivateValue = bsm_tab[source].bs_private;
//
//   if (checkPrivateValue == SETONE) {
//     /* code */
//     restore(ps);
//     return SYSERR;
//   }
//   int checkMapVal = bsm_tab[source].bs_mapping;
//   int checkNpages = bsm_tab[source].bs_npages;
//   if (checkMapVal > SETZERO && npages > checkNpages) {
//     /* code */
//     restore(ps);
//     return SYSERR;
//   }
//   int c = currpid;
//   int v = virtpage;
//   int s = source;
//   int n = npages;
//   bsm_map(c,v,s,n);
//   //bsm_map(currpid,virtpage,source,npages);
//
//   restore(ps);
//   return OK;
// }
//
//
//
// /*-------------------------------------------------------------------------
//  * xmunmap - xmunmap
//  *-------------------------------------------------------------------------
//  */
// SYSCALL xmunmap(int virtpage)
// {
//   //kprintf("To be implemented!");
//
//   STATWORD ps;
//   disable(ps);
//
//   if (virtpage < (TWOTEN * 4)) {
//     restore(ps);
//     return SYSERR;
//   }
//
//   bsm_unmap(currpid, virtpage);
//   restore(ps);
//   return SETONE;
// }

/* xm.c = xmmap xmunmap */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>


/*-------------------------------------------------------------------------
 * xmmap - xmmap
 *-------------------------------------------------------------------------
 */
SYSCALL xmmap(int virtpage, bsd_t source, int npages)
{
 // kprintf("xmmap - to be implemented!\n");
  STATWORD ps;
  disable(ps);

  if(virtpage<4096 || source<0 || source>=NBS || npages<1 || npages>NPPBS){
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

/* bsm.c - manage the backing store mapping*/

#include <conf.h>
#include <kernel.h>
#include <paging.h>
#include <proc.h>

#define SETZERO 0
#define SETONE  1
#define TWOTEN  1024
/*-------------------------------------------------------------------------
 * init_bsm- initialize bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL init_bsm(){

  STATWORD ps;
  disable(ps);

  int index = SETZERO;
  int indexDos;
  int twoFourTen = TWOTEN * 4;
  while (index<8) { //8 = Number of backing Store
    /* code */
    bsm_tab[index].bs_status = SETZERO;
    indexDos = SETZERO;
    /*
    typedef struct{
      int bs_status;			/* MAPPED or UNMAPPED		*
      int bs_pid[NPROC];				/* process id using this slot   *
      int bs_vpno[NPROC];				/* starting virtual page number *
      int bs_npages;			/* number of pages in the store *
      int bs_sem;				/* semaphore mechanism ?	*
      int bs_private;
      int bs_mapping;
    } bs_map_t;
    */
    while (indexDos < NPROC) {
      /* code */
      bsm_tab[index].bs_pid[indexDos] = SETZERO;
      bsm_tab[index].bs_vpno[indexDos] = twoFourTen;
      indexDos = indexDos + SETONE;
    }
    bsm_tab[index].bs_npages  = SETZERO;
    bsm_tab[index].bs_sem     = SETZERO;
    bsm_tab[index].bs_private = SETZERO;
    bsm_tab[index].bs_mapping = SETZERO;

    index = index + SETONE;
  }
}

/*-------------------------------------------------------------------------
 * get_bsm - get a free entry from bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL get_bsm(int* avail) {
  STATWORD ps;
  disable(ps);

  int index = SETZERO;
  while (index < 8) { //8 = Number of backing Store
    /* code */
    if (bsm_tab[index].bs_status == SETZERO) {
      /* code */
      *avail = index;
      restore(ps);
      return OK;
    }
    index = index + 1;
  }
  restore(ps);
  return SYSERR;
}


/*-------------------------------------------------------------------------
 * free_bsm - free an entry from bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL free_bsm(int i){

  STATWORD ps;
	disable(ps);

	bsm_tab[i].bs_status=SETZERO;

	restore(ps);
	return OK;
}

/*-------------------------------------------------------------------------
 * bsm_lookup - lookup bsm_tab and find the corresponding entry
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_lookup(int pid, long vaddr, int* store, int* pageth){
  
}


/*-------------------------------------------------------------------------
 * bsm_map - add an mapping into bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_map(int pid, int vpno, int source, int npages)
{
}



/*-------------------------------------------------------------------------
 * bsm_unmap - delete an mapping from bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_unmap(int pid, int vpno, int flag)
{
}

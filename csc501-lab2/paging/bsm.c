/* bsm.c - manage the backing store mapping*/

#include <conf.h>
#include <kernel.h>
#include <paging.h>
#include <proc.h>

#define SETZERO 0
#define SETONE  1
#define TWOTEN  1024
#define BS      16
#define ANDVAL  0xfffff000
/*-------------------------------------------------------------------------
 * init_bsm- initialize bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL init_bsm(){
//kprintf("INIT BSM");
  STATWORD ps;
  disable(ps);
  int index = SETZERO;
  int indexDos;
  int twoFourTen = TWOTEN * 4;
  while (index<BS) { //8 = Number of backing Store
    /* code */
    bsm_tab[index].bs_status = SETZERO;
    indexDos = SETZERO;
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
  restore(ps);
  return OK;
}

/*-------------------------------------------------------------------------
 * get_bsm - get a free entry from bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL get_bsm(int* avail) {
  STATWORD ps;
  disable(ps);

  int index = SETZERO;
  while (index < BS) { //8 = Number of backing Store
    /* code */
    int checkStatus = bsm_tab[index].bs_status;
    if ( checkStatus == SETZERO) {
      /* code */
      *avail = index;
      restore(ps);
      return OK;
    }
    index = index + SETONE;
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

  STATWORD ps;
	disable(ps);
  //kprintf("bsm_lookup");
  int index = SETZERO;
  int starth = vaddr & ANDVAL;
  int startp = (starth)>>12;
  while (index < BS) {
    /* code */
    int checkPIDBSM = bsm_tab[index].bs_pid[pid];
    if ( checkPIDBSM == SETONE) {
      /* code */
      *store = index;
      int vpn = bsm_tab[index].bs_vpno[pid];
      *pageth = startp - vpn;
      restore(ps);
      return OK;
    }
    index = index + SETONE;
  }
  restore(ps);
  return SYSERR;
}


/*-------------------------------------------------------------------------
 * bsm_map - add an mapping into bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_map(int pid, int vpno, int source, int npages){
  STATWORD ps;
	disable(ps);

  if (bsm_tab[source].bs_status==SETZERO) {
      bsm_tab[source].bs_status = SETONE;
      bsm_tab[source].bs_npages = npages;
  }
  bsm_tab[source].bs_pid[pid] = SETONE;
  bsm_tab[source].bs_sem      = SETZERO;
  bsm_tab[source].bs_vpno[pid]= vpno;
  bsm_tab[source].bs_mapping++;
  proctab[currpid].vhpno = vpno;
  proctab[currpid].store = source;

  restore(ps);
  return OK;
}



/*-------------------------------------------------------------------------
 * bsm_unmap - delete an mapping from bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_unmap(int pid, int vpno, int flag){
  STATWORD ps;
	disable(ps);

  int index = SETZERO;
  int proctabStore = proctab[currpid].store;
  int twoFourTen = TWOTEN * 4;
  int pageth;
  unsigned long virtualAddress = vpno * twoFourTen;
  bsm_tab[proctabStore].bs_mapping--;

  while (index < TWOTEN) {
    /* code */
    int checkPid = frm_tab[index].fr_pid;
    int checkTyp = frm_tab[index].fr_type;

    if (checkPid == pid && checkTyp == SETZERO) {
    //  if (checkTyp == SETZERO) {
        if (bsm_lookup(pid, virtualAddress, &proctabStore, &pageth) == SYSERR) {
          continue;
        }
        int twotenI = TWOTEN + index;
        int mult = twotenI * twoFourTen;
        //kprintf("write_bs pid:%d vaddr:%08x store:%d pageth:%d\n",pid,vaddr,store,pageth);
        write_bs(mult, proctabStore, pageth);
    //  }
    }
    index = index + SETONE;
  }
  restore(ps);
  return OK;
}

// /* bsm.c - manage the backing store mapping*/
//
// #include <conf.h>
// #include <kernel.h>
// #include <paging.h>
// #include <proc.h>
//
// /*-------------------------------------------------------------------------
//  * init_bsm- initialize bsm_tab
//  *-------------------------------------------------------------------------
//  */
// SYSCALL init_bsm()
// {
// 	STATWORD ps;
// 	disable(ps);
//
// 	int i=0;
// 	for(;i<NBS;++i){
// 		bsm_tab[i].bs_status=BSM_UNMAPPED;
// 		//bsm_tab[i].bs_pid=-1;
// 		int j=0;
// 		for(;j<NPROC;++j){
// 			bsm_tab[i].bs_pid[j]=0;
// 			bsm_tab[i].bs_vpno[j]=4096;
// 		}
// 		//bsm_tab[i].bs_vpno=4096;
// 		bsm_tab[i].bs_npages=0;
// 		bsm_tab[i].bs_sem=0;
// 		bsm_tab[i].bs_mapn=0;
// 		bsm_tab[i].bs_private=0;
// 	}
//
// 	restore(ps);
// 	return OK;
// }
//
// /*-------------------------------------------------------------------------
//  * get_bsm - get a free entry from bsm_tab
//  *-------------------------------------------------------------------------
//  */
// SYSCALL get_bsm(int* avail)
// {
// 	STATWORD ps;
// 	disable(ps);
//
// 	int i=0;
// 	for(;i<NBS;++i){
// 		if(bsm_tab[i].bs_status==BSM_UNMAPPED){
// 			*avail=i;
//
// 			restore(ps);
// 			return OK;
// 		}
// 	}
// //  kprintf("Error. cannot find avail backing store");
// 	restore(ps);
// 	return SYSERR;
// }
//
//
// /*-------------------------------------------------------------------------
//  * free_bsm - free an entry from bsm_tab
//  *-------------------------------------------------------------------------
//  */
// SYSCALL free_bsm(int i)
// {
// 	STATWORD ps;
// 	disable(ps);
// 	bsm_tab[i].bs_status=BSM_UNMAPPED;
//
// 	restore(ps);
// 	return OK;
// }
//
// /*-------------------------------------------------------------------------
//  * bsm_lookup - lookup bsm_tab and find the corresponding entry
//  *-------------------------------------------------------------------------
//  */
// SYSCALL bsm_lookup(int pid, long vaddr, int* store, int* pageth)
// {
// 	STATWORD ps;
// 	disable(ps);
// 	int i=0;
// 	int start_page=(vaddr & 0xfffff000)>>12;
// 	//kprintf("vaddr:%08x  start page: %d\n",vaddr,start_page);
// 	for(;i<NBS;++i){
// 		if(bsm_tab[i].bs_pid[pid]==1){
// 			*store=i;
// 			*pageth=start_page-bsm_tab[i].bs_vpno[pid];
// //			kprintf("bsm_lookup find store:%d pageth:%d\n",*store,*pageth);
// 			restore(ps);
// 			return OK;
// 		}
// 	}
// 	restore(ps);
// 	return SYSERR;
// }
//
//
// /*-------------------------------------------------------------------------
//  * bsm_map - add an mapping into bsm_tab
//  *-------------------------------------------------------------------------
//  */
// SYSCALL bsm_map(int pid, int vpno, int source, int npages)
// {
// 	//kprintf("bsm map start pid:%d vpno:0x%x source:%d npages:%d\n",pid,vpno,source,npages);
// 	STATWORD ps;
// 	disable(ps);
// 	if(bsm_tab[source].bs_status==BSM_UNMAPPED){
// 	bsm_tab[source].bs_status=BSM_MAPPED;
// 	bsm_tab[source].bs_npages=npages;
// 	}
// 	bsm_tab[source].bs_pid[pid]=1;
// 	bsm_tab[source].bs_sem=0;
// 	bsm_tab[source].bs_vpno[pid]=vpno;
// 	bsm_tab[source].bs_mapn++;
// 	//kprintf("bsm_map pid:%d store:%d mapn:%d\n",pid,source,bsm_tab[source].bs_mapn);
//
// 	proctab[currpid].vhpno=vpno;
// 	proctab[currpid].store=source;
//
// 	restore(ps);
// //	kprintf("bsm map end\n");
// 	return OK;
// }
//
//
//
// /*-------------------------------------------------------------------------
//  * bsm_unmap - delete an mapping from bsm_tab
//  *-------------------------------------------------------------------------
//  */
// SYSCALL bsm_unmap(int pid, int vpno, int flag)
// {
// 	STATWORD ps;
// 	disable(ps);
// 	int i=0;
// 	int store=proctab[currpid].store;
// 	int pageth;
// 	unsigned long vaddr=vpno*NBPG;
//
// 	bsm_tab[store].bs_mapn--;
// 	//kprintf("bsm_tab[%d].bs_mapn:%d\n",store,bsm_tab[store].bs_mapn);
// //	kprintf("\nbsm_unmap pid:%d store:%d mapn:%d vaddr:%08x npages: %d\n",pid,store,bsm_tab[store].bs_mapn,vaddr,bsm_tab[store].bs_npages);
// 	for(;i<NFRAMES;++i){
// 		if(frm_tab[i].fr_pid==pid && frm_tab[i].fr_type==FR_PAGE){
// 			int res_lookup=bsm_lookup(pid,vaddr,&store,&pageth);
// 			if(res_lookup==SYSERR){
// 				continue;
// 			}
// //			kprintf("write_bs pid:%d vaddr:%08x store:%d pageth:%d\n",pid,vaddr,store,pageth);
// 			write_bs((i+FRAME0)*NBPG,store,pageth);
// 		}
// 	}
// 	restore(ps);
// 	return OK;
// }

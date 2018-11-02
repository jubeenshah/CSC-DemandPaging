#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

#define SETONE  1
#define SETZERO 0
#define NEIGHT  8 //8
#define NPAGES  256 //256
int get_bs(bsd_t bs_id, unsigned int npages) {

STATWORD ps;
disable(ps);
  /* requests a new mapping of npages with ID map_id */
    kprintf("get_bs\n");
    int checkSemValue     = bsm_tab[bs_id].bs_sem;
    int checkPrivateValue = bsm_tab[bs_id].bs_private;
    int checkStatus       = bsm_tab[bs_id].bs_status;
    int pagesValue        = bsm_tab[bs_id].bs_npages;
    if (bs_id >= NEIGHT || bs_id < SETZERO || npages > NPAGES ||
          npages <= SETZERO || checkSemValue == SETONE) {
      /* code */
      restore(ps);
      return SYSERR;
    }

    if (checkPrivateValue == SETONE && checkStatus == SETONE) {
      /* code */
      restore(ps);
      return SYSERR;
    }

    if (checkPrivateValue == SETZERO && checkStatus == SETONE) {
      /* code */
      restore(ps);
      pagesValue = bsm_tab[bs_id].bs_npages;
      return pagesValue;
    }

    pagesValue = npages;
    bsm_tab[bs_id].bs_npages = pagesValue;

    bsm_tab[bs_id].bs_pid[currpid] = SETONE;


    restore(ps);
    return npages;

}

// #include <conf.h>
// #include <kernel.h>
// #include <proc.h>
// #include <paging.h>
//
// int get_bs(bsd_t bs_id, unsigned int npages) {
//
//   /* requests a new mapping of npages with ID map_id */
//   //  kprintf("To be implemented!\n");
//   STATWORD ps;
//   disable(ps);
//
//   if(bs_id>=NBS || bs_id<0 || npages>NPPBS || npages<=0 || bsm_tab[bs_id].bs_sem==1){
//   	restore(ps);
// 	return SYSERR;
//   }
//   if(bsm_tab[bs_id].bs_private==1 && bsm_tab[bs_id].bs_status== BSM_MAPPED){
//   	restore(ps);
// 	return SYSERR;
//   }
//   if(bsm_tab[bs_id].bs_private==0 && bsm_tab[bs_id].bs_status== BSM_MAPPED){
//   	restore(ps);
// 	return bsm_tab[bs_id].bs_npages;
//   }
//   /* bsm_tab[bs_id].bs_status == BSM_UNMAPPED */
// 	bsm_tab[bs_id].bs_npages=npages;
// 	bsm_tab[bs_id].bs_pid[currpid]=1;
//
// 	restore(ps);
//     return npages;
// }

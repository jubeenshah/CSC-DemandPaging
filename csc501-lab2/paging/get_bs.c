#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

#define SETONE  1
#define SETZERO 0
#define NEIGHT  8
#define NPAGES  256
int get_bs(bsd_t bs_id, unsigned int npages) {

  /* requests a new mapping of npages with ID map_id */
    //kprintf("To be implemented!\n");
    int checkSemValue = bsm_tab[bs_id].bs_sem;
    int checkPrivateValue = bsm_tab[bs_id].bs_private;
    int checkStatus = bsm_tab[bs_id].bs_status;
    int pagesValue = bsm_tab[bs_id].bs_npages;
    if (bs_id >= NEIGHT || bs_id < SETZERO ||
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
      return pagesValue;
    }

    pagesValue = npages;
    bsm_tab[bs_id].bs_npages = pagesValue;

    bsm_tab[bs_id].bs_pid[currpid] = SETONE;


    restore(ps);
    return npages;

}

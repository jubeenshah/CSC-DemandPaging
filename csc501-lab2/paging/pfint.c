/* pfint.c - pfint */

#include <conf.h>
#include <kernel.h>
#include <paging.h>
#include <proc.h>

extern int page_replace_policy;

#define SETONE  1
#define SETZERO 0
#define TWOTEN  1024
/*-------------------------------------------------------------------------
 * pfint - paging fault ISR
 *-------------------------------------------------------------------------
 */
SYSCALL pfint()
{
  counter = counter + 1;

  STATWORD ps;
  disable(ps);

  int newPageTable, newFrame, store, pageth;


  unsigned long virtualAddress, pdbr,tempVar,tmp;
  virt_addr_t *virt_addr;
  unsigned int pg, pt, pd;

  pd_t *pd_entry;
  pt_t *pt_entry;
  pt_t *pageTable;
//SEE IF THIS WORKS
/*  STATWORD ps4;

  disable(ps4);
  asm("pushl %eax");
  asm("movl %cr2, %eax");
  asm("movl %eax, tmp");
  asm("popl %eax");*/
  tempVar = read_cr2();
  //restore(ps4);
  virtualAddress = tempVar;


virt_addr = (virt_addr_t *)&virtualAddress;
  pg = virt_addr->pg_offset;
  pt = virt_addr->pt_offset;
  pd = virt_addr->pd_offset;
  pdbr = proctab[currpid].pdbr;

  int a = sizeof(pd_t);
  int b = pd * a;
  int add = pdbr + b;
  pd_entry = add;

  int checkPresVal = pd_entry->pd_pres;

  if (checkPresVal == SETZERO){



    pd_entry->pd_pres   = SETONE;
    pd_entry->pd_write  = SETONE;
    pd_entry->pd_user   = SETZERO;
    pd_entry->pd_pwt    = SETZERO;
    pd_entry->pd_pcd    = SETZERO;
    pd_entry->pd_acc    = SETZERO;
    pd_entry->pd_mbz    = SETZERO;
    pd_entry->pd_fmb    = SETZERO;
    pd_entry->pd_global = SETZERO;
    pd_entry->pd_avail  = SETZERO;
    newPageTable = pageCreate();
    int base = TWOTEN + newPageTable;
    pd_entry->pd_base = base;

    frm_tab[newPageTable].fr_status = SETONE;
    frm_tab[newPageTable].fr_type   = SETONE;
    frm_tab[newPageTable].fr_pid    = currpid;
    }

    int q = sizeof(pt_t);
    int multTwo = pt * q;
    int twoFourTenTwo = TWOTEN * 4;
    int w = pd_entry->pd_base * twoFourTenTwo;
    //int multThree = w * twoFourTenTwo;
    int addTwoTwo =  multTwo + w;
    pt_entry = (pt_t *)(addTwoTwo);

    int checkPDPresVal = pt_entry->pt_pres;

    if (checkPDPresVal == SETZERO) {
      get_frm(&newFrame);

      pt_entry->pt_pres   = SETONE;
      pt_entry->pt_write  = SETONE;

      int newFrameAdd = TWOTEN + newFrame;
      pt_entry->pt_base = newFrameAdd;

      int sub = pd_entry->pd_base - TWOTEN;

      frm_tab[sub].fr_refcnt++;
      frm_tab[newFrame].fr_status = SETONE;
      frm_tab[newFrame].fr_type   = SETZERO;
      frm_tab[newFrame].fr_pid    = currpid;
      //int divi = virtualAddress/twoFourTenTwo;
      frm_tab[newFrame].fr_vpno   = virtualAddress/4096;

      bsm_lookup(currpid, virtualAddress, &store, &pageth);
      int passVal = (TWOTEN + newFrame) * 4096;
      //passVal = passVal * twoFourTenTwo;
      read_bs((char *)(passVal), store, pageth);

      }


  write_cr3(pdbr);

  restore(ps);
  return OK;

}

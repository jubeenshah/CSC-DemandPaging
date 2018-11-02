/* pfint.c - pfint */

#include <conf.h>
#include <kernel.h>
#include <paging.h>
#include <proc.h>

extern int page_replace_policy;

#define SETONE  1
#define SETZERO 0
#define TWOTEN  1024

int pageCreate() {
  STATWORD ps;
  disable(ps);
  int index = SETZERO;
  //kprintf("PFINT To be implemented!\n");
  int frameNumber;
  unsigned int frameAddress;

  get_frm(&frameNumber);
  int twoFourTen = TWOTEN * 4;

  frameAddress = TWOTEN + frameNumber;
  frameAddress = frameAddress * twoFourTen;

  //pt_t *pageTable;
  pt_t *pageTable = (pt_t *)frameAddress;

  while (index < TWOTEN) {
    /* code */
    pageTable[index].pt_pres  = SETZERO;
    pageTable[index].pt_write = SETZERO;
    pageTable[index].pt_user  = SETZERO;
    pageTable[index].pt_pwt   = SETZERO;
    pageTable[index].pt_pcd   = SETZERO;
    pageTable[index].pt_acc   = SETZERO;
    pageTable[index].pt_dirty = SETZERO;
    pageTable[index].pt_mbz   = SETZERO;
    pageTable[index].pt_global= SETZERO;
    pageTable[index].pt_avail = SETZERO;
    pageTable[index].pt_base  = SETZERO;

    index = index + SETONE;
  }
  restore(ps);
  return frameNumber;
}
/*-------------------------------------------------------------------------
 * pfint - paging fault ISR
 *-------------------------------------------------------------------------
 */
SYSCALL pfint()
{
  pfint_cnt = pfint_cnt + SETONE;

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
// /* pfint.c - pfint */
//
// #include <conf.h>
// #include <kernel.h>
// #include <paging.h>
// #include <proc.h>
//
// extern int page_replace_policy;
//
// int create_page_tab(){
// 	STATWORD ps;
// 	disable(ps);
//
// 	int i=0;
// 	int frm_number;
// 	unsigned int frm_addr;
// 	get_frm(&frm_number);
// //	kprintf("create page table in frm %d for pid %d\n",frm_number,currpid);
// 	frm_addr=(FRAME0 + frm_number)*NBPG;
// 	pt_t *pt=(pt_t *)frm_addr;
//
// 	for(;i<1024;++i){
// 		pt[i].pt_pres=0;
// 		pt[i].pt_write=0;
// 		pt[i].pt_user=0;
// 		pt[i].pt_pwt=0;
// 		pt[i].pt_pcd=0;
// 		pt[i].pt_acc=0;
// 		pt[i].pt_dirty=0;
// 		pt[i].pt_mbz=0;
// 		pt[i].pt_global=0;
// 		pt[i].pt_avail=0;
// 		pt[i].pt_base=0;
// 	}
//
// 	restore(ps);
// 	return frm_number;
// }
//
// /*-------------------------------------------------------------------------
//  * pfint - paging fault ISR
//  *-------------------------------------------------------------------------
//  */
// SYSCALL pfint()
// {
//  // kprintf("To be implemented!\n");
//  	pfint_cnt++;
// 	//kprintf("pfint %d\n",pfint_cnt);
//  	STATWORD ps;
// 	disable(ps);
//
// 	unsigned long v_addr;
// 	virt_addr_t *virt_addr;
// 	unsigned int pg_offset,pt_offset,pd_offset;
// 	unsigned long pdbr;
// 	pd_t* pd_entry;
// 	pt_t* pt_entry;
//
// 	int pt_new;
// 	int frm_new;
// 	int store,pageth;
//
// 	v_addr=read_cr2();
// //	kprintf("faulted address:%x\n",v_addr);
// 	virt_addr=(virt_addr_t *)&v_addr;
//
// 	pg_offset=virt_addr->pg_offset;
// 	pt_offset=virt_addr->pt_offset;
// 	pd_offset=virt_addr->pd_offset;
// 	pdbr=proctab[currpid].pdbr;
// 	pd_entry=pdbr+pd_offset*sizeof(pd_t);
// //	kprintf("pd:%d pt:%d pg:%d pd_entry:%x pdbr:%x currpid:%d\n",pd_offset,pt_offset,pg_offset,pd_entry,pdbr,currpid);
// 	if(pd_entry->pd_pres==0){
// 		pt_new=create_page_tab();
// 		pd_entry->pd_pres=1;
// 		pd_entry->pd_write=1;
// 		pd_entry->pd_user=0;
// 		pd_entry->pd_pwt=0;
// 		pd_entry->pd_pcd=0;
// 		pd_entry->pd_acc=0;
// 		pd_entry->pd_mbz=0;
// 		pd_entry->pd_fmb=0;
// 		pd_entry->pd_global=0;
// 		pd_entry->pd_avail=0;
// 		pd_entry->pd_base= FRAME0 + pt_new;
//
// 		frm_tab[pt_new].fr_status=FRM_MAPPED;
// 		frm_tab[pt_new].fr_type=FR_TBL;
// 		frm_tab[pt_new].fr_pid=currpid;
// 	}
//
// 	pt_entry=(pt_t *)(pd_entry->pd_base * NBPG + pt_offset*sizeof(pt_t));
// //	kprintf("pt_entry:%x\n",pt_entry);
//
// 	if(pt_entry->pt_pres==0){
// 		get_frm(&frm_new);
// 		//kprintf("allocate frame as page: %d\n",frm_new);
// 		pt_entry->pt_pres=1;
// 		pt_entry->pt_write=1;
// 		pt_entry->pt_base=FRAME0 + frm_new;
//
// 		frm_tab[pd_entry->pd_base - FRAME0].fr_refcnt++;
// 		frm_tab[frm_new].fr_status=FRM_MAPPED;
// 		frm_tab[frm_new].fr_type=FR_PAGE;
// 		frm_tab[frm_new].fr_pid=currpid;
// 		frm_tab[frm_new].fr_vpno=v_addr/NBPG;
//
// 		bsm_lookup(currpid,v_addr,&store,&pageth);
// 		//kprintf("looking up store:%d\n",store);
//
// 		read_bs((char *)((FRAME0 + frm_new)*NBPG),store,pageth);
//
// 		if(page_replace_policy==SC){
//
// 		}
// 		else if(page_replace_policy==LFU){
//
// 		}
// 	}
// 	write_cr3(pdbr);
// 	restore(ps);
//   return OK;
// }

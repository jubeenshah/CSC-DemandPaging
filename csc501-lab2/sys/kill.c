/* kill.c - kill */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <io.h>
#include <q.h>
#include <stdio.h>
#include <paging.h>

#define SETZERO 0
#define SETONE  1
#define TWOTEN  1024

/*------------------------------------------------------------------------
 * kill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
SYSCALL kill(int pid)
{
	STATWORD ps;
	struct	pentry	*pptr;		/* points to proc. table for pid*/
	int	dev;

	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate==PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	if (--numproc == 0)
		xdone();

	dev = pptr->pdevs[0];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->pdevs[1];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->ppagedev;
	if (! isbaddev(dev) )
		close(dev);

	send(pptr->pnxtkin, pid);

	freestk(pptr->pbase, pptr->pstklen);
	switch (pptr->pstate) {

	case PRCURR:	pptr->pstate = PRFREE;	/* suicide */
			resched();

	case PRWAIT:	semaph[pptr->psem].semcnt++;

	case PRREADY:	dequeue(pid);
			pptr->pstate = PRFREE;
			break;

	case PRSLEEP:
	case PRTRECV:	unsleep(pid);
						/* fall through	*/
	default:	pptr->pstate = PRFREE;
	}
	int persistStoreVal = proctab[pid].store;
	release_bs(persistStoreVal);
	// STATWORD ps2;
	// disable(ps2);
	// int index = SETZERO;
	//
	// while (index < TWOTEN) {
	// 	/* code */
	// 	int checkP_i_d = frm_tab[index].fr_pid;
	// 	if (checkP_i_d != pid) {
	// 		/* code */
	// 		continue;
	// 	}
	// 	frm_tab[index].fr_status 	= SETZERO;
	// 	frm_tab[index].fr_pid		 	= -SETONE;
	// 	frm_tab[index].fr_vpno		= TWOTEN * 4;
	// 	frm_tab[index].fr_refcnt	= SETZERO;
	// 	frm_tab[index].fr_type		= SETZERO;
	// 	frm_tab[index].fr_dirty 	= SETZERO;
	// 	index = index + SETONE;
	// }
	// restore(ps2);
	frameDefine(pid);
	restore(ps);
	return(OK);
}

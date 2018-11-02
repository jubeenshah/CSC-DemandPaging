/* policy.c = srpolicy*/

#include <conf.h>
#include <kernel.h>
#include <paging.h>


extern int page_replace_policy;
/*-------------------------------------------------------------------------
 * srpolicy - set page replace policy
 *-------------------------------------------------------------------------
 */
SYSCALL srpolicy(int policy)
{
  /* sanity check ! */

  kprintf("POLICITY To be implemented!\n");
  int setPolicy = policy;
  page_replace_policy = setPolicy;
  return OK;
}

/*-------------------------------------------------------------------------
 * grpolicy - get page replace policy
 *-------------------------------------------------------------------------
 */
SYSCALL grpolicy()
{
  return page_replace_policy;
}
// /* policy.c = srpolicy*/
//
// #include <conf.h>
// #include <kernel.h>
// #include <paging.h>
//
//
// extern int page_replace_policy;
// /*-------------------------------------------------------------------------
//  * srpolicy - set page replace policy
//  *-------------------------------------------------------------------------
//  */
// SYSCALL srpolicy(int policy)
// {
//   /* sanity check ! */
//
//   //kprintf("To be implemented!\n");
// 	page_replace_policy=policy;
//   return OK;
// }
//
// /*-------------------------------------------------------------------------
//  * grpolicy - get page replace policy
//  *-------------------------------------------------------------------------
//  */
// SYSCALL grpolicy()
// {
//   return page_replace_policy;
// }

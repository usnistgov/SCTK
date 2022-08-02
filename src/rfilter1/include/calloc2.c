/* file calloc2.c */

#if !defined(COMPILE_ENVIRONMENT)
#include "stdcenvf.h" /* std compile environment for functions */
#endif

 void *calloc_safe(size_t nobj, size_t size, Char *calling_proc)
 /*************************************************************/
 /*  Like calloc(nobj,size) except fatal err if calloc fails. */
 /*************************************************************/
 {Char *proc = (Char *)"calloc_safe";
  void *x;
  if (db_level > 3) printf("%sdoing %s\n",(char *)pdb,(char *)proc);
  x = calloc(nobj,size);
  if (x == NULL) fatal_error(calling_proc,(Char *)"MEM ALLOC",-1);
  if (memory_trace) printf("%s CALLOC %lx\n",pdb,(long)x);
  return x;
 }
/* end calloc2.c */

/* file strdup2.c */

#if !defined(COMPILE_ENVIRONMENT)
#include "stdcenvf.h" /* std compile environment for functions */
#endif

 /******************************************************/
 /*  strdup_safe(ps,calling_proc)                      */
 /*  Like strdup(ps) except fatal err if malloc fails. */
 /******************************************************/
 Char *strdup_safe(Char *ps, Char *calling_proc)
 {Char *proc = (Char *)"strdup_safe";
  Char *pd;
if (db_level > 3) printf("%sdoing %s\n",pdb,proc);
 pd = (Char*)malloc((size_t)strlen((char *)ps)+1);
 if (pd == NULL) fatal_error(calling_proc,(Char *)"MEM ALLOC",-1);
  else   pd = (Char *)strcpy((char *)pd,(char *)ps);
 if (memory_trace) printf("%s MALLOC %lx\n",pdb,(long)pd);
  return pd;
 }
/* end strdup2.c */

/* file strdup2.c */

#if !defined(COMPILE_ENVIRONMENT)
#include "stdcenvf.h" /* std compile environment for functions */
#endif

 /******************************************************/
 /*  strdup_safe(ps,calling_proc)                      */
 /*  Like strdup(ps) except fatal err if malloc fails. */
 /******************************************************/
 char *strdup_safe(char *ps, char *calling_proc)
 {char *proc = "strdup_safe";
  char *pd;
if (db_level > 3) printf("%sdoing %s\n",pdb,proc);
  pd = (char*)malloc((size_t)strlen(ps)+1);
  if (pd == NULL) fatal_error(calling_proc,"MEM ALLOC",-1);
  else   pd = strcpy(pd,ps);
  if (memory_trace) printf("%s MALLOC %x\n",pdb,(int)pd);
  return pd;
 }
/* end strdup2.c */

/* file mfname1.c */

#if !defined(COMPILE_ENVIRONMENT)
#include "stdcenvf.h" /* std compile environment for functions */
#endif

  /************************************************************/
  /*                                                          */
  /*  Char *make_full_fname(sx,path,fname)                    */
  /* Makes a full file name by concatenating *path and *fname,*/
  /* puts it in *sx and returns sx.                           */
  /* Assumes length of sx and path is LINE_LENGTH.            */
  /* Inserts '/' between path and fname if necessary.         */
  /*                                                          */
  /************************************************************/
 Char *make_full_fname(Char *sx, Char *path, Char *fname)
 {Char *proc = (Char *)"make_full_fname";
/* prepare full pcode file name */
   sx = (Char *)strcpy((char *)sx,(char *)path);
  sx = pltrim(prtrim2(del_eol(sx)));
  if ((strlen((char *)sx) > 0)&&
      (*prtrim(sx) != '/')&&
      (strlen((char *)sx) < LINE_LENGTH))
    sx = (Char *)strcat((char *)sx,"/");
  if ((strlen((char *)sx)+strlen((char *)fname)) < LINE_LENGTH) sx = (Char *)strcat((char *)sx,(char *)fname);
  else
     {fprintf(stderr,"%s: full path name of file too long\n",proc);
      fprintf(stderr,"  file name: '%s'\n",fname);
      fprintf(stderr,"  path: '%s'\n",path);
      fprintf(stderr,"  Decrease name size or increase LINE_LENGTH.\n");
     }
  return sx;
 }

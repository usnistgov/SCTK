
#define MAIN
#include "sctk.h"

#define SCLITE_VERSION "2.9"

TEXT *prog = "sclite_tolower";

void do_exit(char *desc, char *prog, int ret){
    fprintf(stderr,"sclite <encoding> <flags>",prog);
    fprintf(stderr,"\n%s: Error, %s\n\n",prog,desc);
    exit(ret);
}

int main(int argc, char **argv){
  TEXT *in_buf;
  int in_buf_len = 1024;
  

  if (!TEXT_set_encoding(argv[1]))
    do_exit(rsprintf("Unrecognized character encoding option '%s'",argv[1]),prog,1);
  // Parse the optional localization
  if (argc >= 3)
    if (!TEXT_set_lang_prof(argv[2]))
      do_exit(rsprintf("Optional case conversion localization failed /%s/\n", argv[2]),prog,1);


  alloc_singZ(in_buf,in_buf_len,TEXT,'\0');

  while (!feof(stdin)){
    if (TEXT_ensure_fgets(&in_buf,&in_buf_len,stdin) == NULL)
      *in_buf = NULL_TEXT;
    if (!feof(stdin)){
      TEXT_str_case_change_with_mem_expand(&in_buf, &in_buf_len, 1);
      printf("--%s", in_buf);
    }
  }
  exit(0);
}

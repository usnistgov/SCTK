void init_pad_util PROTO((int pr_width)) ;
int pad_pr_width PROTO((void)) ;
void set_pad PROTO((char *pad, char *str)) ;
void set_pad_n PROTO((char *pad, int n)) ;
void set_pad_cent_n PROTO((char *pad, int len)) ;
char *center PROTO((char *str, int len)) ;
void strncpy_pad PROTO((char *to, char *from, int len, char chr)) ;

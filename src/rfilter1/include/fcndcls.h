/* file fcndcls.h              */
/* local function declarations */
/* last modified 11/8/96       */

#ifndef FCNDCLS_HEADER
#define FCNDCLS_HEADER

extern char          *apply_rules2(char *pb, char *pa, RULESET2 *rset, int *perr);
extern boolean        atobool(char *s);
extern char          *bool_print(boolean x);
extern void          *calloc_safe(size_t nobj, size_t size, char *calling_proc);
extern void           db_enter_msg(char *proc, int level);
extern void           db_leave_msg(char *proc, int level);
extern char          *del_eol(char *ps);
extern void           dump_rules2(RULESET2 *rset);
extern void           dump_rules3(RULESET2 *rset, FILE *fp);
extern char          *expenv(char *s, int slength);
extern void           fatal_error(char *reporting_procedure, char *msg, int error_level);
extern void           free_rules2(RULESET2 *rset);
extern void           free_str(char *s);
extern void           get_comment_flag(char *s, char *comment_flag);
extern void           get_rules2(RULESET2 *rset, char *path, char *fname, int *perr);
extern char          *make_full_fname(char *sx, char *path, char *fname);
extern char          *make_upper(char *s);
extern char          *pltrim(char *s);
extern char          *pltrimf(char *s);
extern char          *prtrim(char *s);
extern char          *prtrim2(char *s);
extern char          *remove_comments(char *s, char *comment_flag);
extern SUBSTRING      sstok2(char *sx, char *delimiters);
extern boolean        string_equal(char *cs, char *ct, int ignore_case);
extern int            strcmpi(char *ps1, char *ps2);  /* BCD 4.2; not TURBO C */
extern int            strncmpi(char *ps1, char *ps2, int n);  /* BCD 4.2; not TURBO C */
extern char          *strdup_safe(char *ps, char *calling_proc);
extern int            substr_length(SUBSTRING *substr);
extern char          *substr_to_str(SUBSTRING *substr, char *str, int lmax);
extern char          *strcutr(char *ps, int n);
extern int            textlen(char *s);
extern boolean        valid_data_line(char *s, char *comment_flag);

#endif
/* end file fcndcls.h        */

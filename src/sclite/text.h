

/* TEXT definitions */
#define NULL_TEXT '\0'
#define TEXT_COLON ':'

#define ALT_BEGIN '{'
#define ALT_END   '}'
#define COMMENT_CHAR ';'
#define COMMENT_INFO_CHAR '*'


typedef unsigned char TEXT;

#define TEXT_xnewline(_s) {int _l = TEXT_strlen(_s); if (_s[_l-1] == '\n') _s[_l-1] = '\0';}

TEXT *TEXT_skip_wspace(TEXT *ptr);
int end_of_TEXT(TEXT text);
int TEXT_set_coding(char *encoding);
void TEXT_separate_chars(TEXT *from, TEXT **to, int *to_size, int not_ASCII);
TEXT *TEXT_add(TEXT *p1, TEXT *p2);
TEXT *TEXT_strndup(TEXT *p, int n);
TEXT *TEXT_strndup_noEscape(TEXT *p, int n);
TEXT *TEXT_strdup(TEXT *p);
TEXT *TEXT_strchr(TEXT *p, TEXT t);
TEXT *TEXT_strncpy(TEXT *p, TEXT *t, int n);
TEXT *TEXT_strstr(TEXT *p, TEXT *t);
TEXT *TEXT_strrchr(TEXT *p, TEXT t);
int TEXT_strcmp(TEXT *p, TEXT *t);
int TEXT_strcasecmp(TEXT *p, TEXT *t);
int TEXT_strncasecmp(TEXT *p1, TEXT *p2, int n);
int TEXT_strncmp(TEXT *p, TEXT *t, int n);
float TEXT_atof(TEXT *p);
TEXT *TEXT_strtok(TEXT *p, TEXT *t);
int qsort_TEXT_strcmp(const void *p, const void *p1);
int bsearch_TEXT_strcmp(const void *p, const void *p1);
int TEXT_strlen(TEXT *p);
int TEXT_chrlen(TEXT *text);
void TEXT_free(TEXT *p);
int find_next_TEXT_token(TEXT **ctext, TEXT *end_token, int len);
int find_next_TEXT_alternation(TEXT **ctext, TEXT *token, int len);
TEXT *TEXT_fgets(TEXT *arr, int len, FILE *fp);
TEXT *TEXT_strcpy(TEXT *p1, TEXT *p2);
TEXT *TEXT_ensure_fgets(TEXT **arr, int *len, FILE *fp);
int TEXT_is_empty(TEXT *p);
int TEXT_is_comment(TEXT *p);
int TEXT_is_comment_info(TEXT *p);
void TEXT_str_to_upp(TEXT *buf);
void TEXT_str_to_low(TEXT *buf);
TEXT *tokenize_TEXT_first_alt(TEXT *p, TEXT *set);
size_t TEXT_strspn(TEXT *str, TEXT *set);
size_t TEXT_strcspn(TEXT *str, TEXT *set);
TEXT *TEXT_strqtok(TEXT *buf, TEXT *set);
TEXT *TEXT_strcat(TEXT *p, TEXT *p1);
int TEXT_nth_field(TEXT **to, int *to_len, TEXT *from, int field);
int TEXT_is_wfrag(TEXT *text);


/***********************************************************************/
/*   The TEXT_LIST utilities                                           */

typedef struct text_list_struct{
    char *file; /* filename read int */
    int max;   /* The max size for elem */
    int num;   /* The current number of elements in elem */
    TEXT **elem;
} TEXT_LIST;

TEXT_LIST *load_TEXT_LIST(char *file, int col);
TEXT_LIST *init_TEXT_LIST(void);
int add_TEXT_LIST(TEXT_LIST *tl, TEXT *str);
void free_TEXT_LIST(TEXT_LIST **tl);
void dump_TEXT_LIST(TEXT_LIST *tl, FILE *);
int in_TEXT_LIST(TEXT_LIST *tl, TEXT *str);
int WORD_in_TEXT_LIST(void *data, void *elem);
void TEXT_delete_chars(TEXT *arr, TEXT *set);


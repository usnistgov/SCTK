 /* File rulestr2.h --  rules data structures #2 */

#if !defined(RULESTR2_HEADER)
#define RULESTR2_HEADER

  typedef struct
   {char *lcontext; int lcontextl; /* left context, length */
    char *sin; int sinl;          /* string in, length */
    char *rcontext; int rcontextl; /* right context, length */
    char *sout; int soutl;        /* string out, lenght */
    int val1; /* general-purpose numerical value */
   } RULE2;

 typedef struct
   {char *name;
    char *directory;
    char *desc;
    char *format;
    boolean copy_no_hit; /* if no hit, copy over instead of delete */
    boolean case_sensitive; /* iff on, match is case-sensitive     */
    boolean indexed; /* uses key character index for speed */
    int nrules;
    int max_nrules;
    RULE2 *rule; /* RULE table, dynamically allocated */
    int *rule_index; /* index to rules in aggregated order, dynamically allocated */
    int *first_rule; /* index to first rule for key char, dynamically allocated */
    int *last_rule; /* index to last rule for key char, dynamically allocated */
   } RULESET2;


/* end of include rulestr2.h */

#endif

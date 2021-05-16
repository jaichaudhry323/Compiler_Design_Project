%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "cal.h"

/* prototypes */
nodeType *operatr(int oper, int nops, ...);
nodeType *identifier(int i);
nodeType *constant(float value);
void freeNode(nodeType *p);
float execute(nodeType *p);
int yylex(void);

void yyerror(char *s);
float symbol_table[26];                    /* symbol table */
%}

%union {
    float iValue;                 /* integer value */
    char sIndex;                /* symbol table index */
    nodeType *nPtr;             /* node pointer */
};

%token <iValue> INTEGER
%token <sIndex> VARIABLE
%token WHILE IF PRINT FOR SCAN
%nonassoc IFX
%nonassoc ELSE

%left GE LE EQ NE '>' '<' AND OR
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS INC DEC NOT

%type <nPtr> stmt expr stmt_list

%%

program:
        function                { exit(0); }
        ;

function:
          function stmt         { execute($2); freeNode($2); }
        | /* NULL */
        ;

stmt:  
          ';'                                 { $$ = operatr(';', 2, NULL, NULL); }
        | expr ';'                            { $$ = $1; }
        | PRINT expr ';'                      { $$ = operatr(PRINT, 1, $2); }
        | SCAN expr ';'                       { $$ = operatr(SCAN, 1, $2); }
        | VARIABLE '=' expr ';'               { $$ = operatr('=', 2, identifier($1), $3); }
        | WHILE '(' expr ')' stmt             { $$ = operatr(WHILE, 2, $3, $5); }
        | FOR '(' stmt expr ';' expr ')' stmt { $$ = operatr(FOR,4,$3,$4,$6,$8); } 
        | IF '(' expr ')' stmt %prec IFX      { $$ = operatr(IF, 2, $3, $5); }
        | IF '(' expr ')' stmt ELSE stmt      { $$ = operatr(IF, 3, $3, $5, $7); }
        | '{' stmt_list '}'                   { $$ = $2; }
        ;

stmt_list:
          stmt                  { $$ = $1; }
        | stmt_list stmt        { $$ = operatr(';', 2, $1, $2); }
        ;

expr:
          INTEGER               { $$ = constant($1); }
        | VARIABLE              { $$ = identifier($1); }
        | '-' expr %prec UMINUS { $$ = operatr(UMINUS, 1, $2); }
        | expr INC 			    { $$ = operatr(INC,1,$1); }
        | expr DEC 			    { $$ = operatr(DEC,1,$1); }
        | expr '+' expr         { $$ = operatr('+', 2, $1, $3); }
        | expr '-' expr         { $$ = operatr('-', 2, $1, $3); }
        | expr '*' expr         { $$ = operatr('*', 2, $1, $3); }
        | expr '/' expr         { $$ = operatr('/', 2, $1, $3); }
        | expr '<' expr         { $$ = operatr('<', 2, $1, $3); }
        | expr '>' expr         { $$ = operatr('>', 2, $1, $3); }
        | expr GE expr          { $$ = operatr(GE, 2, $1, $3); }
        | expr LE expr          { $$ = operatr(LE, 2, $1, $3); }
        | expr NE expr          { $$ = operatr(NE, 2, $1, $3); }
        | expr EQ expr          { $$ = operatr(EQ, 2, $1, $3); }
        | NOT expr				{ $$ = operatr(NOT,1,$2); }
        | expr AND expr			{ $$ = operatr(AND,2,$1,$3); }
        | expr OR expr			{ $$ = operatr(OR,2,$1,$3); }
        | '(' expr ')'          { $$ = $2; }
        ;

%%

nodeType *constant(float value) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->constant.value = value;

    return p;
}

nodeType *identifier(int i) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeId;
    p->identifier.i = i;

    return p;
}

// creates a list of nodeType
nodeType *operatr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    int i;

    /* allocate node, extending op array */
    if ((p = malloc(sizeof(nodeType) + (nops-1) * sizeof(nodeType *))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeOpr;          // like FOR GE AND etc etc
    p->operatr.oper = oper;    // > = < ....
    p->operatr.nops = nops;     // number of operands
    va_start(ap, nops);  
    for (i = 0; i < nops; i++)
        p->operatr.op[i] = va_arg(ap, nodeType*);
    
    va_end(ap);
    return p;
}

void freeNode(nodeType *p) { 
    int i;

    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->operatr.nops; i++)
            freeNode(p->operatr.op[i]);
    }
    free (p);
}

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}


int main(void) {
    yyparse();
    return 0;
}

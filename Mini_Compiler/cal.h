typedef enum { typeCon, typeId, typeOpr } nodeEnum;

/* constants */
typedef struct {
    float value;                  /* value of constant */
} conNodeType;

/* identifiers */
typedef struct {
    int i;                      /* subscript to sym array */
} idNodeType;

/* operators */
typedef struct {
    int oper;                   /* operator */
    int nops;                   /* number of operands */
    struct nodeTypeTag *op[1];	/* operands, extended at runtime */
} oprNodeType;


// This is the only thing that we are using explicitly
typedef struct nodeTypeTag {
    nodeEnum type;              /* type of node */

    union {
        conNodeType constant;        /* constants */
        idNodeType identifier;              /* identifiers */
        oprNodeType operatr;        /* operators */
    };
} nodeType;

extern float symbol_table[26];


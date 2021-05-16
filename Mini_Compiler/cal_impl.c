#include <stdio.h>
#include "cal.h"
#include "y.tab.h"

// interpreter.c

float execute(nodeType *curr_node) {
    float temp;
    if (!curr_node)
        return 0;

    switch (curr_node->type) {
        case typeCon:
            return curr_node->constant.value;

        case typeId:
            return symbol_table[curr_node->identifier.i];

        case typeOpr:
            switch (curr_node->operatr.oper) {
                case FOR:
                // stmt:    FOR '(' stmt expr ';' expr ')' stmt { $$ = operatr(FOR,4,$3,$4,$6,$8); } 
                //  $0       $1  $2  $3   $4  $5  $6   $7  $8
                //                    0   1       2        3      // in op array of operatr function of cal.y
                    for (execute(curr_node->operatr.op[0]); execute(curr_node->operatr.op[1]); execute(curr_node->operatr.op[2]))
                        execute(curr_node->operatr.op[3]);
                    return 0;

            // stmt:    | WHILE '(' expr ')' stmt             { $$ = operatr(WHILE, 2, $3, $5); }

                case WHILE:
                    while (execute(curr_node->operatr.op[0]))
                        execute(curr_node->operatr.op[1]);
                    return 0;

                case IF:
                    if (execute(curr_node->operatr.op[0]))
                        execute(curr_node->operatr.op[1]);
                    else if (curr_node->operatr.nops > 2)
                        execute(curr_node->operatr.op[2]);
                    return 0;

                case PRINT:
                    printf("%f\n", execute(curr_node->operatr.op[0]));
                    return 0;

                case SCAN:
                    ("%f\n", &temp); symbol_table[curr_node->operatr.op[0]->identifier.i] = temp;
                    return 0;

                case ';':
                    execute(curr_node->operatr.op[0]);
                    return execute(curr_node->operatr.op[1]);

                case '=':
                    return symbol_table[curr_node->operatr.op[0]->identifier.i] = execute(curr_node->operatr.op[1]);

                case UMINUS:
                    return execute(curr_node->operatr.op[0]);

                case NOT:
                    return !execute(curr_node->operatr.op[0]);

                case OR:
                    return execute(curr_node->operatr.op[0]) || execute(curr_node->operatr.op[1]);

                case AND:
                    return execute(curr_node->operatr.op[0]) && execute(curr_node->operatr.op[1]);

                case INC:
                    if (curr_node->operatr.op[0]->type == typeId) {
                        symbol_table[curr_node->operatr.op[0]->identifier.i] += 1;
                        return symbol_table[curr_node->operatr.op[0]->identifier.i] - 1;
                    }
                    else {
                        return execute(curr_node->operatr.op[0]);
                    }

                case DEC:
                    if (curr_node->operatr.op[0]->type == typeId) {
                        symbol_table[curr_node->operatr.op[0]->identifier.i] -= 1;
                        return symbol_table[curr_node->operatr.op[0]->identifier.i] - 1;
                    } else {
                        return execute(curr_node->operatr.op[0]);
                    }

                case '+':
                    return execute(curr_node->operatr.op[0]) + execute(curr_node->operatr.op[1]);

                case '-':
                    return execute(curr_node->operatr.op[0]) - execute(curr_node->operatr.op[1]);

                case '*':
                    return execute(curr_node->operatr.op[0]) * execute(curr_node->operatr.op[1]);

                case '/':
                    return execute(curr_node->operatr.op[0]) / execute(curr_node->operatr.op[1]);

                case '<':
                    return execute(curr_node->operatr.op[0]) < execute(curr_node->operatr.op[1]);

                case '>':
                    return execute(curr_node->operatr.op[0]) > execute(curr_node->operatr.op[1]);

                case GE:
                    return execute(curr_node->operatr.op[0]) >= execute(curr_node->operatr.op[1]);

                case LE:
                    return execute(curr_node->operatr.op[0]) <= execute(curr_node->operatr.op[1]);

                case NE:
                    return execute(curr_node->operatr.op[0]) != execute(curr_node->operatr.op[1]);

                case EQ:
                    return execute(curr_node->operatr.op[0]) == execute(curr_node->operatr.op[1]);
        }
    }

    return 0;
}

#include <stdio.h>
#include "cal.h"
#include "y.tab.h"

// COMPILER.c
// three address code printer

static int label;
static int t;

int execute(nodeType *p) {
	int label1, label2;
	int op1, op2;

	if (!p)
		return 0;

	switch (p->type) {
	case typeCon:
		printf("\t_%d = %f\n", t++, p->constant.value);
		return t - 1;
		break;

	case typeId:
		printf("\t_%d = %c\n", t++, p->identifier.i + 'a');
		return t - 1;
		break;

	case typeOpr:
		switch (p->operatr.oper) {
		case FOR:
			execute(p->operatr.op[0]);
			printf("L%03d:\n", label1 = label++);
			op1 = execute(p->operatr.op[1]);
			printf("\tif(_%d==0) goto:\tL%03d\n", op1, label2 = label++);
			execute(p->operatr.op[3]);
			execute(p->operatr.op[2]);
			printf("\tgoto:L%03d\n", label1);
			printf("L%03d:\n", label2);
			break;

		case WHILE:
			printf("L%03d:\n", label1 = label++);
			op1 = execute(p->operatr.op[0]);
			printf("\tif(_%d==0) goto:\tL%03d\n", op1, label2 = label++);
			execute(p->operatr.op[1]);
			printf("\tgoto:L%03d\n", label1);
			printf("L%03d:\n", label2);
			break;

		case INC:
			printf("\t_%d = %c\n", t++, p->operatr.op[0]->identifier.i + 'a');
			printf("\t%c = %c + 1\n", p->operatr.op[0]->identifier.i + 'a', p->operatr.op[0]->identifier.i + 'a');
			return t - 1;
			break;

		case DEC:
			printf("\t_%d = %c\n", t++, p->operatr.op[0]->identifier.i + 'a');
			printf("\t%c = %c - 1\n", p->operatr.op[0]->identifier.i + 'a', p->operatr.op[0]->identifier.i + 'a');
			return t - 1;
			break;

		case NOT:
			if (p->operatr.op[0]->type == typeOpr) {
				op1 = execute(p->operatr.op[0]);
			}
			printf("\t_%d = ", t++);
			if (p->operatr.op[0]->type == typeOpr)
				printf("!_%d", op1);

			else if (p->operatr.op[0]->type == typeCon)
				printf("!%f", p->operatr.op[0]->constant.value);

			else if (p->operatr.op[0]->type == typeId)
				printf("!%c", p->operatr.op[0]->identifier.i + 'a');

			printf("\n");
			return t - 1;
			break;

		case IF:
			op1 = execute(p->operatr.op[0]);
			if (p->operatr.nops > 2) {
				/* if else */
				printf("\tif(_%d==0)\tgoto:L%03d\n", op1, label1 = label++);
				op2 = execute(p->operatr.op[1]);
				printf("\tgoto:L%03d\n", label2 = label++);
				printf("L%03d:\n", label1);
				execute(p->operatr.op[2]);
				printf("L%03d:\n", label2);
			} else {
				/* if */
				printf("\tif(_%d==0)\tgoto:L%03d\n", op1, label1 = label++);
				execute(p->operatr.op[1]);
				printf("L%03d:\n", label1);
			}
			break;

		case PRINT:
			op1 = execute(p->operatr.op[0]);
			printf("\tprintf(\"%%f\",_%d);\n", op1);
			break;

		case SCAN:
			printf("\tscanf(\"%%f\",&%c);\n", p->operatr.op[0]->identifier.i + 'a');
			break;

		case '=':
			printf("\t%c = _%d", p->operatr.op[0]->identifier.i + 'a', execute(p->operatr.op[1]));
			printf("\n");
			break;

		case UMINUS:
			if (p->operatr.op[0]->type == typeOpr)
			{
				op1 = execute(p->operatr.op[0]);
			}
			printf("\t_%d = ", t++);

			if (p->operatr.op[0]->type == typeOpr)
				printf("-_%d", op1);

			else if (p->operatr.op[0]->type == typeCon)
				printf("-%f", p->operatr.op[0]->constant.value);

			else if (p->operatr.op[0]->type == typeId)
				printf("-%c", p->operatr.op[0]->identifier.i + 'a');

			printf("\n");
			return t - 1;
			break;

		default:
			if (p->operatr.op[0]->type == typeOpr) {
				op1 = execute(p->operatr.op[0]);
			}
			if (p->operatr.op[1]->type == typeOpr) {
				op2 = execute(p->operatr.op[1]);
			}
			printf("\t_%d = ", t++);

			if (p->operatr.op[0]->type == typeOpr)
				printf("_%d", op1);

			else if (p->operatr.op[0]->type == typeCon)
				printf("%f", p->operatr.op[0]->constant.value);

			else if (p->operatr.op[0]->type == typeId)
				printf("%c", p->operatr.op[0]->identifier.i + 'a');

			switch (p->operatr.oper) {
			case '+':   printf("+");
				break;

			case '-':   printf("-");
				break;

			case '*':   printf("*");
				break;

			case '/':   printf("/");
				break;

			case '<':   printf("<");
				break;

			case '>':   printf(">");
				break;

			case GE:    printf(">=");
				break;

			case LE:    printf("<=");
				break;

			case NE:    printf("!=");
				break;

			case EQ:    printf("==");
				break;

			case AND:    printf("&&");
				break;

			case OR:    printf("||");
				break;
			}

			if (p->operatr.op[1]->type == typeOpr)
				printf("_%d", op2);

			else if (p->operatr.op[1]->type == typeCon)
				printf("%f", p->operatr.op[1]->constant.value);

			else if (p->operatr.op[1]->type == typeId)
				printf("%c", p->operatr.op[1]->identifier.i + 'a');

			//printf("    ====> %d\n",t);

			printf("\n");
			return t - 1;
		}
	}
	return 0;
}

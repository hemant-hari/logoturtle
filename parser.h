#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define MAXNUMTOKENS 300
#define MAXTOKENSIZE 15
#define NUMVARS 26
#define CTOINT 'A'
#define strsame(A,B) (strcmp(A,B) == 0)
#define ERROR(PHRASE) {fprintf(stderr, "Fatal Error %s occured in %s, line %d\n", PHRASE, __FILE__, __LINE__); exit(2);}

struct prog{
   char wds[MAXNUMTOKENS][MAXTOKENSIZE];
   int vars[NUMVARS];
   int currvar;
   int cl; /* Current Line */
};
typedef struct prog Program;

struct stackelem{
   int x;
   struct stackelem *prev;
};
typedef struct stackelem Elem;

struct polishstack{
   Elem *tp;
   int numelems;
}
typedef struct polishstack Pstack;

void Prog(Program *p);
void InstructionList(Program *p);
void Instruction(Program *p);
void VarNum(Program *p);
void Set(Program *p);
int Var(Program *p);
void Num(Program *p);
void Polish(Program *p);

void InitStack(Pstack *s);
void Push(Pstack *s, int n);
int Pop(Stack *s);

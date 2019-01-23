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
#define ERROR(PHRASE,l) {fprintf(stderr, "Fatal Error %s occured while parsing \
phrase %d\n", PHRASE, l); exit(2);}

enum bool{false, true};
typedef enum bool bool;

struct prog{
   char wds[MAXNUMTOKENS][MAXTOKENSIZE];
   double vars[NUMVARS];
   double currvar;
   int cl; /* Current Line */
};
typedef struct prog Program;

struct stackelem{
   int i;
   struct stackelem *prev;
};
typedef struct stackelem Elem;

struct polishstack{
   Elem *tp;
   int numelems;
};
typedef struct polishstack Pstack;

void Prog(Program *p);
void InstructionList(Program *p);
void Instruction(Program *p);
void VarNum(Program *p);
void Set(Program *p);
int Var(Program *p);
void Num(Program *p);
void Polish(Program *p);
void Do(Program *p);

bool isOperator(Program *p);
bool isVarNum(Program *p);
void Push(Pstack *s, int n);
int Pop(Pstack *s);
int Calculate(Program *p, int a, int b);

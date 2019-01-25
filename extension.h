#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define RESIZE_FCTR 4
#define INITPRIME 23
#define MAXLOAD 0.6
#define MAXWORD 50
#define PI 3.14159265358979323846
#define MINPRECISION 0.000001
#define MILLISECONDDELAY 20
#define MAXNUMTOKENS 300
#define MAXTOKENSIZE 15
#define NUMVARS 26
#define MAXNUMPARAMS 10
#define CTOINT 'A'
#define strsame(A,B) (strcmp(A,B) == 0)
#define ERROR(PHRASE, l) {fprintf(stderr, "Fatal Error %s occured while interpreting \
phrase %d\n", PHRASE, l); exit(2);}

struct entry{
   char* word;
   double params[MAXNUMPARAMS];
   double locvars[NUMVARS];
   int numparams;
   int stl;
   struct entry *next;
};
typedef struct entry Entry;

struct dic{
   Entry **entries;
   int sz;
   int wordsz;
   int occ;
};
typedef struct dic dic;


enum bool{false, true, parse};
typedef enum bool bool;

struct turtle{
   double x;
   double y;
   double dir;
};
typedef struct turtle turtle;

struct prog{
   char wds[MAXNUMTOKENS][MAXTOKENSIZE];
   double vars[NUMVARS];
   double currvar;
   int cl; /* Current Line */
   int numbraces;
   turtle t;
   dic* funcs;
   bool isfunction;
   Entry* currfunc;
   struct SDL_Simplewin *swin;
};
typedef struct prog Program;

struct stackelem{
   double i;
   struct stackelem *prev;
};
typedef struct stackelem Elem;

struct polishstack{
   Elem *tp;
   int numelems;
};
typedef struct polishstack Pstack;

void Prog(Program *p);
void FunctionList(Program *p);
void MakeFunction(Program *p);
void MainProg(Program *p);
void InstructionList(Program *p);
void Instruction(Program *p);
void VarNum(Program *p);
void ParVarNum(Program *p);
void Set(Program *p);
int Par(Program *p);
int Var(Program *p);
void Num(Program *p);
void Polish(Program *p);
void Do(Program *p);
void CallFunction(Program *p);

bool isOperator(Program *p);
bool isVarNum(Program *p);
bool isParVarNum(Program *p);
bool isInbuilt(Program *p);
void Push(Pstack *s, int n);
int Pop(Pstack *s);
int Calculate(Program *p, int a, int b);
void Move(Program *p);
void VarSet(Program *p, int index);
void VarGet(Program *p, int index);
int round(double num);

void ParseInstructionList(Program *p);
void ParseInstruction(Program *p);
void ParseSet(Program *p);
void ParsePolish(Program *p);
void ParseDo(Program *p);

/*####################### LIBRARY FUNCTIONS ##################################*/

/* Create empty dic */
dic* dic_init(int size);
/* Add one element into the dic */
void dic_insert(dic* s, char* v, int nump, int stl);
/* Returns true if v is in the array, false elsewise */
bool dic_isin(dic* s, char* v);
/* Returns entry of a given word, if it exists in the dictionary */
Entry* func_get(dic* s, char* v);
/* Finish up */
/* Clears all space used, and sets pointer to NULL */
void dic_free(dic** s);

/*###########################################################################*/


/*###################### ADDITIONAL FUNCTIONS ##########################*/
/* Modified Bernstein hashing function*/
int hash(unsigned int sz, char *s);

/* Initalises space in memory for table */
Entry** init_table(int sz);

/* Initialises space in memory for table entry */
Entry* init_entry(char* word, int wlen);

/* Inserts entry into a designated location
   in the table, and deals with collisions */
void insert_entry(Entry **dst, Entry *entry, int location);

/* Generates primes for the next size of hash table */
bool newsize(int *currsz);

/* Rehashes every indexed element in the table */
void rehash_table(dic *s, int new_sz);

/* Rehashes all children of a particular element */
void rehash_list(Entry *e, Entry **dst, int new_sz);

/* Testing function to print out the the non-blank
   elements in the dictionary and their index */
void print_dic(dic *s);

/* Checks every element in the linked list
   to see if they match the input string */
bool check_list(Entry *entry, char *v);

Entry* get_list(Entry *entry, char *v);

/* Frees all the pointers in an entry list */
void free_list(Entry *first);

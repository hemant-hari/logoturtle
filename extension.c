#include "neillsdl2.h"

void Prog(Program *p)
{
   p->isfunction = false;
   if (strsame(p->wds[p->cl], "{")){
      p->cl += 1;
      p->numbraces = 1;
      InstructionList(p);
      return;
   }
   if (strsame(p->wds[p->cl], "[")){
      p->funcs = dic_init(MAXTOKENSIZE);
      p->isfunction = parse;
      p->cl += 1;
      FunctionList(p);
      return;
   }

   ERROR("? No opening curly brace '{' or square brace '[' ?",p->cl)
}

void FunctionList(Program *p)
{
   if (strsame(p->wds[p->cl], "]")){
      p->cl += 1;
      p->isfunction = false;
      MainProg(p);
      return;
   }
   if (dic_isin(p->funcs, p->wds[p->cl])){
      ERROR("? Function Redefinition ?",p->cl)
   }
   MakeFunction(p);
   p->cl += 1;
   FunctionList(p);
}

void MakeFunction(Program *p)
{
   char* funcname = p->wds[p->cl];
   int numparams = atoi(p->wds[++(p->cl)]);

   Num(p);

   if (numparams > MAXNUMPARAMS){
      ERROR("? Tried to set too many parameters ?", p->cl)
   }

   p->cl += 1;
   if (!strsame(p->wds[p->cl], "{")){
      ERROR("? No opening curly brace '{' ?",p->cl)
   }

   p->cl += 1;
   dic_insert(p->funcs, funcname, numparams, p->cl);

   p->currfunc = func_get(p->funcs, funcname);

   ParseInstructionList(p);
}

void MainProg(Program *p)
{
   if (!strsame(p->wds[p->cl], "{")){
      ERROR("? No opening curly brace '{' ?",p->cl)
   }
   p->numbraces = 0;
   p->cl += 1;
   InstructionList(p);
   dic_free(&(p->funcs));
}

void InstructionList(Program *p)
{
   if (strsame(p->wds[p->cl], "}")){
      return;
   }
   Instruction(p);
   p->cl = p->cl + 1;
   InstructionList(p);
}

void Instruction(Program *p)
{
   if (strsame(p->wds[p->cl], "FD")){
      p->cl += 1;
      ParVarNum(p);
      Move(p);
      return;
   }
   else if (strsame(p->wds[p->cl], "LT")){
      p->cl += 1;
      ParVarNum(p);
      p->t.dir += p->currvar;
      return;
   }
   else if (strsame(p->wds[p->cl], "RT")){
      p->cl += 1;
      ParVarNum(p);
      p->t.dir -= p->currvar;
      return;
   }
   else if (strsame(p->wds[p->cl], "SET")){
      p->cl += 1;
      Set(p);
      return;
   }
   else if (strsame(p->wds[p->cl], "DO")){
      p->cl += 1;
      Do(p);
      return;
   }
   else if (dic_isin(p->funcs, p->wds[p->cl])){
      CallFunction(p);
      return;
   }

   ERROR("? Invalid instruction given ?",p->cl)
}

void VarNum(Program *p)
{
   if (!isdigit(p->wds[p->cl][0])){
      p->currvar = p->vars[Var(p)];
   }
   else{
      Num(p);
   }
}

void ParVarNum(Program *p)
{
   if (strlen(p->wds[p->cl]) >= 2 && p->wds[p->cl][0] == 'P'){
      if (p->isfunction == true){
         p->currvar = p->currfunc->params[Par(p)];
         return;
      }
      if (p->isfunction == parse){
         Par(p);
         return;
      }
      ERROR("? Parameter called outside a function ?", p->cl)
   }
   else if (strlen(p->wds[p->cl]) == 1 && !isdigit(p->wds[p->cl][0])){
      VarGet(p, Var(p));
   }
   else{
      Num(p);
   }
}

void Set(Program *p)
{
   int charv;

   charv = Var(p);

   p->cl += 1;

   if (!strsame(p->wds[p->cl], ":=")){
      ERROR("? Invalid symbol after variable, use ':=' ?",p->cl)
   }

   p->cl += 1;

   Polish(p);

   VarSet(p, charv);
}

int Par(Program *p)
{
   if (strlen(p->wds[p->cl]) != 2){
      ERROR("? Parameter names must be 2 chars long and start with P followed by a number ?", p->cl)
   }

   if (p->wds[p->cl][0] == 'P'){
      if (isdigit(p->wds[p->cl][1])){
         return p->wds[p->cl][1] - '0';
      }
      ERROR("? Parameter not followed by digit ?",p->cl)
   }
   ERROR("? Parameter name does not start with P ?",p->cl)
}

int Var(Program *p)
{
   int charval;
   if (strlen(p->wds[p->cl]) != 1){
      if (!isupper(p->wds[p->cl][0])){
         ERROR("? Variable not a single capital letter ?",p->cl)
      }
      ERROR("? Variable name too long ?",p->cl)
   }
   charval = p->wds[p->cl][0] - CTOINT;

   return charval;
}

void Num(Program *p)
{
   int i;

   for (i = 0; p->wds[p->cl][i] != '\0'; i++){
      if (!isdigit(p->wds[p->cl][i]) && (p->wds[p->cl][i] != 46) && (p->wds[p->cl][i] != '-')) {
         ERROR("? Value after instruction not an integer ?",p->cl)
      }
   }
   p->currvar = atof(p->wds[p->cl]);
   if (p->currvar - 0 < 0.000001){
      printf("? Value for instruction is 0, is this intentional ?\n");
   }
}

void Polish(Program *p)
{
   Pstack stk;
   int tmp1, tmp2;
   stk.numelems = 0;

   ParVarNum(p);
   p->cl += 1;
   Push(&stk, p->currvar);

   while (!strsame(p->wds[p->cl],";")){
      if (isParVarNum(p)){
         ParVarNum(p);
         Push(&stk, p->currvar);
      }
      else{
         tmp1 = Pop(&stk);
         tmp2 = Pop(&stk);
         Push(&stk, Calculate(p, tmp2, tmp1));
      }
      p->cl += 1;
   }

   if (stk.numelems != 1){
      ERROR("? Incomplete Polish Detected, check number of operators ?",p->cl)
   }

   p->currvar = Pop(&stk);
}

void Do(Program *p)
{
   int charv, retl;
   double limitval, modval;

   /* Get array key for input variable */
   charv = Var(p);
   p->cl += 1;

   /* Check Syntax */
   if (!strsame(p->wds[p->cl], "FROM")){
      ERROR("? Invalid instruction after variable, use 'FROM' ?",p->cl)
   }
   p->cl += 1;

   /* Get Initial Value for Variable */
   ParVarNum(p);
   p->cl += 1;
   VarSet(p, charv);

   /* Check Syntax */
   if (!strsame(p->wds[p->cl], "TO")){
      ERROR("? Invalid instruction after variable or number, use 'TO' ?",p->cl)
   }
   p->cl += 1;

   /* Get limiting value for variable */
   ParVarNum(p);
   limitval = p->currvar;
   p->cl += 1;

   /* Check Syntax */
   if (!strsame(p->wds[p->cl], "{")){
      ERROR("? Invalid symbol after loop start, use '{' ?",p->cl)
   }
   p->cl += 1;
   retl = p->cl;

   /* Checks whether starting value is greater or less than
   initial and decrements or increments accordingly */
   if (p->isfunction == false){
      modval = (limitval < p->vars[charv]) ? -1 : 1 ;
      while (p->vars[charv] - limitval < 0.0001){
         p->cl = retl;
         InstructionList(p);
         p->vars[charv] += modval;
      }
   }
   else{
      modval = (limitval < p->currfunc->locvars[charv]) ? -1 : 1 ;
      while (p->currfunc->locvars[charv] - limitval < 0.0001){
         p->cl = retl;
         InstructionList(p);
         p->currfunc->locvars[charv] += modval;
      }
   }
}

void CallFunction(Program *p)
{
   int i = 0, retl;

   p->currfunc = func_get(p->funcs, p->wds[p->cl]);
   p->isfunction = true;

   p->cl += 1;

   while(!strsame(p->wds[p->cl],";")){
      VarNum(p);
      p->currfunc->params[i] = p->currvar;
      p->cl += 1;
      i++;
   }

   if (i != p->currfunc->numparams){
      ERROR("? Incorrect number of parameters used in function call ?", p->cl)
   }
   retl = p->cl;
   p->cl = p->currfunc->stl;

   InstructionList(p);

   p->cl = retl;
   p->isfunction = false;
}

bool isOperator(Program *p)
{
   char c = p->wds[p->cl][0];
   if (strlen(p->wds[p->cl]) == 1){
      switch(c){
         case '*':
         case '/':
         case '+':
         case '-':
            return true;
         default:
            return false;
      }
   }
   return false;
}

bool isVarNum(Program *p)
{
   int i;

   if(strlen(p->wds[p->cl]) == 1 && isupper(p->wds[p->cl][0])){
      return true;
   }

   for (i=0; p->wds[p->cl][i] != '\0'; i++){
      if (!isdigit(p->wds[p->cl][i])){
         return false;
      }
   }

   return true;
}

bool isParVarNum(Program *p)
{
   int i;

   if(strlen(p->wds[p->cl]) == 1 && isupper(p->wds[p->cl][0])){
      return true;
   }

   if(strlen(p->wds[p->cl]) == 2 && p->wds[p->cl][0] == 'P'){
      for (i=1; p->wds[p->cl][i] != '\0'; i++){
         if (!isdigit(p->wds[p->cl][i])){
            return false;
         }
      }
      return true;
   }

   for (i=0; p->wds[p->cl][i] != '\0'; i++){
      if (!isdigit(p->wds[p->cl][i]) && (p->wds[p->cl][i] != 46) && (p->wds[p->cl][i] != '-')){
         return false;
      }
   }

   return true;
}

void Push(Pstack *s, int n)
{
   Elem *e;
   e = (Elem *)malloc(sizeof(Elem));
   e->i = n;
   e->prev = s->tp;

   s->tp = e;
   s->numelems++;
}

int Pop(Pstack *s)
{
   Elem *tmp;
   int retv;

   if (s->numelems == 0){
      ERROR("? Attempted to pop from empty stack ?", __LINE__)
   }

   retv = s->tp->i;
   tmp = s->tp;
   s->tp = s->tp->prev;
   free(tmp);

   s->numelems--;
   return retv;
}

int Calculate(Program *p, int a, int b)
{
   char c = p->wds[p->cl][0];
   switch(c){
      case '*':
         return (a*b);
      case '/':
         return (a/b);
      case '+':
         return (a+b);
      case '-':
         return (a-b);
      default:
         return 0;
   }
}

void Move(Program *p)
{
   double x2, y2, angle = PI * p->t.dir / 180.0;

   x2 = p->t.x + cos(angle)*p->currvar*2;
   y2 = p->t.y - sin(angle)*p->currvar*2;

   Neill_SDL_SetDrawColour(p->swin, rand()%SDL_8BITCOLOUR,
                                rand()%SDL_8BITCOLOUR,
                                rand()%SDL_8BITCOLOUR);

   SDL_RenderDrawLine(p->swin->renderer, round(p->t.x), round(p->t.y) \
                                       , round(x2), round(y2));

   p->t.x = x2;
   p->t.y = y2;

   Neill_SDL_UpdateScreen(p->swin);

   Neill_SDL_Events(p->swin);

   if(p->swin->finished){
      ERROR("? PROGRAM ABORTED ?", __LINE__)
   }
}

void VarSet(Program *p, int index){
   if (p->isfunction){
      p->currfunc->locvars[index] = p->currvar;
   }
   else{
      p->vars[index] = p->currvar;
   }
}

void VarGet(Program *p, int index){
   if (p->isfunction){
      p->currvar = p->currfunc->locvars[index];
   }
   else{
      p->currvar = p->vars[index];
   }
}

int round(double num)
{
   if (num - (double) (int) num < 0.5){
      return (int) num;
   }
   else{
      return (int) num + 1;
   }
}

void ParseInstructionList(Program *p)
{
   if (strsame(p->wds[p->cl], "}")){
      return;
   }
   ParseInstruction(p);
   p->cl = p->cl + 1;
   ParseInstructionList(p);
}

void ParseInstruction(Program *p)
{
   if (strsame(p->wds[p->cl], "FD")){
      p->cl += 1;
      ParVarNum(p);
      return;
   }
   else if (strsame(p->wds[p->cl], "LT")){
      p->cl += 1;
      ParVarNum(p);
      return;
   }
   else if (strsame(p->wds[p->cl], "RT")){
      p->cl += 1;
      ParVarNum(p);
      return;
   }
   else if (strsame(p->wds[p->cl], "SET")){
      p->cl += 1;
      ParseSet(p);
      return;
   }
   else if (strsame(p->wds[p->cl], "DO")){
      p->cl += 1;
      ParseDo(p);
      return;
   }

   ERROR("? Invalid instruction given ?",p->cl)
}

void ParseSet(Program *p)
{
   Var(p);

   p->cl += 1;

   if (!strsame(p->wds[p->cl], ":=")){
      ERROR("? Invalid symbol after variable, use ':=' ?",p->cl)
   }

   p->cl += 1;

   ParsePolish(p);
}

void ParsePolish(Program *p)
{
   if (strsame(p->wds[p->cl],";")){
      return;
   }
   if (!isOperator(p) && !isParVarNum(p)){
      ERROR("? Not an operator or variable ?",p->cl)
   }

   p->cl += 1;
   ParsePolish(p);
}

void ParseDo(Program *p)
{
   /* Get array key for input variable */
   Var(p);
   p->cl += 1;

   /* Check Syntax */
   if (!strsame(p->wds[p->cl], "FROM")){
      ERROR("? Invalid instruction after variable, use 'FROM' ?",p->cl)
   }
   p->cl += 1;

   /* Get Initial Value for Variable */
   ParVarNum(p);
   p->cl += 1;

   /* Check Syntax */
   if (!strsame(p->wds[p->cl], "TO")){
      ERROR("? Invalid instruction after variable or number, use 'TO' ?",p->cl)
   }
   p->cl += 1;

   /* Get limiting value for variable */
   ParVarNum(p);
   p->cl += 1;

   /* Check Syntax */
   if (!strsame(p->wds[p->cl], "{")){
      ERROR("? Invalid symbol after loop start, use '{' ?",p->cl)
   }
   p->cl += 1;

   ParseInstructionList(p);
}

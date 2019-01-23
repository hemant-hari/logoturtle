#include "neillsdl2.h"

void Prog(Program *p)
{
   if (!strsame(p->wds[p->cl], "{")){
      ERROR("? No opening curly brace '{' ?",p->cl)
   }
   p->cl = p->cl + 1;
   InstructionList(p);
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
      VarNum(p);
      Move(p);
      return;
   }
   else if (strsame(p->wds[p->cl], "LT")){
      p->cl += 1;
      VarNum(p);
      p->t.dir -= p->currvar;
      return;
   }
   else if (strsame(p->wds[p->cl], "RT")){
      p->cl += 1;
      VarNum(p);
      p->t.dir += p->currvar;
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

   p->vars[charv] = p->currvar;
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
      if (!isdigit(p->wds[p->cl][i]) && (p->wds[p->cl][i] != 46)) {
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

   VarNum(p);
   p->cl += 1;
   Push(&stk, p->currvar);

   while (!strsame(p->wds[p->cl],";")){
      if (isVarNum(p)){
         VarNum(p);
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
   VarNum(p);
   p->cl += 1;
   p->vars[charv] = p->currvar;

   /* Check Syntax */
   if (!strsame(p->wds[p->cl], "TO")){
      ERROR("? Invalid instruction after variable or number, use 'TO' ?",p->cl)
   }
   p->cl += 1;

   /* Get limiting value for variable */
   VarNum(p);
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
   modval = (limitval < p->vars[charv]) ? -1 : 1 ;

   while (p->vars[charv] - limitval < 0.0001){
      p->cl = retl;
      InstructionList(p);
      p->vars[charv] += modval;
   }
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
   double x2, y2, angle = 2 * PI * p->t.dir / 360;

   x2 = p->t.x - cos(angle)*p->currvar*2;
   y2 = p->t.y - sin(angle)*p->currvar*2;

   Neill_SDL_SetDrawColour(p->swin, rand()%SDL_8BITCOLOUR,
                                rand()%SDL_8BITCOLOUR,
                                rand()%SDL_8BITCOLOUR);

   SDL_RenderDrawLine(p->swin->renderer, (int) p->t.x, (int)p->t.y, (int)x2, (int)y2);

   p->t.x = x2;
   p->t.y = y2;

   Neill_SDL_UpdateScreen(p->swin);

   Neill_SDL_Events(p->swin);

   if(p->swin->finished){
      ERROR("? PROGRAM ABORTED ?", __LINE__)
   }
}

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
      return;
   }
   else if (strsame(p->wds[p->cl], "LT")){
      p->cl += 1;
      VarNum(p);
      return;
   }
   else if (strsame(p->wds[p->cl], "RT")){
      p->cl += 1;
      VarNum(p);
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

   printf("%s\n", p->wds[p->cl]);
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
   Var(p);

   p->cl += 1;

   if (!strsame(p->wds[p->cl], ":=")){
      ERROR("? Invalid symbol after variable, use ':=' ?",p->cl)
   }

   p->cl += 1;

   Polish(p);
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
      if (!isdigit(p->wds[p->cl][i]) && (p->wds[p->cl][i] != 46)){
         ERROR("? Value after instruction not an integer ?",p->cl)
      }
   }
   p->currvar = atof(p->wds[p->cl]);
   if (p->currvar - 0 < 0.00001){
      printf("? Value for instruction is 0, is this intentional ?\n");
   }
}

void Polish(Program *p)
{
   if (strsame(p->wds[p->cl],";")){
      return;
   }
   if (!isOperator(p) && !isVarNum(p)){
      ERROR("? Not an operator or variable ?",p->cl)
   }

   p->cl += 1;
   Polish(p);
}

void Do(Program *p)
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
   VarNum(p);
   p->cl += 1;

   /* Check Syntax */
   if (!strsame(p->wds[p->cl], "TO")){
      ERROR("? Invalid instruction after variable or number, use 'TO' ?",p->cl)
   }
   p->cl += 1;

   /* Get limiting value for variable */
   VarNum(p);
   p->cl += 1;

   /* Check Syntax */
   if (!strsame(p->wds[p->cl], "{")){
      ERROR("? Invalid symbol after loop start, use '{' ?",p->cl)
   }
   p->cl += 1;

   InstructionList(p);
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

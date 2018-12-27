void Prog(Program *p)
{
   if (!strsame(p->wds[p->cl], "{")){
      ERROR("? No opening curly brace '{' ?")
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
   if (strsame(p->wds[p->cl], "LT")){
      p->cl += 1;
      VarNum(p);
      return;
   }
   if (strsame(p->wds[p->cl], "RT")){
      p->cl += 1;
      VarNum(p);
      return;
   }
   if (strsame(p->wds[p->cl], "SET")){
      p->cl += 1;
      Set(p);
      return;
   }

   ERROR("? Invalid instruction given ?")
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
      ERROR("? Invalid symbol after variable, use ':=' ?")
   }

   p->cl += 1;

   /*Polish(p);*/
   VarNum(p);

   p->vars[charv] = p->currvar;
}

int Var(Program *p)
{
   int charval;
   if (strlen(p->wds[p->cl]) != 1){
      if (!isupper(p->wds[p->cl][0])){
         ERROR("? Variable not a single capital letter ?")
      }
   }
   charval = p->wds[p->cl][0] - CTOINT;

   return charval;
}

void Num(Program *p)
{
   int i;

   for (i = 0; p->wds[p->cl][i] != '\0'; i++){
      if (!isdigit(p->wds[p->cl][i])){
         ERROR("? Value after instruction not an integer ?")
      }
   }
   p->currvar = atoi(p->wds[p->cl]);
   if (p->currvar == 0){
      printf("? Value for instruction is 0, is this intentional ?\n");
   }
}

void Polish(Program *p)
{
   VarNum(p);
   p->cl += 1;


}

void InitStack(Pstack *s)
{
   s->tp = (Elem *)calloc(1, sizeof(Elem));
   s->tp->prev = NULL;
   s->numelems = 0;
}

void Push(Pstack *s, int n)
{
   Elem *e;
   if (s->numelems == 0){
      s->tp->i = n;
      return;
   }

   e = (Elem *)malloc(sizeof(Elem));
   e->i = n;
   e->prev = s->tp;

   s->tp = e;
   s->numelems++;
}

int Pop(Stack *s)
{
   Elem *tmp;
   int retv = s->tp->i;

   if (numelems == 0){
      ERROR("? Attempted to pop from empty stack ?")
   }

   tmp = s->tp;
   s->tp = s->tp->prev;
   free(tmp);

   s->numelems--;
   return retv;
}

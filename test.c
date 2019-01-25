#include "neillsdl2.h"

#define TEST_ILIST1 "testilist.turt"

void test_all(void);
void prog_init(Program *p);
void read_in(Program *p, char *v);

int main(void)
{
   test_all();

   return 0;
}

void test_all(void)
{
   Program prog;
   SDL_Simplewin sw;

   prog_init(&prog);
   read_in(&prog, TEST_ILIST1);
   Neill_SDL_Init(&sw);
   prog.swin = &sw;
   InstructionList(&prog);

   while(!sw.finished){
      Neill_SDL_Events(&sw);
   }

   SDL_Quit();
   atexit(SDL_Quit);
}

void prog_init(Program *p)
{
   int i;

   p->cl = 0;
   for(i=0; i<MAXNUMTOKENS; i++){
      p->wds[i][0] = '\0';
   }

   p->t.x = WWIDTH / 2;
   p->t.y = WHEIGHT / 2;
   p->t.dir = 90.0;
   p->currvar = 0;
}

void read_in(Program *p, char *v)
{
   FILE* fp;
   int i;

   if(!(fp = fopen(v, "r"))){
      fclose(fp);
      fprintf(stderr, "Cannot open %s\n", TEST_ILIST1);
      exit(2);
   }
   i=0;
   while(fscanf(fp, "%s", p->wds[i++])==1 && i<=MAXNUMTOKENS){
      if (i>=MAXNUMTOKENS){
         ERROR("? Too many phrases in input code ?", 300)
      }
   }
   fclose(fp);
}

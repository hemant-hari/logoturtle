#include "neillsdl2.h"

void prog_init(Program *p);

int main(int argc, char **argv)
{
   int i;
   FILE *fp;
   Program prog;
   SDL_Simplewin sw;

   /*test_all();*/

   if (argc != 2){
      fprintf(stderr, "ERROR: Incorrect number of arguments to program\n");
      exit(2);
   }

   prog_init(&prog);

   if(!(fp = fopen(argv[1], "r"))){
      fclose(fp);
      fprintf(stderr, "Cannot open %s\n", argv[1]);
      exit(2);
   }
   i=0;
   while(fscanf(fp, "%s", prog.wds[i++])==1 && i<MAXNUMTOKENS){
      assert(i<MAXNUMTOKENS);
   }

   Neill_SDL_Init(&sw);
   prog.swin = &sw;

   Prog(&prog);

   while(!sw.finished){
      Neill_SDL_Events(&sw);
   }

   SDL_Quit();
   atexit(SDL_Quit);

   fclose(fp);

   return 0;
}

void prog_init(Program *p)
{
   int i;

   p->cl = 0;
   for(i=0; i<MAXNUMTOKENS; i++){
      p->wds[i][0] = '\0';
   }

   for(i=0; i<NUMVARS; i++){
      p->vars[i] = 0;
   }

   p->t.x = WWIDTH / 2;
   p->t.y = WHEIGHT / 2;
   p->t.dir = 90.0;
   p->currvar = 0;
}

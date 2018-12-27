int main(int argc, char **argv)
{
   int i;
   FILE *fp;
   Program prog;

   if (argc != 2){
      fprintf(stderr, "ERROR: Incorrect number of arguments to program\n");
      exit(2);
   }

   prog.cl = 0;
   for(i=0; i<MAXNUMTOKENS; i++){
      prog.wds[i][0] = '\0';
   }

   if(!(fp = fopen(argv[1], "r"))){
      fprintf(stderr, "Cannot open %s\n", argv[1]);
      exit(2);
   }
   i=0;
   while(fscanf(fp, "%s", prog.wds[i++])==1 && i<MAXNUMTOKENS){
      assert(i<MAXNUMTOKENS);
   }

   Prog(&prog);
   printf("Parsed OK\n");

   return 0;
}
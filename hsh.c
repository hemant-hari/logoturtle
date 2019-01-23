dic* dic_init(int size)
{
   dic* init;

   init = (dic *) malloc(sizeof(dic));
   if (init == NULL){
      fprintf(stderr, "Could not allocate memory for dictionary\n");
      exit(2);
   }

   init->sz = INITPRIME;
   init->wordsz = size;
   init->occ = 0;
   init->entries = init_table(init->sz);

   return init;
}

void dic_insert(dic* s, char* v, int nump, int stl)
{
   Entry* input;
   int location, new_sz, i;
   double loadfactor = (((double) s->occ) / ((double) s->sz));

   input = init_entry(v, s->wordsz);
   input->numparams = nump;
   input->stl = stl;

   for (i=0; i<MAXNUMPARAMS; i++){
      input->params[i] = 0;
   }
   for (i=0; i<NUMVARS; i++){
      input->locvars[i] = 0;
   }

   /* If the load factor of the table is too high, rehash the table */
   if (loadfactor>MAXLOAD){
      new_sz = s->sz;
      while (RESIZE_FCTR * s->sz > new_sz){
         if (newsize(&new_sz) == false){
            printf("The dictionary cannot be made any bigger!\n");
            exit(3);
         }
      }
      rehash_table(s, new_sz);
   }

   /* Find the designated location and insert the entry */
   location = hash(s->sz, v);
   insert_entry(s->entries, input, location);

   s->occ++;
}

bool dic_isin(dic* s, char* v)
{
   int location = hash(s->sz, v);

   if (s->entries[location] != NULL){
      return check_list(s->entries[location], v);
   }

   return false;
}

Entry* func_get(dic* s, char* v)
{
   int location = hash(s->sz, v);

   return get_list(s->entries[location], v);
}

void dic_free(dic** s)
{
   int i;
   dic *dc = *s;

   for (i=0; i < dc->sz; i++){
      if (dc->entries[i] != NULL){
         free_list(dc->entries[i]);
      }
   }

   free(dc->entries);
   free(*s);
   dc = NULL;
}

/* Takes a string and returns a hash of size */
/*5381 & 33 are magic numbers required by the algorithm*/
int hash(unsigned int sz, char *s)
{
   unsigned long hash = 5381;
   int c;

   while((c = *s++)){
      hash = 33 * hash ^ c;
   }

   return (int)(hash%sz);
}

Entry** init_table(int sz)
{
   Entry** table;

   table = (Entry **) calloc(sz, sizeof(Entry *));
   if (table == NULL){
      fprintf(stderr, "Could not allocate memory for table of hashes\n");
      exit(2);
   }

   return table;
}

/* Initialises an 'Entry' struct with a given word and returns a pointer to it */
Entry* init_entry(char* word, int wlen)
{
   Entry* new;
   char* wordcpy;

   new = (Entry *) malloc(sizeof(Entry));
   wordcpy = (char *) calloc(wlen, sizeof(char));

   wordcpy = strcpy(wordcpy, word);
   new->word = wordcpy;

   new->next = NULL;

   return new;
}

void insert_entry(Entry **dst, Entry *entry, int location)
{
   Entry *current;

   if (dst[location] == NULL){
      dst[location] = entry;
   }
   else{
      for(current = dst[location];
            current->next != NULL;
            current = current->next){
         if(strcmp(current->word, entry->word) == 0){
            return;
         }
      }
      current->next = entry;
   }
}

bool newsize(int *currsz)
{
   int divisor, nextprime;

   /* Next prime cannot be even, and current prime must be odd */
   for (nextprime = *currsz+2; nextprime <= INT_MAX - 2; nextprime += 2){
      /* If a number cannot be divided by any number
      less than its square root it is a prime */
      for(divisor=3; divisor < sqrt((double) nextprime); divisor++){
         if (nextprime%divisor == 0){
            nextprime += 2;
         }
      }
      if (divisor > sqrt((double) nextprime)){
         *currsz = nextprime;
         return true;
      }
   }

   printf("Could not generate bigger prime \n");
   return false;
}

/* Moves through initial hash table elementwise and rehashes
   every element to its new hash location in new table. It then
   frees the old table and changes the current dictionary pointer*/
void rehash_table(dic *s, int new_sz)
{
   int i;
   Entry **newtable = NULL;

   printf("Resizing and rehashing from size %d to size %d\n", s->sz, new_sz);

   newtable = init_table(new_sz);

   for(i=0; i < s->sz; i++){
      if (s->entries[i] != NULL){
         rehash_list(s->entries[i], newtable, new_sz);
      }
   }
   /* Free old table */
   free(s->entries);

   /* Update size and point dictionary to new table */
   s->sz = new_sz;
   s->entries = newtable;
}

/* Rehashes all elements in a list to a new table and
   removes current links in linked list */
void rehash_list(Entry *first, Entry **dst, int new_sz)
{
   Entry *nextentry = first->next, *current;

   insert_entry(dst, first, hash(new_sz, first->word));

   for(current = first; nextentry != NULL; current = nextentry){
      insert_entry(dst, current, hash(new_sz, current->word));
      nextentry = current->next;
      current->next = NULL;
   }
}

void print_dic(dic *s)
{
   int i;
   Entry* current;

   printf("size = %d\n", s->sz);
   printf("occ = %d\n", s->occ);

   for(i=0; i<s->sz; i++){
      if (s->entries[i] != NULL){
         printf("%d %s ", i, s->entries[i]->word);
         for(current = s->entries[i]; current->next != NULL; current=current->next){
            printf("%d %s ", i, current->next->word);
         }
         printf("\n");
      }
   }
}

/* Recursively checks if all entries in the linked list contain the given word */
bool check_list(Entry *entry, char *v)
{
   if (strcmp(entry->word, v) == 0){
      return true;
   }

   if (entry->next == NULL){
      return false;
   }

   return check_list(entry->next, v);
}

Entry* get_list(Entry *entry, char *v)
{
   if (strcmp(entry->word, v) == 0){
      return entry;
   }

   if (entry->next == NULL){
      return NULL;
   }

   return get_list(entry->next, v);
}

/* Recursively frees all elements in a list with end
   condition when there are no further elements */
void free_list(Entry *first)
{
   if (first->next == NULL){
      free(first->word);
      free(first);
      return;
   }

   free_list(first->next);

   free(first->word);
   free(first);
}

#include "directory.h"
#include <assert.h>


void directory_create(struct directory *self)
{
  self->data = NULL;
  self->capacity = self->size = 0;
}

/* desalloue les cases mémoires du tableau data*/
void directory_data_destroy(struct directory *self)
{
  assert(self);
  
  for(int i=0 ;i<self->capacity;i++)
    {
      free(self->data[i]);
    }
      
  
}

/* desalloue le tableau data */
void directory_destroy(struct directory *self)
{
  assert(self);
  
  if( self->data )
    {
      directory_data_destroy(self);
      free(self->data);
    }

}

/* random */

int random_int(int min,int max)
{
  return rand()%(max-min)+min;
}


/*return a name randomly
  we have to free the name later !
*/
char* random_name()
{

  const size_t LENGTH = random_int(NAME_LENGTH_MIN, NAME_LENGTH_MAX+1);

  char *name = calloc(LENGTH+1,sizeof(char)); 
  name[LENGTH] = '\0';

  char vowel[] = {'A','E','I','O','U'};
  char consonant[] = {'B', 'C', 'D', 'F', 'G', 'H', 'J', 'L', 'M',
'N', 'P', 'R', 'S', 'T', 'V'};
  
  for(int i=0; i<LENGTH; i++)
    {
      if(i%2 == 1)
	{
	  name[i] = vowel[random_int(0,5)];
	}
      else
	{
	  name[i] = consonant[random_int(0,15)];
	}
    }


  return name;

}

char *random_telephone()
{
  char *telephone = calloc(TELEPHONE_LENGTH+1, sizeof(char));
  telephone[TELEPHONE_LENGTH] = '\0';
  
  for(size_t i = 0; i < TELEPHONE_LENGTH; i++)
    {
      telephone[i] = (char)('0' + random_int(0,10));
    }
  
  return telephone;
}

void directory_data_random(struct directory_data *self)
{
  assert(self);
  char *last_name = random_name();
  char *first_name = random_name();
  char *telephone = random_telephone();

  strcpy(self->last_name, last_name);
  strcpy(self->first_name, first_name);
  strcpy(self->telephone, telephone);
  
  free(last_name);
  free(first_name);
  free(telephone);
}

void directory_random(struct directory *self, size_t n)
{
  assert(self);
  assert(self >= 0);
  
  for(size_t i = 0; i < n; i++)
    {
      struct directory_data *dir_d = malloc(sizeof(struct directory_data));
      directory_data_random(dir_d);

      directory_raw_add(self,dir_d);
    }

  directory_sort(self);
}

/* print function */
void directory_data_print(const struct directory_data *data)
{
  printf("- %s %s: %s\n",data->last_name, data->first_name,data->telephone);
}

void directory_print(const struct directory *self)
{
  assert(self);

  printf("Directory: \n");
  for(int i=0; i<self->size; i++)
    {
      printf("\t%d:",i+1);
      directory_data_print(self->data[i]);
    }
}


int directory_is_full(struct directory *self)
{
  assert(self);
  return (self->size >= self->capacity);
}

void directory_grow(struct directory *self)
{
  assert(self);

  size_t capacity = self->capacity;
  
  if( capacity == 0 )
    {
      capacity = 10;
      self->data = calloc(capacity,sizeof(struct directory_data *));;
    }
  else
    {
      capacity *= 2;
      struct directory_data **tmp = calloc(capacity, sizeof(struct directory_data *));
      memcpy(tmp, self->data, sizeof(struct directory_data*) * self->capacity);

      free(self->data);

      self->data = tmp;
      
    }

  self->capacity = capacity;

}

/* add */
void directory_add(struct directory *self, struct directory_data *data)
{
  assert(self);
  assert(data);
  
  directory_raw_add(self,data);
  directory_insert_sort(self);
}

void directory_raw_add(struct directory *self, struct directory_data *data)
{
  assert(self);
  assert(data);
  
  if( directory_is_full(self) )
    {
      directory_grow(self);
    }

  self->data[self->size] = data;
  self->size++;
}


/* search */

void directory_search(const struct directory *self, const char *last_name)
{

  for(size_t i=0 ; i < self->size; i++)
    {
      if( strcmp( self->data[i]->last_name, last_name ) == 0 )
	{
	  directory_data_print(self->data[i]);
	}
    }
}

void directory_search_opt(const struct directory *self, const char *last_name)
{
  assert(self);
  assert(self->size >= 0);
  
  if( self->size != 0 )
    {
      directory_search_opt_partial(self, last_name,0,self->size-1);
    }
}

void directory_search_opt_partial(const struct directory *self, const char *last_name, size_t begin, size_t end)
{
  assert(self);
  assert(begin >= 0 && end < self->size);

  if(begin > end ){ return; }
  
  int found = 0;
  size_t medium = (begin + end + 1 )/2;

  if( strcmp(self->data[medium]->last_name, last_name) > 0)
    {
      if( medium - 1 > begin )
	{
	  directory_search_opt_partial(self,last_name, begin, medium-1);
	}
    }
  else if( strcmp(self->data[medium]->last_name, last_name) < 0)
    {
      directory_search_opt_partial(self,last_name, medium+1, end);    
    }
  else
    {
      found = 1;
      size_t i = medium;

      
      while( i < self->size && strcmp(self->data[i]->last_name, last_name) == 0  )
	{
	  directory_data_print(self->data[i]);
	  i++;
	}


      i = medium-1;
      
      while( i + 1 > 0 && strcmp(self->data[i]->last_name, last_name) == 0)
	{
	  directory_data_print(self->data[i]);
	  i--;
	}

    }
  
  if( !found && begin == end - 1)//only on the last iteration
    {
      printf("Can't find %s in the database !\n", last_name);
    }
}


/* sort */
///TODO tri opti combinaison
void directory_sort(struct directory *self)
{
  //directory_insert_sort(self);
  directory_quick_sort(self);
}


void directory_quick_sort(struct directory *self)
{
  assert(self);
  assert(self->size >= 0);

  if( self->size != 0 )
    {
      directory_quick_sort_partial(self,0,self->size-1);
    }
}


void directory_quick_sort_partial(struct directory *self, size_t begin, size_t end)
{	
  if(begin < end)
    {
      size_t pivot = directory_quick_sort_partition(self,begin,end);
      
      if( pivot > begin ) 
	{
	  directory_quick_sort_partial(self, begin, pivot-1);
	}

      if( pivot < end )
	{
	  directory_quick_sort_partial(self, pivot+1, end);
	}
	
    }
}

void directory_swap(struct directory *self, size_t a, size_t b)
{
  struct directory_data *tmp = self->data[a];
  self->data[a] = self->data[b];
  self->data[b] = tmp;
}

size_t directory_quick_sort_partition(struct directory *self, size_t begin, size_t end)
{
  
  size_t pivot_index = begin;
  const char* pivot = self->data[pivot_index]->last_name;
  directory_swap(self,pivot_index,end);
  size_t up = begin;

  for(size_t i = begin; i < end; i++)
    {
      if( strcmp(self->data[i]->last_name, pivot) < 0 )
	{
	  directory_swap(self,i,up);
	  up++;
	} 
    }

  directory_swap(self,up,end);

  return up;
}
 
 
void directory_insert_sort(struct directory *self)
{
  assert(self);

  if(self->size != 0 )
    {
      directory_insert_sort_partial(self,0,self->size - 1);
    }
}

void directory_insert_sort_partial(struct directory *self,size_t begin, size_t end)
{
  assert(self);
  assert(end+1 >= begin);
  assert( begin >= 0 );
  assert( end <= self->size );
  
  if( begin == end )
    {
      return;
    }

  
  for( size_t i = begin+1; i <= end; i++)
    {
      struct directory_data* tmp = self->data[i];
      size_t j = i;

      while(j > 0 && strcmp( self->data[j-1]->last_name, tmp->last_name ) > 0 )
	{
	  self->data[j]= self->data[j-1];
	  --j;
	}

      self->data[j] = tmp;
    }
  
}



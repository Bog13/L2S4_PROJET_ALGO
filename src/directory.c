/**
 * Algorithmique et structures de données 
 * 
 * PROJET DATABASE
 *
 * Licence 2 Informatique 2014-2015
 *
 * @author DUMONTET Sylvain 
 * @author OSSETE GOMBE Bérenger alias bog
 *
 * DIRECTORY
 * directory management 
 *
 **/

#include "directory.h"
#include <assert.h>

/* CREATE */

//create a directory and initialize it
void directory_create(struct directory *self)
{
  assert(self);
  
  self->data = NULL;
  self->capacity = self->size = 0;
}

/* DESTROY */

//free data content 
void directory_data_destroy(struct directory *self)
{
  assert(self);
  
  for(int i=0 ;i<self->capacity;i++)
    {
      free(self->data[i]);
    }
}

//free all data of a directory  
void directory_destroy(struct directory *self)
{
  assert(self);
  
  if( self->data )
    {
      directory_data_destroy(self);
      free(self->data);
    }
}

/* RANDOM */

//return if the name is correct
int name_is_correct(char* name)
{
  size_t length = strlen(name);
  
  for(size_t i = 0; i< length; i++)
    {
      if( !(isalpha(name[i]) || name[i] == ' ' || name[i] == '-') )
	{
	  return 0;
	}
    }

  return (length >= NAME_LENGTH_MIN && length <= NAME_LENGTH_MAX);
}

//return if the phone number is correct
int telephone_is_correct(char *tel)
{
  size_t length = strlen(tel);
  
  for(size_t i = 0; i< length; i++)
    {
      if( !(tel[i] >= '0' && tel[i] <= '9') )
	{
	  return 0;
	}
    }
  
  return (length == TELEPHONE_LENGTH);
}

//random number between min and max-1 
int random_int(int min,int max)
{
  assert(max > min);
  return rand()%(max-min)+min;
}

//return a random name 
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

//return a random telephone number 
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

//fill a directory_data 
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

//fill a directory with n random directory_data 
void directory_random(struct directory *self, size_t n)
{
  assert(self);
  assert(n >= 0);
  
  for(size_t i = 0; i < n; i++)
    {
      struct directory_data *dir_d = malloc(sizeof(struct directory_data));
      directory_data_random(dir_d);

      directory_raw_add(self,dir_d);
    }

  directory_sort(self);
}

/* TEST */

//return 1 if the directory is full, else return 0
int directory_is_full(struct directory *self)
{
  assert(self);
  return (self->size >= self->capacity);
}

/* PRINT */

//print a directory_data on stdout
void directory_data_print(const struct directory_data *data)
{
  assert(data);
  printf("- %s %s: %s\n",data->last_name, data->first_name,data->telephone);
}

//print all the directory_data of a directory on stdout
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

/* ADD */

//double the size of the directory or allocate it
void directory_grow(struct directory *self)
{
  assert(self);

  size_t capacity = self->capacity;
  
  if( capacity == 0 )//allocate
    {
      capacity = 10;
      self->data = calloc(capacity,sizeof(struct directory_data *));;
    }
  else//double the size
    {
      capacity *= 2;
      struct directory_data **tmp = calloc(capacity, sizeof(struct directory_data *));
      memcpy(tmp, self->data, sizeof(struct directory_data*) * self->capacity);

      free(self->data);

      self->data = tmp;
      
    }

  self->capacity = capacity;

}

//add a directory_data in a sorted directory
void directory_add(struct directory *self, struct directory_data *data)
{
  assert(self);
  assert(data);
  
  directory_raw_add(self,data);
  directory_insert_sort(self);
}

//add a directory_data in a directory
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


/* SEARCH */

//print all the directory_data matching with last_name in linear time
void directory_search(const struct directory *self, const char *last_name)
{
  assert(self);
  assert(last_name);
  
  int found = 0;
  
  for(size_t i=0 ; i < self->size; i++)
    {
      if( strcmp( self->data[i]->last_name, last_name ) == 0 )
	{
	  directory_data_print(self->data[i]);
	  found = 1;
	}
    }

  if( !found )//only on the last iteration
    {
      printf("Can't find %s in the database !\n", last_name);
    }
}

//print all the directory_data matching with last_name in a dichotomic way
void directory_search_opt(const struct directory *self, const char *last_name)
{
  assert(self);
  assert(self->size >= 0);
  assert(last_name);
  
  if( self->size != 0 )
    {
      directory_search_opt_partial(self, last_name,0,self->size-1);
    }
}

//print all the directory_data matching with last_name from begin to end
void directory_search_opt_partial(const struct directory *self, const char *last_name, size_t begin, size_t end)
{
  assert(self);
  assert(last_name);
  assert(begin >= 0);
  assert(end < self->size);
  assert(begin <= end);

  int found = 0;
  size_t medium = (begin + end)/2;

  //one remaining
  if(begin == end)
    {
      if( strcmp(self->data[begin]->last_name, last_name) == 0 )
	{
	  directory_data_print(self->data[begin]);
	  found = 1;
	}
    }
  //last_name belongs to the first half of self->data
  else if( strcmp(self->data[medium]->last_name, last_name) > 0 )
    {
      directory_search_opt_partial(self,last_name, begin, medium);
    }
  //last_name belongs to the second half of self->data
  else if( strcmp(self->data[medium]->last_name, last_name) < 0 )
    {
      directory_search_opt_partial(self,last_name, medium+1, end);    
    }
  //last_name found 
  else
    { 
      found = 1;
      size_t i = medium;

      //print directory_data matching with last_name AFTER the medium value
      while( i < self->size && strcmp(self->data[i]->last_name, last_name) == 0  )
	{
	  directory_data_print(self->data[i]);
	  i++;
	}

      i = medium-1;
      //print directory_data matching with last_name BEFORE the medium value
      while( i + 1 > 0 && strcmp(self->data[i]->last_name, last_name) == 0)
	{
	  directory_data_print(self->data[i]);
	  i--;
	}
    }
  //  printf("found: %d\n",found);
  if( !found && begin == end)//only on the last iteration
    {
      printf("Can't find %s in the database !\n", last_name);
    }
}


/* SORT */

//sort a directory with the apropriate sort algorithm
void directory_sort(struct directory *self)
{
  
  assert(self);
  
  if (self->size > MIN_QUICK_SORT)
    {
      directory_quick_sort(self);
    }
  else
    {
      directory_insert_sort(self);
    }
}

//sort a directory using quick sort
void directory_quick_sort(struct directory *self)
{
  assert(self);
  assert(self->size >= 0);

  if( self->size != 0 )
    {
      directory_quick_sort_partial(self,0,self->size-1);
    }
}

//sort a directory between begin and end, using insert and quick sorts
void directory_quick_sort_partial(struct directory *self, size_t begin, size_t end)
{
  assert(self);
  
  if(begin < end)
    {
      if (end-begin > MIN_QUICK_SORT)//a lot of values: quick sort
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
      else//MIN_QUICK_SORT values (or less) remains: insert sort
	{
	  directory_insert_sort_partial(self,begin,end);
	}
    }
}

//swap self->data[a] and self->data[b]
void directory_swap(struct directory *self, size_t a, size_t b)
{
  assert(self);
  assert(self->data[a]);
  assert(self->data[b]);
  
  struct directory_data *tmp = self->data[a];
  self->data[a] = self->data[b];
  self->data[b] = tmp;
}

//move all the directory_data depending with the comparison with a choosen pivot
size_t directory_quick_sort_partition(struct directory *self, size_t begin, size_t end)
{
  assert(self);
  assert(begin <= end);
  assert(begin >= 0);
  assert(end <= self->size);
  
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

//sort a directory using insert sort
void directory_insert_sort(struct directory *self)
{
  assert(self);

  if(self->size != 0 )
    {
      directory_insert_sort_partial(self,0,self->size - 1);
    }
}

//sort a directory using insert sort between begin and end
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

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

      directory_add(self,dir_d);
    }
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


void directory_add(struct directory *self, struct directory_data *data)
{
  assert(self);
  
  if( directory_is_full(self) )
    {
      directory_grow(self);
    }

  self->data[self->size] = data;
  self->size++;
}

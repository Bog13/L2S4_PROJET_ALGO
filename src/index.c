#include <index.h>

/* init destroy */

void index_bucket_create(struct index_bucket *self)
{
  assert(self);
  self->data = NULL;
  self->next = NULL;
}

void index_bucket_destroy(struct index_bucket *self)
{
  if( self != NULL )
    {
      index_bucket_destroy( self->next );
      free(self);
    }
}


void index_create(struct index *self, index_hash_func_t func)
{
  assert(self);
  
  self->count = 0;
  self->size = 0;
  self->func = func;
  self->buckets = NULL;
}

void index_destroy(struct index *self)
{
  assert(self);

  for(size_t i=0; i < self->size; i++)
    {
      if( self->buckets[i] )
	{
	  index_bucket_destroy(self->buckets[i]);
	}
    }
  free(self->buckets);
}


/* add */
struct index_bucket *index_bucket_add(struct index_bucket *self, const struct directory_data *data)
{
  assert(data);

  if( self == NULL )
    {
      struct index_bucket *new_bucket = malloc(sizeof(struct index_bucket));
      index_bucket_create(new_bucket);
      new_bucket->data = data;

      return new_bucket;
    }
  else if( self->data == NULL )
    {
      self->data = data;
      self->next = NULL;

      return self;
    }
  else
    {
        struct index_bucket *new_bucket = malloc(sizeof(struct index_bucket));
	index_bucket_create(new_bucket);
	new_bucket->data = data;
  
      if( self->next == NULL )
	{
	  new_bucket->next = NULL;
	  self->next = new_bucket;

	}
      else
	{
	  new_bucket->next = self->next;
	  self->next = new_bucket;
	}

      return new_bucket;
    }


}

void index_add(struct index *self, const struct directory_data *data)
{
  assert(self);
  assert(data);

  if(self->buckets == NULL || ((float)self->count/(float)self->size) >= 0.5)
    {
      index_rehash(self);
    }

  size_t i = self->func(data) % self->size;

  #ifdef DEBUG_MSG
  printf("--> Ajout de  %s à l'indice %zd\n", data->first_name, i);
  #endif
  
  if( self->buckets[i] == NULL)
    {
      self->buckets[i] = malloc(sizeof(struct index_bucket));
      index_bucket_create(self->buckets[i]);
    }
  
  index_bucket_add( self->buckets[i], data );
  
  self->count++;

}

/* print */
void index_bucket_print(const struct index_bucket *self)
{
  if(self != NULL)
    {
      if(self->data)
	{
	  directory_data_print(self->data);
	}
      
      index_bucket_print(self->next);
    }
  
}


void index_print(const struct index *self)
{
  assert(self);

  for(size_t i = 0; i<self->size; i++)
    {
      printf("%zd: ",i);

      if(self->buckets[i] == NULL)
	{
	  printf("x \n");
	}

      index_bucket_print(self->buckets[i]);
      
    }
}

/* hash */

///TODO 32bit
size_t fnv_hash(const char *key)
{

  //const size_t FNV_OFFSET_BASIS = 16777619;//0xcbf29ce484222325;      32 bits
  //const size_t FNV_PRIME = 2166136261;//0x100000001b3;

  const size_t FNV_OFFSET_BASIS = 0xcbf29ce484222325;
  const size_t FNV_PRIME = 0x100000001b3;
  
  const size_t BYTES = strlen(key);
  
  size_t hash = FNV_OFFSET_BASIS; 
  
  for(size_t i=0; i < BYTES; i++)
    {
      hash |= key[i];
      hash = hash * FNV_PRIME;
    }

  return hash;
  
}

size_t index_first_name_hash(const struct directory_data *data)
{
  return fnv_hash(data->first_name);
}

void index_bucket_rehash(struct index_bucket* self,struct index* dest)
{
  if( self != NULL )
    {
      index_bucket_rehash(self->next, dest);
      
      index_add(dest, self->data);
      free(self);
    }
}

void index_rehash(struct index *self)
{
  #ifdef DEBUG_MSG
  printf("REHASH ! \n");
  #endif
  
  size_t size = self->size;
	
  if( self->buckets == NULL )
    {
      size = 10;
      self->buckets = calloc(size, sizeof(struct index_bucket *));
      
      for(size_t i=0; i < size; i++)
	{
	  self->buckets[i] = NULL;
	}
    }
  else
    {
      size *= 2;

     /* create tmp */
     struct index *tmp = calloc(size, sizeof(struct index));
     index_create(tmp, self->func);
     
     /* create tmp->buckets*/
     tmp->buckets = calloc(size, sizeof(struct index_bucket *));
     tmp->size = size;
     
     for( size_t i = 0; i < size; i++ )
     	{
     	  tmp->buckets[i] = NULL;
	}
     
     
     /* add */
     for(size_t i = 0; i<self->size; i++)
       {
	 if( self->buckets[i] != NULL  )
	   {
	     index_bucket_rehash(self->buckets[i],tmp);
	   }
     
       }

     free(self->buckets);
     
     self->buckets = tmp->buckets;

     free(tmp);
    }

  self->size = size;


}

/* import */
void index_fill_with_directory(struct index *self, const struct directory *dir)
{
  assert(self);
  assert(dir);
  
  for(size_t i = 0; i < dir->size; i++)
    {
      index_add(self, dir->data[i]);
    }
}

#include <index.h>

/* INIT */

//initialize an index_bucket
void index_bucket_create(struct index_bucket *self)
{
  assert(self);
  self->data = NULL;
  self->next = NULL;
}

//initialize an index
void index_create(struct index *self, index_hash_func_t func)
{
  assert(self);
  
  self->count = 0;
  self->size = 0;
  self->func = func;
  self->buckets = NULL;
}


/* DESTROY */

//free a whole list of index_bucket from self
void index_bucket_destroy(struct index_bucket *self)
{
  if( self != NULL )
    {
      index_bucket_destroy( self->next );
      free(self);
    }
}

//free all index_bucket of self
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


/* ADD */

//add a directory_data between self and self->next in an index_bucket
struct index_bucket *index_bucket_add(struct index_bucket *self, const struct directory_data *data)
{
  assert(data);

  if( self == NULL )//create the index_bucket
    {
      struct index_bucket *new_bucket = malloc(sizeof(struct index_bucket));
      index_bucket_create(new_bucket);
      new_bucket->data = data;

      return new_bucket;
    }
  else if( self->data == NULL )//add in place
    {
      self->data = data;
      self->next = NULL;

      return self;
    }
  else//add in the linked list
    {
        struct index_bucket *new_bucket = malloc(sizeof(struct index_bucket));
	index_bucket_create(new_bucket);
	new_bucket->data = data;
  
	if( self->next == NULL )//add at the end of the list
	{
	  new_bucket->next = NULL;
	  self->next = new_bucket;
	}
	else//add after self
	{
	  new_bucket->next = self->next;
	  self->next = new_bucket;
	}

      return new_bucket;
    }
}

//add a directory_data in an index using index's hash function
void index_add(struct index *self, const struct directory_data *data)
{
  assert(self);
  assert(data);
  float collision_ratio = (((float)self->count/(float)self->size));
  
  if(self->buckets == NULL || collision_ratio >= 0.5 )
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

/* PRINT */

//print the content of an index_bucket
void index_bucket_print(const struct index_bucket *self)
{
  assert(self);
  
  if(self != NULL)
    {
      if(self->data)
	{
	  directory_data_print(self->data);
	}
      
      index_bucket_print(self->next);
    }
}

//print the content of an index_bucket
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

/* HASH */

//return the hashcode of key for 32bits systems
size_t fnv_hash_32bits(const char *key)
{
  assert(key);
  
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

//return the hashcode of key for 64bits systems
size_t fnv_hash_64bits(const char *key)
{
  assert(key);
  
  const size_t FNV_OFFSET_BASIS = 16777619;
  const size_t FNV_PRIME = 2166136261;
  
  const size_t BYTES = strlen(key);
  
  size_t hash = FNV_OFFSET_BASIS; 
  
  for(size_t i=0; i < BYTES; i++)
    {
      hash |= key[i];
      hash = hash * FNV_PRIME;
    }

  return hash;
}

//return the hashcode of key
size_t fnv_hash(const char *key)
{
  assert(key);
  
  if ((size_t)-1 > 0xffffffffUL)//> 32bits
    {
      return fnv_hash_64bits(key);
    }
  else//<= 32bits
    {
      return fnv_hash_32bits(key);
    }
}

//return the hashcode of data->first_name
size_t index_first_name_hash(const struct directory_data *data)
{
  assert(data);
  return fnv_hash(data->first_name);
}

//return the hashcode of data->telephone
size_t index_telephone_hash(const struct directory_data *data)
{
  assert(data);
  return fnv_hash(data->telephone);
}

//hash a whole index_bucket list into an index
void index_bucket_rehash(struct index_bucket* self,struct index* dest)
{
  assert(dest);
  
  if( self != NULL )
    {
      index_bucket_rehash(self->next, dest);
      
      index_add(dest, self->data);
      free(self);
    }
}

//grow the size of an index and rehash all the index_bucket in it
void index_rehash(struct index *self)
{
  #ifdef DEBUG_MSG
  printf("REHASH ! \n");
  #endif
  
  size_t size = self->size;
	
  if( self->buckets == NULL )//create a new self->buckets
    {
      size = 10;
      self->buckets = calloc(size, sizeof(struct index_bucket *));
      
      for(size_t i=0; i < size; i++)
	{
	  self->buckets[i] = NULL;
	}
    }
  else//double the size of self->buckets
    {
      size *= 2;

     //create tmp
     struct index *tmp = calloc(size, sizeof(struct index));
     index_create(tmp, self->func);
     
     //create tmp->buckets
     tmp->buckets = calloc(size, sizeof(struct index_bucket *));
     tmp->size = size;
     
     for( size_t i = 0; i < size; i++ )
     	{
     	  tmp->buckets[i] = NULL;
	}
     
     //rehash self->buckets into tmp 
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

/* IMPORT */

//fill the content of an index with a directory
void index_fill_with_directory(struct index *self, const struct directory *dir)
{
  assert(self);
  assert(dir);
  
  for(size_t i = 0; i < dir->size; i++)
    {
      index_add(self, dir->data[i]);
    }
}

/* SEARCH */

//print all the directory_data matching with first_name in an index
void index_search_by_first_name(const struct index *self, const char *first_name)
{
  assert(self);
  assert(self->size > 0);

  size_t i = fnv_hash(first_name)%self->size;//index of the first_name in the index->buckets
  struct index_bucket* it = self->buckets[i];
  int found = 0;

  while( it != NULL )//read the linked list
    {
      if( strcmp(it->data->first_name, first_name) == 0 )
	{
	  directory_data_print(it->data);
	  found++;
	}
      
      it = it->next;
    }

  if( !found )
    {
      printf("Can't find %s in the database !\n", first_name);
    }
}

//print all the directory_data matching with telephone in an index
void index_search_by_telephone(const struct index *self, const char *telephone)
{
  assert(self);
  assert(self->size > 0);
  
  size_t i = fnv_hash(telephone)%self->size;
  struct index_bucket* it = self->buckets[i];
  int found = 0;

  while( it != NULL )
    {
      if( strcmp(it->data->telephone, telephone) == 0 )
	{
	  directory_data_print(it->data);
	  found++;
	}
      
      it = it->next;
    }

  if( !found )
    {
      printf("Can't find %s in the database !\n", telephone);
    }
}

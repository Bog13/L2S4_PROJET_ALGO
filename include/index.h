#ifndef INDEX_H
#define INDEX_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "directory.h"
#define BIT_OS 32

typedef size_t (*index_hash_func_t)(const struct directory_data *data);

struct index_bucket
{
  const struct directory_data *data;
  struct index_bucket *next;
};

struct index
{
  struct index_bucket **buckets;
  size_t count;
  size_t size;
  index_hash_func_t func;
};


/* init destroy */
void index_bucket_create(struct index_bucket *self);
void index_bucket_destroy(struct index_bucket *self);

void index_create(struct index *self, index_hash_func_t func);
void index_destroy(struct index *self);

/* add */
struct index_bucket *index_bucket_add(struct index_bucket *self, const struct directory_data *data);
void index_add(struct index *self, const struct directory_data *data);

/* print */
void index_bucket_print(const struct index_bucket *self);
void index_print(const struct index *self);

/* hash */
size_t fnv_hash(const char *key);
size_t index_first_name_hash(const struct directory_data *data);

void index_rehash(struct index *self);
void index_bucket_rehash(struct index_bucket* self,struct index* dest);

/* import */
void index_fill_with_directory(struct index *self, const struct directory *dir);

#endif

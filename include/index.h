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
 * INDEX
 * index management 
 *
 **/

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


/* init */
void index_bucket_create(struct index_bucket *self);
void index_create(struct index *self, index_hash_func_t func);

/* destroy */
void index_destroy(struct index *self);
void index_bucket_destroy(struct index_bucket *self);

/* add */
struct index_bucket *index_bucket_add(struct index_bucket *self, const struct directory_data *data);
void index_add(struct index *self, const struct directory_data *data);

/* print */
void index_bucket_print(const struct index_bucket *self);
void index_print(const struct index *self);

/* hash */
size_t fnv_hash(const char *key);
size_t fnv_hash_32bits(const char *key);
size_t fnv_hash_64bits(const char *key);

size_t index_first_name_hash(const struct directory_data *data);
size_t index_telephone_hash(const struct directory_data *data);

void index_rehash(struct index *self);
void index_bucket_rehash(struct index_bucket* self,struct index* dest);

/* import */
void index_fill_with_directory(struct index *self, const struct directory *dir);

/* search */
void index_search_by_first_name(const struct index *self, const char *first_name);
void index_search_by_telephone(const struct index *self, const char *telephone);

#endif

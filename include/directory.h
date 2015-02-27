#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH_MIN 3
#define NAME_LENGTH_MAX 10
#define TELEPHONE_LENGTH 8

struct directory
{
  struct directory_data **data;
  size_t size;
  size_t capacity;
};

struct directory_data
{
  char last_name[NAME_LENGTH_MAX + 1];
  char first_name[NAME_LENGTH_MAX + 1];
  char telephone[TELEPHONE_LENGTH + 1];
};


/*init*/
void directory_create(struct directory *self);
void directory_grow(struct directory *self);
  
/* destroy */
void directory_data_destroy(struct directory *self);
void directory_destroy(struct directory *self);

/* tests */
int directory_is_full(struct directory *self);

/* print */
void directory_data_print(const struct directory_data *data);
void directory_print(const struct directory *self);

/* random name */
char* random_name();
int random_int(int min,int max);

void directory_random(struct directory *self, size_t n);
void directory_data_random(struct directory_data *self);

/* add */
//add data into self and sort self
void directory_add(struct directory *self, struct directory_data *data);
//add data into self without sorting
void directory_raw_add(struct directory *self, struct directory_data *data);

/* search */
void directory_search(const struct directory *self, const char *last_name);
void directory_search_opt(const struct directory *self, const char *last_name);
void directory_search_opt_partial(const struct directory *self, const char *last_name, size_t begin, size_t end);

/* sort */
void directory_swap(struct directory *self, size_t a, size_t b);
void directory_sort(struct directory *self);
void directory_quick_sort(struct directory *self);
void directory_quick_sort_partial(struct directory *self, size_t begin, size_t end);
size_t directory_quick_sort_partition(struct directory *self, size_t begin, size_t end);

void directory_insert_sort_partial(struct directory *self,size_t begin, size_t end);
void directory_insert_sort(struct directory *self);

#endif

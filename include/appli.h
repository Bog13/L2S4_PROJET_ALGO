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
 * APPLI
 * user interface
 *
 **/

#ifndef APPLI_H
#define APLLI_H
#define BUFFER_SIZE 256 //names and telephone numbers MUST'N exceed BUFFER_SIZE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <ctype.h>
#include "index.h"
#include "directory.h"

void clean_newline(char *buf, size_t size);
char* get_entry();
int convert_entry(const char* entry);
suseconds_t get_microseconds();

void print_title(size_t db_size);
void credit_display();
void menu_display();
void appli_main_loop(struct directory *dir, struct index* index_first_name, struct index* index_telephone);
void user_add(struct directory* dir, struct index *ifn, struct index *itel);
void toupper_string(char* str);

#endif

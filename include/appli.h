#ifndef APPLI_H
#define APLLI_H
#define BUFFER_SIZE 256 //todo msg 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>

void clean_newline(char *buf, size_t size);

void menu_display();
char* get_entry();

int convert_entry(const char* entry);

suseconds_t get_microseconds();
#endif

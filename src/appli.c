#include "appli.h"


void clean_newline(char *buf, size_t size)
{
  assert(buf);
  assert(size >= 0);

  size_t i = 0;
  while( i < size )
    {
      if( buf[i] == '\n' )
	{
	  buf[i] = '\0';
	  return;
	}
      i++;
    }
}


void menu_display()
{
  printf("What do you want to do ?\n");
  printf("\t1: Search by last name (not optimised)\n");
  printf("\t2: Search by last name (optimised)\n");
  printf("\t3: Search by first name\n");
  printf("\t4: Search by telephone\n");
  printf("\tq: Quit\n");
  printf("Your choice: ");
}

int convert_entry(const char* entry)
{
  assert(entry);
  
  if( strcmp(entry, "q") == 0 )
    {
      return 0;
    }

  int int_entry = atoi(entry);
  if( int_entry >= 1 && int_entry <= 4 )
    {
      return int_entry;
    }


  return -1;
}


char* get_entry()
{
  char *buffer = calloc(BUFFER_SIZE, sizeof(char));
  
  fgets(buffer, BUFFER_SIZE, stdin);
  clean_newline(buffer,BUFFER_SIZE);
  return buffer;
}

suseconds_t get_microseconds()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  
  return tv.tv_sec * 1000000 + tv.tv_usec;
}





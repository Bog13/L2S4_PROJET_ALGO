#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include "directory.h"
#include "index.h"
#include "appli.h"
#define DEBUG_MSG

int main(int argc, char *argv[])
{
  #ifdef DEBUG_MSG
  printf("\n\t! DEBUG MODE ACTIVATED !\n\n");
  #endif
  /* init */
  #ifndef DEBUG_MSG
  srand(time(NULL));
  #endif
  /* */
  suseconds_t timer = get_microseconds();
  
  //directory
  struct directory *dir = malloc(sizeof(struct directory));
  directory_create(dir);
  directory_random(dir, 50);

  //index first name
  struct index* index_first_name = malloc(sizeof(struct index));
  index_create(index_first_name,index_first_name_hash);
  index_fill_with_directory(index_first_name, dir);

  //index telephone
  struct index* index_telephone = malloc(sizeof(struct index));
  index_create(index_telephone,index_telephone_hash);
  index_fill_with_directory(index_telephone, dir);

  #ifdef DEBUG_MSG
  directory_print(dir);
  printf("----------\n\n");
  #endif
  
  timer = get_microseconds() - timer;

  printf("initialization time %fs\n\n",(float)(timer/1000000.0));
  
  //main loop
  appli_main_loop(dir,index_first_name, index_telephone);
  
  // destroy && free 
  directory_destroy(dir);
  free(dir);
  
  index_destroy(index_first_name);
  free(index_first_name);

  index_destroy(index_telephone);
  free(index_telephone);

  return 0;
}


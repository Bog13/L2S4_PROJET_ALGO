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
 * MAIN
 *
 **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include "directory.h"
#include "index.h"
#include "appli.h"
//#define DEBUG
//#define DEBUG_MSG
#define DATABASE_SIZE 1000000

int main(int argc, char *argv[])
{
  print_title(DATABASE_SIZE);
  
  #ifdef DEBUG_MSG
  printf("\n\t! DEBUG MODE ACTIVATED !\n\n");
  #endif
  
  /* INIT */
  #ifndef DEBUG
  srand(time(NULL));
  #endif
  
  suseconds_t timer = get_microseconds();
  
  /* DIRECTORY */
  struct directory *dir = malloc(sizeof(struct directory));
  directory_create(dir);
  directory_random(dir, DATABASE_SIZE);

  /* INDEX FIRST NAME */
  struct index* index_first_name = malloc(sizeof(struct index));
  index_create(index_first_name,index_first_name_hash);
  index_fill_with_directory(index_first_name, dir);

  /* INDEX TELEPHONE */
  struct index* index_telephone = malloc(sizeof(struct index));
  index_create(index_telephone,index_telephone_hash);
  index_fill_with_directory(index_telephone, dir);

  /* PRINT (DEBUG) */
  #ifdef DEBUG_MSG
  directory_print(dir);
  printf("----------\n\n");
  index_print(index_first_name);
  printf("----------\n\n");
  index_print(index_telephone);
  printf("----------\n\n");
  #endif
  
  timer = get_microseconds() - timer;

  
  
  printf("\tInitialization time: %fs\n\n",(float)(timer/1000000.0));
  
  /* MAIN LOOP */
  appli_main_loop(dir,index_first_name, index_telephone);
  
  /* DESTROY && FREE */
  directory_destroy(dir);
  free(dir);
  
  index_destroy(index_first_name);
  free(index_first_name);

  index_destroy(index_telephone);
  free(index_telephone);

  return 0;
}


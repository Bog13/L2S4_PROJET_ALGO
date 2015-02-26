#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "directory.h"
#include "index.h"

int main(int argc, char *argv[])
{
  /* init */
  srand(time(NULL));
  /* */


  printf("########## DATABASE ########## \n");


  struct directory *dir = malloc(sizeof(struct directory));
  directory_create(dir);

  directory_random(dir, 1000);
  directory_print(dir);
  
  directory_destroy(dir);
  free(dir);

  return 0;
}

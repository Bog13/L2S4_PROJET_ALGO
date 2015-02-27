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

  directory_random(dir, 50000);

  /* TEST
  struct directory_data *jb = malloc(sizeof(struct directory_data));
  strcpy(jb->last_name,"BERNARD");
  strcpy(jb->first_name, "Julien");
  strcpy(jb->telephone,"0742424242");
  directory_add(dir, jb);
  
  struct directory_data *jb2 = malloc(sizeof(struct directory_data));
  strcpy(jb2->last_name,"BERNARD");
  strcpy(jb2->first_name, "Juliette");
  strcpy(jb2->telephone,"0729387845");
  directory_add(dir, jb2);
  */
  
  directory_print(dir);

  
  directory_destroy(dir);
  free(dir);

  return 0;
}

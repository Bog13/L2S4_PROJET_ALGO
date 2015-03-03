#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "directory.h"
#include "index.h"

//static void test_directory();
static void test_index();

int main(int argc, char *argv[])
{
  /* init */
  srand(time(NULL));
  /* */


  printf("########## DATABASE ########## \n");
  test_index();

  return 0;
}


static void test_index()
{
  /* directory data exemple */
 //struct directory_data *jb = malloc(sizeof(struct directory_data));
 //strcpy(jb->last_name,"BERNARD");
 //strcpy(jb->first_name, "Julien");
 //strcpy(jb->telephone,"07424242");
 //
 //struct directory_data *jb2 = malloc(sizeof(struct directory_data));
 //strcpy(jb2->last_name,"BERNARD");
 //strcpy(jb2->first_name, "Juliette");
 //strcpy(jb2->telephone,"07293845");
 //
 //struct directory_data *jb3 = malloc(sizeof(struct directory_data));
 //strcpy(jb3->last_name,"BERNARD");
 //strcpy(jb3->first_name, "Julie");
 //strcpy(jb3->telephone,"07293845");

  /* dir */
  struct directory *dir = malloc(sizeof(struct directory));
  directory_create(dir);
  directory_random(dir, 10);

  /* index last_name */
  struct index* idx = malloc(sizeof(struct index));
  index_create(idx,index_first_name_hash);
 
  /* add & import */
  index_fill_with_directory(idx, dir);
  
  /* print */
  directory_print(dir);
  index_print(idx);
  
  /* free */
  directory_destroy(dir);
  free(dir);

  index_destroy(idx);
  free(idx);
 }


/*
  static void test_directory()
  {
  struct directory *dir = malloc(sizeof(struct directory));
  directory_create(dir);

  directory_random(dir, 50000);

  struct directory_data *jb = malloc(sizeof(struct directory_data));
  strcpy(jb->last_name,"BERNARD");
  strcpy(jb->first_name, "Julien");
  strcpy(jb->telephone,"07424242");
  directory_add(dir, jb);
  
  struct directory_data *jb2 = malloc(sizeof(struct directory_data));
  strcpy(jb2->last_name,"BERNARD");
  strcpy(jb2->first_name, "Juliette");
  strcpy(jb2->telephone,"07293845");
  directory_add(dir, jb2);
  
  directory_print(dir);

  
  directory_destroy(dir);
  free(dir);
  }



*/

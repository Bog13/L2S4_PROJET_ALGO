#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include "directory.h"
#include "index.h"
#include "appli.h"

int main(int argc, char *argv[])
{
  /* init */
  srand(time(NULL));
  /* */
  suseconds_t timer = get_microseconds();

  //directory
  struct directory *dir = malloc(sizeof(struct directory));
  directory_create(dir);
  directory_random(dir, 1000000);


  //index first name
  struct index* index_first_name = malloc(sizeof(struct index));
  index_create(index_first_name,index_first_name_hash);
  index_fill_with_directory(index_first_name, dir);

  //index telephone
  struct index* index_telephone = malloc(sizeof(struct index));
  index_create(index_telephone,index_telephone_hash);
  index_fill_with_directory(index_telephone, dir);

  
  // print
  //directory_print(dir);
  //printf("----------\n\n");

  timer = get_microseconds() - timer;

  printf("initialization time %fs\n\n",(float)(timer/1000000.0));

  
  //main loop
  char * entry;
  int running = 1;

  printf("########## DATABASE ########## \n");
  while( running )
    {
      char* search_entry;
      menu_display();
      entry = get_entry();

      int convert = convert_entry(entry);
      int entry_is_ok = (convert >= 0 && convert <= 4);
      running = (convert != 0);
      int is_searching = ( running && entry_is_ok);
      
      if(is_searching)
	{
	  printf("\n---\nSearch: ");

	  search_entry = get_entry();
	}
      
      switch( convert )
	{
	case 0:
	  printf("Goodbye !\n");
	  break;
	  
	case 1:
	  timer =  get_microseconds();
	  directory_search(dir,search_entry);
	  timer =  get_microseconds() - timer;
	  break;

	case 2:
	  timer =  get_microseconds();
	  directory_search_opt(dir,search_entry);
	  timer =  get_microseconds() - timer;
	  break;

	case 3:
	  timer =  get_microseconds();
	  index_search_by_first_name(index_first_name,search_entry);
	  timer =  get_microseconds() - timer;	
	  break;

	case 4:
	  timer =  get_microseconds();
	  index_search_by_telephone(index_telephone,search_entry);
	  timer =  get_microseconds() - timer;
	  break;
	  
	default:
	  printf("erreur saisie\n");
	  break;
	}
      

      if( is_searching )
	{
	  printf("\tsearching time: %.3fms\n---\n\n",(float)(timer/1000.0));
	  free(search_entry);
	}
       
      free(entry);
    }
  
  // destroy && free 
  directory_destroy(dir);
  free(dir);
  
  index_destroy(index_first_name);
  free(index_first_name);

  index_destroy(index_telephone);
  free(index_telephone);

  return 0;
}


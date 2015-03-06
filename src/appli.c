#include "appli.h"

//replace the last \n with \0
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

//print the menu
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

//return the menu entry associated to entry 
int convert_entry(const char* entry)
{
  assert(entry);
  
  if( strcmp(entry, "Q") == 0 )
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

//read stdin 
char* get_entry()
{
  char *buffer = calloc(BUFFER_SIZE, sizeof(char));
  
  fgets(buffer, BUFFER_SIZE, stdin);
  clean_newline(buffer,BUFFER_SIZE);
  return buffer;
}

//get the current time of day in milliseconds
suseconds_t get_microseconds()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

//capitalize a string in place
void toupper_string(char* str)
{
  size_t i = -1;
  
  while(str[++i] != '\0')
    {
      str[i] = toupper(str[i]);
    }
}

//execute the application and manage user inputs
void appli_main_loop(struct directory *dir, struct index* index_first_name, struct index* index_telephone)
{
  char * entry;
  int running = 1;
  suseconds_t timer;
  
  printf("########## DATABASE ########## \n");
  while( running )
    {
      char* search_entry;
      menu_display();
      entry = get_entry();
      toupper_string(entry);
      
      int convert = convert_entry(entry);
      int entry_is_ok = (convert >= 0 && convert <= 4);
      running = (convert != 0);
      int is_searching = ( running && entry_is_ok);
      
      if(is_searching)
	{
	  printf("\n---\nSearch: ");

	  search_entry = get_entry();
	  toupper_string(search_entry);

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
}

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

  printf("\n############ MENU ############ \n");
  printf("What do you want to do ?\n");
  printf("\t1: Search by last name (not optimised)\n");
  printf("\t2: Search by last name (optimised)\n");
  printf("\t3: Search by first name\n");
  printf("\t4: Search by telephone\n");
  printf("\t5: Add a new data\n");
  printf("\tq: Quit\n");
  printf("Your choice: ");
}

//print the title
void print_title(size_t db_size)
{
  printf("\n########## DATABASE ########## \n");
  printf("Welcome to THE epic database !\n\n");
  printf("Please wait until your database of %zd elements is ready.\n", db_size);
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
  if( int_entry >= 1 && int_entry <= 5 )
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

void user_add(struct directory* dir, struct index *ifn, struct index *itel)
{
  struct directory_data *data = malloc(sizeof(struct directory_data));
  //  directory_data_create(data);

  
  printf("Entry first name (length: %d min, %d max): ",NAME_LENGTH_MIN,NAME_LENGTH_MAX);
  char * fn = get_entry();
  printf("\n");
  
  printf("Entry last name (length: %d min, %d max): ",NAME_LENGTH_MIN,NAME_LENGTH_MAX);
  char * ln = get_entry();
  printf("\n");
  
  printf("Entry telephone (length: %d): ", TELEPHONE_LENGTH);
  char * tel = get_entry();
  printf("\n");

  if(name_is_correct(fn) && name_is_correct(ln) && telephone_is_correct(tel))
    {
      toupper_string(fn);
      toupper_string(ln);
      
      strcpy(data->first_name, fn);
      strcpy(data->last_name , ln);
      strcpy(data->telephone, tel);

      directory_add(dir, data);
      index_add(ifn, data);
      index_add(itel, data);
    }
  else
    {
      printf("Invalid name (length: %d min, %d max) or phone number (length: %d)\n",NAME_LENGTH_MIN,NAME_LENGTH_MAX,TELEPHONE_LENGTH);
    }
  
  free(fn);
  free(ln);
  free(tel);

}

//execute the application and manage user inputs
void appli_main_loop(struct directory *dir, struct index* index_first_name, struct index* index_telephone)
{
  char * entry;
  int running = 1;
  suseconds_t timer;

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
	  printf("See you soon !\n");
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

	case 5:
	  user_add(dir,index_first_name, index_telephone);
	  break;
	  
	default:
	  printf("Typing error\n");
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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "tables.h"

const int SYMBOLTBL_NON_UNIQUE = 0;
const int SYMBOLTBL_UNIQUE_NAME = 1;

/*******************************
 * Helper Functions
 *******************************/

void allocation_failed() {
    write_to_log("Error: allocation failed\n");
    exit(1);
}

void addr_alignment_incorrect() {
    write_to_log("Error: address is not a multiple of 4.\n");
}

void name_already_exists(const char* name) {
    write_to_log("Error: name '%s' already exists in table.\n", name);
}

void test(int a) {
  write_to_log("Error: test '%s' .\n", a);
}

void write_sym(FILE* output, uint32_t addr, const char* name) {
    fprintf(output, "%u\t%s\n", addr, name);
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time. 
   If memory allocation fails, you should call allocation_failed(). 
   Mode will be either SYMBOLTBL_NON_UNIQUE or SYMBOLTBL_UNIQUE_NAME. You will need
   to store this value for use during add_to_table().
 */
SymbolTable* create_table(int mode) {
  SymbolTable* rettable = (SymbolTable*)malloc(sizeof(SymbolTable));
  if (rettable == NULL)
    allocation_failed();
	/*memory allocation fails*/
  rettable -> len = 0;
  rettable -> mode = mode;
  rettable -> cap = 10;
  rettable -> tbl = (Symbol*)malloc(100*sizeof(Symbol));
  /*initializating the attributes*/
  if (rettable->tbl == NULL)
    allocation_failed();
	/*memory allocation fails*/
  return rettable;
} 

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable* table) 
{
  int i;
  for(i = 0; i < table->len; i++) 
  {
  	/*free the name*/
    free((table-> tbl)[i].name);
  }
  free(table -> tbl);
  free(table);
  /*free all the stuff*/
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE. 
   1. ADDR is given as the byte offset from the first instruction. 
   2. The SymbolTable must be able to resize itself as more elements are added. 

   3. Note that NAME may point to a temporary array, so it is not safe to simply
   store the NAME pointer. You must store a copy of the given string.

   4. If ADDR is not word-aligned, you should call addr_alignment_incorrect() 
   and return -1. 

   5. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists 
   in the table, you should call name_already_exists() and return -1. 

   6.If memory allocation fails, you should call allocation_failed(). 

   Otherwise, you should store the symbol name and address and return 0.
 */
int add_to_table(SymbolTable* table, const char* name, uint32_t addr) {
  char myname[strlen(name)+1];
  int i;
  uint32_t newsize;
  /*ADDR is not word-aligned */
  if (addr % 4 != 0)
  {
    addr_alignment_incorrect();
    return -1;
  }
  /*deep copy the name*/
  strcpy(myname,name);

  if ((table->mode) == 1)
  {
    for (i = 0; i < (table->len); i++)
    {
    	/*test whether the name already exists*/
      if (strcmp(myname, ((table->tbl)[i]).name)== 0)
      {
        name_already_exists(name);
        return -1;
      }
    }
  }
  /*test if there is enough capacity*/
  if ((table->len) < (table->cap))
  {
    newsize = strlen(myname)+1;
    table->tbl[(table->len)].name = (char*)malloc(newsize * sizeof(char));
    /*memory allocation fails*/
    if (table->tbl[(table->len)].name == NULL)
    {
      allocation_failed();
    }
    /*copy the name adn address to my table*/
    strcpy(table->tbl[(table->len)].name, myname);
    table->tbl[(table->len)].addr = addr;
    (table->len)++;
  }
  else 
  {
  	/*enlarge the capacity*/
    (table->cap) = (table->cap)*2;
    table->tbl = realloc(table->tbl, (table->cap)*(sizeof(Symbol)));
    /*memory allocation fails*/
    if (table->tbl == NULL)
    {
      allocation_failed();
    }
    /*memory allocation*/
    newsize = strlen(myname)+1;
    table->tbl[(table->len)].name = (char*)malloc(newsize * sizeof(char));
    /*memory allocation fails*/
    if (table->tbl[(table->len)].name == NULL)
    {
      allocation_failed();
    }
    /*copy the name and address to my table*/
    strcpy(table->tbl[(table->len)].name, myname);
    table->tbl[(table->len)].addr = addr;
    (table->len)++;
  }
  return 0;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable* table, const char* name) {
  /* YOUR CODE HERE */   
  int i;  
  int64_t address;

  for (i = 0; i < table->len; i++)
  {
  	/*search for the name*/
    if (strcmp(name, (table->tbl[i]).name) == 0)
    {
      address = (int64_t)table->tbl[i].addr;
      return address;
    }
  }
  /*there is no such name*/
  return -1;
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_sym() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable* table, FILE* output) {
  char* myname;
  int64_t myaddress;
  int i;
  /*traversal my table*/
  for (i = 0; i < table->len; i++)
  {
    myname = (table->tbl[i]).name;
    myaddress = (int64_t)(table->tbl[i]).addr;
    write_sym(output, myaddress, myname);
    /*write to system*/
  }
}


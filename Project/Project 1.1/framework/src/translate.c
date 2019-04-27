#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tables.h"
#include "translate_utils.h"
#include "translate.h"

/* Writes instructions during the assembler's first pass to OUTPUT. The case
   for general instructions has already been completed, but you need to write
   code to translate the li, bge and move pseudoinstructions. Your pseudoinstruction 
   expansions should not have any side effects.

   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.

   Error checking for regular instructions are done in pass two. However, for
   pseudoinstructions, you must make sure that ARGS contains the correct number
   of arguments. You do NOT need to check whether the registers / label are 
   valid, since that will be checked in part two.

   Also for li:
    - make sure that the number is representable by 32 bits. (Hint: the number 
        can be both signed or unsigned).
    - if the immediate can fit in the imm field of an addiu instruction, then
        expand li into a single addiu instruction. Otherwise, expand it into 
        a lui-ori pair.

   And for bge and move:
    - your expansion should use the fewest number of instructions possible.

   MARS has slightly different translation rules for li, and it allows numbers
   larger than the largest 32 bit number to be loaded with li. You should follow
   the above rules if MARS behaves differently.

   Use fprintf() to write. If writing multiple instructions, make sure that 
   each instruction is on a different line.

   Returns the number of instructions written (so 0 if there were any errors).
 */
unsigned write_pass_one(FILE* output, const char* name, char** args, int num_args) {
  if (!output || !name || !args)
    return 0;
  /*if it is empty, do nothing*/
  if (strcmp(name, "li") == 0) {
    long int imm;
    uint16_t highpart;
    uint16_t lowpart;
  /*we meet li*/
    if (num_args != 2)
    {
      return 0;
    }
  /*wrong nums of args*/
    
    if (translate_num(&imm, args[1], -32768,65535) != -1)
    {
      fprintf(output, "%s %s %s %ld\n", "addiu", args[0], "$0", imm);
      return 1;
    }
  /*in -32768 to 65535, addiu works*/
    else
    {
      if (translate_num(&imm, args[1], -2147483648,4294967295) == -1)
      {
        return 0;
      }
      highpart = imm>>16;
      lowpart = imm&0xFFFF;
    /*imm too big or too small, record two part of it seperatly*/
      fprintf(output, "%s %s %d\n","lui", "$at", highpart);
      fprintf(output, "%s %s %s %d\n","ori", args[0], "$at", lowpart);
      return 2;
    }
  /*we meet bge*/
  } else if (strcmp(name, "bge") == 0) {
    if (num_args != 3)
    {
      return 0;
    }
  /*wrong nums of args*/
    fprintf(output, "%s %s %s %s\n", "slt","$at", args[0], args[1]);
    fprintf(output, "%s %s %s %s\n","beq", "$at", "$0", args[2]);
    return 2;
  /*we meet move*/
  } else if (strcmp(name, "move") == 0 ) {
    if (num_args != 2)
    {
      return 0;
    }
    /*wrong nums of args*/
    fprintf(output, "%s %s %s %s\n", "addu", args[0], "$0", args[1]);
    return 1;
  /*what we get is not pseudo instruction, just deal with it*/
  }else {
    write_inst_string(output, name, args, num_args);
    return 1;
  }
}

/* Writes the instruction in hexadecimal format to OUTPUT during pass #2.
   
   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS. 

   The symbol table (SYMTBL) is given for any symbols that need to be resolved
   at this step. If a symbol should be relocated, it should be added to the
   relocation table (RELTBL), and the fields for that symbol should be set to
   all zeros. 

   You must perform error checking on all instructions and make sure that their
   arguments are valid. If an instruction is invalid, you should not write 
   anything to OUTPUT but simply return -1. MARS may be a useful resource for
   this step.

   Note the use of helper functions. Consider writing your own! If the function
   definition comes afterwards, you must declare it first (see translate.h).

   Returns 0 on success and -1 on error. 
 */
int translate_inst(FILE* output, const char* name, char** args, size_t num_args, uint32_t addr, SymbolTable* symtbl, SymbolTable* reltbl) {
    if (strcmp(name, "addu") == 0)       return write_rtype (0x21, output, args, num_args);
    else if (strcmp(name, "or") == 0)    return write_rtype (0x25, output, args, num_args);
    else if (strcmp(name, "sll") == 0)   return write_shift (0x00, output, args, num_args);
    /*get after looking up in mips green sheet*/
    else if (strcmp(name, "slt") == 0)   return write_rtype (0x2a, output, args, num_args);
    else if (strcmp(name, "sltu") == 0)  return write_rtype (0x2b, output, args, num_args);
    else if (strcmp(name, "jr") == 0)    return write_jr (0x08, output, args, num_args);
    else if (strcmp(name, "addiu") == 0) return write_addiu (0x9, output, args, num_args);
    /*get after looking up in mips green sheet*/
    else if (strcmp(name, "ori") == 0)   return write_ori (0xd, output, args, num_args);
    else if (strcmp(name, "lui") == 0)   return write_lui (0xf, output, args, num_args);
    else if (strcmp(name, "lb") == 0)    return write_mem (0x20, output, args, num_args);
    else if (strcmp(name, "lbu") == 0)   return write_mem (0x24, output, args, num_args);
    /*get after looking up in mips green sheet*/
    else if (strcmp(name, "lw") == 0)    return write_mem (0x23, output, args, num_args);
    else if (strcmp(name, "sb") == 0)    return write_mem (0x28, output, args, num_args);
    else if (strcmp(name, "sw") == 0)    return write_mem (0x2b, output, args, num_args);
    else if (strcmp(name, "beq") == 0)   return write_branch (0x4, output, args, num_args, addr, symtbl);
    /*get after looking up in mips green sheet*/
    else if (strcmp(name, "bne") == 0)   return write_branch (0x5, output, args, num_args, addr, symtbl);
    else if (strcmp(name, "j") == 0)     return write_jump (0x2, output, args, num_args, addr, reltbl);
    else if (strcmp(name, "jal") == 0)   return write_jump (0x3, output, args, num_args, addr, reltbl);
    /*the instruction is not included*/
    else                                 return -1;
}

/* A helper function for writing most R-type instructions. You should use
   translate_reg() to parse registers and write_inst_hex() to write to 
   OUTPUT. Both are defined in translate_utils.h.

   This function is INCOMPLETE. Complete the implementation below. You will
   find bitwise operations to be the cleanest way to complete this function.
 */
int write_rtype(uint8_t funct, FILE* output, char** args, size_t num_args) {
  int rd;
  int rs;
  int rt;
  uint32_t instruction;
  /*wrong numbers of arguments*/
  if (num_args != 3)
  {
    return -1;
  }
  /*get registers*/
  rd = translate_reg(args[0]);
  rs = translate_reg(args[1]);
  rt = translate_reg(args[2]);
  /* error checking for register ids */
  if (rd==-1 || rs==-1 || rt==-1)
  {
    return -1;
  }
  /* generate instruction by bit-wise operation*/
  instruction = (funct<<0)|(0<<6)|(rd<<11)|(rt<<16)|(rs<<21)|(0<<26);
  write_inst_hex(output, instruction);
  return 0;
}

/* A helper function for writing shift instructions. You should use 
   translate_num() to parse numerical arguments. translate_num() is defined
   in translate_utils.h.

   This function is INCOMPLETE. Complete the implementation below. You will
   find bitwise operations to be the cleanest way to complete this function.
 */
int write_shift(uint8_t funct, FILE* output, char** args, size_t num_args) {
  uint32_t instruction;
  long int shamt;
  int rd ;
  int rt ;
  /*wrong numbers of arguments*/
  if (num_args != 3)
  {
    return -1;
  }
  /*get registers*/
  rd = translate_reg(args[0]);
  rt = translate_reg(args[1]);
  /* error check for register id */
  if (rd==-1 || rt==-1 || translate_num(&shamt, args[2], 0, 31)==-1)
  {
    return -1;
  }
  /* generate instruction by bit-wise operation*/
  instruction = (0<<0)|(shamt<<6)|(rd<<11)|(rt<<16)|(0<<21)|(0<<26);
  write_inst_hex(output, instruction);
  return 0;
}


int write_jr(uint8_t funct, FILE* output, char** args, size_t num_args) {
  int rs;
  uint32_t instruction;
  /*wrong numbers of arguments*/
  if (num_args != 1)
  {
    return -1;
  }
  /*get registers*/
  /* error check for register id */
  rs = translate_reg(args[0]);
  if (rs == -1)
  {
    return -1;
  }
  /* generate instruction by bit-wise operation*/
  instruction = (funct<<0)|(0<<6)|(0<<11)|(0<<16)|(rs<<21)|(0<<26);
  write_inst_hex(output, instruction);
  return 0;
}

int write_addiu(uint8_t opcode, FILE* output, char** args, size_t num_args) {
  int rt;
  int rs;
  long int imm;
  uint32_t instruction;
  /*wrong numbers of arguments*/
  if (num_args != 3)
  {
    return -1;
  }
  /*get registers*/
  rt = translate_reg(args[0]);
  rs = translate_reg(args[1]);
  /* error check for register id and imm interval*/
  if (rs == -1 || rt == -1)
  {
    return -1;
  }
  if (translate_num(&imm, args[2], -32768, 32767) == -1)
  {
    return -1;
  }

  /*ensure that during bit-wise operation, negative does not bother*/
  imm = imm & 0xFFFF;
  instruction = imm|(rt<<16)|(rs<<21)|(opcode<<26);
  /* generate instruction by bit-wise operation*/
  write_inst_hex(output, instruction);
  return 0;
}

int write_ori(uint8_t opcode, FILE* output, char** args, size_t num_args) {
  int rt;
  int rs;
  long int imm;
  uint32_t instruction;
  /*wrong numbers of arguments*/
  if (num_args != 3)
  {
    return -1;
  }
  /*get registers*/
  rt = translate_reg(args[0]);
  rs = translate_reg(args[1]);
  /* error check for register id and imm interval*/
  if (rt==-1||rs==-1||translate_num(&imm, args[2], 0, 65535) == -1)
  {
    return -1;
  }
  /*ensure that during bit-wise operation, negative does not bother*/
  imm = imm & 0xFFFF;
  /* generate instruction by bit-wise operation*/
  instruction = imm|(rt<<16)|(rs<<21)|(opcode<<26);
  write_inst_hex(output, instruction);
  return 0;
}

int write_lui(uint8_t opcode, FILE* output, char** args, size_t num_args) {
  int rt;
  long int imm;
  uint32_t instruction;
  /*wrong numbers of arguments*/
  if (num_args != 2)
  {
    return -1;
  }
  /*get registers*/
  rt = translate_reg(args[0]);
  if (rt == -1)
  {
    return -1;
  }
  /* error check for register id and imm interval*/
  if (translate_num(&imm, args[1], 0, 65535) == -1)
  {
    return -1;
  }
  /*ensure that during bit-wise operation, negative does not bother*/
  imm = imm &0xFFFF;
  /* generate instruction by bit-wise operation*/
  instruction = imm|(rt<<16)|(0<<21)|(opcode<<26);
  write_inst_hex(output, instruction);
  return 0;
}


int write_mem(uint8_t opcode, FILE* output, char** args, size_t num_args) {
  int rs;
  int rt;
  long int imm;
  uint32_t instruction;
  /*wrong numbers of arguments*/
  if (num_args != 3)
  {
    return -1;
  }
  /*get registers*/
  rt = translate_reg(args[0]);
  rs = translate_reg(args[2]);
  /* error check for register id and imm interval*/
  if (rt==-1 || rs==-1 ||translate_num(&imm, args[1], -32768, 32767)==-1)
  {
    return -1;
  }
  /*ensure that during bit-wise operation, negative does not bother*/
  imm = imm & 0xFFFF;
  /* generate instruction by bit-wise operation*/
  instruction = imm|(rt<<16)|(rs<<21)|(opcode<<26);
  write_inst_hex(output, instruction);
  return 0;
}

/* Hint: the way for branch to calculate relative address. e.g. bne
     bne $rs $rt label
   assume the byte_offset(addr) of label is L, 
   current instruction byte_offset(addr) is A
   the relative address I  for label satisfy:
     L = (A + 4) + I * 4
   so the relative addres is 
     I = (L - A - 4) / 4;  */
int write_branch(uint8_t opcode, FILE* output, char** args, size_t num_args, 
		 uint32_t addr, SymbolTable* symtbl) {
  int rs;
  int rt;
  int offset;
  /*to store the offset*/
  char* name;
  int64_t tmp;
  uint32_t address;
  uint32_t instruction;
  /*wrong numbers of arguments*/
  if (num_args != 3)
  {
    return -1;
  }
  /*get registers*/
  rs = translate_reg(args[0]);
  rt = translate_reg(args[1]);
  name = args[2];
  tmp = get_addr_for_symbol(symtbl, name);
  /* error check for register id and imm interval*/
  if (rt==-1 || rs==-1 || tmp==-1)
  {
    return -1;
  }

  /*
  if ((tmp - addr - 4)%4 != 0)
  {
    return -1;
  }
  */
  address = (tmp - addr - 4)/4;
  /*get the relative address*/
  offset = (address & 0xFFFF);
  /* generate instruction by bit-wise operation*/
  instruction = offset|(rt<<16)|(rs<<21)|(opcode<<26);
  write_inst_hex(output, instruction);
  return 0;
}

/* Hint: the relocation table should record
   1. the current instruction byte_offset(addr)
   2. the unsolved LABEL in the jump instruction  */
int write_jump(uint8_t opcode, FILE* output, char** args, size_t num_args, 
	       uint32_t addr, SymbolTable* reltbl) {
  uint32_t instruction;
  /*wrong numbers of arguments*/
  if (num_args != 1)
  {
    return -1;
  }
  /* generate instruction by bit-wise operation*/
  instruction = (opcode<<26);
  add_to_table(reltbl, args[0], addr);
  /*add the address to symbol table*/
  write_inst_hex(output, instruction);
  return 0;
}

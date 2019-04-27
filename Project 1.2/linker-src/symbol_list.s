# CS 61C Spring 2015 Project 1-2 
# symbol_list.s

#==============================================================================
#                              Project 1-2 Part 2
#                               SymbolList README
#==============================================================================
# In this file you will be implementing a linked list-based data structure for
# storing symbols. Its purpose is analogous to SymbolTable from Proj1-1, but to
# disinguish it we will call it SymbolList.
#
# Each node in the SymbolList contains a (addr, name) pair. An empty SymbolList
# is simply a pointer to NULL. As (addr, name) pairs are added to a SymbolList,
# new list nodes are dynamically allocated and appended to the front of the list. 
# However, as there is no free() in MARS, you do not need to write a free_list()
# function. You may use the functions in string.s during your implementation.
#
# You do not need to perform error checking on duplicate addr or name entries
# in SymbolList, nor do you need to do alignment checking. Don't worry about
# provided addresses being too big.
#
# If SymbolList were to be written in C, each list node would be declared as 
# follows:
#   typedef struct symbollist { 
#       int addr;
#       char* name;
#       struct symbollist* next; 
#   } SymbolList;
#
# You need to write addr_for_symbol() and add_to_list().
# You will find test cases in linker-tests/test_symbol_list.s
#==============================================================================

.include "string.s"

.text

#------------------------------------------------------------------------------
# function addr_for_symbol()
#------------------------------------------------------------------------------
# Iterates through the SymbolList and searches for an entry with the given name.
# If an entry is found, return that addr. Otherwise return -1.
#
# Arguments:
#  $a0 = pointer to a SymbolList (NULL indicates empty list)
#  $a1 = name to look for
#
# Returns:  address of symbol if found or -1 if not found
#------------------------------------------------------------------------------
addr_for_symbol:
	addiu $sp, $sp, -12
	sw $ra, 0($sp)    #save on stack
	sw $s6, 4($sp)
	sw $s7, 8($sp)
	move $s6, $a0     #s6: pointer to a SymbolList
	move $s7, $a1     #s7: name to look for

loop11:
	move $s6, $a0     #pointer to a SymbolList
	lw $a0, 4($s6)    #first 4 is addr, second 4 is name 
	jal streq

	move $t2, $s6  #t2: pointer to a SymbolList
	move $a1, $s7  #a1: name to look for, again

	beq $v0, $zero, found_addr_for_symbol
	lw $a0, 8($s6)	#go to next item's name in table
	beq $a0, $zero, not_found_addr_for_symbol #reach the end, not found
	j loop11 #loop

found_addr_for_symbol:       #found
	lw $v0, 0($t2)           #return address
	j end_addr_for_symbol

not_found_addr_for_symbol:    #not found
	li $v0, -1                #return -1
	j end_addr_for_symbol

end_addr_for_symbol:          
	lw $ra, 0($sp)            #load back from stack
	lw $s6, 4($sp)
	lw $s7, 8($sp)
	addiu $sp, $sp, 12        #restore pointer
	jr $ra            

#------------------------------------------------------------------------------
# function add_to_list()
#------------------------------------------------------------------------------
# Adds a (name, addr) pair to the FRONT of the list. You should call new_node()
# to create a new node. You do not need to perform error checking on duplicate 
# addr or name entries in SymbolList, nor do you need to do alignment checking.
#
# As with Project 1-1, you WILL need to create a copy of the string that was
# passed in. copy_of_str() from Part 1 will be useful. After new entry has been
# added to the list, return the new list.
#
# Arguments:
#   $a0 = ptr to list (may be NULL)
#   $a1 = address of symbol (integer)
#   $a2 = pointer to name of symbol (string)
#
# Returns: the new list
#------------------------------------------------------------------------------
create_new_node:      #create new node
	li $a0, 12
	li $v0, 9
	syscall         #allocate memory
	jr $ra

add_to_list:	
	# YOUR CODE HERE
	addiu $sp, $sp, -20  
	sw $ra, 0($sp) 
	sw $s6, 4($sp)          #save the new node
	sw $s7, 8($sp)			#save a0
	sw $a1, 12($sp)
	sw $a2, 16($sp)         #save on stack

	move $s7, $a0        #s7 = ptr to list (may be NULL)

	jal create_new_node
	move $a0, $s7	  #restore a0 = ptr to list (may be NULL)
	lw $ra, 0($sp)     #restore ra 
	move $s6, $v0      #save the new node

	lw $a0, 16($sp)   #a0 get a2: name of symbol, for func below
	jal copy_of_str

	lw $ra, 0($sp)    #restore ra

	move $t2, $v0    # t2 holds copy of name
	move $a0, $s7      #get back args
	lw $a1, 12($sp)
	lw $a2, 16($sp)

	sw $a1, 0($s6)    #address of symbol
	sw $t2, 4($s6)    #name of symbol
	sw $a0, 8($s6)    #set the pointer to the next
	move $v0, $s6

	lw $ra, 0($sp)    #load back from stack
	lw $s6, 4($sp)   
	lw $s7, 8($sp)	
	lw $a1, 12($sp)
	lw $a2, 16($sp)
	addiu $sp, $sp, 20    #restore the pointer
	jr $ra

###############################################################################
#                 DO NOT MODIFY ANYTHING BELOW THIS POINT                       
###############################################################################

#------------------------------------------------------------------------------
# function symbol_for_addr()
#------------------------------------------------------------------------------
#  Iterates through the SymbolList and searches for an entry with the given addr.
#  If an entry is found, return a pointer to the name. Otherwise return NULL.
#
#  Arguments:
#   $a0 = addr to look for
#   $a1 = pointer to a SymbolList (NULL indicates empty list)
#
#  Returns: a pointer to the name if found or NULL if not found
#------------------------------------------------------------------------------
symbol_for_addr:
	beq $a1, $0, symbol_not_found	# Begin symbol_for_addr
	lw $t0, 0($a1)
	beq $t0, $a0, symbol_found
	lw $a1, 8($a1)
	j symbol_for_addr
symbol_found:
	lw $v0, 4($a1)
	jr $ra
symbol_not_found:
	li $v0, 0
	jr $ra			# End addr_for_symbol

#------------------------------------------------------------------------------
# function print_list() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Arguments:
#  $a0 = pointer to a SymbolList (NULL indicates empty list)
#  $a1 = print function
#  $a2 = file pointer
#------------------------------------------------------------------------------
print_list:
	addiu $sp, $sp, -16		# Begin print_list
	sw $s2, 12($sp)
	sw $s1, 8($sp)
	sw $s0, 4($sp)
	sw $ra, 0($sp)
	move $s0, $a0
	move $s1, $a1
	move $s2, $a2
print_list_loop:
	beq $s0, $0, print_list_end
	lw $a0, 0($s0)
	lw $a1, 4($s0)
	move $a2, $s2
	jalr $s1
	lw $s0, 8($s0)
	j print_list_loop
print_list_end:
	lw $s2, 12($sp)
	lw $s1, 8($sp)
	lw $s0, 4($sp)
	lw $ra, 0($sp)
	addiu $sp, $sp, 16
	jr $ra			# End print_list	

#------------------------------------------------------------------------------
# function print_symbol() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Prints one symbol to standard output.
#
# Arguments:
#  $a0 = addr of symbol
#  $a1 = name of symbol
#  $a2 = file pointer (this argument is actually ignored)
#
# Returns: none
#------------------------------------------------------------------------------
print_symbol:
	li $v0, 36			# Begin print_symbol()
	syscall
	la $a0, tab
	li $v0, 4
	syscall
	move $a0, $a1
	syscall
	la $a0, newline
	syscall
	jr $ra			# End print_symbol()

#------------------------------------------------------------------------------
# function write_symbol() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Writes one symbol to the file specified
#
# Arguments:
#  $a0 = addr of symbol
#  $a1 = name of symbol
#  $a2 = file pointer
#
# Returns: none
#------------------------------------------------------------------------------
write_symbol:		
	addiu $sp, $sp, -20		# Begin write_symbol()
	sw $s0, 16($sp)
	sw $s1, 12($sp)
	sw $a1, 8($sp)
	sw $a2, 4($sp)
	sw $ra, 0($sp)
	
	la $s0, temp_buf
	
	move $a1, $s0
	jal dec_to_str	# write int
	move $s1, $v0	
	
	addu $a0, $s0, $s1
	addiu $s1, $s1, 1
	la $a1, tab
	li $a2, 1
	jal strncpy		# write tab
	
	lw $a0, 8($sp)
	jal strlen
	
	addu $a0, $s0, $s1
	addu $s1, $s1, $v0
	lw $a1, 8($sp)
	move $a2, $v0
	jal strncpy		# write string
	
	addu $a0, $s0, $s1
	addiu $s1, $s1, 1
	la $a1, newline
	li $a2, 1
	jal strncpy		# write newline
	
	lw $a0, 4($sp)	# file ptr
	move $a1, $s0	# buffer to write
	move $a2, $s1	# num chars to write
	li $v0, 15
	syscall		# write to file
	
	lw $s0, 16($sp)
	lw $s1, 12($sp)
	lw $ra, 0($sp)
	addiu $sp, $sp, 20
	jr $ra			# End write_symbol()
	
#------------------------------------------------------------------------------
# function new_node() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Creates a new uninitialized SymbolList node.
# Arguments: none
# Returns: pointer to a SymbolList node
#------------------------------------------------------------------------------
new_node:	
	li $a0, 12			# Begin new_node()
	li $v0, 9
	syscall
	jr $ra			# End new_node()
	
.data 
temp_buf:	.space 1024

# CS 110 Spring 2018 hw3
# quick_sort.s

#==============================================================================
#                              Quick Sort MIPS
#                                   README
#==============================================================================
# In this file you will be implementing the def_quick_sort and def_partition 
# function, which form the most important part of this code.
# Input part and output part is given for you and do not modify it(or you may 
# fail some testcases).
# In line 48 we give you the choice to test your code using predefined array: array_1,
# you can decomment it and use array_1 to test your code.
# 
#                               IMPORTANT!!!
# Our asembler will support the following registers:
# 
# $zero, $at, $v0, $a0 - $a3, $t0 - $t3, $s0 - $s3, $sp, and $ra. 
# The name $0 can be used in lieu of $zero. Other register numbers 
# (eg. $1, $2, etc.) are not supported.
# 
# We will manually check whether you use registers properly after the ddl.
# So the grade on your autolab is not the final grade of your homework.
#==============================================================================

.data
    space:          .asciiz  " "            # a $space string.
    line:           .asciiz "\n"            # a newline string.
    colonsp:        .asciiz ": "            # a colon string with $space.
    .align 2
    array:          .space 1000             # array to be sorted
    array_1:        .space 1000   	        # test array for you 
    #array_1:       .word 678 567 456 765 876 987 543 654 684 374 # use this line if you want to uncomment line 48
    #array:         .word 678 567 456 765 876 987 543 654 684 374 # use this line if you want to uncomment line 48
    size:           .word  10             # actual count of the elements in the array
                                            # which will change later.                            
    question:               .asciiz "Input number of values to be sorted and traversaled(0 < N < 1000): "
    instruct:               .asciiz "Input each value: "
    inputstring:            .asciiz "Input list:  "
    sorted_array_string:    .asciiz "Sorted:      "
    post_order_traversal:   .asciiz "traversaled: "
    receive_values_loop_iter_string:    .asciiz "Input value#"
                                        

.text
.globl  main
main:
    #j receive_values_end            #if you want to read in instead of using default, delete this line.
    params_info:
        li    $v0, 4                # v0 = 4 means print_string syscall.
        la    $a0, question         # load params_info_string to argument register $a0.
        syscall                     # issue a system call.
    params:
        li    $v0, 5                # v0 = 5 means read_int syscall.
        syscall                     # get length of the array from key_board input
        la    $t0, size       
        sw    $v0, 0($t0)     
    receive_values_loop_info:
        li    $v0, 4                # prompt user to start feeding data into the array
        la    $a0, instruct   
        syscall             
        li    $v0, 4                # print new line
        la    $a0, line       
        syscall             

#   the input loop
    receive_values_loop_prep:
        la    $t0, array 
        la    $t3, array_1          # load array to $t3.
        lw    $t1, size             # load size to $t1.
        li    $t2, 0                # loop iter, starting from 0.
    receive_values_loop:
        bge   $t2, $t1, receive_values_end    # while ($t2 < $t1).
        li    $v0, 4                # prompt at every iteration during input
        la    $a0, receive_values_loop_iter_string 
        syscall             
        li    $v0, 1          
        addi  $a0, $t2, 1           # load (iter + 1) to argument register $a0.
        syscall             
        li    $v0, 4          
        la    $a0, colonsp        
        syscall             

        li    $v0, 5          
        syscall                     # USER INPUT
        sw    $v0, 0($t0)
        sw    $v0, 0($t3)           # store the user input in the array.
        addi  $t3, $t3, 4
        addi  $t0, $t0, 4           # increment array pointer by 4.
        addi  $t2, $t2, 1           # increment loop iter by 1.
        j receive_values_loop       # jump back to the beginning of the loop.

    receive_values_end:
        li    $v0, 4                # 4 = print_string syscall.
        la    $a0, inputstring        
        syscall
        la  $a0, array
        jal print                   # printing user input values

    # Set up the main mergesort call.
    # Arrays are	
    la $a0, array                   # a0 adrs of the array
    li $a1, 0                       # left val
    lw $a2, size                    # right val
    addi $a2, $a2, -1
    jal def_quick_sort  
    li    $v0, 4                # 4 = print_string syscall.
    la    $a0, sorted_array_string   
    syscall
    la  $a0, array
    jal print                   #print out the sorted list

    j exit


########################################################
####################your code here######################
########################################################

# In this part you will implemente quick sort and partition seperately.
# You should learn how to use stack and function call before implemente.
# WARNING: using registers properly or you will get 30% deduction after ddl.
#          50% meaningful comments is needed or you will get 50% deduction after ddl.




#quick_sort part
# a0 - address of array
# a1 - begin index
# a2 - end index
def_quick_sort:	
	addi $sp, $sp, -20              #move pointer to save data
	sw $ra, 0($sp)               #save address
	sw $s0, 4($sp)                        #also save address
	sw $s1, 8($sp)                  #save lo
	sw $s2, 12($sp)                #save hi
	sw $s3, 16($sp)                #save value p
	
	add $s0, $zero, $a0           #s0 is address
	add $s1, $zero, $a1             #s1 is lo
	add $s2, $zero, $a2             #s2 is hi
	slt $t0, $a1, $a2               #if lo < hi
	beq $t0, $zero, End_Qsort       #go to else 
	
	add $a1, $zero, $s1            #get lo
	add $a2, $zero, $s2          #get hi
	add $a0, $zero, $s0             #get address
	jal def_partition                #call partition
	
	add $s3, $v0, $zero             #s3 is return value p 
	addi $a2, $s3, -1               #get a new hi
	add $a1, $zero, $s1            #get lo
	add $a0, $zero, $s0             #get address
	jal def_quick_sort              #call quick sort
	
	addi $a1, $s3, 1              #get new lo
	add $a2, $s2, $zero            #get hi
	add $a0, $zero, $s0            #get address
	jal def_quick_sort            #call quick sort
	
	lw $ra, 0($sp)                 #load address    
	lw $s0, 4($sp)                 #load address
	lw $s1, 8($sp)                #load lo
	lw $s2, 12($sp)                  #load hi
	lw $s3, 16($sp)              #load value p
	addiu $sp, $sp, 20             #reset the pointer on stack
End_Qsort:
	jr $ra                        #return
	
# partition subroutine
# a0 - address of array
# a1 - begin index
# a2 - end index

def_partition:
	addi $sp, $sp, -20            #move pointer to save data
	sw $ra, 0($sp)                #save address
	sw $s0, 4($sp)                #also save address
	sw $s1, 8($sp)                #save lo
	sw $s2, 12($sp)               #save hi
	sw $s3, 16($sp)               #save value p
	
	
mul $t0, $a2, 4     # a word
add $t0, $t0, $a0   # t0 = address of a[hi]

lw $a3, 0($t0) #    is  A[hi]   a3 is pivot

addi $s0, $a1, -1     # i = lo-1   s0 is i

addi $s1, $a1, 0  #low index ,s1 is loop var j

addi $s2, $a2, -1  #s2 is only for upper bound

L1:
	bgt $s1, $s2, loop_end        #j to hi-1
	#if 
	mul $t0, $s1, 4          #a word
	add  $t0, $a0, $t0        #aj address
	lw $t0, 0($t0)    #t0 is a[j]
	blt $t0, $a3, then      #a[j] < pivot
	j else   
then:
	addi $s0, $s0, 1       #i+1
	
	mul $t0, $s0, 4        #a word
	add  $t0, $a0, $t0     #i address
	
	mul $t1, $s1, 4        #a word
	add  $t1, $a0, $t1     #j address
	
	lw $t2, 0($t0)      #t2 is a[i]
	lw $t3, 0($t1)      #t3 is a[j]
	sw $t3, 0($t0)       #save to the other ad
	sw $t2, 0($t1)       #swap  complete
	j final              #end if
else:
	j final             #end if
final:
	addi $s1,$s1, 1 
	j L1                #for loop
loop_end:
	addi $t0, $s0, 1    #i +1
	mul $t0, $t0, 4     #a word
	add  $t0, $a0, $t0  #i+1 address
	lw $t2, 0($t0)      #t2 is a[i+1]
	
	mul $t1, $a2, 4     #a word
	add  $t1, $a0, $t1  # hi address
	lw $t3, 0($t1)      #t3 is a[hi]
	
	sw $t3, 0($t0)      #save to the other address 
	sw $t2, 0($t1)      #swap  complete
	
	addi $v0, $s0, 1    #i+1
	
	lw $ra, 0($sp)                        #load address    
	lw $s0, 4($sp)                      #load address
	lw $s1, 8($sp)                        #load lo
	lw $s2, 12($sp)                    #load hi
	lw $s3, 16($sp)                    #load value p
	addiu $sp, $sp, 20                 #reset the pointer on stack
	
	jr $ra                             #return
# prog$rams ends
# 


###       Printing array with size
print:
    print_loop_prep:
        move $t0,$a0
        lw  $t1, size
        li  $t2, 0
    print_loop:
        bge $t2, $t1, print_end
        li  $v0, 1
        lw  $a0, 0($t0)
        syscall
        li  $v0, 4
        la  $a0, space
        syscall
        addi    $t0, $t0, 4
        addi    $t2, $t2, 1
        j   print_loop
    print_end:
        li  $v0, 4
        la  $a0, line
        syscall
        jr  $ra
exit:

########################################################
####################End of your code####################
########################################################

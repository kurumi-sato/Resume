   .data
str1:
   .asciiz "abba"
str2:
   .asciiz "racecar"
str3:
   .asciiz "swap paws",
str4:
   .asciiz "not a palindrome"
str5:
   .asciiz "another non palindrome"
str6:
   .asciiz "almost but tsomla"

# array of char pointers = {&str1, &str2, ..., &str6}
ptr_arr:
   .word str1, str2, str3, str4, str5, str6, 0

yes_str:
   .asciiz " --> Y\n"
no_str:
   .asciiz " --> N\n"

   .text

# main(): ##################################################
#   char ** j = ptr_arr
#   while (*j != 0):
#     rval = is_palindrome(*j)
#     printf("%s --> %c\n", *j, rval ? yes_str: no_str)
#     j++
#
main:
   li   $sp, 0x7ffffffc    # initialize $sp

   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   subu $sp, $sp, 8        # save s0, s1 on stack before using them
   sw   $s0, 8($sp)        # push $s0
   sw   $s1, 4($sp)        # push $s1

   la   $s0, ptr_arr        # use s0 for j. init ptr_arr
main_while:
   lw   $s1, ($s0)         # use s1 for *j
   beqz $s1, main_end      # while (*j != 0):
   move $a0, $s1           #    print_str(*j)
   li   $v0, 4
   syscall
   move $a0, $s1           #    v0 = is_palindrome(*j)
   jal  is_palindrome
   beqz $v0, main_print_no #    if v0 != 0:
   la   $a0, yes_str       #       print_str(yes_str)
   b    main_print_resp
main_print_no:             #    else:
   la   $a0, no_str        #       print_str(no_str)
main_print_resp:
   li   $v0, 4
   syscall

   addu $s0, $s0, 4       #     j++
   b    main_while        # end while
main_end:

   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel
# end main ################################################
# Kurumi sato
# UID: 117185836
# ksato1


	.text
is_palindrome:
   # PROLOGUE
   subu $sp, $sp, 8              
   sw   $ra, 8($sp)                
   sw   $fp, 4($sp)                
   addu $fp, $sp, 8                


   # BODY
   li $t1, 0 			# i = 0
   sw  $a0, 12($sp)
	
   jal  strlen 			# $v0 = strlen
   lw   $a0, 12($sp)
   move $t0, $v0 		# $t0 is strlen
   move $t2, $a0 		# arg = t2

   div $t4, $t0, 2 		# $t4 = strlen / 2
	
loop:			
   beq $t1, $t4, return_one 	# if i = strlen / 2, return one
   lb  $t3, 0($a0) 		# load first character

   sub $t5, $t0, $t1 		# len - i
   sub $t5, $t5, 1		# len - i -1
   add $t6, $t5, $t2 		# $t6 address of character
   lb  $t7, 0($t6) 		# load end character
	
   bne $t3, $t7, return_zero 	# if string[i] != string[len-i-1] return 0
	
				# if equal
   add $t1, $t1, 1 		# increment i by one
   add $a0, $a0, 1 		# increment array by one
   j loop 			# go back through loop

return_zero:
   li $v0, 0  			# if not a palindrome return 0
   j end

return_one:
   li $v0, 1


end:
   # EPILOGUE

   move $sp, $fp                    
   lw   $ra, ($fp)                  
   lw   $fp, -4($sp)               
   j    $ra                         




	
#strlen function
strlen:
   li $v0, 0
	
str_loop:
   lb     $t0, 0($a0)
   beqz   $t0, endloop
   add    $v0, $v0, 1
   add    $a0, $a0, 1
   j    str_loop

endloop:
   jr $ra 			#return address

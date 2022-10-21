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

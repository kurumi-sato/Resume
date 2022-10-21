# fibonacci

    .text
fibonacci:	
    
   # PROLOGUE
	subu    $sp, $sp, 8
	sw    	$ra, 8($sp)
	sw    	$fp, 4($sp)
	addu    $fp, $sp, 8

   # BODY
	beq     $a0, 1, base     	# if n == 1 go to base
	beq     $a0, 0, base	 	# if n == 0 go to base

   # OTHERWISE
	subu    $sp, $sp, 4		# make space on stack for n
	sw      $a0, 4($sp)		# save arg n on stack
	sub     $a0, $a0, 1		# $a0 = n - 1
	jal     fibonacci       	# call fibonacci


	lw	$t0, 4($sp)		# $t1 = n
	sub     $a0, $t0, 2		# $a0 = n - 2
	subu 	$sp, $sp, 4		# make space for f(n-1)
	sw	$v0, 4($sp)		# save f(n-1) onto stack
	jal     fibonacci       	# call fibonacci

	lw	$t1, 4($sp)		# load f(n-1) into $t1
	add    	$v0, $t1, $v0		# $v0 = f(n-1) + f(n-2)
	j 	end			# return the value

base:
	move $v0, $a0

end:	
   # EPILOGUE
	move    $sp, $fp
	lw    $ra, ($fp)
	lw    $fp, -4($sp)
	jr     $ra



	

   .data

# array terminated by 0 (which is not part of the array)
xarr:
   .word 1
   .word 2
   .word 3
   .word 4
   .word 10
   .word 11
   .word 12
   .word 13
   .word 14
   .word 16
   .word 18
   .word 20
   .word 24
   .word 0

   .text

# main(): ##################################################
#   uint* j = xarr
#   while (*j != 0):
#     printf(" %d\n", fibonacci(*j))
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

   la   $s0, xarr          # use s0 for j. init to xarr
main_while:
   lw   $s1, ($s0)         # use s1 for *j
   beqz $s1, main_end      # if *j == 0 go to main_end
   move $a0, $s1
   jal  fibonacci          # result = fibonacci(*j)
   move $a0, $v0           # print_int(result)
   li   $v0, 1
   syscall
   li   $a0, 10            # print_char('\n')
   li   $v0, 11
   syscall
   addu $s0, $s0, 4        # j++
   b    main_while
main_end:
   lw   $s0, -8($fp)       # restore s0
   lw   $s1, -12($fp)      # restore s1

   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel
## end main #################################################

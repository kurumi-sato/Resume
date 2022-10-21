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

	lw	$t0, -8($fp)		# $t1 = n
	sub     $a0, $t0, 2		# $a0 = n - 2
	subu 	$sp, $sp, 4		# make space for f(n-1)
	sw	$v0, 4($sp)		# save f(n-1) onto stack
	jal     fibonacci       	# call fibonacci

	lw	$t1, -12($fp)		# load f(n-1) into $t1
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



	


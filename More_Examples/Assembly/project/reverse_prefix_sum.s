#reverse_prefix_sum

	.text
reverse_prefix_sum:
	
   # PROLOGUE
	subu   	$sp, $sp, 8
	sw     	$ra, 8($sp)
	sw     	$fp, 4($sp)
	addu   	$fp, $sp, 8

   #BODY
	li	$t1, -1
	subu	$sp, $sp, 4		# add r onto stack
	sw	$a0, 4($sp)		# set r = *arr
	lw	$t2, ($a0)		# dereference arr
	beq   	$t2, $t1, zero		# if(*arr == -1) return zero	
	addu   	$a0, $a0, 4		# arr+1
	jal	reverse_prefix_sum
	
	lw	$t0, -8($fp)		# $t0 = *arr
	lw	$t0, 0($t0)
	addu	$t0, $v0, $t0		# r = reverse_prefix_sum(arr+1) + *arr
	move	$a0, $t0
	move	$v0, $t0		# return r
	j 	ret

zero:
	li 	$v0, 0		# return 0

	
ret:	
	# EPILOGUE
	move    $sp, $fp
	lw    	$ra, ($fp)
	lw    	$fp, -4($sp)
	jr    	$ra
     

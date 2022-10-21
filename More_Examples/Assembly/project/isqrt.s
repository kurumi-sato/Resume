# isqrt(n)
	
	.text
isqrt:
    
   # PROLOGUE
	subu	$sp, $sp, 8
	sw    	$ra, 8($sp)
	sw    	$fp, 4($sp)
	addu   	$fp, $sp, 8
    
   # BODY
	li	$t0, 2
	subu	$sp, $sp, 4		# make space on stack for n
	sw	$a0, 4($sp)		# save arg n on stack
	bgt	$t0, $a0, ret_n		# if (2 > n) return n
	srl	$a0, $a0, 2		# shift n >> 2
	jal	isqrt			# isqrt(n >> 2)

	lw	$t4, -8($fp)		# $t4 = n			
	sll	$t1, $v0, 1		# $t1 = isqrt(n >> 2) << 1 is small
	add	$t2, $t1, 1		# $t2 is large
	
	mul	$t3, $t2, $t2		# large * large
	bgt	$t3, $t4, small		# if (large * large > n) return small
	move	$v0, $t2		# return large
	j	end
	
small:
	move	$v0, $t1		#return small
	j	end
	
ret_n:
	move	$v0, $a0		#return n

end:
   # EPILOGUE
	move	$sp, $fp
	lw	$ra, ($fp)
	lw	$fp, -4($sp)
	jr	$ra

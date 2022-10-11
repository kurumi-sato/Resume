# strlen(str)

	.text
strlen:


   # PROLOGUE
   subu    $sp, $sp, 8
   sw    $ra, 8($sp)
   sw    $fp, 4($sp)
   addu    $fp, $sp, 8
	
   # BODY
   li $v0, 0

loop:
   lb     $t0, 0($a0)
   beqz   $t0, endloop
   add    $v0, $v0, 1
   add    $a0, $a0, 1
   j    loop

endloop:
	
   # EPILOGUE
   move   $sp, $fp
   lw    $ra, ($fp)
   lw    $fp, -4($sp)
   jr     $ra

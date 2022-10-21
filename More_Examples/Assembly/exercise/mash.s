# mash(x, y)


	.text
mash:
	
   # PROLOGUE
   subu   $sp, $sp, 8
   sw     $ra, 8($sp)
   sw     $fp, 4($sp)
   addu   $fp, $sp, 8
	
   # BODY
   li     $t0, 10
   lw 	  $t1, 4($fp)
   lw 	  $t2, 8($fp)
   mul    $t0, $t1, $t0
   add    $t0, $t0, $t2
   move   $v0, $t0

   # EPILOGUE
   move    $sp, $fp
   lw    $ra, ($fp)
   lw    $fp, -4($sp)
   jr    $ra

# max(x, y)

	.text
max:
   # PROLOGUE
   subu    $sp, $sp, 8
   sw    $ra, 8($sp)
   sw    $fp, 4($sp)
   addu    $fp, $sp, 8

   # BODY
   bgt    $a1, $a0, else
   move    $v0, $a0
   j     endif

else:	
   move     $v0, $a1
   j     endif

endif:	
   # EPILOGUE
   move    $sp, $fp
   lw    $ra, ($fp)
   lw    $fp, -4($sp)
   jr    $ra

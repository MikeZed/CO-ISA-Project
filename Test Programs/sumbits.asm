# sumbits.asm 

main: 	
		.word 0x100 156			 # set x = 156

		limm $sp, $zero, $zero, -54	 # set $sp = 128
		lw $a0, $zero, $zero, 0x100	 # get x from address 64

		jal $zero, $zero, $zero, sumbits #  calc $v0 = sumbits(x,n)
		
		sw $v0, $zero, $zero, 0x101	 # store sumbits(x, n) in 0x101
		halt $zero, $zero, $zero, 0	 # halt
		
sumbits: 
		limm $sp , S0 , $0 , 128
	    	limm $t0 , $0 , $0 , -6
		add  $sp , $0 , $t0 ,0
		sw   $s0 , $sp , $0 , 0 
		sw   $s1 , $sp , $0 , 1
		sw   $s2 , $sp , $0 , 2
		sw   $a0 , $sp , $0 , 3
		sw   $a1 , $sp , $0 , 4
		sw   $v0 , $sp , $0 , 5
		
		
	
	
	
	
Done: 	
		lw   $s0 , $sp , $0 , 0
		lw   $s1 , $sp , $0 , 1
		lw   $s2 , $sp , $0 , 2
		lw   $a0 , $sp , $0 , 3
		lw   $a1 , $sp , $0 , 4
		lw   $v0 , $sp , $0 , 5
		limm $t0 , $0 , $0 , 6
		add  $sp , $0 , $t0, 0

		branch jr, $ra, $0, 0

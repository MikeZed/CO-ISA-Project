#sort.asm
Main: 	
		limm $sp , $0 , $0 , 500
		.word 0x100 2 # set A[0] = 2
		.word 0x101 5 # set A[1] = 5
		.word 0x102 7 # set A[2] = 7
		.word 0x103 10 # set A[3] = 10
		.word 0x104 15 # set A[4] = 15
		.word 0x105 8 # set A[5] = 8
		.word 0x106 3 # set A[6] = 3
		.word 0x107 12 # set A[7] = 12
		.word 0x108 1 # set A[8] = 1
		.word 0x109 22 # set A[9] = 22
		.word 0x10a 24 # set A[10] = 24
		.word 0x10b 28 # set A[11] = 28
		.word 0x10c 23 # set A[12] = 23
		.word 0x10d 31 # set A[13] = 31
		.word 0x10e 34 # set A[14] = 34
		.word 0x10f 6 # set A[15] = 6
		limm $a0 , $0 , $0 ,0x100 #Start of the array let it's name be A (for comment purposes)
		limm $a1 , $0 , $0 , 16
		jal  $0 , $0 , $0 , Sort
		halt $0, $0, $0, 0

Sort: 		
	   	limm $t0 , $0 , $0 , -6
		add  $sp , $sp , $t0 , 0
		sw   $s0 , $sp , $0 , 0 
		sw   $s1 , $sp , $0 , 1
		sw   $s2 , $sp , $0 , 2
		sw   $a0 , $sp , $0 , 3
		sw   $a1 , $sp , $0 , 4
		sw   $v0 , $sp , $0 , 5
		
		limm $t0 , $0 , $0 , 1
		sub $a1, $a1 , $t0 ,0
	While:
			add $v0 , $0 , $0, 0	# vo: bool swapped = 0 (false)			
			sub $s2 , $0 , $t0, 0  # set j = -1
	For: 
			add $s2 , $s2 , $t0, 0 # j++
			branch bge, $s2 , $a1 , IF
			add $t2 , $a0 , $s2, 0 # A + j
			lw  $s0 , $t2 , $0 , 0 # s0 = A[j]
			lw  $s1 , $t2 , $0 , 1 # s1 = A[j+1]
			branch blt, $s0 , $s1 , Swap
			branch beq, $0, $0, For
	IF:
			branch bne, $v0 , $0 , While
			branch beq, $0 , $0 , End

	Swap: 
			sw  $s1 , $t2 , $0 , 0 #set A[j] = A[j+1]
			sw  $s0 , $t2 , $0 , 1 #set A[j+1] = A[j]
			add $v0 , $t0 , $0 , 0 #set v0 = 1 (true)
			branch beq, $0, $0, For
	End:			
			lw   $s0 , $sp , $0 , 0
			lw   $s1 , $sp , $0 , 1
			lw   $s2 , $sp , $0 , 2
			lw   $a0 , $sp , $0 , 3
			lw   $a1 , $sp , $0 , 4
			lw   $v0 , $sp , $0 , 5
			limm $t0 , $0 , $0 , 6
			add  $sp , $sp , $t0, 0
			branch jr, $ra, $0, 0
	

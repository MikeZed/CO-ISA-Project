#sort.asm
Sort:   
		limm $sp , S0 , $0 , 128
	    	limm $t0 , $0 , $0 , -6
		add  $sp , $0 , $t0
		sw   $s0 , $sp , $0 , 0 
		sw   $s1 , $sp , $0 , 1
		sw   $s2 , $sp , $0 , 2
		sw   $a0 , $sp , $0 , 3
		sw   $a1 , $sp , $0 , 4
		sw   $v0 , $sp , $0 , 5
		
		limm $a0 , $0 , $0 ,0x100 #Start of the array let it's name be A (for comment purposes)
		limm $a1 , $0 , $0 , 16   #Length of the array
		
		limm $t0 , $0 , $0 , 1
	While:
			add $v0 , $0 , $0	# vo: bool swapped = 0 (false)			
			sub $s2 , $0 , $t0  # set j = -1
	For: 
			add $s2 , $s2 , $t0 # j++
			branch bge, $s2 , $a1 , IF # Branch if index out of array
			add $t2 , $a0 , $s2 # A + j
			lw  $s0 , $t2 , $0 , 0 # s0 = A[j]
			lw  $s1 , $t2 , $0 , 1 # s1 = A[j+1]
			branch blt, $s0 , $s1 , Swap #Branch if swap is needed
	IF:
			branch bne, $vo , $0 , While

	Swap: 		#Swap two numbers 			
			sw  $s1 , $t2 , $0 , 0 #set A[j] = A[j+1]
			sw  $s0 , $t2 , $0 , 1 #set A[j+1] = A[j]
			add $v0 , $t0 , $0 #set v0 = 1 (true)
			branch beq, $0, $0, For
	End:			
			lw   $s0 , $sp , $0 , 0
			lw   $s1 , $sp , $0 , 1
			lw   $s2 , $sp , $0 , 2
			lw   $a0 , $sp , $0 , 3
			lw   $a1 , $sp , $0 , 4
			lw   $v0 , $sp , $0 , 5
			limm $t0 , $0 , $0 , 6
			add  $sp , $0 , $t0
	

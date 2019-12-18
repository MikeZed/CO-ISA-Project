#mult_mat

mult_mat:	
		.word 0x100 1
		.word 0x101 0
		.word 0x102 0
		.word 0x103 0
		.word 0x104 0
		.word 0x105 1
		.word 0x106 0
		.word 0x107 0
		.word 0x108 0
		.word 0x109 0
		.word 0x10A 1
		.word 0x10B 0
		.word 0x10C 0
		.word 0x10D 0
		.word 0x10E 0
		.word 0x10F 1
		.word 0x110 1
		.word 0x111 2
		.word 0x112 3
		.word 0x113 4
		.word 0x114 5
		.word 0x115 6
		.word 0x116 7
		.word 0x117 8
		.word 0x118 9
		.word 0x119 10
		.word 0x11A 11
		.word 0x11B 12
		.word 0x11C 13
		.word 0x11D 14
		.word 0x11E 15
		.word 0x11F 16						#set values of matrices 
		
		
		limm $sp , $0 , $0 , 0x200
		limm $t0 , $0 , $0 , -3				# t0 = -3
		add $sp , $sp , $t0, 0  			# sp = sp - 3 --- allocate space for stored values in stack
		sw $s0 , $sp , $0 , 0 				# store value of s0 in stack
		sw $s1 , $sp , $0 , 1 				# store value of s1 in stack
		sw $s2 , $sp , $0 , 2 				# store value of s2 in stack
		
		limm $s0, $0, $0, 0x0FC
		limm $s1 , $0 , $0 , 0x110 			# set $s1 to the address of the first argument of matrix B.
		limm $s2 , $0 , $0 , 0x120			# set $s2 to the address of the first argument of matrix C.
		
for1:	limm $t0 , $0 , $0 , 4 				# t0 = 4
		add  $s0 , $s0 , $t0 , 0  			# s0 is now pointing the next row of matrix A. if reached here first time - set s0 to first row of A.
		limm $t0 , $0 , $0 , 0x110
		branch bge, $s0 , $t0 , END 		#finished running on rhe columns of B, branch to END.
		limm $s1 , $0 , $0 , 0x110 			# set $s0 to the address of the first argument of matrix A.
		
for2:	lw $t0 , $s0 , $0 , 0 				# t0 = A[s0] 
		lw $t1 , $s1 , $0 , 0 				# t1 = B[s1]
		mul $t0 , $t0 , $t1, 0   			# t0 = t0*t1

		lw $t1 , $s0 , $0 , 1 				# t1 = A[s0+1] 
		lw $t2 , $s1 , $0 , 4 				# t2 = B[s1+4]
		mul $t1 , $t1 , $t2 , 0  				# t1 = t1*t0

		add $t0 , $t0 , $t1 , 0				# t0 = t1 + t0

		lw $t1 , $s0 , $0 , 2 				# t1 = A[s0+2] 
		lw $t2 , $s1 , $0 , 8 				# t2 = B[s1+8]
		mul $t1 , $t1 , $t2 , 0  			# t1 = t1*t2
		
		lw $t2  , $s0 , $0 , 3  			# t2 = A[s0+3] 
		lw $t3 , $s1 , $0 , 12 				# t3 = B[s1+12]
		mul $t2 , $t2 , $t3 , 0    			# t2 = t2*t3

		add $t1 , $t2 , $t1, 0				# t1 = t2 + t1
		
		add $t0 , $t0 , $t1 , 0				# t0 = t0 + t1 --- t0 has the value of the first argument in result matrix.
		sw $t0 , $s2 , $0 , 0 				# save result to the matrix. 
		
		limm $t0 , $0 , $0 , 1 				# t0 = 1
		add $s2 , $s2 , $t0 , 0   			# s2 is now pointing to the next argument in the result matrix C.
		add $s1 , $s1 , $t0  , 0  			# s1 is now pointing to the next column of matrix B.
		limm $t1 , $0 , $0 , 0x114		
		branch bge, $s1 , $t1 , for1 		#finished running on rhe rows of A, branch to for1.
		branch beq, $0 , $0 , for2 			#if reached here, go to for2.
		
END:	lw $s0 , $sp , $0 , 0 				# restore values from stack
		lw $s1 , $sp , $0 , 1 				# ""
		lw $s2 , $sp , $0 , 2 				# ""
		limm $t0 , $0 , $0 , 3 				# t0 = 3
		add $sp , $sp , $t0 , 0				# sp = sp + 3 --- release space in stack.
		halt $0, $0, $0 , 0
		
		
		
			

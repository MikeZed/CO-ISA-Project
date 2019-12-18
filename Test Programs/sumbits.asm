# sumbits.asm 

main: 	
		 .word 0x100 156		  # set x = 156

		 limm $sp, $zero, $zero, 128	  # set $sp = 128

		 lw $a0, $zero, $zero, 0x100	  # get x from 256 

		 limm $a1, $zero, $zero, 16 	  # set n = 16

		 jal $zero, $zero, $zero, sumbits # calc $v0 = sumbits(x, n)
		
		 sw $v0, $zero, $zero, 0x101	  # store sumbits(x, n) in 0x10

		 halt $zero, $zero, $zero, 0	  # halt
		
sumbits: 
		 limm $t0 , $zero, $0, -6 	  # $t0 = -6
		 add  $sp , $sp, $t0,  0	  # make room for 6 registers in stack 			
		 sw   $s0 , $sp , $zero, 0	  # store $s0	
		 sw   $s1 , $sp , $zero, 1	  # store $s1	
		 sw   $s2 , $sp , $zero, 2	  # store $s2	
		 sw   $a0 , $sp , $zero, 3	  # store $a0	
		 sw   $a1 , $sp , $zero, 4	  # store $a1		
		 sw   $ra , $sp , $zero, 5	  # store $ra	

		 limm $t0, $zero, $zero, 1        # $t0 = 1
	   	 sra $s1, $a1, $t0, 0  	    	  # $s1 = h = n >> 1
		 sll $s2, $t0, $s1, 0  		  # $s2 = 1 << h 
		 sub $s2, $s2, $t0, 0    	  # $s2 = mask = (1 << h) - 1 	

		 branch bne, $a1, $t0, else 	  # check if n==1 , it not jump to else 
		 and $s0, $a0, $t0, 0		  # $s0 = x & 1
		 branch beq, $zero, $zero, Done   # jump to Done 	
else: 		
		 sra $a0, $a0, $s1, 0   	  # x -> x >> h
		 and $a0, $a0, $s2, 0		  # x -> (x >> h) & mask
		 add $a1, $zero, $s1, 0 	  # n -> h 
			
		 jal $zero, $zero, $zero, sumbits # calc $v0 = sumbits(x >> h) & mask, h)
		 add $s0, $v0, $zero, 0           # $s0 = sumbits(x >> h) & mask, h) 

		 lw  $a0 , $sp , $0 , 3	          # restore x 
		 and $a0, $a0, $s2, 0	          # x -> x & mask 
	 	 add $a1, $zero, $s1, 0 	  # n -> h 
 
		 jal $zero, $zero, $zero, sumbits # calc $v0 = sumbits(x & mask, h)
		 add $s0, $s0, $v0, 0             # $s0 = sumbits(x >> h) & mask, h) + sumbits(x & mask, h)

Done: 	
		 add $v0, $s0, $zero, 0           # save result in $v0

		 lw   $s0, $sp , $zero, 0	  # restore $s0 
		 lw   $s1, $sp , $zero, 1	  # restore $s1 
	    	 lw   $s2, $sp , $zero, 2	  # restore $s2 
		 lw   $a0, $sp , $zero, 3	  # restore $a0 
		 lw   $a1, $sp , $zero, 4 	  # restore $a1 
		 lw   $ra, $sp , $zero, 5	  # restore $s0 
		 limm $t0, $zero, $zero, 6	  # $t0 = 6
		 add  $sp, $zero, $t0, 0	  # release room in stack 

		 branch jr, $ra, $0, 0		  # return 

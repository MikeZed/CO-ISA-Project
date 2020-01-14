

main: 
							# initialize IO Registers 
		limm $t0, $zero, $zero, 1 		# $t0 = 1 for IO Registers[0]
		
		
		limm $t1, $zero, $zero, 75637863783673	# $t1 = 75637863783673 for IO Registers[2] - hndl_int הכתובת של
		
		
		limm $t2, $zero, $zero, 256	        # $t2 = 256 for IO Registers[3]
		
		
		
		out $zero, $zero, $zero, 0		# initialize IORegister[0]
		out $zero, $zero, $zero, 1		# initialize IORegister[1]
		out $t1,   $zero, $zero, 2		# initialize IORegister[2]
		out $t2,   $zero, $zero, 3		# initialize IORegister[3]
		out $zero, $zero, $zero, 4		# initialize IORegister[4] 
		out $zero, $zero, $zero, 5		# initialize IORegister[5]
		
		out $t0, $zero, $zero, 0		# enable interrupts - IORegister[0] = 1 
		
		
			
		jal $zero, $zero, $zero, cnt_time 	#  start counting 
		
		
		halt $zero, $zero, $zero, 0 		# halt




hndl_int: 						# handles interrupts 
		
		

							# handles interrupt 1 - Timer
							# handles interrupt 2 - BTNC
							# handles interrupt 3 - BTND 	


		branch reti, $zero, $zero, 0 		# return to program









cnt_time:						# updates time counters 
		
		
		 limm $t0 , $zero, $0, -6 		# $t0 = -6
		 add  $sp , $sp, $t0,  0		# make room for 6 registers in stack 			
		 sw   $s0 , $sp , $zero, 0		# store $s0	
		 sw   $s1 , $sp , $zero, 1		# store $s1	
		 sw   $s2 , $sp , $zero, 2		# store $s2	
		 sw   $a0 , $sp , $zero, 3		# store $a0	
		 sw   $a1 , $sp , $zero, 4		# store $a1		
		 sw   $ra , $sp , $zero, 5		# store $ra	
		 
							# save total elapsed seconds
							# save seconds
							# save minutes
						
							# check if min is 1, 3 , 5...
							# if true play music  
							
							
							
						
						
					
							
							# check if need to interrupt
		in $t0, $zero, $zero, 1			# get IORegister[1]
		branch bnq, $t0, $zero, hndl_int 	# if IORegister[1] is not zero -> handle interrupt

		
		 limm $t0 , $zero, $0, -6 		# $t0 = -6
		 add  $sp , $sp, $t0,  0		# make room for 6 registers in stack 			
		 sw   $s0 , $sp , $zero, 0		# store $s0	
		 sw   $s1 , $sp , $zero, 1		# store $s1	
		 sw   $s2 , $sp , $zero, 2		# store $s2	
		 sw   $a0 , $sp , $zero, 3		# store $a0	
		 sw   $a1 , $sp , $zero, 4		# store $a1		
		 sw   $ra , $sp , $zero, 5		# store $ra	
		 






		branch jr $ra, $zero, 0  		# return to main 

















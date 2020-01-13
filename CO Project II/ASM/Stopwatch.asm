

main: 
							# intialize IO Registers 
		limm $t0, $zero, $zero, 256	
	 	limm $t1, $zero, $zero, 1 
		
		out $zero, $zero, $zero, 0		# intialize IORegister[0]
		out $zero, $zero, $zero, 1		# intialize IO Registers[1]
		out $zero, $zero, $zero, 2		# intialize IO Registers[2]
		out $t0, $zero, $zero, 3		# intialize IO Registers[3]
		out $zero, $zero, $zero, 4		# intialize IO Registers[4]
		out $zero, $zero, $zero, 5		# intialize IO Registers[5]
		
		out $t1, $zero, $zero, 1		# enable interrupts 
		
			
		jal $zero, $zero, $zero, cnt_time 	#  start counting 
		
	
		
		halt $zero, $zero, $zero, 0 		# halt





cnt_time:						# updates time counters 
						
							# save total elapsed seconds
							# save seconds
							# save minutes
						
							# check if min is 1, 3 , 5...
							# if true play music  


		branch jr $ra, $zero, 0  		# return to main 




hndl_int: 						# handles interrupts 

							# handles interrupt 1 - Timer
							# handles interrupt 2 - BTNC
							# handles interrupt 3 - BTND 	


		branch reti, $zero, $zero, 0 		# return to program








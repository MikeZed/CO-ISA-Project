

main: 
		#----------------------#	
		# initialize IO Registers 
		
		limm $t0, $zero, $zero, 1 			# $t0 = 1 for IO Registers[0]
		
		
		limm $t1, $zero, $zero, 75637863783673		# $t1 = 75637863783673 for IO Registers[2] - hndl_int הכתובת של
		
		
		limm $t2, $zero, $zero, 256	       		# $t2 = 256 for IO Registers[3]
		
		
		
		out $zero, $zero, $zero, 0			# initialize IORegister[0]
		out $zero, $zero, $zero, 1			# initialize IORegister[1]
		out $t1,   $zero, $zero, 2			# initialize IORegister[2]
		out $t2,   $zero, $zero, 3			# initialize IORegister[3]
		out $zero, $zero, $zero, 4			# initialize IORegister[4] 
		out $zero, $zero, $zero, 5			# initialize IORegister[5]
		
		out $t0, $zero, $zero, 0			# enable interrupts - IORegister[0] = 1 
				
				
		#----------------------#	
		# initialize counters
		
		limm $s0, $zero, $zero, 0			# $s0 - total elapsed seconds
		limm $s1, $zero, $zero, 0			# $s0 - seconds
		limm $s2, $zero, $zero, 0 			# $s0 - minutes
	
		limm $s3, $zero, $zero, 0 			# $s3 - for pausing -> 1 - pouse, 0 - running

		#----------------------#	
cnt_time:	# updates time counters 
		
		# check $s3 
		branch bne $s3, $zero, $zero, branch_pause  		# $s3 != 0 -> $s3 == 1 -> in pause 
		
		# calc $s1 (seconds) from $s0 
		# calc $s2 (minutes)  from $s0 
						
		# check if min is 1, 3 , 5...
		# if true play music  
							
							

branch_pause:

		branch beq $zero, $zero, $zero, cnt_time  	# jump to cnt_time


Done:
		halt $zero, $zero, $zero, 0 			# halt










hndl_int: 							# handles interrupts 
		
		#----------------------#	
		# handles interrupt 1 - Timer
		# check if timer interrupt - if it is add 1 to $s0 
		
		#----------------------#
		# handles interrupt 2 - BTNC
		# use $s3 for pausing 
		
		

		#----------------------#
		# handles interrupt 3 - BTND 	
		# check if BTND interrupt - if it is $s0 = 0 
		
		
		
		#----------------------#		
								
								


		branch reti, $zero, $zero, 0 			# return to program







///////////////////////////////////////////////////////////////////////////////////////////////////////


								# check if need to interrupt
		in $t0, $zero, $zero, 1				# get IORegister[1]
		branch bnq, $t0, $zero, hndl_int 		# if IORegister[1] is not zero -> handle interrupt




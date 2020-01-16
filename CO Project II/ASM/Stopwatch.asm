
branch beq $zero, $zero, $zero, main  				# jump to main 


hndl_int: 							# handles interrupts 
		in $t0, $zero, $zero, 1				# get IORegister[1] - interrupt reason
			
		#----------------------#	
		# handles interrupt 1 - Timer
		
		limm $t1, $zero, $zero, 1			# $t1 = 1
		and $t1, $t0, $t1, 0				# $t1 = timer interrupt bit
		branch beq, $t1, $zero, no_int0			# $t1 == 0 -> no timer interrupt 
	
		branch bne $s0, $zero, $zero, no_int0		# $s0 != 0 -> $s0 == 1 -> in pause 
		
		limm $t1, $zero, $zero, 1			# $t1 = 1
		add $s1, $s1, $t1, 0 				# $s1 += 1	
			
no_int0:	
		#----------------------#
		# handles interrupt 2 - BTNC
		
		limm $t1, $zero, $zero, 2			# $t1 = 2
		and $t1, $t0, $t1, 0				# $t1 = btnc interrupt bit
		branch beq, $t1, $zero, no_int1			# $t1 == 0 -> no btnc interrupt 
		
		
		limm $t1, $zero, $zero, 1			# $t1 = 1
		sub $s0, $t1, $s0, 0 				# $s1 = $t1 - $s0 = 1 - $s0 = NOT($s0)	
		
no_int1:
		#----------------------#
		# handles interrupt 3 - BTND 	
		
		limm $t1, $zero, $zero, 2			# $t1 = 4
		and $t1, $t0, $t1, 0				# $t1 = btnc interrupt bit
		branch beq, $t1, $zero, no_int2  		# $t1 == 0 -> no btnc interrupt  
		
		limm $s1, $zero, $zero, 0 			# $s1 = 0 
		limm $s2, $zero, $zero, 0 			# $s2 = 0 
		
no_int2:		
		#----------------------#		
												
		branch reti, $zero, $zero, 0 			# return to program



main: 
		#----------------------#	
		# initialize IO Registers 
		
		limm $t0, $zero, $zero, 1 			# $t0 = 1 for IO Registers[0]
		
		limm $t1, $zero, $zero, 2			# $t1 = 2 for IO Registers[2] - hndl_int הכתובת של
		
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
		
		limm $s1, $zero, $zero, 0			# $s1 - seconds
		limm $s2, $zero, $zero, 0 			# $s2 - minutes
	
		limm $s0, $zero, $zero, 0 			# $s0 - for pausing -> 1 - pause, 0 - running 

		#----------------------#	
cnt_time:	# updates time counters 
		
		limm $t2, $zero, $zero, 60			# $t2 = 60
		branch blt, $s1, $t2, no_add_min 		# $s0 < 60 -> a minute has not yet passed 
	
		limm $t3, $zero, $zero, 1 			# $t3 = 1 
		add $s2, $s2, $t3, 0				# $s2 += 1 
		sub $s1, $s1, $t2, 0				# $s1 -= 60   
		
no_add_min:
		#----------------------#
		# audio
		# check if min is 1, 3 , 5...
		# if true play music  
		
		limm $t2, $zero, $zero, 1			# $t2 = 1 
		and $t2, $s2, $t2, 0 				# $t2 = $s2 & 1 -> $t2 == 1 then $s2 is odd, else $s2 is even 
		branch beq $t2, $zero, no_play 			# $t2 == 0 -> $s2 is even -> no_play 
		branch bne $s1, $zero, no_play 			# $s1 != 0 -> a full minute hasn't passed yet -> no_play 
		
		play $zero, $zero, $zero, 0 			# play audio 
		
no_play: 		







		#----------------------#
		# display
		# calc seconds, minutes for display, e.g. if min = 0x14 then minutes in SSD will be: 20  
		
		sll $t2
		out $t0, $zero, $zero, 5			# update SSD 				
				
	
		#----------------------#
		
		
		
		
		
		
		
		
		
		
	
		branch beq $zero, $zero, $zero, cnt_time  	# jump to cnt_time

Done:		halt $zero, $zero, $zero, 0 			# halt






///////////////////////////////////////////////////////////////////////////////////////////////////////


								# check if need to interrupt
		in $t0, $zero, $zero, 1				# get IORegister[1]
		branch bnq, $t0, $zero, hndl_int 		# if IORegister[1] is not zero -> handle interrupt




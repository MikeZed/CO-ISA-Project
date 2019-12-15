
string2int:
	add t0 a0 0 
	add v0 0 0 
Loop: 	
	lbu t1 0(t0) 
	beq t1 0 Done 
	addi t1 t1 -48 

	addi t2 t1 -9 
	bgtz t2 Error

	addi t2 t1 +1
	blez t2 Error

	
	muli v0 v0 10 
	add v0 v0 t1 



	
	addi t0 t0 +1
	j Loop
Error: 
	addi v0 0 -1
Done: 
	jr ra
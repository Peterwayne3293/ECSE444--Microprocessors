	AREA test, CODE, READONLY
	
	export asm_variance          ; label must be exported if it is to be used as a function in C
asm_variance

	MOV R2, R0											; move R0 to R2, to preserve R0 - base address
	MOV R3, R1									 		; move R1 to R3, to preserve R1 - size
	VSUB.F32 S1, S1, S1					 		;
	
total
	SUBS R3, R3, #1							 		;	loop counter (N = N-1)
	BLT mean										 		;	loop has finished?
	VLDR.F32 S0, [R2]						 		;	move matrix value to S0
	VADD.F32 S1, S0, S0				 			;	Sum S0 to S1 get total
	ADD R2, R2, #4									;	go to the address of next index in the array
	B total													; branch total

mean
	MOV S0, R1											; move size of array(N) to S0
	VCVT.F32.U32 S0, S0 						; convert N to float
	VDIV S1, S1, S0								; divide to get total by N to get mean
	;total
	
	;divide
	
	; find subtraction of data from mean
	
	; square the result 
	
	; sum the squares
		

	END
	AREA test, CODE, READONLY
	
	export asm_variance          ; label must be exported if it is to be used as a function in C
asm_variance

	MOV R3, R0											; move R0 to R2, to preserve R0 - base address
	MOV R4, R1									 		; move R1 to R3, to preserve R1 - size
	VSUB.F32 S1, S1, S1					 		;
	VSUB.F32 S2, S2, S2					 		
	VSUB.F32 S3, S3, S3					
	
total
	SUBS R4, R4, #1							 		;	loop counter (N = N-1)
	BLT mean										 		;	loop has finished? branch mean
	VLDR.F32 S0, [R3]						 		;	move matrix value to S0
	VADD.F32 S1, S1, S0				 			;	Sum S0 to S1 get total
	ADD R3, R3, #4									;	go to the address of next index in the array
	B total													; branch total

mean
	VMOV.F32 S0, R1									; move size of array(N) to S0
	VCVT.F32.U32 S0, S0 						; convert N to float
	VDIV.F32 S1, S1, S0							; divide to get total by N to get mean
	
variance
	SUBS R1, R1, #1									;	loop counter (N = N-1)
	BLT divide											;	loop has finished? branch done
	VLDR.F32 S2, [R0]								;	move matrix value to S2	
	VSUB.F32 S2, S2, S1							; subtract matrix value from mean
	VMLA.F32 S3, S2, S2							;	multiply to get (matrixVal-mean)^2
	ADD R0, R0, #4									; go to next index in the array
	B variance											; branch variance
	
divide
	;VLDR.F32 S4, =1.0							;	use if TA wants to see
	;VSUB.F32 S0, S0, S4						;	use if TA wants to see
	VDIV.F32 S3, S3, S0							; Divide to get (matrixVal-mean)^2/N
	
done	
	VSTR.F32 S3, [R2]               ; store dot product in the pointer (float *dot_product) that was provided
	
	BX LR                           ; return

	END
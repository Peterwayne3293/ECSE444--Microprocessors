	AREA test, CODE, READONLY
	
	export asm_dot_product          ; label must be exported if it is to be used as a function in C
asm_dot_product
	
	PUSH{R4, R5}	                 	; saving context according to calling convention
	;VSUB S3, S3, S3			            ; result = 0
	
loop
	SUBS R2, R2, #1                 ; loop counter (N = N-1)
	BLT done                        ; loop has finished?
	ADD R4, R0, R2, LSL #2          ; creating base address for the next element in the array
	ADD R5, R1, R2, LSL #2          ; creating base address for the next element in the array
	VLDR.f32 S0, [R4]								; Matrix 1 first index value move to S0
	VLDR.f32 S1, [R5]								;	Matrix 2 first index valuemove to S1
	VMUL.f32 S2, S0, S1							;	multiply S0 and S1
	VADD.f32 S3, S3, S2							; add to result
	BGT continue                    ; branch loop
	
continue
	B loop                          ; loop
	
done
	VSTR.f32 S3, [R3]               ; store dot product in the pointer (float *dot_product) that was provided
	
	POP{R4, R5}                    	; restore context
	BX LR                           ; return
	
	END
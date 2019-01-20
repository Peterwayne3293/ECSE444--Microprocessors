	AREA test, CODE, READONLY
	
	export asm_dot_product          ; label must be exported if it is to be used as a function in C
asm_dot_product
	
	VSUB.F32 S2, S2, S2			        ; result = 0
	
loop
	SUBS R2, R2, #1                 ; loop counter (N = N-1)
	BLT done                        ; loop has finished?
	VLDR.F32 S0, [R0]								; Matrix 1 index value move to S0
	VLDR.F32 S1, [R1]								;	Matrix 2 index value move to S1
	VMLA.F32 S2, S0, S1							;	multiply S0 and S1 and add to S2
	ADD R0, R0, #4									; go to the address of next index in first array
	ADD R1, R1, #4									;	go to the address of next index in second array
	BGT continue                    ; branch loop
	
continue
	B loop                          ; loop
	
done
	VSTR.F32 S2, [R3]               ; store dot product in the pointer (float *dot_product) that was provided
	
	BX LR                           ; return
	
	END

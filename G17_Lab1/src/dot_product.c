#include <stdio.h>
#include <arm_math.h>
#include <asm_dot_product.h>

void dot_product(float *f1_array, float *f2_array, int N) {
	
	float dot_product;
	
	float a_dot_product;
	
	float DSP_dot_product;
	
	//----C variance function-----
	int i;
	dot_product = 0;
	for(i=0; i<N ; i++) {
		dot_product += f1_array[i]*f2_array[i];
	}
	//---------------------------
	
	/* Assembly dot product function */
	asm_dot_product(f1_array, f2_array, N, &a_dot_product);
	
	/* CMSIS-DSP dot product function */
	arm_dot_prod_f32(f1_array, f2_array, N, &DSP_dot_product);
	
	printf("Pure C   : Dot Product = %f\n", dot_product);
	printf("Assembly : Dot Product = %f\n", a_dot_product);
	printf("C: DSP   : Dot Product = %f\n", DSP_dot_product);
}

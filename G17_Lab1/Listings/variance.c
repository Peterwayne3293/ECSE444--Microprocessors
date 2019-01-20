#include <stdio.h>
//#include <arm_math.h>

#include <asm_variance.h>

void variance(float *f10_array, float *f1000_array){
	
	float variance;
	
	float a_variance;
	
	float DSP_variance;
	
	int i;
	
	/* LOOP TO FIND MAX*/

	variance = 0;
	
	for(i=0 ; i<10 ; i++) {
		variance += f10_array[i]*f10_array[i];
	}
	
	/* Assembly max function */
	asm_variance(f10_array, f10_array, 10, &a_variance);
	
	/* CMSIS-DSP max function */
	//arm_dot_prod_f32(f10_array, f10_array, 10, &DSP_dot_product);
	
	printf("Pure C   : Variance = %f\n", variance);
	printf("Assembly : Variance = %f\n", a_variance);
	printf("C: DSP   : Variance = %f\n", DSP_variance);

	
	/* LOOP TO FIND MAX*/
	variance = 0;
	for(i=0 ; i<1000 ; i++) {
		variance += f10_array[i]*f10_array[i];
	}
	
	/* Assembly max function */
	asm_variance(f1000_array, f1000_array, 1000, &a_variance);
	
	/* CMSIS-DSP max function */
	//arm_dot_prod_f32(f1000_array, f1000_array, 1000, &DSP_dot_product);
	
	printf("Pure C   : Variance = %f\n", variance);
	printf("Assembly : Variance = %f\n", a_variance);
	printf("C: DSP   : Variance = %f\n", DSP_variance);
	
	
}
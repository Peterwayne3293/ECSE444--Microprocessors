#include <stdio.h>
#include <arm_math.h>
#include <asm_variance.h>

void variance(float *f_array, int N){
	
	float variance;
	float sum;
	float mean;
	
	float a_variance;
	
	float DSP_variance;
	
	//----C variance function-----
	/* LOOP TO FIND SUM*/
	int i;
	sum = 0;
	for(i=0 ; i<N ; i++) {
		sum += f_array[i];
	}
	
	//find mean
	mean = 0;
	mean = sum/N;
	
	//find variance
	variance = 0;
	for(i=0 ; i<N; i++){
		variance+= (f_array[i]-mean)*(f_array[i]-mean)/(N-1);
	}
	//-----------------------------
	
	/* Assembly variance function */
	asm_variance(f_array, N, &a_variance);
	
	/* CMSIS-DSP variance function */
	arm_var_f32(f_array, N, &DSP_variance);
	
	
		
	printf("Pure C   : Variance = %f\n", variance);
	printf("Assembly : Variance = %f\n", a_variance);
	printf("C: DSP   : Variance = %f\n", DSP_variance);	
}

#include <stdio.h>
#include <arm_math.h>
#include <asm_max.h>

void max(float *f_array, int N){
	
	float max;
	int max_idx;

	float a_max;
	int a_max_idx;
	
	float DSP_max;
	uint32_t DSP_max_idx;
	
	int i;
	
	/* LOOP TO FIND MAX*/
	max = f_array[0];
	max_idx = 0;
	for(i=0 ; i<N ; i++) {
		if(f_array[i] > max) {
			max = f_array[i];
			max_idx = i;
		}
	}
	
	/* Assembly max function */
	asm_max(f_array, N, &a_max, &a_max_idx);
	
	/* CMSIS-DSP max function */
	arm_max_f32(f_array, N, &DSP_max, &DSP_max_idx);
	
	printf("Pure C   : Max element f[%d] = %f\n", max_idx, max);
	printf("Assembly : Max element f[%d] = %f\n", a_max_idx, a_max);
	printf("C: DSP   : Max element f[%d] = %f\n", DSP_max_idx, DSP_max);
}

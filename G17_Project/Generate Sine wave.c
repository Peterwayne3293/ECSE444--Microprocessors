//----------Sine Function---------------
//float32_t arm_sin_f32(float32_t x)
//http://www.keil.com/pack/doc/CMSIS/DSP/html/group__sin.html#gae164899c4a3fc0e946dc5d55555fe541

//-------------Matrix Multiplication----------------
//arm_status arm_mat_mult_f32(const arm_matrix_instance_f32* pSrcA, const arm_matrix_instance_f32* pSrcB, arm_matrix_instance_f32 *pDst )	
//https://www.keil.com/pack/doc/CMSIS/DSP/html/group__MatrixMult.html#ga917bf0270310c1d3f0eda1fc7c0026a0

/*--------------------Private function prototypes -----------------*/
void generateSineWave(float frequency1, float frequency2);
void signalMixer(float *mixCoef);
/*-----------------------------------------------------------------*/

/*-------------User Global Variables----------------------------*/

//to be changed to float32_t
float signal1Array[32000];	//most probably will not need
float signal2Array[32000];	//most probably will not need
float mixedSignal1[32000];
float mixedSignal2[32000];
float mixerArray[2][2];
float mixedSignalContainer[2][32000];
float signalContainer[2][32000];	//consider using 2-D array
/*-----------------------------------------------------------------*/

void generateSineWave(float signalFrequency1, float signalFrequency2){
	int samplingFrequency = 16000;
	int signalTime = 2;
	signalFrequency1 = 440;
	
	int sample;
	//generate samples worth 2 seconds of a sine wave
	for (sample = 0; sample < samplingFrequency*signalTime; sample++){
        signalContainer[0][sample] = arm_sin_f32(2*signalFrequency1*sample/samplingFrequency);     //store to array
		signalContainer[1][sample] = arm_sin_f32(2*signalFrequency2*sample/samplingFrequency);     //store to array

		signal1Array[sample] = arm_sin_f32(2*signalFrequency1*sample/samplingFrequency);
		signal2Array[sample] = arm_sin_f32(2*signalFrequency2*sample/samplingFrequency);
	}
}

void signalMixer(void){
	arm_mat_mult_f32 (mixerArray, signalContainer, mixedSignalContainer);
}

int main (void){
	float frequency1 = 440;
	float frequency2 = 440;

	generateSineWave(frequency1, frequency2);
	
	signalMixer();
}
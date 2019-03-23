//----------Sine Function---------------
//float32_t arm_sin_f32(float32_t x)
//http://www.keil.com/pack/doc/CMSIS/DSP/html/group__sin.html#gae164899c4a3fc0e946dc5d55555fe541

//-------------Matrix Multiplication----------------
//arm_status arm_mat_mult_f32(const arm_matrix_instance_f32* pSrcA, const arm_matrix_instance_f32* pSrcB, arm_matrix_instance_f32 *pDst )	
//https://www.keil.com/pack/doc/CMSIS/DSP/html/group__MatrixMult.html#ga917bf0270310c1d3f0eda1fc7c0026a0

//https://github.com/ARM-software/CMSIS/blob/master/CMSIS/DSP_Lib/Examples/arm_matrix_example/ARM/arm_matrix_example_f32.c
//Doing multiplication
//define an array
//instantiate the array


/*-----------------library inlcuded------------------*/
#include "arm_math.h"

/*--------------------Private function prototypes -----------------*/
void generateSineWave(float_32 frequency1, float_32 frequency2);
void signalMixer(float_32 *mixCoef);
/*-----------------------------------------------------------------*/

/*-------------User Global Variables----------------------------*/

//to be changed to float32_t

#define SamplingFrequency 16000		//16000 Hz
#define SignalTime 2				//2 seconds
#define SampleSize SamplingFrequency*SignalTime

float32_t signal1Array[SampleSize];	//most probably will not need
float32_t signal2Array[SampleSize];	//most probably will not need
float32_t signalContainer[2][SampleSize];	//consider using 2-D array

float32_t mixedSignal1[SampleSize];
float32_t mixedSignal2[SampleSize];
float32_t mixerArray[2][2];
float32_t mixedSignalContainer[2][SampleSize];	//consider using 2-D array 

//array definitions
float32_t signal_f32[SampleSize*2];
float32_t mixer_f32[4];
float32_t mixedSignal_f32[SampleSize*2]

//array instances
arm_matrix_instance_f32 signal;
arm_matrix_instance_f32 mixer;
arm_matrix_instance_f32 mixedSignal;


// Generates two signals based on the frequency given
void generateSineWave(float32_t signalFrequency1, float32_t signalFrequency2){
	signalFrequency1 = 440;
	int sample;
	//generate samples worth 2 seconds of a sine wave
	for (sample = 0; sample < SampleSize; sample++){
        
		/*-----------------------signal generation using 2-D array---------------------------*/
		signalContainer[0][sample] = arm_sin_f32(2*signalFrequency1*sample/SamplingFrequency);     //Signal1
		signalContainer[1][sample] = arm_sin_f32(2*signalFrequency2*sample/SamplingFrequency);     //Signal2
		/*-----------------------------------------------------------------------------------*/

		/*-----------------------signal generation using 1-D array---------------------------*/
		signal1Array[sample] = arm_sin_f32(2*signalFrequency1*sample/SamplingFrequency);	//Signal1
		signal2Array[sample] = arm_sin_f32(2*signalFrequency2*sample/SamplingFrequency);	//Signal2
		/*-----------------------------------------------------------------------------------*/

		/*-----------------------signal generation to be used in ARM matrix instances---------------------------*/
		signal_f32[sample] = arm_sin_f32(2*signalFrequency1*sample/SamplingFrequency);     //Signal1
		signal_f32[sample+SampleSize] = arm_sin_f32(2*signalFrequency2*sample/SamplingFrequency);     //Signal2
		/*-----------------------------------------------------------------------------------*/
	}	
}


// Generates a mixed signal based on the two generated signals and given linear coefficients
void signalMixer(void){

	/*-------------------------Signal Mixing without using ARM CMSIS library functions--------------------------*/
	int sample;
	for (sample = 0; sample<32000; sample++){
		//Linear summation of two signals
		mixedSignalContainer[0][sample] = mixerArray[0][0]*signalContainer[0][sample] + mixerArray[0][1]*signalContainer[1][sample];	//Mixed signal 1
		mixedSignalContainer[1][sample] = mixerArray[1][0]*signalContainer[0][sample] + mixerArray[1][1]*signalContainer[1][sample];	//Mixed signal 2
	}
	/*----------------------------------------------------------------------------------------------------------*/


	/*------------------------initializing matrix instances---------------------*/
	int rows = 2;
	int columns = 2;
	arm_mat_init_f32(&mixer, rows, columns, mixer_f32);
	
	
	int rows = 2;	//number of sign waves
  	int columns = SampleSize;
	arm_mat_init_f32(&signal, rows, columns, (float32_t *)signal_f32);
	arm_mat_init_f32(&mixedSignal, rows, columns, (float32_t *)mixedSignal_f32);

	/*-------------------------------------------------------------------------*/
	
	/*------------Multiply MIXER with SIGNAL to get MIXED SIGNALS---*/
	arm_mat_mult_f32(&mixer, &signal, &mixedSignal);
	/*--------------------------------------------------------------*/
}

int main (void){
	float32_t frequency1 = 440;
	float32_t frequency2 = 440;

	//take input for frequency from UART
	generateSineWave(frequency1, frequency2);

	//take input for mixerArray from UART
	signalMixer();
}



#include "arm_math.h"
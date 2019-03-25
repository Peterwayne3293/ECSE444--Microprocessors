#ifndef __Generate_Sine_Wave__   /* Include guard */
#define __Generate_Sine_Wave__

/*-----------------library inlcuded------------------*/
#include "arm_math.h"

/*-------------User Global Variables----------------------------*/

#define SamplingFrequency 16000		//16000 Hz
#define SignalTime 2				//2 seconds
#define SampleSize SamplingFrequency*SignalTime

//User input values
float32_t frequencyVal[2] = {0.0,0.0};	//frequency values
float32_t mixerVal[4] = {0.0,0.0,0.0,0.0};	//mixer values

//Signal arrays
float32_t signal1Array[SampleSize];	//most probably will not need
float32_t signal2Array[SampleSize];	//most probably will not need
float32_t signalContainer[2][SampleSize];	//consider using 2-D array

//Mixed Signal arrays
float32_t mixedSignal1[SampleSize];	//most probably will not need
float32_t mixedSignal2[SampleSize];	//most probably will not need
float32_t mixerArray[2][2];	//consider using 2-D array
float32_t mixedSignalContainer[2][SampleSize];	//consider using 2-D array 

//array definitions
float32_t signal_f32[SampleSize*2];
float32_t mixer_f32[4];
float32_t mixedSignal_f32[SampleSize*2];

//array instances
arm_matrix_instance_f32 signal;
arm_matrix_instance_f32 mixer;
arm_matrix_instance_f32 mixedSignal;

//Enumerator for system state
enum systemOption{None, GenerateWaves, MixWaves, PlayUnmixed, PlayMixed} optionState;
enum inputState{OK, NOT_OK};

//Function Declarations
void userInterfaceWelcome(void);//UI function to show welcome message 
void userInterfaceOptions(void);//UI function to input option state
void userInterfaceGenerateSignal();//UI function to input signal frequency 
void userInterfaceMixer(void);//UI function to input matrix coefficients
int isCorrect();//Function to verify input with the user
void generateSineWave(void);//Generates two signals based on the frequency given
void signalMixer(void);//// Generates a mixed signal based on the two generated signals and given linear coefficients

#endif // __Generate_Sine_Wave__

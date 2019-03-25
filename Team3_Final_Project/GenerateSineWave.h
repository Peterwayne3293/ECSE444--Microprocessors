#ifndef __Generate_Sine_Wave__   /* Include guard */
#define __Generate_Sine_Wave__

/*-----------------library inlcuded------------------*/
#include "arm_math.h"
#include "stm32l4xx_hal.h"

//Enumerator for system state
enum systemOption{None, GenerateWaves, MixWaves, PlayUnmixed, PlayMixed};
enum inputState{OK, NOT_OK};

//External Variable
extern UART_HandleTypeDef huart1;
extern enum systemOption optionState;

//Function Declarations
void userInterfaceWelcome(void);//UI function to show welcome message 
void userInterfaceOptions(void);//UI function to input option state
void userInterfaceGenerateSignal(void);//UI function to input signal frequency 
void userInterfaceMixer(void);//UI function to input matrix coefficients
int isCorrect(void);//Function to verify input with the user
void generateSineWave(void);//Generates two signals based on the frequency given
void signalMixer(void);//// Generates a mixed signal based on the two generated signals and given linear coefficients

#endif // __Generate_Sine_Wave__

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

// Enumerator for system state
enum inputState{OK, NOT_OK};


int main (void){

	/*---------------------------------------------------------------------------------------------------------------------*/
		//Character	0		1		2		3		4		5		6		7		8		9
	//ASCII			48	49	50	51	52	53	54	55	56	57
	//--------------Take input from UART for mixerArray---------------
	
	//Input Arrays
	char inputCoeff[4][5] = {
							{0,0,'.',0,0},
							{0,0,'.',0,0},
							{0,0,'.',0,0},
							{0,0,'.',0,0}
							};
	
	char inputFreq[2][6] = {
							{0,0,'.',0,0},
							{0,0,'.',0,0},
							};
							
	
	char comfirmInput[2];
	
	//Output Message Arrays
	char linearCoefficients[33] = {'\n','L','i','n','e','a','r',' ','C','o','m','b','i','n','a','t','i','o','n',' ','C','o','e','f','f','i','c','i','e','n','t','s','\n'};
	
	char mixerExampleMessage[65] = {'E','n','t','e','r',' ','t','o','t','a','l',' ','4',' ','d','i','g','i','t','s',',',' ','w','i','t',
									'h',' ','2',' ','d','e','c','i','m','a','l',' ','p','l','a','c','e','s',' ','a','c','c','u','r','a','c','y',
									'\n','L','i','k','e',' ','5','5','.','7','7','\n'};
	
	char mixerVarMessage[4][13] = {
		{'\n','E','n','t','e','r',' ','a','0','0', ' ', ':', ' '},
		{'\n','E','n','t','e','r',' ','a','0','1', ' ', ':', ' '},
		{'\n','E','n','t','e','r',' ','a','1','0', ' ', ':', ' '},
		{'\n','E','n','t','e','r',' ','a','1','1', ' ', ':', ' '}
		};

	char frequencyValues[16] = {'\n','S','i','g','n','a','l',' ','F','r','e','q','u','e','n','c','y'};

	char frequencyExampleMessage[66] = {'E','n','t','e','r',' ','t','o','t','a','l',' ','5',' ','d','i','g','i','t','s',',',' ','w','i','t',
										'h',' ','2',' ','d','e','c','i','m','a','l',' ','p','l','a','c','e','s',' ','a','c','c','u','r','a','c','y',
										'\n','L','i','k','e',' ','5','5','5','.','7','7','\n'};
	char frequencyVarMessage[2][12] = {
		{'\n','E','n','t','e','r',' ','f','0', ' ', ':', ' '},
		{'\n','E','n','t','e','r',' ','f','1', ' ', ':', ' '},
		};

	char toConfirmMessage[42] = {'\n','A','r','e',' ','y','o','u',' ','s','u','r','e',' ','t','h','e',' ',
								'v','a','l','u','e','s',' ','a','r','e',' ','c','o','r','r','e','c','t',
								'?','(','y','/','n',')'};
	
	char confirmation[3] = {'O','K','\n'};
	
	char tryAgain[10] = {'T','r','y',' ','a','g','a','i','n','\n'};
	char nextLine[1] = {'\n'};
	/*---------------------------------------------------------------------------------------------------------------------*/

	float32_t frequency1 = 440;
	float32_t frequency2 = 440;

	//take input for frequency from UART
	generateSineWave(frequency1, frequency2);

	//take input for mixerArray from UART
	


	/*------------------------------------------UI to input signal frequency --------------------------------*/
		// Flag to define if frequency input is correct
		enum inputstate freqFlag;
		freqFlag = NOT_OK;

		int i;
		int j;
		while(freqFlag == NOT_OK){
			/*-----------Transmits generic explanation and example for frequency to user--------------*/
			HAL_UART_Transmit(&huart1, (uint8_t *)frequencyValues, 33, 30000);
			HAL_UART_Transmit(&huart1, (uint8_t *)frequencyExampleMessage, 65, 30000);
			/*-----------------------------------------------------------------------------------------*/

			/*-------------------------Receivies and transmits frequency values-------------------------*/
			for(i=0; i<4; i++){
				HAL_UART_Transmit(&huart1, (uint8_t *)frequencyVarMessage[i], 13, 30000);
				for(j=0; j<6;j++){
					if (inputFreq[i][j] == '.'){}
					else {while(HAL_UART_Receive (&huart1, (uint8_t *)&inputFreq[i][j], 1, 30000) != HAL_OK){}}
					HAL_UART_Transmit(&huart1, (uint8_t *)&inputFreq[i][j], 1, 30000);
				}
			}
			/*---------------------------------------------------------------------------------------*/

			/*----------------Confirms with the user if the input is correct------------------------*/
			HAL_UART_Transmit(&huart1, (uint8_t *)toConfirmMessage, 42, 30000);
			while(HAL_UART_Receive(&huart1, (uint8_t *)comfirmInput, 1, 30000) != HAL_OK){}
			HAL_UART_Transmit(&huart1, (uint8_t *)comfirmInput, 1, 30000);
			while(HAL_UART_Receive(&huart1, (uint8_t *)&comfirmInput[1], 1, 30000) != HAL_OK){}
			HAL_UART_Transmit(&huart1, (uint8_t *)&comfirmInput[1], 1, 30000);
			HAL_UART_Transmit(&huart1, (uint8_t *)nextLine, 1, 30000);
			if (comfirmInput[1] == 'n'){
				HAL_UART_Transmit(&huart1, (uint8_t *)tryAgain, 10, 30000);
			}
			else if (comfirmInput[0] == 'y'){
					freqFlag = OK;
					HAL_UART_Transmit(&huart1, (uint8_t *)confirmation, 3, 30000);
			}
			else {
				HAL_UART_Transmit(&huart1, (uint8_t *)tryAgain, 10, 30000);
			}
			/*--------------------------------------------------------------------------------------*/
		}


		/*--------------------------------UI to input matrix coefficients------------------------------*/

		enum inputState mixerFlag;
		mixerFlag = NOT_OK;
		while(mixerFlag == NOT_OK){
			/*-----------Transmits generic explanation and example for matrix coefficients to user--------------*/
			HAL_UART_Transmit(&huart1, (uint8_t *)linearCoefficients, 33, 30000);
			HAL_UART_Transmit(&huart1, (uint8_t *)mixerExampleMessage, 65, 30000);
			/*--------------------------------------------------------------------------------------------------*/

			/*-----------------Receivies and transmits matrix coefficient values---------------------------*/
			for(i=0; i<4; i++){
				HAL_UART_Transmit(&huart1, (uint8_t *)mixerVarMessage[i], 13, 30000);
				for(j=0; j<5;j++){
					if (inputCoeff[i][j] == '.'){}
					else {while(HAL_UART_Receive (&huart1, (uint8_t *)&inputCoeff[i][j], 1, 30000) != HAL_OK){}}
					HAL_UART_Transmit(&huart1, (uint8_t *)&inputCoeff[i][j], 1, 30000);
				}
			}
			/*---------------------------------------------------------------------------------------------*/

			/*----------------Confirms with the user if the input is correct------------------------*/
			HAL_UART_Transmit(&huart1, (uint8_t *)toConfirmMessage, 42, 30000);
			while(HAL_UART_Receive(&huart1, (uint8_t *)comfirmInput, 1, 30000) != HAL_OK){}
			HAL_UART_Transmit(&huart1, (uint8_t *)comfirmInput, 1, 30000);
			while(HAL_UART_Receive(&huart1, (uint8_t *)&comfirmInput[1], 1, 30000) != HAL_OK){}
			HAL_UART_Transmit(&huart1, (uint8_t *)&comfirmInput[1], 1, 30000);
			HAL_UART_Transmit(&huart1, (uint8_t *)nextLine, 1, 30000);
			if (comfirmInput[1] == 'n'){
				HAL_UART_Transmit(&huart1, (uint8_t *)tryAgain, 10, 30000);
			}
			else if (comfirmInput[0] == 'y'){
					mixerFlag = OK;
					HAL_UART_Transmit(&huart1, (uint8_t *)confirmation, 3, 30000);
			}
			else {
				HAL_UART_Transmit(&huart1, (uint8_t *)tryAgain, 10, 30000);
			}
			/*--------------------------------------------------------------------------------------*/
		}


	/*---------------------------------------------------------------------------------------------------------------------*/
	signalMixer();
}

#include "lab5.h"

int myTicks = 0;
int value = 0;

void exercise_1(void){
	
	setup_delay();
	setup_gpio();
	setup_timer();
	//setup_adc();
	
	while(1){
	
		TIM2->CCR2 = 100;
		delay_us(1000000);
		TIM2->CCR2 = 1000;
		delay_us(1000000);
	
	}
  
}
void setup_gpio(void){
	
	//habilitamos el clock para el GPIOA y GPIOC
	RCC->AHBENR |= (1U << 17);	//GPIOA
	RCC->AHBENR |= (1U << 19);	//GPIOC
	
	//PA1 como funcion alternativa
	GPIOA->MODER |=  (1U << 3);			
	GPIOA->MODER &= ~(1U << 2);
	
	
	//Seleccionamos la AF1, es la funcion para utilizar TIM2/CH2
	GPIOA->AFR[0] &= ~(1U << 7);			
	GPIOA->AFR[0] &= ~(1U << 6);
	GPIOA->AFR[0] &= ~(1U << 5);
	GPIOA->AFR[0] |=  (1U << 4);
	
	//PC1 como analog input
	GPIOC->MODER |= (1U << 3);			
	GPIOC->MODER |= (1U << 2);
	
	
}
	
void setup_timer(void){
		
	//Habilitamos el clock para TIM2
	RCC->APB1ENR |= (1U << 0);
	
	//Seleccionamos el PWM mode 1, en modo "activo" mientras no llegue al valor de CCR2 (OC2M)
	TIM2->CCMR1 &= ~(1U << 24);
	TIM2->CCMR1 |= (1U << 14);
	TIM2->CCMR1 |= (1U << 13);
	TIM2->CCMR1 &= ~(1U << 12);	
	
	//Habilitamos el Preload Register (OC2PE)
	TIM2->CCMR1 |= (1U << 11);
	
	//Habilitamos el buffer para el ARR  (ARPE)
	TIM2->CR1 |= (1U << 7);
	
	//Declaramos que "activo" sea High mode  (CC2P)
	TIM2->CCER &= ~(1U << 5);
	
	//Habilitamos el Pwm como salida
	TIM2->CCER |= (1U << 4);
	
	//PWM 1Khz, 50% duty cicle 
	TIM2->PSC = 72;			//1Mhz de frecuencia
	TIM2->ARR = 1000;		//1Khz de frecuencia PWM
	TIM2->CCR2 = 0;
	
	//Se setea el contador a 0
	TIM2->CNT = 0;

	//Habilitamos el (UG) para el conteo
	TIM2->EGR |= (1U << 0);
	
	//Comienza el conteo (CEN)
	TIM2->CR1 |= (1U << 0);
}

void setup_adc(void){

	//Habilitamos el clock para ADC12
	RCC->AHBENR |= (1U << 28);	
	
	//Habilitamos el regulador de voltaje interno (ADVREGEN)
	ADC1->CR &= ~(1U<<29);		//ADVREGEN de 10 (deshabilitado) a 00 (intermediate state)
	ADC1->CR &= ~(1U<<28);
	
	ADC1->CR &= ~(1U<<29);		//ADVREGEN de 00 (intermediate state) a 01 (habilitado)
	ADC1->CR |= (1U<<28);
	
	delay_us(100);
	
	//Habilitamos las interrupciones del bit EOC (EOCEN)
	ADC1->IER |= (1U<<2);
	NVIC_EnableIRQ(ADC1_2_IRQn);
	
	//Configuramos la secuencia de conversion
	ADC1->SQR1 &= ~(1U<<10);					// (296-1124) Configuramos la secuencia de conversión del canal ADC
	ADC1->SQR1 &= ~(1U<<9);				
	ADC1->SQR1 |= (1U<<8);
	ADC1->SQR1 |= (1U<<7);
	ADC1->SQR1 |= (1U<<6);
	
	//Habilitamos el modo continuo (CONT)
	ADC1->CFGR |= (1U << 13);
	
	//Habilitamos el ADC (ADEN) 
	ADC1->CR |= (1U << 0);
	while(!(ADC1->ISR & (1U << 0)));		//Esperamos el ADRDY flag bit para comenzar las conversiones
	ADC1->ISR |= (1U << 0);
	
	//Comenzamos la conversion (ADSTART)
	ADC1->CR |= (1U << 2);
	
}

void ADC1_2_IRQHandler(void){

	if(ADC1->ISR & (1U << 2)){
		value = ADC1->DR;
	}
		
}




//Configuracion del delay con TIM3
void setup_delay(void){
	
	RCC->APB1ENR |= (1U << 1);		//Habilitamos el clock para TIM3

	TIM3->PSC = 0;								
	TIM3->ARR = 72;								//frecuencia del timer 1Mhz
	TIM3->CR1 |= (1U << 2);	
	TIM3->DIER |= (1U << 0);
	TIM3->EGR |= (1U << 0);
	
	NVIC_EnableIRQ(TIM3_IRQn);

}

void TIM3_IRQHandler(void){

	myTicks++;
	TIM3->SR &= ~(1U<<0);

}

void delay_us(int us){

	TIM3->CR1 |= (1U << 0);
	myTicks = 0;
	while(myTicks < us);
	
	TIM3->CR1 &= ~(1U << 0);
}

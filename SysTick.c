#include "stm32l476xx.h"
#include "stdlib.h"
#include "stdio.h"
#include "LCD.h"
#include "LED.h"

// SysTick functions
volatile uint32_t TimeDelay;
void SysTick_Initialize(uint32_t ticks);
void SysTick_Handler(void);
void Delay(uint32_t nTime);

// LCD functions
extern void LCD_DispGame(uint32_t num, char* str);
extern void LCD_Initialization(void);
extern void LCD_DisplayString(uint8_t* ptr);

// LED functions
extern void LED_Init();
extern void Red_LED_On();
extern void Red_LED_Off();
extern void Green_LED_On();
extern void Green_LED_Off();

// Helper functions
void PortA_Init(void);
char generate_char(void);
void game_over(uint32_t num);
void blink_led(int num);

int main(void) {
	
	// Declaring variables
	uint32_t score = 3;
	char scroll[4] = "    ";
	int correct = 0;
	uint32_t delaycnt = 1000;
	uint32_t max_score = 0;
	
	// Seeding random number generator
	srand((unsigned)score);
	
	// Initialize LED
	LED_Init();
	
	//Initialize LCD
	LCD_Initialization();
	
	// Initialize SysTick
	SysTick_Initialize((uint32_t)4000);
	
	// Initialize LCD to game
	LCD_DispGame(score, scroll);
	
	// Initialize Port A
	PortA_Init();
	
	
	while(1) {
		
		correct = 0;
		Delay(delaycnt);
		if((GPIOA->IDR & GPIO_IDR_IDR_1) && scroll[0] == 'L') {
				//while(GPIOA->IDR&GPIO_IDR_IDR_0);
				score = score + 1;
				correct = 1;
			  delaycnt -= 30;
		} else if((GPIOA->IDR & GPIO_IDR_IDR_2) && scroll[0] == 'R') {
				//while(GPIOA->IDR&GPIO_IDR_IDR_0);
				score = score + 1;
			  correct = 1;
			  delaycnt -= 30;
		} else if((GPIOA->IDR & GPIO_IDR_IDR_3) && scroll[0] == 'U') {
				//while(GPIOA->IDR&GPIO_IDR_IDR_0);
				score = score + 1;
			  correct = 1;
			  delaycnt -= 30;
		} else if((GPIOA->IDR & GPIO_IDR_IDR_5) && scroll[0] == 'D') {
				//while(GPIOA->IDR&GPIO_IDR_IDR_0);
				score = score + 1;
			  correct = 1;
			  delaycnt -= 30;
		} else if (scroll[0] == ' ') {
			  correct = 1;
		}
		
		blink_led(correct);
		
		if(delaycnt < 250) {
			delaycnt = 250;
		}
		
		if(score > max_score) {
			max_score = score;
		}
		score = correct ? score : score - 1;
		if(score == 0){
			game_over(max_score);
		}
		
		char last_letter = generate_char();
		for (int i = 0; i < 3; i++) {
			scroll[i] = scroll[i + 1];
		}
		scroll[3] = last_letter;
		LCD_DispGame(score, scroll);
	}  
}

// ticks: number of ticks between two interrupts
void SysTick_Initialize(uint32_t ticks) {
	
	// Disable SysTick IRQ and SysTick counter
	SysTick->CTRL = 0;
	
	// Set reload register
	SysTick->LOAD = ticks - 1;
	
	// Set interrupt priority of SysTick
	// Make SysTick least urgent (i.e. highest priority number)
	// __NVIC_PRIO_BITS: number of bits for priority levels, defined in CMSIS
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	
	// Reset the SysTick counter value
	SysTick->VAL = 0;
	
	// Select processor clock
	// 1 = processor clock; 0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	
	// Enable SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does no assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Enable SysTick timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

// SysTick interrupt service routine
void SysTick_Handler(void) {
	if (TimeDelay > 0)
		TimeDelay--;
}

// nTime: specifies the delay time length
void Delay(uint32_t nTime) {
	TimeDelay = nTime;
	while(TimeDelay != 0);
}

// Initialize Port A
void PortA_Init(void) {
	
	// Enable clock for Port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	// Set mode to input (00)
	GPIOA->MODER &= ~(GPIO_MODER_MODER1);
	GPIOA->MODER &= ~(GPIO_MODER_MODER2);
	GPIOA->MODER &= ~(GPIO_MODER_MODER3);
	GPIOA->MODER &= ~(GPIO_MODER_MODER5);
	
	// Pull down joystick left, right, up, down inputs
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR1_1);
	GPIOA->PUPDR |= 1 << 3;
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR2_1);
	GPIOA->PUPDR |= 1 << 5;
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR3_1);
	GPIOA->PUPDR |= 1 << 7;
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR5_1);
	GPIOA->PUPDR |= 1 << 11;
}

// Function to generate the last letter for the game
char generate_char(void) {
	int rand_num = rand() % (4 + 1 - 0) + 0;
	
	switch (rand_num) {
		case 0: return 'U';
		case 1: return 'D';
		case 2: return 'L';
		case 3: return 'R';
		default: return 'D';
	}
}

// Function to display a game over scroll on LCD
void game_over(uint32_t num) {
	
	int i = 0;
	char str[16] = " GAME OVER      ";
	
	while(1) {
		for (i = 0; i < 12; i++) {
			Delay(500);
			LCD_DispGame(num, &(str[i]));
		}
	}
}

// Function to blink change between red and green LED depending on
// whether the user input is correct or not
void blink_led(int num) {
	
	if (num == 1) {
		Red_LED_Off();
		Green_LED_On();
	}
	else {
		Green_LED_Off();
		Red_LED_On();
	}
}

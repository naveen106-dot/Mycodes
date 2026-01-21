#include "stm32f4xx.h"

void PWM_Init(void)
{
    // 1. Enable GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // PA6 -> Alternate Function
    GPIOA->MODER &= ~(3 << (6 * 2));     // Clear bits
    GPIOA->MODER |=  (2 << (6 * 2));     // AF mode

    // Select AF2 for PA6
    GPIOA->AFR[0] &= ~(0xF << (6 * 4));
    GPIOA->AFR[0] |=  (2 << (6 * 4));     // AF2 = TIM3_CH1

    // 2. Enable TIM3 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // 3. Configure TIM3 for PWM

    TIM3->PSC = 99;    // Prescaler → Timer clock = 84MHz/(PSC+1) = 840kHz
    TIM3->ARR = 839;   // ARR → PWM Frequency = 840kHz / 840 = 1kHz

    TIM3->CCR1 = 420;  // Duty ~ 50%

    // PWM Mode 1 on CH1: OC1M = 110, OC1PE=1
    TIM3->CCMR1 &= ~(7 << 4);
    TIM3->CCMR1 |=  (6 << 4);  // PWM Mode 1
    TIM3->CCMR1 |=  (1 << 3);  // Preload enable

    // Enable CH1 output
    TIM3->CCER |= TIM_CCER_CC1E;

    // Auto-reload preload enable
    TIM3->CR1 |= TIM_CR1_ARPE;

    // 4. Start timer
    TIM3->CR1 |= TIM_CR1_CEN;
}

void changeDuty(uint16_t duty)
{
    TIM3->CCR1 = duty;   // just update CCR1
}

int main(void)
{
    PWM_Init();

    while (1)
    {
        changeDuty(0);     // 0%
        for (int i=0; i<200000; i++);

        changeDuty(200);   // 25%
        for (int i=0; i<200000; i++);

        changeDuty(420);   // 50%
        for (int i=0; i<200000; i++);

        changeDuty(600);   // 75%
        for (int i=0; i<200000; i++);

        changeDuty(839);   // 100%
        for (int i=0; i<200000; i++);
    }
}

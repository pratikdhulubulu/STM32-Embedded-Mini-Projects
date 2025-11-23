#include "stm32f446xx.h"

#define BUFFER_SIZE 100
char uart_buffer[BUFFER_SIZE];

void UART_SendChar(char c){
    while(!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

void UART_SendString(char *str){
    while(*str) UART_SendChar(*str++);
}

char UART_ReceiveChar(void){
    while(!(USART2->SR & USART_SR_RXNE));
    return USART2->DR;
}

void UART_Echo(void){
    char c = UART_ReceiveChar();  // Echo single character
    UART_SendChar(c);
}

// Receive a full string until Enter (\r or \n)
int UART_ReceiveString(char *buffer, int max_len){
    int i = 0;
    char c;
    while(1){
        while(!(USART2->SR & USART_SR_RXNE));
        c = USART2->DR;
        if(c=='\r' || c=='\n'){      // Enter pressed
            buffer[i] = '\0';         // Null terminate string
            return i;                 // Return length
        } else if(i < max_len-1){    // Store char
            buffer[i++] = c;
        }
    }
}

int main(void){
    // Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Configure PA2(TX) & PA3(RX) as AF7
    GPIOA->MODER &= ~((3<<4)|(3<<6));
    GPIOA->MODER |= (2<<4)|(2<<6);

    GPIOA->AFR[0] &= ~((0xF<<8)|(0xF<<12));
    GPIOA->AFR[0] |= (7<<8)|(7<<12);

    // USART2: 115200 baud @ 16 MHz
    USART2->BRR = 0x8B;
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE;
    USART2->CR1 |= USART_CR1_UE;

    while(1){
        char c = UART_ReceiveChar();
        UART_SendChar(c);  // Echo immediately
    }
}

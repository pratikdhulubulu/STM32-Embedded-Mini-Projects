/**
 * @file    main.c
 * @author  Pratik Dhulubulu
 * @brief   UART2 echo program on STM32F446RE.
 * @details
 *  - PA2 = USART2_TX (AF7)
 *  - PA3 = USART2_RX (AF7)
 *  - Baud = 115200 @ APB1 = 25 MHz  → BRR = 0xD9
 *  - Blocking transmit and receive
 */

#include "stm32f446xx.h"

#define BUFFER_SIZE 100

/** @brief Buffer for receiving strings from UART. */
char uart_buffer[BUFFER_SIZE];

/**
 * @brief  Send a single character over UART2 (blocking).
 * @param  c  Character to send.
 * @retval None
 */
void UART_SendChar(char c)
{
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

/**
 * @brief  Send a null-terminated string over UART2.
 * @param  str  Pointer to string.
 * @retval None
 */
void UART_SendString(char *str)
{
    while (*str)
        UART_SendChar(*str++);
}

/**
 * @brief  Receive one character from UART2 (blocking).
 * @retval Received character
 */
char UART_ReceiveChar(void)
{
    while (!(USART2->SR & USART_SR_RXNE));
    return USART2->DR;
}

/**
 * @brief  Echo a single received character back over UART2.
 * @details Helper function for testing UART.
 * @retval None
 */
void UART_Echo(void)
{
    char c = UART_ReceiveChar();
    UART_SendChar(c);
}

/**
 * @brief  Receive a full string until Enter key is pressed.
 * @param  buffer    Pointer to buffer to store received characters.
 * @param  max_len   Maximum buffer length.
 * @retval Number of characters received (length of string).
 */
int UART_ReceiveString(char *buffer, int max_len)
{
    int i = 0;
    char c;

    while (1)
    {
        while (!(USART2->SR & USART_SR_RXNE));
        c = USART2->DR;

        if (c == '\r' || c == '\n')
        {
            buffer[i] = '\0';
            return i; 
        }
        else if (i < max_len - 1)
        {
            buffer[i++] = c;
        }
    }
}

/**
 * @brief  Main entry point.
 * @details
 *  - Enables GPIOA and USART2 clocks  
 *  - Configures PA2/PA3 as AF7  
 *  - Sets USART2 to 115200 baud @ 25 MHz  
 *  - Continuously echoes received characters  
 *
 * @retval int (never returns)
 */
int main(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA->MODER &= ~((3 << 4) | (3 << 6));
    GPIOA->MODER |=  (2 << 4) | (2 << 6);

    GPIOA->AFR[0] &= ~((0xF << 8) | (0xF << 12));
    GPIOA->AFR[0] |=  (7 << 8) | (7 << 12);

    USART2->BRR = 0xD9;
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE;
    USART2->CR1 |= USART_CR1_UE;

    while (1)
    {
        char c = UART_ReceiveChar();
        UART_SendChar(c);
    }
}

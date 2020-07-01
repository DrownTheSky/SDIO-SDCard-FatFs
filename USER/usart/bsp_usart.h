#ifndef   __BSP_USART_H
#define   __BSP_USART_H

#include "stm32f4xx.h"
#include <stdio.h>


#define  USART_GPIO_CLK         RCC_AHB1Periph_GPIOA     
#define  USART_GPIO_RX_PORT     GPIOA
#define  USART_GPIO_RX_PIN      GPIO_Pin_10
#define  USART_GPIO_RX_SOURCE   GPIO_PinSource10
#define  USART_GPIO_RX_AF       GPIO_AF_USART1
#define  USART_GPIO_TX_PORT     GPIOA
#define  USART_GPIO_TX_PIN      GPIO_Pin_9
#define  USART_GPIO_TX_SOURCE   GPIO_PinSource9
#define  USART_GPIO_TX_AF       GPIO_AF_USART1
#define  USART                  USART1
#define  USART_CLK              RCC_APB2Periph_USART1
#define  USART_BAUDRATE         115200
#define  USART_IRQN             USART1_IRQn
#define  USART_IRQHANDLER       USART1_IRQHandler
#define  USART_DMA_CLK          RCC_AHB1Periph_DMA2
#define  USART_DMA_CHANNEL      DMA_Channel_4
#define  USART_DMA_BUFFSIZE     50
#define  USART_DMA_DR           (USART1_BASE + 0x04)
#define  USART_DMA_STREAM       DMA2_Stream7


void USART_Config(void);
void USART_SendByte(char ch);
void USART_SendString(char* str);

#endif   /* __BSP_USART_H */


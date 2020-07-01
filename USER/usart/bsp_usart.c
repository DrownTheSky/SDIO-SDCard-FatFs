#include "./usart/bsp_usart.h"

//uint8_t DMA_SendBuff[USART_DMA_BUFFSIZE] = {0};

void USART_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
//    DMA_InitTypeDef DMA_InitStruct;

    RCC_AHB1PeriphClockCmd(USART_GPIO_CLK,ENABLE);
    RCC_APB2PeriphClockCmd(USART_CLK,ENABLE);
    RCC_AHB1PeriphClockCmd(USART_DMA_CLK,ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStruct.NVIC_IRQChannel = USART_IRQN;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;

    GPIO_InitStruct.GPIO_Pin = USART_GPIO_RX_PIN;
    GPIO_PinAFConfig(USART_GPIO_RX_PORT,USART_GPIO_RX_SOURCE,USART_GPIO_RX_AF);
    GPIO_Init(USART_GPIO_RX_PORT,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = USART_GPIO_TX_PIN;
    GPIO_PinAFConfig(USART_GPIO_TX_PORT,USART_GPIO_TX_SOURCE,USART_GPIO_TX_AF);
    GPIO_Init(USART_GPIO_TX_PORT,&GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate = USART_BAUDRATE;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART,&USART_InitStruct);

//    DMA_DeInit(USART_DMA_STREAM);
//    while (DMA_GetCmdStatus(USART_DMA_STREAM) != DISABLE);
//    DMA_InitStruct.DMA_BufferSize = USART_DMA_BUFFSIZE;
//    DMA_InitStruct.DMA_Channel = USART_DMA_CHANNEL;
//    DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
//    DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//    DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)DMA_SendBuff;
//    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
//    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
//    DMA_InitStruct.DMA_PeripheralBaseAddr = USART_DMA_DR;
//    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
//    DMA_Init(USART_DMA_STREAM,&DMA_InitStruct);

    USART_ITConfig(USART,USART_IT_RXNE,ENABLE);
    USART_ClearFlag(USART,USART_IT_RXNE);

    USART_Cmd(USART,ENABLE);
//    DMA_Cmd(USART_DMA_STREAM,ENABLE);

//    while (DMA_GetCmdStatus(USART_DMA_STREAM) != ENABLE);
}

void USART_SendByte(char ch)
{
    USART_SendData(USART,ch);
    while (USART_GetFlagStatus(USART,USART_FLAG_TXE) == RESET);   
}

void USART_SendString(char* str)
{
    uint16_t i = 0;
    do
    {
        USART_SendData(USART,*(str + i));
		while (USART_GetFlagStatus(USART,USART_FLAG_TXE) == RESET);
        i++;

    } while (*(str + i) != '\0');

    while (USART_GetFlagStatus(USART,USART_FLAG_TXE) == RESET);
    
}

int fputc(int ch, FILE *f)
{
    USART_SendData(USART,(uint8_t)ch);
    while (USART_GetFlagStatus(USART,USART_FLAG_TXE) == RESET);
    return ch;
}



//char USART_RX_Temp = 0;
//void USART_IRQHANDLER(void)
//{
//  if (USART_GetFlagStatus(USART,USART_FLAG_RXNE) != RESET)
//  {
//    USART_ClearFlag(USART,USART_IT_RXNE);

//    USART_RX_Temp = USART_ReceiveData(USART);
//    USART_SendString(&USART_RX_Temp);
//  }
//  
//}


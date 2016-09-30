// /**
//  * @file 
//  */
// #include "LSM6DS3.h"

// LSM6DS3::LSM6DS3() :
// 	Spi(SPI3, GPIOA, GPIO_Pin_15)
// {
// 	gyro_send_buf[0] = static_cast<uint8_t>(GyroCommands::OUTZ_H_G) | 0x80;
// 	gyro_send_buf[2] = static_cast<uint8_t>(GyroCommands::OUTZ_L_G) | 0x80;
// 	initialize();
// }

// void LSM6DS3::initialize(){
// 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
// 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
// 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
// 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
// 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	GPIO_StructInit(&GPIO_InitStructure);
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);
// 	GPIO_SetBits(GPIOA, GPIO_Pin_15);

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(GPIOD, &GPIO_InitStructure);
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);

// 	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
// 	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
// 	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

// 	SPI_InitTypeDef SPI_InitStructure;
// 	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
// 	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
// 	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
// 	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
// 	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
// 	SPI_InitStructure.SPI_NSS = SPI_NSSInternalSoft_Set | SPI_NSS_Soft;
// 	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
// 	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
// 	SPI_InitStructure.SPI_CRCPolynomial = 7;
// 	SPI_Init(SPI3, &SPI_InitStructure);
// 	SPI_Cmd(SPI3, ENABLE);

// 	DMA_InitTypeDef DMA_InitStructure;
// 	DMA_DeInit(DMA1_Stream0);
// 	DMA_DeInit(DMA1_Stream5);
// 	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
// 	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
// 	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
// 	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
// 	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
// 	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
// 	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
// 	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
// 	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

// 	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI3->DR));
// 	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)gyro_send_buf;
// 	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
// 	DMA_InitStructure.DMA_BufferSize = 4;
// 	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
// 	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
// 	DMA_Init(DMA1_Stream5, &DMA_InitStructure); //TX

// 	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI3->DR));
// 	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)gyro_single_buf;
// 	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
// 	DMA_InitStructure.DMA_BufferSize = 4;
// 	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
// 	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
// 	DMA_Init(DMA1_Stream0, &DMA_InitStructure); //RX

// 	NVIC_InitTypeDef NVIC_InitStructure;
// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
// 	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
// }

// void LSM6DS3::DMAStart(){
// 	setChipSelect();
// 	DMA_Cmd(DMA1_Stream5, ENABLE);
// 	DMA_Cmd(DMA1_Stream0, ENABLE);
// 	SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Rx, ENABLE);
// 	SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Tx, ENABLE);
// }

// void LSM6DS3::DMAStop(){
	
// }

// int16_t LSM6DS3::getAccel(){
	
// }

// int16_t LSM6DS3::getGyro(){
// 	return gyro_single_buf;
// }


// LSM6DS3* LSM6DS3::getInstance(){
// 	static LSM6DS3 instance;
// 	return &instance;
// }

/* /\* */
/*  *  */
/*  *\/ */

/* #pragma once */

/* #include "stm32f4xx.h" */
/* #include "Spi.h" */
/* #include "RingBufferHistory.h" */

/* const static uint16_t GYRO_BUFFER_SIZE = 10; */
/* const static uint16_t ACCEL_BUFFER_SIZE = 50; */

/* enum class GyroCommands : uint8_t { */
/* 	FUNC_CFG_ACCESS = 0x01, */
/* 	SENSOR_SYNC_TIME_FRAME = 0x04, */
/* 	FIFO_CTRL1, */
/* 	FIFO_CTRL2, */
/* 	FIFO_CTRL3, */
/* 	FIFO_CTRL4, */
/* 	FIFO_CTRL5, */
/* 	ORIENT_CFG_G, */
/* 	INT1_CTRL = 0x0D, */
/* 	INT2_CTRL, */
/* 	WHO_AM_I, */
/* 	CTRL1_XL, */
/* 	CTRL2_G, */
/* 	CTRL3_C, */
/* 	CTRL4_C, */
/* 	CTRL5_C, */
/* 	CTRL6_C, */
/* 	CTRL7_G, */
/* 	CTRL8_XL, */
/* 	CTRL9_XL, */
/* 	CTRL10_C, */
/* 	MASTER_CONFIG, */
/* 	WAKE_UP_SRC, */
/* 	TAP_SRC, */
/* 	D6D_SRC, */
/* 	STATUS_REG, */
/* 	OUT_TEMP_L = 0x20, */
/* 	OUT_TEMP, */
/* 	OUTX_L_G, */
/* 	OUTX_H_G, */
/* 	OUTY_L_G, */
/* 	OUTY_H_G, */
/* 	OUTZ_L_G, */
/* 	OUTZ_H_G, */
/* 	OUTX_L_XL, */
/* 	OUTX_H_XL, */
/* 	OUTY_L_XL, */
/* 	OUTY_H_XL, */
/* 	OUTZ_L_XL, */
/* 	OUTZ_H_XL, */
/* 	SENSORHUB1_REG, */
/* 	SENSORHUB2_REG, */
/* 	SENSORHUB3_REG, */
/* 	SENSORHUB4_REG, */
/* 	SENSORHUB5_REG, */
/* 	SENSORHUB6_REG, */
/* 	SENSORHUB7_REG, */
/* 	SENSORHUB8_REG, */
/* 	SENSORHUB9_REG, */
/* 	SENSORHUB10_REG, */
/* 	SENSORHUB11_REG, */
/* 	SENSORHUB12_REG, */
/* 	FIFO_STATUS1, */
/* 	FIFO_STATUS2, */
/* 	FIFO_STATUS3, */
/* 	FIFO_STATUS4, */
/* 	FIFO_DATA_OUT_L, */
/* 	FIFO_DATA_OUT_H, */
/* 	TIMESTAMP0_REG, */
/* 	TIMESTAMP1_REG, */
/* 	TIMESTAMP2_REG, */
/* 	STEP_TIMESTAMP_L, */
/* 	STEP_TIMESTAMP_H, */
/* 	STEP_COUNTER_L, */
/* 	STEP_COUNTER_H, */
/* 	SENSORHUB13_REG, */
/* 	SENSORHUB14_REG, */
/* 	SENSORHUB15_REG, */
/* 	SENSORHUB16_REG, */
/* 	SENSORHUB17_REG, */
/* 	SENSORHUB18_REG, */
/* 	FUNC_SRC, */
/* 	TAP_CFG = 0x58, */
/* 	TAP_THS_6D, */
/* 	INT_DUR2, */
/* 	WAKE_UP_THS, */
/* 	WAKE_UP_DUR, */
/* 	FREE_FALL, */
/* 	MD1_CFG, */
/* 	MD2_CFG, */
/* 	OUT_MAG_RAW_X_L = 0x66, */
/* 	OUT_MAG_RAW_X_H, */
/* 	OUT_MAG_RAW_Y_L, */
/* 	OUT_MAG_RAW_Y_H, */
/* 	OUT_MAG_RAW_Z_L, */
/* 	OUT_MAG_RAW_Z_H, */
/* }; */

/* class LSM6DS3 : protected Spi{ */
/* private: */

/* 	LSM6DS3(); */

/* 	RingBufferHistory<int16_t, GYRO_BUFFER_SIZE> gyro_buf; */
/* 	RingBufferHistory<int16_t, ACCEL_BUFFER_SIZE> accel_buf; */

/* public: */
/* 	uint8_t gyro_single_buf[4]; */
/* 	uint8_t accel_single_buf[4]; */

/* 	uint8_t gyro_send_buf[4]; */
/* 	uint8_t accel_send_buf[4]; */

/* 	void initialize(); */

/* 	void DMAStart(); */
/* 	void DMAStop(); */

/* 	int16_t getAccel(); */
/* 	int16_t getGyro(); */

/* 	static LSM6DS3 *getInstance(); */
/* }; */

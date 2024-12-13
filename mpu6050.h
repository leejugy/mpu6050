#ifndef __MPU6050__
#define __MPU6050__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MPU6050_SLAVE_ADDRESS 0x68
#define MPU6050_PWR_SLEEP_OFF_AND_RESET_ALL 0
#define GET_TEMPERATURE_FROM_TEMP_REG_OUT(reg_val) ((float)reg_val)/((float)340) + 36.53

typedef enum
{
    MPU6050_REG_ACCEL_CONFIG = 28,
    MPU6050_REG_ACCEL_XOUT_H = 59,
    MPU6050_REG_ACCEL_XOUT_L = 60,
    MPU6050_REG_ACCEL_YOUT_H = 61,
    MPU6050_REG_ACCEL_YOUT_L = 62,
    MPU6050_REG_ACCEL_ZOUT_H = 63,
    MPU6050_REG_ACCEL_ZOUT_L = 64,
    MPU6050_REG_TEMP_OUT_H = 65,
    MPU6050_REG_TEMP_OUT_L = 66,
    MPU6050_REG_PWR_MGMT_1 = 107,
    MPU6050_REG_WHO_AM_I = 117,
}MPU6050_REG;

typedef enum
{
    ACC_X,
    ACC_Y,
    ACC_Z,
    MAX_ACC_SIZE,
}ACCEL_CORDINATE;

void init_mpu6050();
void mpu6050_read_accel();
void start_mpu6050_thread();
void mpu6050_read_temp();

#endif
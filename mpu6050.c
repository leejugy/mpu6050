#include "mpu6050.h"
#include "i2c_ctl.h"
#include "define.h"

i2c_clt_t mpu6050_i2c;

static int mpu6050_single_read_reg(MPU6050_REG reg_addr, uint8_t *buffer, size_t buffer_size)
{
    if (buffer_size != 1)
    {
        err_p("invalid buffer size");
        return -1;
    }
    return i2c_reg_8bit_read(&mpu6050_i2c, reg_addr, buffer, buffer_size);
}

static int mpu6050_burst_read_reg(MPU6050_REG reg_addr, uint8_t *buffer, size_t buffer_size)
{
    if (buffer_size != 2)
    {
        err_p("invalid buffer size");
        return -1;
    }
    return i2c_reg_8bit_read(&mpu6050_i2c, reg_addr, buffer, buffer_size);
}

static int mpu6050_single_write_reg(MPU6050_REG reg_addr, uint8_t *buffer, size_t buffer_size)
{
    if (buffer_size != 1)
    {
        err_p("invalid buffer size");
        return -1;
    }
    return i2c_reg_8bit_write(&mpu6050_i2c, reg_addr, buffer, buffer_size);
}

static uint16_t u8_to_u16(uint8_t *u8)
{
    uint16_t u16 = 0;
    u16 = (u8[0] << 8) + u8[1];
    return u16;
}

static uint16_t mpu6050_get_2bytes(MPU6050_REG reg_addr)
{
    int ret = 0;
    uint8_t double_buf[2] = {
        0,
    };

    ret = mpu6050_burst_read_reg(reg_addr, double_buf, sizeof(double_buf));
    if (ret < 0)
    {
        err_p("read reg data error on read double");
        return 0;
    }

    return u8_to_u16(double_buf);
}

static uint8_t mpu6050_get_1bytes(MPU6050_REG reg_addr)
{
    int ret = 0;
    uint8_t single_buf = 0;

    ret = mpu6050_single_read_reg(reg_addr, &single_buf, sizeof(single_buf));
    if (ret < 0)
    {
        err_p("read reg data error on read single");
        return 0;
    }
    
    return single_buf;
}

static void mpu6050_set_1byte(MPU6050_REG reg_addr, uint8_t set_buffer)
{
    int ret = 0;
    ret = mpu6050_single_write_reg(reg_addr, &set_buffer, sizeof(set_buffer));
    if (ret < 0)
    {
        err_p("fail to single write reg");
    }
}

static void mpu6050_sleep_mode_off()
{
    mpu6050_set_1byte(MPU6050_REG_PWR_MGMT_1, MPU6050_PWR_SLEEP_OFF_AND_RESET_ALL); // sleep mode off
}

void init_mpu6050()
{
    init_i2c();
    strcpy(mpu6050_i2c.file_route, I2C1_ROUTE);
    mpu6050_i2c.slave_address = MPU6050_SLAVE_ADDRESS;
    mpu6050_sleep_mode_off();
}

void mpu6050_read_accel()
{
    uint8_t afs_sel = 0;
    uint8_t i = 0;
    short int reg_data = 0;
    float accel_data[MAX_ACC_SIZE] = {
        0.0,
    };
    int resolution_table[] = {16384, 8192, 4096, 2048};

    afs_sel = mpu6050_get_1bytes(MPU6050_REG_ACCEL_CONFIG);
    afs_sel = (afs_sel >> 3) & 0x03;
    want_p("afs_sel val is : %d resolution[%d]", afs_sel, resolution_table[afs_sel]);

    for (i = 0; i < MAX_ACC_SIZE; i++) // read xyz acc
    {
        reg_data = mpu6050_get_2bytes(MPU6050_REG_ACCEL_XOUT_H + i * 2);
        accel_data[ACC_X + i] = ((float)reg_data) / ((float)resolution_table[afs_sel]);
    }

    want_p("x : %02.02fg, y : %02.02fg, z : %02.02fg", accel_data[0], accel_data[1], accel_data[2]);
}

void mpu6050_read_temp()
{
    short int reg_data = mpu6050_get_2bytes(MPU6050_REG_TEMP_OUT_H);
    float temperature = 0;
    temperature = GET_TEMPERATURE_FROM_TEMP_REG_OUT(reg_data);
    want_p("%02.02f", temperature);
}

void thread_mpu6050()
{
    while(1)
    {
        mpu6050_sleep_mode_off(); //if power reset(reason that use poor connect with voltage line), mpu6050 automatically in sleep mode. so, at every reading, we must set sleep bit 1.
        mpu6050_read_accel();
        mpu6050_read_temp();
        sleep(1);
    }
}

void start_mpu6050_thread()
{
    pthread_t tid = 0;
    int ret = 0;

    init_mpu6050();
    ret = pthread_create(&tid, NULL, (void *)&thread_mpu6050, NULL);
    if(ret < 0)
    {
        perror("create thread");
        return;
    }

    ret = pthread_detach(tid);
    if(ret < 0)
    {
        perror("detach thread");
    }
}
#ifndef __I2C__
#define __I2C__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <semaphore.h>

#define MAX_FILE_ROUTE_LEN 128
#define I2C0_ROUTE "/dev/i2c-0"
#define I2C1_ROUTE "/dev/i2c-1"
#define I2C2_ROUTE "/dev/i2c-2"

typedef struct
{
    uint8_t slave_address;
    char file_route[MAX_FILE_ROUTE_LEN];
    int fd;
}i2c_clt_t;

void init_i2c();
int i2c_start(i2c_clt_t *i2c);
int i2c_reg_8bit_read(i2c_clt_t *i2c, uint8_t reg_addr ,uint8_t *buffer, size_t buffer_size);
int i2c_reg_8bit_write(i2c_clt_t *i2c, uint8_t reg_addr ,uint8_t *buffer, size_t buffer_size);

#endif
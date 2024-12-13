#include "i2c_ctl.h"
sem_t i2c_sem;


static int i2c_reg_8bit_read_function(i2c_clt_t *i2c, uint8_t reg_addr ,uint8_t *buffer, size_t buffer_size)
{
    int ret = 0;

    if(i2c_start(i2c) < 0)
    {
        close(i2c->fd);
        return -1;
    }

    ret = write(i2c->fd, &reg_addr, sizeof(reg_addr));
    if(ret < 0)
    {
        perror("fail to write i2c reg addr");
        close(i2c->fd);
        return -1;
    }

    ret = read(i2c->fd, buffer, buffer_size);
    if(ret < 0)
    {
        perror("fail to read i2c reg addr");
        close(i2c->fd);
        return -1;
    }

    close(i2c->fd);
    return ret;
}

static int i2c_reg_8bit_write_function(i2c_clt_t *i2c, uint8_t reg_addr ,uint8_t *buffer, size_t buffer_size)
{
    int ret = 0;
    uint8_t *temp = (uint8_t *)malloc(buffer_size + 1);
    temp[0] = reg_addr;
    memcpy(temp + 1, buffer, buffer_size);

    if(i2c_start(i2c) < 0)
    {
        close(i2c->fd);
        free(temp);
        return -1;
    }

    ret = write(i2c->fd, temp, buffer_size + 1);
    if(ret < 0)
    {
        perror("fail to write i2c reg addr");
        close(i2c->fd);
        free(temp);
        return -1;
    }

    close(i2c->fd);
    free(temp);
    return ret;
}

void init_i2c()
{
    int ret = sem_init(&i2c_sem, 0, 1);
    if(ret < 0)
    {
        perror("sem init fail");
        return;
    }
}

int i2c_reg_8bit_read(i2c_clt_t *i2c, uint8_t reg_addr ,uint8_t *buffer, size_t buffer_size)
{
    int ret = 0;

    sem_wait(&i2c_sem);
    ret = i2c_reg_8bit_read_function(i2c, reg_addr, buffer, buffer_size);
    sem_post(&i2c_sem);

    return ret;
}

int i2c_reg_8bit_write(i2c_clt_t *i2c, uint8_t reg_addr ,uint8_t *buffer, size_t buffer_size)
{
    int ret = 0;

    sem_wait(&i2c_sem);
    ret = i2c_reg_8bit_write_function(i2c, reg_addr, buffer, buffer_size);
    sem_post(&i2c_sem);

    return ret;
}

int i2c_start(i2c_clt_t *i2c)
{
    int ret = 0;
    ret = open(i2c->file_route, O_RDWR);
    if(ret < 0)
    {
        printf("[%s]", i2c->file_route);
        perror("fail to open file");
        return -1;
    }

    i2c->fd = ret;
    ret = ioctl(i2c->fd, I2C_SLAVE, i2c->slave_address);
    if(ret < 0)
    {
        perror("i2c ioclt slave address set fail");
        close(i2c->fd);
        return -1;
    }

    return 1;
}

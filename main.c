#include "i2c_ctl.h"
#include "mpu6050.h"

int main()
{
    start_mpu6050_thread();
    while(1)
    {
        sleep(1);
    }
}
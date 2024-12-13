OUT=main
FILES=main.c i2c_ctl.c mpu6050.c
OPT=-pthread -lrt
all: clean
	$(CC) -o $(OUT) $(OPT) $(FILES)

clean:
	rm -rf $(OUT)
OUT=main
FILES=main.c i2c_ctl.c mpu6050.c
OPT=-pthread -lrt -lm
all: clean
	$(CC) -o $(OUT) $(FILES) $(OPT)

clean:
	rm -rf $(OUT)
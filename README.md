Hello
This is library for stm32 for bmp280 
quick step-by-step
1. create new project in stm32Cubeide
2. set all peripherials, this library require i2c
3. in bmp280.c in 10 line define which i2c you are using (default is i2c1 (put hi2c)
4. in bmp280.c in 11 line define which i2c you are using (default is i2c1 (put &hi2c)
5. connect your bmp280 module (power(depend if you have 3v or 5v, check your module, gnd(gnd), scl and sda (check in stm32Cubeide)
6. if you want to get values of temperature and pressure in 32 bit format in include type (extern int32_t temperature1) next line (extern int32_t pressure1) then you can call this values


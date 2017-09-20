cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=toolchain-arm-none-eabi-rpi.cmake .
#arm-none-eabi-gcc -O0 -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -nostartfiles -g led_blink.c cstubs.c cstartup.c start.s -o kernel.elf -Wl,-T,rpi.x
#arm-none-eabi-objcopy kernel.elf -O binary kernel.img


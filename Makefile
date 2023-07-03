main.bin: main.elf
	arm-none-eabi-objcopy -O binary main.elf main.bin

sartup.o: startup.s
	arm-none-eabi-as -mcpu=arm926ej-s -g startup.s -o startup.o

main.o: main.c
	arm-none-eabi-gcc -c -mcpu=arm926ej-s -g main.c -o main.o

main.elf: main.o startup.o linker.ld
	arm-none-eabi-ld -T linker.ld main.o startup.o -o main.elf

gdb:
	arm-none-eabi-gdb

run:
	arm-none-eabi-nm -f sysv main.elf | rg '.logs$$' | rg -v '__log_\d+_label'
	qemu-system-arm -M versatilepb -m 128M -nographic -semihosting  -kernel main.bin

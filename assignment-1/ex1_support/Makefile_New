# Example Makefile
#
# Exercise 1, TDT4258

LD=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

LDFLAGS=-nostdlib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g

LINKERSCRIPT=efm32gg.ld

all : ex1.bin ex1_simple.bin


ex1.bin : ex1.elf
	${OBJCOPY} -j .text -O binary $< $@

ex1.elf : ex1.o 
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

ex1.o : ex1.s
	${AS} ${ASFLAGS} $< -o $@




ex1_simple.bin : ex1_simple.elf
	${OBJCOPY} -j .text -O binary $< $@

ex1_simple.elf : ex1_simple.o 
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

ex1_simple.o : ex1_simple.s
	${AS} ${ASFLAGS} $< -o $@




.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "ex1.bin" -r

upload_simple :
	-eACommander.sh -r --address 0x00000000 -f "ex1_simple.bin" -r

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex

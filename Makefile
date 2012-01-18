CC=powerpc-eabi-gcc
OBJCOPY=powerpc-eabi-objcopy
CFLAGS=-c -g -Wall $(INCDIRS) -fno-builtin -msoft-float
INCDIRS=-Isrc/ -Inewlib/powerpc-eabi/include -Isrc/include
LDFLAGS=-T src/kernel.lcf -fno-builtin -msoft-float
LOADERLFFLAGS=-T src/boot.lcf -nostdlib -fno-builtin
DOT=dot
IMGTYPE=-Tpng
SIMICS=`which simics`

all: make-all

docs:
	$(DOT) $(IMGTYPE) -o doc/os.png doc/os.dot

emu:
	$(SIMICS) simics/ebony-ppcOS.simics

make-all: kernel loader

kernel: start.o start-asm.o mm.o syscall_stubs.o uart.o sched.o
	$(CC) $(LDFLAGS) build/timer.o build/sched.o build/syscall_stubs.o build/start.o\
					build/start-asm.o build/mm.o build/uart.o\
					newlib/powerpc-eabi/lib/libc.a newlib/powerpc-eabi/lib/libm.a\
					 -o kernel.ppc.elf
	$(OBJCOPY) -O binary kernel.ppc.elf kernel.ppc.bin

loader:	loader.o
	$(CC) $(LOADERLFFLAGS) build/loader.o -o loader.ppc.elf
	$(OBJCOPY) -O binary loader.ppc.elf loader.ppc.bin

start.o:
	$(CC) $(CFLAGS) src/crt0/start.c -o build/start.o

start-asm.o:
	$(CC) $(CFLAGS) -mregnames src/crt0/start.S -o build/start-asm.o

loader.o:
	$(CC) $(CFLAGS) -mregnames src/boot/loader.S -o build/loader.o

mm.o:
	$(CC) $(CFLAGS) src/mm/mm.c -o build/mm.o

uart.o:
	$(CC) $(CFLAGS) src/uart/uart.c -o build/uart.o

syscall_stubs.o:
	$(CC) $(CFLAGS) src/stubs/syscall_stubs.c -o build/syscall_stubs.o

sched.o: timer.o
	$(CC) $(CFLAGS) src/sched/sched.c -o build/sched.o

timer.o:
	$(CC) $(CFLAGS) src/sched/timer.c -o build/timer.o

clean:
	rm -f kernel.ppc.elf
	rm -f kernel.ppc.bin
	rm -f loader.ppc.elf
	rm -f loader.ppc.bin
	rm -rf build/*.o

CC=powerpc-eabi-gcc
OBJCOPY=powerpc-eabi-objcopy
CFLAGS=-c -g -Wall $(INCDIRS) -fno-builtin -msoft-float -mcpu=440
INCDIRS=-I src -I newlib/powerpc-eabi/include -I src/include
LDFLAGS=-T src/kernel.lcf -fno-builtin -msoft-float -mcpu=440
LOADERLFFLAGS=-T src/boot.lcf -nostdlib -fno-builtin
DOT=dot
IMGTYPE=-Tpng
SIMICS=$(which simics)

all: make-all

.PHONY: docs emu clean
docs:
	$(DOT) $(IMGTYPE) -o doc/os.png doc/os.dot

emu:
	$(SIMICS) -no-win simics/ebony-ppcOS.simics

make-all: kernel loader

kernel: start.o start-asm.o mm.o syscall_stubs.o uart.o exception_handlers.o asm_exception_handlers.o irq.o timer.o sched.o log.o ppc.o
	$(CC) $(LDFLAGS) build/syscall_stubs.o build/start.o build/start-asm.o build/mm.o build/uart.o build/timer.o build/sched.o build/exception_handlers.o build/asm_exception_handlers.o build/irq.o build/log.o build/ppc.o newlib/powerpc-eabi/lib/libc.a newlib/powerpc-eabi/lib/libm.a -o kernel.ppc.elf
	$(OBJCOPY) -O binary kernel.ppc.elf kernel.ppc.bin

loader:	loader.o
	$(CC) $(LOADERLFFLAGS) build/loader.o -o loader.ppc.elf
	$(OBJCOPY) -O binary loader.ppc.elf loader.ppc.bin

start.o: src/crt0/start.c
	$(CC) $(CFLAGS) $< -o build/start.o

start-asm.o: src/crt0/start.S
	$(CC) $(CFLAGS) -mregnames $< -o build/$@

loader.o: src/boot/loader.S
	$(CC) $(CFLAGS) -mregnames $< -o build/$@

mm.o: src/mm/mm.c
	$(CC) $(CFLAGS) $< -o build/$@

uart.o: src/uart/uart.c
	$(CC) $(CFLAGS) $< -o build/$@

syscall_stubs.o: src/stubs/syscall_stubs.c
	$(CC) $(CFLAGS) $< -o build/$@

sched.o: src/sched/sched.c
	$(CC) $(CFLAGS) $< -o build/$@

timer.o: src/timer/timer.c
	$(CC) $(CFLAGS) $< -o build/$@

exception_handlers.o: src/irq/exception_handlers.c
	$(CC) $(CFLAGS) $< -o build/$@

asm_exception_handlers.o: src/irq/exception_handlers.S
	$(CC) $(CFLAGS) -mregnames $< -o build/$@

irq.o: src/irq/irq.c
	$(CC) $(CFLAGS) $< -o build/$@

log.o: src/log/log.c
	$(CC) $(CFLAGS) $< -o build/$@

ppc.o: src/arch/ppc.c
	$(CC) $(CFLAGS) $< -o build/$@

clean:
	rm -f kernel.ppc.elf
	rm -f kernel.ppc.bin
	rm -f loader.ppc.elf
	rm -f loader.ppc.bin
	rm -rf build/*.o

CC= gcc
CC1=nasm
CFLAGS= -Wall -Wcomment -Wshadow -O -Wwrite-strings -fno-stack-protector -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdlib -nostdinc -fno-builtin -fno-stack-protector -I./_include/ -m32 -c
ASFLAGS= -f elf32
LDFLAGS= -m elf_i386 -T link.ld
SOURCES= *.o
IDIR = ../_include
LDIR = ../_include/include
PRINT=echo


link:
	$(PRINT) -e "compilando kernel\n"
	$(CC1) $(ASFLAGS) kernel.asm -o kernel.o
	$(CC1) $(ASFLAGS) gdt.asm -o gdt.o
	$(CC1) $(ASFLAGS) shutdown.asm -o shutdown.o


	$(PRINT) -e "compilando modulos\n"
	$(CC) $(CFLAGS) kc.c -o kc.o

	$(PRINT) -e "linkado completado\n"
	ld $(LDFLAGS) -o kernel $(SOURCES)

finally:
	echo "\ncompilacion terminada"
	rm -rf *.o;

virtual:
	qemu-system-x86_64 -cdrom desmonOS.iso

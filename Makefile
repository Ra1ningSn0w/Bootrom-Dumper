CC = gcc
CROSS ?= arm-elf-
CFLAGS =  -I./ -lusb-1.0 ./libsyringe.a -lsetupapi
CROSS-AS = $(CROSS)as
CROSS-OBJCOPY = $(CROSS)objcopy

all:
	$(CC) bdu.c -o bdu $(CFLAGS)
	$(CROSS-AS -mthumb) --fatal-warnings -o payload.o payload.s
	$(CROSS-OBJCOPY) -O binary  payload.o payload.bin
	rm payload.o
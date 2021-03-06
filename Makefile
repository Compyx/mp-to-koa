# vim: noet ts=8 sw=8 sts=8:
#
# Makefile for mp-to-koa

VPATH = src

ASM = 64tass
ASM_FLAGS = --case-sensitive --ascii --m6502 -Wshadow -Wbranch-page
CC = gcc
# CC = clang-10
LD = $(CC)
CFLAGS = -Wall -Wextra -std=c99 -O2 -g \
	 -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align \
	 -Wstrict-prototypes -Wmissing-prototypes \
	 -Wswitch-default -Wswitch-enum -Wuninitialized -Wconversion \
	 -Wredundant-decls -Wnested-externs -Wunreachable-code

TARGET_OBJS = main.o

ifeq ($(OS),Windows_NT)
	TARGET_EXE = mp-to-koa.exe
else
	TARGET_EXE = mp-to-koa
endif

SHOW_KOALA = show_koala.prg


all: $(TARGET_EXE)


$(SHOW_KOALA): src/show_koala.s
	$(ASM) $(ASM_FLAGS) -D KOALA_FILE=$(KOALA) -o $@ $<

$(TARGET_EXE): $(TARGET_OBJS)
	$(LD) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<



.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGET_OBJS)
	rm -f $(TARGET_EXE)
	rm -f $(SHOW_KOALA)


.PHONY: doc
doc:
	doxygen


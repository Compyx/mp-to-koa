# vim: noet ts=8 sw=8 sts=8:
#
# Makefile for mp-to-koa

VPATH = src

ASM = 64tass
CC = gcc
LD = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g \
	 -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align \
	 -Wstrict-prototypes -Wmissing-prototypes \
	 -Wswitch-default -Wswitch-enum -Wuninitialized -Wconversion \
	 -Wredundant-decls -Wnested-externs -Wunreachable-code
ASM_FLAGS = --case-sensitive --ascii --m6502 -Wshadow -Wbranch-page

TARGET_OBJS = main.o

TARGET_EXE = mp-to-koa

SHOW_KOALA = show_koala.prg


all: $(TARGET_EXE)

$(SHOW_KOALA): src/show_koala.s
	$(ASM) $(ASM_FLAGS) -o $@ $<



$(TARGET_EXE): $(TARGET_OBJS)
	$(LD) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<



.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGET_OBJS)
	rm -f $(TARGET_EXE)


.PHONY: doc
doc:
	doxygen


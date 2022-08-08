CC?=gcc

BUILDDIR=build
BIN=raytracer

CSRC=$(wildcard *.c)
COBJ=$(patsubst %.c, $(BUILDDIR)/%.c.o, $(CSRC))
CDEF=$(patsubst %.c, $(BUILDDIR)/%.c.d, $(CSRC))

CFLAGS+=-Og -g -Wall -Werror -Wextra -I. -c -MMD 
LFLAGS+=-lm

DEPS=sdl2

CFLAGS+=$(foreach dep, $(DEPS), $(shell pkg-config --cflags $(dep)))
LFLAGS+=$(foreach dep, $(DEPS), $(shell pkg-config --libs $(dep)))

$(BUILDDIR)/%.c.o: %.c
	mkdir -p $(basename $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILDDIR)/$(BIN): $(COBJ)
	$(CC) $(COBJ) $(LFLAGS) -o $@

.PHONY: all run clean

all: $(BUILDDIR)/$(BIN)

run: $(BUILDDIR)/$(BIN)
	./$(BUILDDIR)/$(BIN)

clean:
	rm -rf $(BUILDDIR)/

-include build/*.d
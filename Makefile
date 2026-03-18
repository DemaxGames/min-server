TARGET = min

CC = gcc
LD = gcc
CFLAGS += -Iinclude -g

CSRC := $(wildcard src/*.cpp)
CSRC := $(CSRC) $(wildcard src/*.c)

COBJ := $(patsubst %.cpp, %.o, $(CSRC))
COBJ := $(patsubst %.c, %.o, $(COBJ))
COBJ := $(subst src, build, $(COBJ))

$(TARGET): debug $(COBJ)
	$(LD) $(COBJ) -o $(TARGET).exe

build/%.o: src/%.cpp
	$(CC) $< $(CFLAGS) -c -o $@

build/%.o: src/%.c
	$(CC) $< $(CFLAGS) -c -o $@

clean:
	rm -r $(COBJ)
	
debug:
	@echo CFLAGS = $(CFLAGS)
	@echo sources = $(CSRC)
	@echo objects = $(COBJ)



CC = gcc
BIN = yauc

DEPS = $(wildcard deps/*/*.c)
SRC = $(filter-out src/$(BIN).c, $(wildcard src/*.c))
TESTS = $(wildcard test/*.c)

DEPS_OBJS = $(DEPS:.c=.o)
SRC_OBJS = $(SRC:.c=.o)

CFLAGS += -std=c11 -Wall -O2 -pedantic -Ideps -Isrc

$(DEPS_OBJS):
	 $(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)

$(SRC_OBJS):
	 $(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)

$(TESTS):
	 $(CC) $(DEPS_OBJS) $(SRC_OBJS) $(@) $(CFLAGS) -o $(@:.c=)
	 ./$(@:.c=)
	 rm $(@:.c=)

build: $(DEPS_OBJS) $(SRC_OBJS)
	 $(CC) $(DEPS_OBJS) $(SRC_OBJS) src/$(BIN).c $(CFLAGS) -o $(BIN)

test: $(DEPS_OBJS) $(SRC_OBJS) $(TESTS)

clean:
	rm -f $(DEPS_OBJS) $(SRC_OBJS) $(BIN) $(TARGET_STATIC) $(TARGET_DSO) $(TARGET_DSOLIB) *.so*

all: clean test build

.PHONY: $(SRC_OBJS) $(TESTS) test

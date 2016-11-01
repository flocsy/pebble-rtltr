#PEBBLE_HEADERS=tests/include
PEBBLE_HEADERS=build/include

CC=gcc
ifeq ($(TRAVIS), true)
	CC_TEST=$(CC)
	CFLAGS=
	AR=ar
else
	CC_TEST=arm-none-eabi-gcc
	CFLAGS=-std=c11
	AR=arm-none-eabi-ar
endif
CINCLUDES=-I $(PEBBLE_HEADERS) -I tests/ -I include/ -I build/ -D MESSAGE_KEY_ENABLE_RTLTR
LIBS=
#tests/lib/libpebble.a

TEST_FILES=tests/rtltr-test.c
SRC_FILES=src/c/*.c
TEST_EXTRAS=tests/test.c tests/pebble-mock.c

all: test

test:
	@$(CC) $(CFLAGS) $(CINCLUDES) $(LIBS) $(TEST_FILES) $(SRC_FILES) $(TEST_EXTRAS) -o tests/run
	@tests/run
	@rm tests/run
	@printf "\x1B[0m"

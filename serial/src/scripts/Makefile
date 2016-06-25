CC := gcc
CFLAGS := -Wall -O3
Libs = -lpthread
Target := serial
Source := $(wildcard src/*.c)
Objs := $(patsubst %.c,%.o,$(Source))
Modules += main tcp serial_control
AllObjs := $(addsuffix /src/*.o,$(Modules))


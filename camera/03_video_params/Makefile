CC = arm-buildroot-linux-gnueabihf-gcc
CFLAGS = -Wall -O2
LDFLAGS = 

SRC = videotest.c
OBJ = $(SRC:.c=.o)
TARGET = test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

CROSSCOMPILE := arm-linux-

CFLAGS 	:= -Wall -O2 -c
CFLAGS  += -I$(PWD)/include 

# for 100ask imx6ull
CFLAGS += -I /home/book/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/freetype2

# for 100ask stm32mp157
CFLAGS += -I /home/book/100ask_stm32mp157_pro-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/freetype2

LDFLAGS := -lm -lfreetype

CC 	:= $(CROSSCOMPILE)gcc
LD 	:= $(CROSSCOMPILE)ld

OBJS := main.o \
			display/disp_manager.o        \
			display/fb.o                  \
			encoding/ascii.o              \
			encoding/utf-16be.o           \
			encoding/encoding_manager.o   \
			encoding/utf-8.o              \
			encoding/utf-16le.o           \
			draw/draw.o                   \
			fonts/ascii.o                 \
			fonts/gbk.o                   \
			fonts/freetype.o              \
			fonts/fonts_manager.o

all: $(OBJS)
	$(CC) $(LDFLAGS) -o show_file $^

clean:
	rm -f show_file
	rm -f $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) -o $@ $<


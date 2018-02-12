KPATH := ~/linux
TARGET := nrf24l01+.ko
SRC := $(wildcard *.c)

ccflags-y := -Wno-declaration-after-statement
obj-m := nrf24l01+.o

.DEFAULT: all

all: $(TARGET)

$(TARGET): $(SRC)
	make -C $(KPATH) M=$(PWD) modules

ins:
	insmod $(TARGET) spi_busnum=0 spi_cs_pin=0

rm:
	rmmod $(TARGET:.ko=)

clean:
	make -C $(KPATH) M=$(PWD) clean

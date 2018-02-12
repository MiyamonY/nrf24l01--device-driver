KPATH := ~/linux
TARGET := nrf24l01+.ko
obj-m := nrf24l01+.o

.DEFAULT: all

all: $(TARGET)

$(TARGET):
	make -C $(KPATH) M=$(PWD) modules

ins:
	insmod $(TARGET) spi_cs_pin=0

rm:
	rmmod $(TARGET:.ko=)

clean:
	make -C $(KPATH) M=$(PWD) clean

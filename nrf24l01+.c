/**
 * File:  nrf24l01+.c
 * Author: ymiyamoto
 *
 * Created on Mon Feb 12 13:47:32 2018
 */
#include <linux/device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/spi/spi.h>

#define OK 0
#define DRIVER_NAME "nrf24l01"

#define SPI_MAX_SPEED (1000000)
#define SPI_MODE (SPI_MODE_3)

static int spi_busnum = 0;
static int spi_cs_pin = 0;
module_param(spi_busnum, int, S_IRUSR | S_IRGRP | S_IROTH);
module_param(spi_cs_pin, int, S_IRUSR | S_IRGRP | S_IROTH);

static struct spi_board_info nrf24l01_board_info = {
    .modalias = DRIVER_NAME,
    .max_speed_hz = SPI_MAX_SPEED,
    .mode = SPI_MODE,
};

static struct spi_device_id nrf24l01_id[] = {
    {DRIVER_NAME, 0},
    {},
};
MODULE_DEVICE_TABLE(spi, nrf24l01_id);

static struct spi_driver nrf24l01_driver = {
    .id_table = nrf24l01_id,
    .driver =
        {
            .name = DRIVER_NAME,
            .owner = THIS_MODULE,
        },
};

static struct spi_device *nrf24l01_device;

static int remove_existing_spi_device(int bus_num, int cs_pin)
{
  struct spi_master *master = spi_busnum_to_master(bus_num);
  if (master == NULL) {
    pr_err("%s: spi_master not found", DRIVER_NAME);
    return -ENODEV;
  }

  char name[128];
  snprintf(name, sizeof(name), "%s.%u", dev_name(&master->dev), cs_pin);

  struct device *spidev = bus_find_device_by_name(&spi_bus_type, NULL, name);
  if (spidev == NULL) {
    pr_info("%s: spidev not found", DRIVER_NAME);
    return OK;
  }

  pr_info("%s: delete %s", DRIVER_NAME, name);
  device_del(spidev);
  return OK;
}

static int nrf24l01_init(void)
{
  pr_info("%s: initalize", DRIVER_NAME);

  int err = remove_existing_spi_device(spi_busnum, spi_cs_pin);
  if (err != OK) {
    pr_err("%s: remove_existing_spi_device error", DRIVER_NAME);
    return err;
  }

  spi_register_driver(&nrf24l01_driver);

  struct spi_master *master = spi_busnum_to_master(spi_busnum);
  if (master == NULL) {
    pr_err("%s: spi_master not found", DRIVER_NAME);
    return -ENODEV;
  }

  nrf24l01_board_info.bus_num = spi_busnum;
  nrf24l01_board_info.chip_select = spi_cs_pin;

  nrf24l01_device = spi_new_device(master, &nrf24l01_board_info);
  if (nrf24l01_device == NULL) {
    pr_err("%s: spi_new_device error", DRIVER_NAME);
    spi_unregister_driver(&nrf24l01_driver);
    return -ENODEV;
  }

  return 0;
}

static void nrf24l01_exit(void)
{
  pr_info("%s: exit", DRIVER_NAME);

  spi_unregister_device(nrf24l01_device);
  spi_unregister_driver(&nrf24l01_driver);
}

module_init(nrf24l01_init);
module_exit(nrf24l01_exit);

MODULE_AUTHOR("MIYAMOTO Yohei");
MODULE_LICENSE("GPL v2");

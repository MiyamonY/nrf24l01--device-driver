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

static int spi_busnum = 0;
static int spi_cs_pin = 0;
module_param(spi_cs_pin, int, S_IRUSR | S_IRGRP | S_IROTH);

static int remove_existing_spi_device(int spi_cs_pin)
{
  struct spi_master *master = spi_busnum_to_master(spi_busnum);
  if (master == NULL) {
    pr_err("%s: spi_master not found", DRIVER_NAME);
    return -ENODEV;
  }

  char name[128];
  snprintf(name, sizeof(name), "%s.%u", dev_name(&master->dev), spi_cs_pin);

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

  int err = remove_existing_spi_device(spi_cs_pin);
  if (err != OK) {
    pr_err("%s: remove_existing_spi_device error", DRIVER_NAME);
    return err;
  }

  return 0;
}

static void nrf24l01_exit(void)
{
  pr_info("%s: exit", DRIVER_NAME);
}

module_init(nrf24l01_init);
module_exit(nrf24l01_exit);

MODULE_AUTHOR("MIYAMOTO Yohei");
MODULE_LICENSE("GPL v2");

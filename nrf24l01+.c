/**
 * File:  nrf24l01+.c
 * Author: ymiyamoto
 *
 * Created on Mon Feb 12 13:47:32 2018
 */
#include <linux/init.h>
#include <linux/module.h>

#define DRIVER_NAME "nrf24l01"

static int spi_cs_pin = 0;
module_param(spi_cs_pin, int, S_IRUSR | S_IRGRP | S_IROTH);

static int nrf24l01_init(void)
{
  pr_info("%s initalize", DRIVER_NAME);
  return 0;
}

static void nrf24l01_exit(void)
{
  pr_info("%s exit", DRIVER_NAME);
}

module_init(nrf24l01_init);
module_exit(nrf24l01_exit);

MODULE_AUTHOR("MIYAMOTO Yohei");
MODULE_LICENSE("GPL v2");

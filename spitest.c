#include <linux/init.h>
#include <linux/module.h>
#include <linux/spi/spi.h>

/* META INFO */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Clemens Wegener");
MODULE_DESCRIPTION("A simple spi test driver");

int bus_num = 2;
module_param(bus_num, int, 0);
int cs = 1;
module_param(cs, int, 0);

static struct spi_device *spi_test_dev;

static int __init spi_test_init(void){
	printk(KERN_DEBUG "spitest loading...\n");
	struct spi_master *master;
	struct spi_board_info spi_device_info = 
	{
  		.modalias     = "spi-test-driver",
  		.max_speed_hz = 4000000,              // speed your device (slave) can handle
  		.bus_num      = bus_num,		      // SPI 1
  		.chip_select  = cs,                    // Use 0 Chip select (GPIO 18)
  		.mode         = SPI_MODE_0            // SPI mode 0
	};

	master = spi_busnum_to_master(bus_num);

	if(!master) {
		printk("There is no spi bus with no. %d\n", bus_num);
		return -ENODEV;
	}
	else printk("Master No. %d registered\n", bus_num);

	spi_test_dev = spi_new_device(master, &spi_device_info);

	if(!spi_test_dev) {
  		printk("FAILED to create slave.\n");
  		return -ENODEV;
	}
	else printk("Created slave at chip select %d.\n", cs);

	spi_test_dev -> bits_per_word = 8;

	if(spi_setup(spi_test_dev) != 0) {
			printk("Could not change bus setup\n");
			spi_unregister_device(spi_test_dev);
			return -1;
	}
	/*u8 id;
	id = spi_w8r8(spi_test_dev, 0xD0);
	printk("Chip ID: 0x%x\n", id);
	*/
	u8 val[] = {0x75, 0x40, 0x75, 0x40, 0x75, 0x40, 0x75, 0x40, 0x75, 0x40};
	int error = spi_write(spi_test_dev, val, sizeof(val));
	if(error == 0) {
		printk("written some values\n");
	}
	else {
		printk("could not write to spi device\n");
		return error;
	}
	return 0;
}

static void __exit spi_test_exit(void)
{
	if(spi_test_dev) {
		spi_unregister_device(spi_test_dev);
	}
	printk(KERN_DEBUG "spitest unloaded\n");
}

module_init(spi_test_init);
module_exit(spi_test_exit);

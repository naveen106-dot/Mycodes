/*
 * Simple I2C Device Driver for Linux Kernel
 * This driver demonstrates I2C communication with a slave device
 * Compile: make
 * Load: sudo insmod i2c_driver.ko
 * Test: cat /sys/devices/platform/i2c_test_device.0/name
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/sysfs.h>
#include <linux/device.h>
#include <linux/mutex.h>

#define DRIVER_NAME "i2c_test_driver"
#define DEVICE_NAME "i2c_test_device"
#define I2C_SLAVE_ADDR 0x50  /* Example: EEPROM address, adjust as needed */
#define I2C_BUS_NUM 0        /* Bus number, check with i2cdetect -l */

/* Device private data structure */
struct i2c_test_device {
    struct i2c_client *client;
    struct mutex lock;
    u8 data_buffer[256];
    int data_length;
};

static struct i2c_test_device *g_dev;

/* ==================== I2C Read/Write Operations ==================== */

/* Read data from I2C device */
static int i2c_read_data(struct i2c_test_device *dev, u8 reg, u8 *data, int len)
{
    struct i2c_msg msgs[2];
    int ret;

    /* Message 1: Send register address */
    msgs[0].addr = dev->client->addr;
    msgs[0].flags = 0;  /* Write */
    msgs[0].len = 1;
    msgs[0].buf = &reg;

    /* Message 2: Read data */
    msgs[1].addr = dev->client->addr;
    msgs[1].flags = I2C_M_RD;  /* Read flag */
    msgs[1].len = len;
    msgs[1].buf = data;

    /* Perform I2C transfer */
    ret = i2c_transfer(dev->client->adapter, msgs, 2);
    if (ret < 0) {
        dev_err(&dev->client->dev, "I2C read failed: %d\n", ret);
        return ret;
    }

    return 0;
}

/* Write data to I2C device */
static int i2c_write_data(struct i2c_test_device *dev, u8 reg, const u8 *data, int len)
{
    u8 buf[len + 1];
    struct i2c_msg msg;
    int ret;

    buf[0] = reg;
    memcpy(&buf[1], data, len);

    msg.addr = dev->client->addr;
    msg.flags = 0;  /* Write */
    msg.len = len + 1;
    msg.buf = buf;

    ret = i2c_transfer(dev->client->adapter, &msg, 1);
    if (ret < 0) {
        dev_err(&dev->client->dev, "I2C write failed: %d\n", ret);
        return ret;
    }

    return 0;
}

/* Single byte read (SMBus) */
static int i2c_read_byte(struct i2c_test_device *dev, u8 reg)
{
    int ret = i2c_smbus_read_byte_data(dev->client, reg);
    if (ret < 0) {
        dev_err(&dev->client->dev, "SMBus read failed: %d\n", ret);
    }
    return ret;
}

/* Single byte write (SMBus) */
static int i2c_write_byte(struct i2c_test_device *dev, u8 reg, u8 value)
{
    int ret = i2c_smbus_write_byte_data(dev->client, reg, value);
    if (ret < 0) {
        dev_err(&dev->client->dev, "SMBus write failed: %d\n", ret);
    }
    return ret;
}

/* ==================== Sysfs Attributes ==================== */

/* Read device name */
static ssize_t name_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "%s\n", DEVICE_NAME);
}

/* Read from I2C device */
static ssize_t read_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct i2c_test_device *i2c_dev = dev_get_drvdata(dev);
    u8 data[4];
    int ret;

    mutex_lock(&i2c_dev->lock);
    ret = i2c_read_data(i2c_dev, 0x00, data, 4);
    mutex_unlock(&i2c_dev->lock);

    if (ret < 0)
        return ret;

    return sprintf(buf, "0x%02X 0x%02X 0x%02X 0x%02X\n", 
                   data[0], data[1], data[2], data[3]);
}

/* Write to I2C device */
static ssize_t write_store(struct device *dev, struct device_attribute *attr,
                           const char *buf, size_t count)
{
    struct i2c_test_device *i2c_dev = dev_get_drvdata(dev);
    u8 reg, value;
    int ret;

    if (sscanf(buf, "%hhu %hhu", &reg, &value) != 2)
        return -EINVAL;

    mutex_lock(&i2c_dev->lock);
    ret = i2c_write_byte(i2c_dev, reg, value);
    mutex_unlock(&i2c_dev->lock);

    return ret < 0 ? ret : count;
}

/* Define sysfs attributes */
static DEVICE_ATTR_RO(name);
static DEVICE_ATTR_RO(read);
static DEVICE_ATTR_WO(write);

static struct attribute *i2c_test_attrs[] = {
    &dev_attr_name.attr,
    &dev_attr_read.attr,
    &dev_attr_write.attr,
    NULL,
};

static struct attribute_group i2c_test_group = {
    .attrs = i2c_test_attrs,
};

/* ==================== I2C Client Driver ==================== */

/* Probe function - called when device is detected */
static int i2c_test_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    struct i2c_test_device *dev;
    int ret;

    pr_info("I2C Device found at address 0x%02x\n", client->addr);

    /* Allocate device structure */
    dev = devm_kzalloc(&client->dev, sizeof(*dev), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;

    dev->client = client;
    mutex_init(&dev->lock);
    i2c_set_clientdata(client, dev);
    g_dev = dev;

    /* Test I2C communication */
    pr_info("Testing I2C communication...\n");
    ret = i2c_read_byte(dev, 0x00);
    if (ret < 0) {
        dev_err(&client->dev, "Failed to read from device\n");
        return ret;
    }
    pr_info("Successfully read from device: 0x%02x\n", ret);

    /* Create sysfs attributes */
    ret = sysfs_create_group(&client->dev.kobj, &i2c_test_group);
    if (ret) {
        dev_err(&client->dev, "Failed to create sysfs group\n");
        return ret;
    }

    dev_info(&client->dev, "I2C Device Driver Loaded Successfully\n");
    return 0;
}

/* Remove function - called when driver is unloaded */
static int i2c_test_remove(struct i2c_client *client)
{
    struct i2c_test_device *dev = i2c_get_clientdata(client);

    sysfs_remove_group(&client->dev.kobj, &i2c_test_group);
    dev_info(&client->dev, "I2C Device Driver Removed\n");
    return 0;
}

/* Device ID table */
static const struct i2c_device_id i2c_test_ids[] = {
    { DEVICE_NAME, 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, i2c_test_ids);

/* I2C driver structure */
static struct i2c_driver i2c_test_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
    },
    .probe = i2c_test_probe,
    .remove = i2c_test_remove,
    .id_table = i2c_test_ids,
};

/* ==================== Module Init/Exit ==================== */

static int __init i2c_test_init(void)
{
    int ret;

    pr_info("Loading I2C Test Driver\n");

    /* Register I2C driver */
    ret = i2c_add_driver(&i2c_test_driver);
    if (ret) {
        pr_err("Failed to register I2C driver: %d\n", ret);
        return ret;
    }

    pr_info("I2C Test Driver initialized\n");
    return 0;
}

static void __exit i2c_test_exit(void)
{
    pr_info("Unloading I2C Test Driver\n");
    i2c_del_driver(&i2c_test_driver);
}

module_init(i2c_test_init);
module_exit(i2c_test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple I2C Device Driver");
MODULE_VERSION("1.0");
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define DRIVER_NAME     "i2c_eeprom"
#define DEVICE_NAME     "eeprom_i2c"
#define CLASS_NAME      "i2c_class"

#define EEPROM_SIZE     256   // 24C02 = 256 bytes

static struct i2c_client *eeprom_client;
static dev_t dev_num;
static struct cdev eeprom_cdev;
static struct class *eeprom_class;

/* ================= I2C OPERATIONS ================= */

static int eeprom_read_byte(u8 addr, u8 *data)
{
    return i2c_smbus_read_byte_data(eeprom_client, addr);
}

static int eeprom_write_byte(u8 addr, u8 data)
{
    return i2c_smbus_write_byte_data(eeprom_client, addr, data);
}

/* ================= CHAR DEVICE ================= */

static ssize_t eeprom_read(struct file *file, char __user *buf,
                           size_t count, loff_t *offset)
{
    u8 data;
    int ret;

    if (*offset >= EEPROM_SIZE)
        return 0;

    ret = eeprom_read_byte(*offset, &data);
    if (ret < 0)
        return ret;

    if (copy_to_user(buf, &data, 1))
        return -EFAULT;

    (*offset)++;
    return 1;
}

static ssize_t eeprom_write(struct file *file, const char __user *buf,
                            size_t count, loff_t *offset)
{
    u8 data;

    if (*offset >= EEPROM_SIZE)
        return -EINVAL;

    if (copy_from_user(&data, buf, 1))
        return -EFAULT;

    eeprom_write_byte(*offset, data);
    msleep(10); // EEPROM write delay

    (*offset)++;
    return 1;
}

static const struct file_operations eeprom_fops = {
    .owner = THIS_MODULE,
    .read  = eeprom_read,
    .write = eeprom_write,
};

/* ================= I2C DRIVER ================= */

static int eeprom_probe(struct i2c_client *client)
{
    int ret;

    pr_info("EEPROM I2C probe: addr=0x%x\n", client->addr);
    eeprom_client = client;

    /* Allocate char device number */
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0)
        return ret;

    cdev_init(&eeprom_cdev, &eeprom_fops);
    cdev_add(&eeprom_cdev, dev_num, 1);

    eeprom_class = class_create(CLASS_NAME);
    device_create(eeprom_class, NULL, dev_num, NULL, DEVICE_NAME);

    pr_info("EEPROM driver loaded: /dev/%s\n", DEVICE_NAME);
    return 0;
}

static void eeprom_remove(struct i2c_client *client)
{
    device_destroy(eeprom_class, dev_num);
    class_destroy(eeprom_class);
    cdev_del(&eeprom_cdev);
    unregister_chrdev_region(dev_num, 1);

    pr_info("EEPROM driver removed\n");
}

/* ================= DEVICE TREE MATCH ================= */

static const struct of_device_id eeprom_of_match[] = {
    { .compatible = "atmel,24c02" },
    { }
};
MODULE_DEVICE_TABLE(of, eeprom_of_match);

static struct i2c_driver eeprom_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = eeprom_of_match,
    },
    .probe  = eeprom_probe,
    .remove = eeprom_remove,
};

module_i2c_driver(eeprom_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Naveen");
MODULE_DESCRIPTION("Raspberry Pi I2C EEPROM Driver");

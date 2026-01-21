#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

/* Global variable for device number */
static dev_t first;

/* Global cdev structure */
static struct cdev c_dev;

/* Global class pointer */
static struct class *cl;

/* Open function */
static int my_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Driver: open()\n");
    return 0;
}

/* Close function */
static int my_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Driver: close()\n");
    return 0;
}

/* Read function */
static ssize_t my_read(struct file *f, char __user *buf,
                       size_t len, loff_t *off)
{
    printk(KERN_INFO "Driver: read()\n");
    return 0;
}

/* Write function */
static ssize_t my_write(struct file *f, const char __user *buf,
                        size_t len, loff_t *off)
{
    printk(KERN_INFO "Driver: write()\n");
    return len;
}

/* File operations structure */
static struct file_operations pugs_fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_close,
    .read    = my_read,
    .write   = my_write,
};

/* Module init function */
static int __init ofcd_init(void)
{
    printk(KERN_INFO "NAMASKAR: OFCD registration\n");

    /* Allocate major and minor numbers */
    if (alloc_chrdev_region(&first, 0, 3, "Mahesh") < 0) {
        return -1;
    }

    /* Create device class */
    if ((cl = class_create("chardrv")) == NULL) {
        unregister_chrdev_region(first, 1);
        return -1;
    }

    /* Create device file */
    if (device_create(cl, NULL, first, NULL, "mydeficefile") == NULL) {
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }

    /* Initialize cdev */
    cdev_init(&c_dev, &pugs_fops);

    /* Add cdev to kernel */
    if (cdev_add(&c_dev, first, 1) == -1) {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }

    return 0;
}

/* Module exit function */
static void __exit ofcd_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);

    printk(KERN_INFO "By Gn: OFCD unregistered\n");
}

module_init(ofcd_init);
module_exit(ofcd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MJ");
MODULE_DESCRIPTION("OUR FIRST char driver");

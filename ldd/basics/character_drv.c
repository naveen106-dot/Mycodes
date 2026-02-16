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

/* creation in buffer to read & write*/
#define BUF_SIZE 1024

static char kbuf[BUF_SIZE];
static int kbuf_len = 0;

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
static ssize_t my_read(struct file *f,
                       char __user *buf,
                       size_t len,
                       loff_t *off)
{
    int bytes_to_copy;

    printk(KERN_INFO "Driver: read() called\n");

    if (*off >= kbuf_len)
        return 0;   // EOF

    bytes_to_copy = min(len, (size_t)(kbuf_len - *off));

    if (copy_to_user(buf, kbuf + *off, bytes_to_copy)) {
        return -EFAULT;
    }

    *off += bytes_to_copy;

    return bytes_to_copy;
}


/* Write function */
static ssize_t my_write(struct file *f,
                        const char __user *buf,
                        size_t len,
                        loff_t *off)
{
    int bytes_to_copy;

    printk(KERN_INFO "Driver: write() called\n");

    bytes_to_copy = min(len, (size_t)BUF_SIZE);

    if (copy_from_user(kbuf, buf, bytes_to_copy)) {
        return -EFAULT;
    }

    kbuf_len = bytes_to_copy;

    printk(KERN_INFO "Driver: received %d bytes: %s\n",
           kbuf_len, kbuf);

    return bytes_to_copy;
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
    printk("major:%d",major(first));


    /* Create device class */
    cl = class_create("chardrv");
    if (IS_ERR(cl)) {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cl);
    }

    /* Create device file */
    dev = device_create(cl, NULL, first, NULL, "mydevicefile");
    if (IS_ERR(dev)) {
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
    return PTR_ERR(dev);
    }

    /* Initialize cdev */
    cdev_init(&c_dev, &pugs_fops);

    /* Add cdev to kernel 
    it will true return 0 */
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

#include <linux/module.h>
#include <linux/init.h>+-
#include <linux/fs.h>
#include <linux/kernel.h>

static dev_t first;


// int alloc_chrdev_region(dev_t * dev, unsigned baseminor, unsigned count, const char * name);

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world module loaded\n");

    if (alloc_chrdev_region(&first, 0,3, "Mahesh") <0)
    {
        return -1;
    }
    printk(KERN_INFO "<Major,Minor>: <%d,%d> \n", MAJOR(first),MINOR(first));
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "Hello world module unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Naveen");
MODULE_DESCRIPTION("Simple Hello World Kernel Module");

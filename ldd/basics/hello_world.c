#include <linux/module.h>
#include <linux/init.h>

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world module loaded\n");
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

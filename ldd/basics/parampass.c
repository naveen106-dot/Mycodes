#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

int passvalue;
int array[3] = {100,200,300};
int array_len = 3;
module_param(passvalue,int,S_IRUSR | S_IWUSR);
module_param_array(array,int,&array_len,S_IRUSR | S_IWUSR);
/*module_param_array_named(name, name, type, nump, perm)*/

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world module loaded\n");
    printk(KERN_DEBUG "Module value passed:%d\n",passvalue);
    passvalue = 20;
    printk(KERN_INFO "Module value passed:%d\n",passvalue);
    printk(KERN_INFO "array size:%d\n",array_len);
    printk(KERN_INFO "Module value passed inarray :%d\t%d\t%d\n",array[0],array[1],array[2]);

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

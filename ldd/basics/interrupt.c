#include <linux/module.h>
#include <linux/interrupt.h>

static int irq = 17;

void helper_function(void)
{
    printk("Helper function called\n");
}

static irqreturn_t my_isr(int irq, void *dev_id)
{
    printk("ISR triggered\n");
    helper_function();
    return IRQ_HANDLED;
}

static int __init my_init(void)
{
    request_irq(irq, my_isr, IRQF_SHARED, "my_irq", &irq);
    return 0;
}

static void __exit my_exit(void)
{
    free_irq(irq, &irq);
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");

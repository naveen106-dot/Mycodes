#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>

#define UART_BASE   0x3F8      // Example: COM1 (x86)
#define UART_IRQ    4          // UART IRQ number

#define UART_RBR    0x00       // Receiver Buffer Register
#define UART_IER    0x01       // Interrupt Enable Register
#define UART_IIR    0x02       // Interrupt Identification Register
#define UART_LSR    0x05       // Line Status Register

static void __iomem *uart_base;

/* ---------------- ISR ---------------- */
static irqreturn_t uart_irq_handler(int irq, void *dev_id)
{
    unsigned char status;
    unsigned char ch;

    /* Read Line Status Register */
    status = readb(uart_base + UART_LSR);

    /* Check if data is ready */
    if (status & 0x01) {
        ch = readb(uart_base + UART_RBR);
        printk(KERN_INFO "UART RX char: %c\n", ch);
        return IRQ_HANDLED;
    }

    return IRQ_NONE;
}

/* ---------------- INIT ---------------- */
static int __init uart_irq_init(void)
{
    int ret;

    printk(KERN_INFO "UART IRQ driver init\n");

    /* Map UART registers */
    uart_base = ioremap(UART_BASE, 8);
    if (!uart_base)
        return -ENOMEM;

    /* Request IRQ */
    ret = request_irq(UART_IRQ,
                      uart_irq_handler,
                      IRQF_SHARED,
                      "uart_irq_driver",
                      (void *)uart_irq_handler);
    if (ret) {
        printk(KERN_ERR "Failed to request IRQ\n");
        iounmap(uart_base);
        return ret;
    }

    /* Enable RX interrupt */
    writeb(0x01, uart_base + UART_IER);

    return 0;
}

/* ---------------- EXIT ---------------- */
static void __exit uart_irq_exit(void)
{
    free_irq(UART_IRQ, (void *)uart_irq_handler);
    iounmap(uart_base);
    printk(KERN_INFO "UART IRQ driver removed\n");
}

module_init(uart_irq_init);
module_exit(uart_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Naveen");
MODULE_DESCRIPTION("Simple UART IRQ Driver");

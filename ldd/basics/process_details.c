#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>


static int __init hello_init(void)
{

    struct task_struct *task;
     printk(KERN_INFO "Hello world module loaded\n");
       printk(KERN_INFO "Process list:\n");

    rcu_read_lock();   // REQUIRED for process traversal

    for_each_process(task) {
        printk(KERN_INFO
               "Name: %-16s PID: %5d State: %ld\n",
               task->comm,
               task->pid,
               task_state_index(task));
    }

    rcu_read_unlock();
   
   
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

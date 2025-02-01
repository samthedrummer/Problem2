#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>  // for copy_to_user
#include <asm/param.h>      // for HZ

#define PROC_NAME "seconds"
#define BUFFER_SIZE 128

static unsigned long start_jiffies;  // Records the jiffies when the module is loaded

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

static struct proc_ops proc_ops = {
    .proc_read = proc_read,
};

/* This function is called when the module is loaded. */
int proc_init(void)
{
    // Save the current jiffies value as the start time.
    start_jiffies = jiffies;

    // Create the /proc/seconds entry.
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void)
{
    // Remove the /proc/seconds entry.
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/* This function is called each time /proc/seconds is read. */
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    // Ensure that the output is only returned once per read.
    if (completed) {
        completed = 0;
        return 0;
    }
    completed = 1;

    // Calculate elapsed time in seconds.
    unsigned long elapsed_jiffies = jiffies - start_jiffies;
    unsigned long seconds = elapsed_jiffies / HZ;

    // Format the elapsed seconds into the buffer.
    rv = sprintf(buffer, "Elapsed seconds: %lu\n", seconds);

    // Copy the buffer to user space.
    if (copy_to_user(usr_buf, buffer, rv))
        return -EFAULT;

    return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Module that creates /proc/seconds to report elapsed time");
MODULE_AUTHOR("SGG");

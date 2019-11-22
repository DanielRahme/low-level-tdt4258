#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include "efm32gg.h"

static dev_t dev;
static struct cdev cdev;
static struct class *cl;
static struct fasync_struct *fasync_queue;


static int my_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset)
{
    unsigned int value = ioread32(GPIO_PC_DIN);
    copy_to_user(user_buffer, &value, sizeof(value));
    return sizeof(value);
}


static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_NOTICE "Game-pad: We have opened the device!");
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    printk(KERN_NOTICE "Game-pad: We have opened the device!");
    return 0;
}


struct file_operations my_fops =
{
    .owner = THIS_MODULE,
    .read  = my_read,
    .open = my_open,
    .release = my_release
};


static int register_device()
{
    if (alloc_chrdev_region(&dev, 0, 1, "gamepad") < 0) {
        printk(KERN_INFO "Failed to allocate\n", MAJOR(dev), MINOR(dev));
        return -1;
    }

    printk(KERN_INFO "Register success! Major: %d, Minor: %d\n", MAJOR(dev), MINOR(dev));

    cdev_init(&cdev, &my_fops);
    cdev_add(&cdev, dev, 1);
    cl = class_create(THIS_MODULE, "gamepad");
    device_create(cl, NULL, dev, NULL, "gamepad");

    // Init GPIO
    if (request_mem_region(GPIO_PC_MODEL, 1, DRIVER_NAME ) == NULL ) {
        printk(KERN_ALERT "Error requesting GPIO_PC_MODEL memory region, already in use?\n");
        return -1;
    }
    if (request_mem_region(GPIO_PC_DOUT, 1, DRIVER_NAME ) == NULL ) {
        printk(KERN_ALERT "Error requesting GPIO_PC_DOUT memory region, already in use?\n");
        return -1;
    }
     if (request_mem_region(GPIO_PC_DIN, 1, DRIVER_NAME ) == NULL ) {
        printk(KERN_ALERT "Error requesting GPIO_PC_DIN memory region, already in use?\n");
        return -1;
    }
    iowrite32(0x33333333, GPIO_PC_MODEL);
    iowrite32(0xFF, GPIO_PC_DOUT);
    //iowrite32(0x22222222, GPIO_EXTIPSELL);

    printk(KERN_WARNING "Gamepad-driver succeeded to register");
    return 0;
}


static void unregister_device()
{
    printk(KERN_NOTICE "Gamepad-drive: unregister device");

    // release gpio

    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&cdev);
    unregister_chrdev_region(dev, 1);
}



// Run when accessing the device
static int __init template_init(void)
{
    printk("Bye bye cruel World! Here is your module speaking\n");
    register_device();
    return 0;
}


// Run when exiting the device
static void __exit template_cleanup(void)
{
        /* Release memory regions */
    release_mem_region(GPIO_PC_MODEL, 1);
    release_mem_region(GPIO_PC_DIN, 1);
    release_mem_region(GPIO_PC_DOUT, 1);

    printk("Short life for a small module...\n");
    unregister_device();
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");


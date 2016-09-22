#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include "myfops.h"

#define CHARDEV_NAME "mychar"
#define CLASS_NAME "myclass"

static int major_number;
//static struct class * chardev_class = NULL;
//static struct device* chardev_device = NULL;

static int my_init(void)
{
  int rc = 0;
  const struct file_operations fops = {
    .read = my_read,
    .write = my_write,
    .open  = my_open,
    .release = my_close,
  };

  printk(KERN_INFO "%s: Hello World!\n", __func__);

  // Dynamically allocate a major number
  major_number = register_chrdev(0, CHARDEV_NAME, &fops);
  if(major_number < 0)
  {
    printk(KERN_INFO "%s: Failed to register a major number.\n", __func__);
    return major_number;
  }

  printk(KERN_INFO "%s: got chardev major number %d\n", __func__, major_number);


  return rc;
}

static void
my_exit(void)
{
  unregister_chrdev(major_number, CHARDEV_NAME);         // Unregister major number
  printk(KERN_INFO "%s: Goodbye World!\n", __func__);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");


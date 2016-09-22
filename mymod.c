#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include "myfops.h"

#define CHARDEV_NAME "mychar"
#define CLASS_NAME "myclass"

static dev_t first_dev;

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
  rc = alloc_chrdev_region(&first_dev, 0, 1, CHARDEV_NAME);
  if(rc < 0)
  {
    printk(KERN_INFO "%s: Failed to allocate chardev region.\n", __func__);
    goto out_nothing;
  }

  printk(KERN_INFO "%s: got chardev region, major number %d\n", __func__, MAJOR(first_dev));

out_nothing:
  return rc;
}

static void
my_exit(void)
{
  unregister_chrdev_region(first_dev, 1); // Unregister region of chardevs
  printk(KERN_INFO "%s: Goodbye World!\n", __func__);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");


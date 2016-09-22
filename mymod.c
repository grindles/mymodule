#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include "myfops.h"

#define CHARDEV_NAME "mychar"
#define CLASS_NAME "myclass"

static dev_t first_dev;
static struct class * myclass = NULL;
static struct device * mydev = NULL;
static struct cdev mycdev;

static struct file_operations fops = {
  .read = my_read,
  .write = my_write,
  .open  = my_open,
  .release = my_close,
};

static int my_init(void)
{
  int rc = 0;

  printk(KERN_INFO "%s: Hello World!\n", __func__);

  // Dynamically allocate a major number
  rc = alloc_chrdev_region(&first_dev, 0, 1, CHARDEV_NAME);
  if(rc < 0)
  {
    printk(KERN_INFO "%s: Failed to allocate chardev region.\n", __func__);
    goto out_nothing;
  }

  printk(KERN_INFO "%s: got chardev region, major number %d\n", __func__, MAJOR(first_dev));

  // Create a class
  myclass = class_create(THIS_MODULE, CLASS_NAME);
  if(IS_ERR(myclass))
  {
    printk(KERN_INFO "%s: Failed to create class.\n", __func__);
    rc = PTR_ERR(myclass);
    goto out_unreg;
  }

  printk(KERN_INFO "%s: class created, check out /sys/class/%s\n", __func__, CLASS_NAME);

  // Create device (class, parent - pointer to parent struct if any, dev_t, drvdata - the data to be added to the device for callbacks, format for name)
  mydev = device_create(myclass, NULL, first_dev, NULL, CHARDEV_NAME);
  printk(KERN_INFO "%s: device creation returned 0x%016llx\n", __func__, (uint64_t)mydev);
  if(mydev == NULL)
  {
    printk(KERN_INFO "%s: Failed to create device.\n", __func__);
    rc = -ENOMEM;
    goto out_destroyclass;
  }

  cdev_init(&mycdev, &fops);

  rc = cdev_add(&mycdev, first_dev, 1);
  if(rc < 0)
  {
    printk(KERN_INFO "%s: Failed to add device.\n", __func__);
    goto out_destroydevice;
  }

  printk(KERN_INFO "%s: device created, check out /dev/%s\n", __func__, CHARDEV_NAME);

  goto out_nothing;

out_destroydevice:
  device_destroy(myclass, first_dev);
out_destroyclass:
  class_destroy(myclass);
out_unreg:
  unregister_chrdev_region(first_dev, 1);
out_nothing:
  return rc;
}

static void
my_exit(void)
{
  device_destroy(myclass, first_dev);     // Destroy the device
  class_destroy(myclass);                 // Destroy the class
  unregister_chrdev_region(first_dev, 1); // Unregister region of chardevs
  printk(KERN_INFO "%s: Goodbye World!\n", __func__);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");


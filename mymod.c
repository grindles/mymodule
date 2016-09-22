#include <linux/kernel.h>
#include <linux/module.h>

static int
my_init(void)
{
  printk(KERN_INFO "%s: Hello World!\n", __func__);

  return 0;
}

static void
my_exit(void)
{
  printk(KERN_INFO "%s: Goodbye World!\n", __func__);
}

module_init(my_init);
module_exit(my_exit);


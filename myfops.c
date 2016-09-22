#include <linux/fs.h>
#include <linux/module.h>
  
int my_open(struct inode * inode_p, struct file * file_p)
{
  try_module_get(THIS_MODULE);
  printk(KERN_INFO "%s: Open!\n", __func__);
  return 0;
}

int my_close(struct inode * inode_p, struct file * file_p)
{
  module_put(THIS_MODULE);
  printk(KERN_INFO "%s: Close!\n", __func__);
  return 0;
}

ssize_t my_read(struct file * file_p, char * user_p, size_t len, loff_t * offset)
{
  printk(KERN_INFO "%s: Read %lu!\n", __func__, len);  
  return len;
}

ssize_t my_write(struct file * file_p, const char * user_p, size_t len, loff_t * offset)
{
  printk(KERN_INFO "%s: Write %lu!\n", __func__, len);  
  return len;
}


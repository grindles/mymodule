#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define BUFF_SIZE 4096

static char * message = "Hello Userspace!\n";
  
int my_open(struct inode * inode_p, struct file * file_p)
{
  char* buffer = (char*)kmalloc(BUFF_SIZE, GFP_KERNEL);
  memcpy(buffer, message, strlen(message));
  file_p->private_data = buffer;
  printk(KERN_INFO "%s: Open!\n", __func__);
  return 0;
}

int my_close(struct inode * inode_p, struct file * file_p)
{
  char* buffer = (char*)file_p->private_data;
  kfree(buffer);
  printk(KERN_INFO "%s: Close!\n", __func__);
  return 0;
}

ssize_t my_read(struct file * file_p, char * user_p, size_t len, loff_t * offset)
{
  char* buffer = (char*)file_p->private_data;
  size_t length = strlen(buffer);
  size_t missing;
  if(len < length)
  {
    length = len;
  }
  missing = copy_to_user(user_p, buffer, length);

  printk(KERN_INFO "%s: Read %lu (%lu)!\n", __func__, len, length);

  return length - missing;
}

ssize_t my_write(struct file * file_p, const char * user_p, size_t len, loff_t * offset)
{
  printk(KERN_INFO "%s: Write %lu!\n", __func__, len);  
  return len;
}


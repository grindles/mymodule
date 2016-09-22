int my_open(struct inode *, struct file *);
int my_close(struct inode *, struct file *);
ssize_t my_read(struct file *, char *, size_t, loff_t *);
ssize_t my_write(struct file *, const char *, size_t, loff_t *);

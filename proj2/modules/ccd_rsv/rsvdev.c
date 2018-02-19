#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include<linux/slab.h>

#define  DEVICE_NAME "rsvdev"
#define  CLASS_NAME  "rsv"
#define  BUFFER_LENGTH 4096
MODULE_LICENSE("GPL");

static int    major_number;
static char   *message;
static bool   is_module_available;
static struct class*  rsvdev_class  = NULL;
static struct device* rsvdev_devise = NULL;

static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

static int __init rsvdev_init(void){
   printk(KERN_INFO "rsvdev: Initializing the rsvdev LKM\n");

   major_number = register_chrdev(0, DEVICE_NAME, &fops);
   if (major_number<0){
      printk(KERN_ALERT "rsvdev failed to register a major number\n");
      return major_number;
   }
   printk(KERN_INFO "rsvdev: registered correctly with major number %d\n", major_number);

   rsvdev_class = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(rsvdev_class)){
      unregister_chrdev(major_number, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(rsvdev_class);
   }
   printk(KERN_INFO "rsvdev: device class registered correctly\n");
   is_module_available = true;
   rsvdev_devise = device_create(rsvdev_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
   if (IS_ERR(rsvdev_devise)){
      class_destroy(rsvdev_class);
      unregister_chrdev(major_number, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(rsvdev_devise);
   }
   printk(KERN_INFO "rsvdev: device class created correctly\n");
   return 0;
}

static void __exit rsvdev_exit(void){
   device_destroy(rsvdev_class, MKDEV(major_number, 0));
   class_unregister(rsvdev_class);
   class_destroy(rsvdev_class);
   unregister_chrdev(major_number, DEVICE_NAME);
   printk(KERN_INFO "rsvdev: Goodbye from the LKM!\n");
}

static int dev_open(struct inode *inodep, struct file *filep){
   if (is_module_available) {
      printk(KERN_INFO "rsvdev: Device has been opened");
      is_module_available = false;
      return 0;
   }
   else {
      return -EBUSY;
   }
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   struct task_struct *task;
   unsigned long error_count = 0;
   int buf_length = BUFFER_LENGTH;
   message = kmalloc(buf_length, GFP_KERNEL);
   for_each_process(task) {
      printk("%d\t%d\t%d\t%s", (int)task->pid, (int)task->tgid, task->rt_priority, task->comm);
      if(snprintf(message, BUFFER_LENGTH, "%s\n%d\t%d\t%d\t%s", message, (int)task->pid, (int)task->tgid, task->rt_priority, task->comm) >= buf_length) {
         // printk(KERN_ALERT "HI, %d", buf_length);
         char *temp = kmalloc(buf_length, GFP_KERNEL);
         strncpy(temp, message, buf_length);
         buf_length *= 2;
         kfree(message);
         message = kmalloc(buf_length, GFP_KERNEL);
         strncpy(message, temp, buf_length);
         kfree(temp);
      }
   }
   pr_info("%d\n%s", strlen(message), message);
   error_count = copy_to_user(buffer, message, strlen(message));
   if (error_count == 0){
      printk(KERN_INFO "rsvdev: Sent info correctly\n");
      return 0;
   }
   else {
      printk(KERN_INFO "rsvdev: Failed to send %lu characters to the user\n", error_count);
      return -EFAULT;
   }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
   return -EINVAL;
}

static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "rsvdev: Device successfully closed\n");
   is_module_available = true;
   return 0;
}

module_init(rsvdev_init);
module_exit(rsvdev_exit);
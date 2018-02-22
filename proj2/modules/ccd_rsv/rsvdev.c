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
#define  MESSAGE_LENGTH 1024
MODULE_LICENSE("GPL");

static int    major_number;
char   message[MESSAGE_LENGTH];
char   *message_ptr;
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
   if (major_number < 0){
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
   memset(message, 0, sizeof(message[0]));
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

static void list_rsv_active_task(struct task_struct *task) {
   struct list_head *list;
   struct task_struct *child;
   list_for_each(list, &task->children) {
      child = list_entry(list, struct task_struct, sibling);
      if (child && child->is_rsv_valid) {
         sprintf(message, "%s%d\t%d\t%d\t%s\n", message, child->pid, child->tgid, child->rt_priority, child->comm);
      }
      list_rsv_active_task(child);
   }
}

static int dev_open(struct inode *inodep, struct file *filep){
   if (is_module_available) {
      printk(KERN_INFO "rsvdev: Device has been opened");
      is_module_available = false;
      sprintf(message, "pid\ttgid\tprio\tname\n");
      list_rsv_active_task(&init_task);
      message_ptr = message;
      return 0;
   }
   else {
      return -EBUSY;
   }
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   int count = 0;
   if (*message_ptr == 0) return 0;
   while (len && *message_ptr) {
      put_user(*(message_ptr++), buffer++);
      len--;
      count++;
   }
   return count;
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
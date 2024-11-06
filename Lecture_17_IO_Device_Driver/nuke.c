#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define MAX_DEV 2

static int dev_major = 0;
static struct class *lx_class = NULL;

static ssize_t lx_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t lx_write(struct file *, const char __user *, size_t, loff_t *);

static struct file_operations fops = {
  .owner = THIS_MODULE,
  .read = lx_read,
  .write = lx_write,
};

static struct nuke {
  struct cdev cdev;
} devs[MAX_DEV];

static int __init lx_init(void) {
  dev_t dev;
  int i;
  int ret;

  ret = alloc_chrdev_region(&dev, 0, MAX_DEV, "nuke");
  if (ret < 0) {
    printk(KERN_ALERT "nuke: Failed to allocate char device region\n");
    return ret;
  }

  dev_major = MAJOR(dev);

  lx_class = class_create("nuke");
  if (IS_ERR(lx_class)) {
    unregister_chrdev_region(MKDEV(dev_major, 0), MAX_DEV);
    printk(KERN_ALERT "nuke: Failed to create class\n");
    return PTR_ERR(lx_class);
  }

  for (i = 0; i < MAX_DEV; i++) {
    cdev_init(&devs[i].cdev, &fops);
    devs[i].cdev.owner = THIS_MODULE;
    ret = cdev_add(&devs[i].cdev, MKDEV(dev_major, i), 1);
    if (ret) {
      printk(KERN_ALERT "nuke: Failed to add cdev for device %d\n", i);
      while (--i >= 0) {
        cdev_del(&devs[i].cdev);
        device_destroy(lx_class, MKDEV(dev_major, i));
      }
      class_destroy(lx_class);
      unregister_chrdev_region(MKDEV(dev_major, 0), MAX_DEV);
      return ret;
    }
    device_create(lx_class, NULL, MKDEV(dev_major, i), NULL, "nuke%d", i);
  }
  return 0;    
}

static void __exit lx_exit(void) {
  int i;
  for (i = 0; i < MAX_DEV; i++) {
    device_destroy(lx_class, MKDEV(dev_major, i));
    cdev_del(&devs[i].cdev);
  }
  class_destroy(lx_class);
  unregister_chrdev_region(MKDEV(dev_major, 0), MAX_DEV);
}

static ssize_t lx_read(struct file *file, char __user *buf, size_t count, loff_t *offset) {
  if (*offset != 0) {
    return 0;
  } else {
    const char *data = "This is dangerous!\n";
    size_t datalen = strlen(data);
    if (count > datalen) {
      count = datalen;
    }
    if (copy_to_user(buf, data, count)) {
      return -EFAULT;
    }
    *offset += count;
    return count;
  }
}

static ssize_t lx_write(struct file *file, const char __user *buf, size_t count, loff_t *offset) {
  char databuf[16] = {0};
  size_t len;

  if (count > sizeof(databuf) - 1) {
    count = sizeof(databuf) - 1;
  }

  if (copy_from_user(databuf, buf, count)) {
    return -EFAULT;
  }

  databuf[count] = '\0';

  len = strlen(databuf);
  while (len > 0 && (databuf[len - 1] == '\n' || databuf[len - 1] == '\r')) {
    databuf[len - 1] = '\0';
    len--;
  }

  printk(KERN_INFO "nuke: received input: '%s', length=%zu\n", databuf, len);

  int cmp_result = strcmp(databuf, "COP4610");
  printk(KERN_INFO "nuke: strcmp result: %d\n", cmp_result);

  if (cmp_result == 0) {
    printk(KERN_INFO "nuke: correct password entered.\n");
    const char *EXPLODE[] = {
      "    ⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⣀⣀⠀⠀⣀⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
      "    ⠀⠀⠀⣀⣠⣤⣤⣾⣿⣿⣿⣿⣷⣾⣿⣿⣿⣿⣿⣶⣿⣿⣿⣶⣤⡀⠀⠀⠀⠀",
      "    ⠀⢠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀",
      "    ⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⡀⠀",
      "    ⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀",
      "    ⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢿⣿⣿⣿⣿⣿⣿⠿⠟⠁",
      "    ⠀⠀⠻⢿⡿⢿⣿⣿⣿⣿⠟⠛⠛⠋⣀⣀⠙⠻⠿⠿⠋⠻⢿⣿⣿⠟⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⠀⠀⠈⠉⣉⣠⣴⣷⣶⣿⣿⣿⣿⣶⣶⣶⣾⣶⠀⠀⠀⠀⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠋⠈⠛⠿⠟⠉⠻⠿⠋⠉⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣶⣷⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⠀⠀⢀⣀⣠⣤⣤⣤⣤⣶⣿⣿⣷⣦⣤⣤⣤⣤⣀⣀⠀⠀⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⢰⣿⠛⠉⠉⠁⠀⠀⠀⢸⣿⣿⣧⠀⠀⠀⠀⠉⠉⠙⢻⣷⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⠀⠙⠻⠷⠶⣶⣤⣤⣤⣿⣿⣿⣿⣦⣤⣤⣴⡶⠶⠟⠛⠁⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠒⠛⠛⠛⠛⠛⠛⠛⠛⠛⠓⠀⠀⠀⠀⠀⠀⠀⠀⠀",
    };
    int i;
    for (i = 0; i < sizeof(EXPLODE) / sizeof(EXPLODE[0]); i++) {
      printk(KERN_INFO "%s\n", EXPLODE[i]);
    }
  } else {
    printk(KERN_INFO "nuke: incorrect secret, cannot launch.\n");
  }
  return count;
}

module_init(lx_init);
module_exit(lx_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("jyy");

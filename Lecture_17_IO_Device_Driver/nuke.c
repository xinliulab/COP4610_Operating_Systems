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

  // 分配设备号范围
  ret = alloc_chrdev_region(&dev, 0, MAX_DEV, "nuke");
  if (ret < 0) {
    printk(KERN_ALERT "nuke: Failed to allocate char device region\n");
    return ret;
  }

  // 获取主设备号
  dev_major = MAJOR(dev);

  // 创建设备类
  lx_class = class_create(THIS_MODULE, "nuke");
  if (IS_ERR(lx_class)) {
    unregister_chrdev_region(MKDEV(dev_major, 0), MAX_DEV);
    printk(KERN_ALERT "nuke: Failed to create class\n");
    return PTR_ERR(lx_class);
  }

  for (i = 0; i < MAX_DEV; i++) {
    // 初始化 cdev
    cdev_init(&devs[i].cdev, &fops);
    devs[i].cdev.owner = THIS_MODULE;

    // 添加 cdev
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

    // 创建设备节点
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
  char databuf[4] = "\0\0\0\0";
  if (count > 4) {
    count = 4;
  }

  if (copy_from_user(databuf, buf, count)) {
    return -EFAULT;
  }
  if (strncmp(databuf, "\x01\x14\x05\x14", 4) == 0) {
    const char *EXPLODE[] = {
      "    ⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⣀⣀⠀⠀⣀⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
      "    ⠀⠀⠀⣀⣠⣤⣤⣾⣿⣿⣿⣿⣷⣾⣿⣿⣿⣿⣿⣶⣿⣿⣿⣶⣤⡀⠀⠀⠀⠀",
      "    ⠀⢠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀",
      "    ⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⡀⠀",
      "    ⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀",
      "    ⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠟⠁⠀",
      "    ⠀⠀⠻⢿⡿⢿⣿⣿⣿⣿⠟⠛⠛⠋⣀⣀⠙⠻⠿⠿⠋⠻⢿⣿⣿⠟⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⠀⠀⠈⠉⣉⣠⣴⣷⣶⣿⣿⣿⣿⣶⣶⣶⣾⣶⠀⠀⠀⠀⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠋⠈⠛⠿⠟⠉⠻⠿⠋⠉⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣶⣷⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⠀⠀⢀⣀⣠⣤⣤⣤⣤⣶⣿⣿⣷⣦⣤⣤⣤⣤⣀⣀⠀⠀⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⢰⣿⠛⠉⠉⠁⠀⠀⠀⢸⣿⣿⣧⠀⠀⠀⠀⠉⠉⠙⢻⣷⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⠀⠙⠻⠷⠶⣶⣤⣤⣤⣿⣿⣿⣿⣦⣤⣤⣴⡶⠶⠟⠛⠁⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
      "    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠒⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠓⠀⠀⠀⠀⠀⠀⠀⠀⠀",
    };
    int i;

    for (i = 0; i < sizeof(EXPLODE) / sizeof(EXPLODE[0]); i++) {
      printk(KERN_INFO "\033[01;31m%s\033[0m\n", EXPLODE[i]);
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

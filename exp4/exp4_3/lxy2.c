#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/slab.h> /*kmalloc*/

#define NAME "lxy2"
MODULE_LICENSE("GPL");

static struct device *mydemodrv_device;
static char *device_buffer;
static size_t pos; 
#define MAX_DEVICE_BUFFER_SIZE 64

static int demodrv_open(struct inode *inode, struct file *file)
{
	device_buffer = kmalloc(MAX_DEVICE_BUFFER_SIZE, GFP_KERNEL);
	pos=0;
	return 0;
}

static ssize_t
demodrv_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	if(pos < lbuf)
	{
		lbuf = pos;
	}
	if(copy_to_user(buf,device_buffer+pos-lbuf,lbuf))
	{
		return -EFAULT;
	}
	pos=pos-lbuf;
	printk("%s: 读出%ld字节，读出后指针位置为%ld\n", __func__, lbuf, pos);
	return lbuf;
}

static ssize_t
demodrv_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos)
{
	if(pos + count > 64)
	{
		count = 64-pos;
		if(count < 0)
			return count;
	}
	if(copy_from_user(device_buffer+pos,buf,count))
	{
		return -EFAULT;
	}
	pos=pos+count;
	printk("%s: 写入%ld字节，写完后缓冲区为%s\n", __func__, count, device_buffer);
	return count;
}

static const struct file_operations demodrv_fops = {
	.owner = THIS_MODULE,
	.open = demodrv_open,
	.read = demodrv_read,
	.write = demodrv_write
};

static struct miscdevice mydemodrv_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = NAME,
	.fops = &demodrv_fops,
};

static int __init code3_init(void)
{
	int ret;
	ret = misc_register(&mydemodrv_misc_device);
	if (ret) {
		printk("failed register code2 misc device\n");
		return ret;
	}
	mydemodrv_device = mydemodrv_misc_device.this_device;
	printk("succeeded register char device: %s\n", NAME);
	return 0;
}

static void __exit code3_exit(void)
{
	kfree(device_buffer);
	printk("removing device: %s\n", NAME);
	misc_deregister(&mydemodrv_misc_device);
}

module_init(code3_init);
module_exit(code3_exit);


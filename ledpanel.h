/*
 * ledpanel.h - definitions for XC3746 8x16 LED Panel
 */

#ifndef _LEDPANEL_H_
#define _LEDPANEL_H_

#define LEDPANEL_MODULE_LICENSE "GPL"
#define LEDPANEL_MODULE_AUTHOR "timulations"
#define LEDPANEL_MODULE_DESCRIPTION "Driver for XC3746 8x16 LED Panel"
#define LEDPANEL_MODULE_VERSION "0.1"

/* The entire board is represented as 16 bytes, where byte represents the entire column of 8. */
#define LEDPANEL_DATA_LINE_SIZE 16

/* GPIO configurations on a Raspberry Pi 4B */
#define LEDPANEL_DEFAULT_SDA 2
#define LEDPANEL_DEFAULT_SCL 3

/* Debugging macros */
#ifdef LEDPANEL_DEBUG
#define PDEBUG(fmt, args...) printk( KERN_DEBUG "ledpanel: " fmt, ## args)
#else
#define PDEBUG(fmt, args...) 
#endif

struct ledpanel_device {
	unsigned char data[LEDPANEL_DATA_LINE_SIZE];
	struct semaphore sem;
	struct cdev cdev;
};

extern int sda_gpio;
extern int scl_gpio;

int ledpanel_open(struct inode *inode, struct file *filp);
int ledpanel_release(struct inode *inode, struct file *filp);
ssize_t ledpanel_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t ledpanel_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
loff_t  ledpanel_llseek(struct file *filp, loff_t off, int whence);

#endif /* _LEDPANEL_H_ */
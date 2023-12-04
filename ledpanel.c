#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include "ledpanel.h"

#define LOW 0
#define HIGH 1

#define NANOSEC_DELAY 100

#define MINOR_NR 0 

/* GPIO numbers */
int sda_gpio = LEDPANEL_DEFAULT_SDA;
int scl_gpio = LEDPANEL_DEFAULT_SCL;

module_param(sda_gpio, int, S_IRUGO);
module_param(scl_gpio, int, S_IRUGO);

MODULE_LICENSE(LEDPANEL_MODULE_LICENSE);
MODULE_AUTHOR(LEDPANEL_MODULE_LICENSE);
MODULE_DESCRIPTION(LEDPANEL_MODULE_DESCRIPTION);
MODULE_VERSION(LEDPANEL_MODULE_VERSION);

struct ledpanel_device dev;
static int ledpanel_major;

#ifdef LEDPANEL_DEBUG

static void *ledpanel_seq_start(struct seq_file *s, loff_t *pos) {
    return NULL;
}

static void *ledpanel_seq_next(struct seq_file *s, void *v, loff_t *pos) {
    return NULL;
}

static void ledpanel_seq_stop(struct seq_file *s, void *v) {
    
}

static int ledpanel_seq_show(struct seq_file *s, void *v) {
    /* hexdump the current data buffer */
    size_t i;
    
    if (down_interruptible(&dev.sem)) return -ERESTARTSYS;

    seq_printf(s, "Currently contained data: \n");
    for (i = 0; i < LEDPANEL_DATA_LINE_SIZE; ++i) {
        seq_printf(s, "%02X ", dev.data[i]);
    }
    seq_printf(s, "\n");
    
    up(&dev.sem);
    return 0;
}

static struct seq_operations ledpanel_seq_ops = {
    .start = ledpanel_seq_start,
    .next  = ledpanel_seq_next,
    .stop  = ledpanel_seq_stop,
    .show  = ledpanel_seq_show
};
    
static int ledpanelseq_proc_open(struct inode *inode, struct file *file) {
    return seq_open(file, &ledpanel_seq_ops);
}

static struct proc_ops ledpanel_proc_ops = {
    .proc_open    = ledpanelseq_proc_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = seq_release
};

static void ledpanel_create_proc(void) {
    proc_create("ledpanelseq", 0, NULL, &ledpanel_proc_ops);
}

static void ledpanel_remove_proc(void) {
    remove_proc_entry("ledpanelseq", NULL);
}

#endif // LEDPANEL_DEBUG


/* I2C Bit-Banging */
static void i2c_start(void) {
    gpio_set_value(scl_gpio, LOW);
    ndelay(NANOSEC_DELAY);
    gpio_set_value(sda_gpio, HIGH);
    ndelay(NANOSEC_DELAY);
    gpio_set_value(scl_gpio, HIGH);
    ndelay(NANOSEC_DELAY);
    gpio_set_value(sda_gpio, LOW);
    ndelay(NANOSEC_DELAY);
}

static void i2c_sendbyte(unsigned char data) {
    size_t i;
    for (i = 0; i < 8; ++i) {
        gpio_set_value(scl_gpio, LOW);
        ndelay(NANOSEC_DELAY);
        if (data & 0x01) {
            gpio_set_value(sda_gpio, HIGH);
        } else {
            gpio_set_value(sda_gpio, LOW);
        }

        ndelay(NANOSEC_DELAY);
        gpio_set_value(scl_gpio, HIGH);
        ndelay(NANOSEC_DELAY);
        data = data >> 1;
    }
}

static void i2c_end(void) {
    gpio_set_value(scl_gpio, LOW);
    ndelay(NANOSEC_DELAY);
    gpio_set_value(sda_gpio, LOW);
    ndelay(NANOSEC_DELAY);
    gpio_set_value(scl_gpio, HIGH);
    ndelay(NANOSEC_DELAY);
    gpio_set_value(sda_gpio, HIGH);
    ndelay(NANOSEC_DELAY);
}

/* LED panel rendering
 * bitmap must point to LEDPANEL_DATA_LINE_SIZE elements */
static void set_screen(unsigned char *bitmap) {
    size_t i;

    /* set address plus 1 */
    i2c_start();
    i2c_sendbyte(0x40);
    i2c_end();

    /* send data */
    i2c_start();
    i2c_sendbyte(0xC0); /* set initial address to 0 */
    for (i = 0; i < LEDPANEL_DATA_LINE_SIZE; ++i) {
        i2c_sendbyte(bitmap[i]);
    }
    i2c_end();

    /* send brightness */
    i2c_start();
    i2c_sendbyte(0x8A);
    i2c_end();
}

static void clear_screen(void) {
    /* loop 10 times to completely clear screen */
    size_t i;
    unsigned char zeros[LEDPANEL_DATA_LINE_SIZE] = {0};
    
    for (i = 0; i < 10; ++i) {
        set_screen(&(zeros[0]));
    }
}

/* Open and close */
int ledpanel_open(struct inode *inode, struct file *filp) {
    /* nothing to be done. One global buffer that is allocated initially. */
    return 0;
}

int ledpanel_release(struct inode *inode, struct file *filp) {
    return 0;
}

ssize_t ledpanel_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
    /* disable direct reading. Should use seqfile */
    return 0;
}

ssize_t ledpanel_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) {
    size_t i;
    if (down_interruptible(&dev.sem)) return -ERESTARTSYS;
    
    if (count > LEDPANEL_DATA_LINE_SIZE) {
        PDEBUG("Received %lu bytes to write to device. Can only accept %d\n", (unsigned long)count, LEDPANEL_DATA_LINE_SIZE);
        return -EINVAL;
    }

    if (copy_from_user(dev.data, buf, count)) {
        return -EFAULT;
    }
    memset(dev.data + count, 0, LEDPANEL_DATA_LINE_SIZE - count);

    set_screen(dev.data);
    up(&dev.sem);

    return count;
}

struct file_operations ledpanel_fops = {
    .owner   = THIS_MODULE,
    .write   = ledpanel_write,
    .read    = ledpanel_read,
    .open    = ledpanel_open,
    .release = ledpanel_release
};


static int ledpanel_setup_cdev(void) {
    int err, devno;

    devno = MKDEV(ledpanel_major, MINOR_NR);
    cdev_init(&dev.cdev, &ledpanel_fops);
    dev.cdev.owner = THIS_MODULE;
    err = cdev_add(&dev.cdev, devno, 1);
    
    if (err) {
        PDEBUG("Error: %d\n", err);
        return err;
    } else {
        return 0;
    }
}

static int __init ledpanel_init_module(void) {
    int result, i;
    dev_t devno = 0;

    /* setup GPIO pins before allocating character device */
    if (!gpio_is_valid(sda_gpio) || !gpio_is_valid(scl_gpio)) {
        printk(KERN_INFO "Invalid GPIO\n");
        return -ENODEV;
    }

    gpio_request(sda_gpio, "sysfs");
    gpio_direction_output(sda_gpio, LOW);
    gpio_export(sda_gpio, false);

    gpio_request(scl_gpio, "sysfs");
    gpio_direction_output(scl_gpio, LOW);
    gpio_export(scl_gpio, false);

    clear_screen();

    /* allocate character device */
    result = alloc_chrdev_region(&devno, MINOR_NR, 1, "ledpanel");
    ledpanel_major = MAJOR(devno);

    if (result < 0) {
        PDEBUG("Couldn't get major number\n");
        return result;
    }

    sema_init(&dev.sem, 1);

    result = ledpanel_setup_cdev();
    if (result < 0) {
        unregister_chrdev_region(devno, 1);
        return result;
    }

#ifdef LEDPANEL_DEBUG
    ledpanel_create_proc();
#endif

    PDEBUG("GPIO LED panel module successfully loaded\n");
    return 0;
}

static void __exit ledpanel_exit_module(void) {
    dev_t devno = MKDEV(ledpanel_major, MINOR_NR);
    cdev_del(&dev.cdev);
    unregister_chrdev_region(devno, 1);
    clear_screen();

    gpio_set_value(sda_gpio, LOW);
    gpio_unexport(sda_gpio);
    gpio_free(sda_gpio);

    gpio_set_value(scl_gpio, LOW);
    gpio_unexport(scl_gpio);
    gpio_free(scl_gpio);

    PDEBUG("GPIO LED panel module unloaded\n");
}

module_init(ledpanel_init_module);
module_exit(ledpanel_exit_module);

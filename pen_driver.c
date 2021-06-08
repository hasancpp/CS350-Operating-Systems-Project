#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/kernel.h>
MODULE_LICENSE("Dual BSD/GPL");

static struct usb_driver pen_driver = {
        .name        = "Lexar Media JumpDrive 16GB",
        .probe       = pen_driver_probe,
        .disconnect  = pen_driver_disconnect,
        .id_table    = pen_table,
};

static int __init pen_driver_init(void)
{
        int ret;

        ret = usb_register(&pen_driver);
        if (ret < 0) {
                printk(KERN_ALERT "USB registration failed!");
                return -1;
        }
		printk(KERN_ALERT "USB registration succesful");
        return 0;
}

static void __exit pen_driver_exit(void)
{
	printk(KERN_ALERT "USB module removed from the kernel");	
	usb_deregister(&pen_driver);
}

module_init(pen_driver_init);
module_exit(pen_driver_exit);

static struct usb_device_id pen_table [] = {
        { USB_DEVICE(0x05dc, 0xa81d) },
        { }
};

MODULE_DEVICE_TABLE (usb, pen_table);

static int pen_driver_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	printk(KERN_ALERT "hell yeah");
    return 0;
}

static void pen_driver_disconnect(struct usb_interface *interface)
{
	printk(KERN_ALERT "nice job");
}




#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/kernel.h>
MODULE_LICENSE("Dual BSD/GPL");

static struct usb_driver my_usb_driver = {
        .name        = "my usb driver",
        .probe       = my_usb_probe,
        .disconnect  = my_usb_disconnect,
        .id_table    = my_usb_table,
};

static int __init my_usb_init(void)
{
        int result;

        result = usb_register(&my_usb_driver);
        if (result < 0) {
                err("usb_register failed for the "__FILE__ "driver."
                    "Error number %d", result);
                return -1;
        }
	printk(KERN_ALERT "Hello World! from CS350 machine");
        return 0;
}

static void __exit my_usb_exit(void)
{
	printk(KERN_ALERT "Bye Bye module- CS350 Hw");	
	usb_deregister(&my_usb_driver);
}

module_init(my_usb_init);
module_exit(my_usb_exit);

static struct usb_device_id my_usb_table [] = {
        { USB_DEVICE(0x05dc, 0xa81d) },
        { }
};

MODULE_DEVICE_TABLE (usb, my_usb_table);

static int my_usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    return 0;
}



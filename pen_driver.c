#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/kernel.h>

MODULE_LICENSE("Dual BSD/GPL");

static struct usb_class_driver pen_class;
static struct usb_driver pen_driver;
static int __init pen_driver_init(void)
{
        usb_register(&pen_driver);
		printk(KERN_ALERT "USB driver module is successfully loaded into kernel. Waiting for the USB to be plugged in...");
        return 0;
}

static void __exit pen_driver_exit(void)
{
	printk(KERN_ALERT "USB driver module is removed from the kernel!");	
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
	struct usb_host_interface *interface_desc;
	struct usb_endpoint_descriptor *endpoint;
	int ret;
	
	interface_desc = interface->cur_altsetting;
	
	printk(KERN_INFO "USB info %d now probed: %04X, %04X\n", interface_desc->desc.bInterfaceNumber, id->idVendor, id->idProduct);
	printk(KERN_INFO "ID -> bNumEndpoints: %02X\n", interface_desc->desc.bNumEndpoints);
	printk(KERN_INFO "ID -> bInterfaceClass: %02X\n", interface_desc->desc.bInterfaceClass);

	ret = usb_register_dev(interface, &pen_class);
	if (ret)
	{
		printk(KERN_INFO "Not able to get the minor number");
		return ret;
	}
	else
	{
		printk(KERN_INFO "Minor number = %d\n", interface->minor);
	}

    return 0;
}

static void pen_driver_disconnect(struct usb_interface *interface)
{
	printk(KERN_INFO "Disconnected and release the minor number %d\n", interface->minor);
	usb_deregister_dev(interface, &pen_class);
}

static struct usb_driver pen_driver = {
        .name        = "Lexar Media JumpDrive 16GB",
        .probe       = pen_driver_probe,
        .disconnect  = pen_driver_disconnect,
        .id_table    = pen_table,
};
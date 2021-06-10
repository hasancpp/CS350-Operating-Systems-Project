#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/kernel.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 4096
#define OUTPUT_MODE 0700

MODULE_LICENSE("Dual BSD/GPL");

int i;
char buf[BUF_SIZE];
size_t nbytes;
time_t start;

static void create_write(int arr[]) {
    int wt_count;

    int create_fd = creat("data.txt", OUTPUT_MODE);
    if (create_fd < 0) printk(KERN_ALERT "Data file could not be created\n");

    i = 0;
    while (i < 3) {
        snprintf(buf, BUF_SIZE, "%d\n", arr[i]);
        nbytes = strlen(buf);

        wt_count = write(create_fd, buf, nbytes);
        if (wt_count <= 0) printk(KERN_ALERT "Could not write into the data file\n");

        bzero(buf, BUF_SIZE);
        ++i;
    }

    close(create_fd);
}

static struct usb_driver pen_driver;

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
	int rd_count, num;
    FILE* read_fd;
    char* line;
    size_t len = 0;

    if (access("data.txt", F_OK) == 0) {
        read_fd = fopen("data.txt", "r");

        if (read_fd < 0) printk("An error occurred while accessing the data file\n");

        int data[3];

        if (getline(&line, &len, read_fd) == -1) printk(KERN_ALERT "Could not read from the data file\n");
        num = atoi(line);
        data[0] = ++num;

        if (getline(&line, &len, read_fd) == -1) printk(KERN_ALERT "Could not read from the data file\n");
        num = atoi(line);
        data[1] = num;
        
        if (getline(&line, &len, read_fd) == -1) printk(KERN_ALERT "Could not read from the data file\n");
        num = atoi(line);
        data[2] = num;

        fclose(read_fd);

        if (remove("data.txt") > 0) printk(KERN_ALERT "The data file couldn't be updated\n");

        create_write(data);
    } else {
        int data[3] = {1, 0, 0};
        create_write(data);
    }
	
	printk(KERN_ALERT "Usb plugged in");
	
	start = time(NULL);
	
    return 0;
}

static void pen_driver_disconnect(struct usb_interface *interface)
{
	read_fd = fopen("data.txt", "r");

    if (read_fd < 0) printk("An error occurred while accessing the data file\n");

    int data[3];

    if (getline(&line, &len, read_fd) == -1) printk(KERN_ALERT "Could not read from the data file\n");
    num = atoi(line);
    data[0] = num;

    if (getline(&line, &len, read_fd) == -1) printk(KERN_ALERT "Could not read from the data file\n");
    num = atoi(line);
    data[1] = ++num;
        
    if (getline(&line, &len, read_fd) == -1) printk(KERN_ALERT "Could not read from the data file\n");
    num = atoi(line);
    data[2] = num + (time(NULL) - start);

    fclose(read_fd);

    if (remove("data.txt") > 0) printk(KERN_ALERT "The data file couldn't be updated\n");

    create_write(data);
	
	printk(KERN_ALERT "Usb plugged out");
}

static struct usb_driver pen_driver = {
        .name        = "Lexar Media JumpDrive 16GB",
        .probe       = pen_driver_probe,
        .disconnect  = pen_driver_disconnect,
        .id_table    = pen_table,
};
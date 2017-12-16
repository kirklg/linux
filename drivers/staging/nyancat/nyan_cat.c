/*
 * Streaming /dev/cat driver
 *
 * Author: Kirk Leon Guerrero <kirk.leonguerrero@gmail.com>
 * Copyright (C) 2017 Kirk Leon Guerrero
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */


#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define  DEVICE_NAME "cat"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kirk Leon Guerrero <kirk.leonguerrero@gmail.com>");
MODULE_DESCRIPTION("Nyan Cat /dev/cat streaming driver");
MODULE_VERSION("0.1");

static int major_number;
// Ugly ass cat!
static char message[] = "          \n          \n   ,---/V\\\n  ~|__(o.o)\n   U U U U\n          \n          \n   ,---/V\\\n  ~|__(o.o)\n    UU  UU\n";
static int number_opens = 0;

static int nyan_cat_open(struct inode *, struct file *);
static int nyan_cat_release(struct inode *, struct file *);
static ssize_t nyan_cat_read(struct file *, char *, size_t, loff_t *);

static struct file_operations fops =
{
	.open = nyan_cat_open,
	.read = nyan_cat_read,
	.release = nyan_cat_release,
};

static int __init nyan_cat_init(void)
{
	printk(KERN_INFO "nyan_cat: Initializing\n");
	major_number = register_chrdev(0, DEVICE_NAME, &fops);

	if (major_number < 0) {
		printk(KERN_ALERT "nyan_cat: Failed to register a major number\n");
		return major_number;
	}

	printk(KERN_INFO "nyan_cat: Registered correctly with major number %d\n", major_number);
	return 0;
}

static void __exit nyan_cat_exit(void)
{
   unregister_chrdev(major_number, DEVICE_NAME);
   printk(KERN_INFO "nyan_cat: Exitting\n");
}

static int nyan_cat_open(struct inode *inodep, struct file *filep)
{
   number_opens++;
   printk(KERN_INFO "nyan_cat: Device has been opened %d time(s)\n", number_opens);
   return 0;
}

static ssize_t nyan_cat_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
   int error_count = 0;
   error_count = copy_to_user(buffer, message, strlen(message));

   if (error_count == 0) {
      printk(KERN_INFO "nyan_cat: Sent %u characters to the user\n", strlen(message));
      return strlen(message);
   } else {
      printk(KERN_INFO "nyan_cat: Failed to send %d characters to the user\n", error_count);
      return -EFAULT;
   }
}

static int nyan_cat_release(struct inode *inodep, struct file *filep)
{
   printk(KERN_INFO "nyan_cat: Device successfully closed\n");
   return 0;
}

module_init(cat_init);
module_exit(cat_exit);

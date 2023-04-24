/* SPDX-License-Identifier: GPL-2.0 or MIT                               */
/* Copyright(c) 2021 Ross K.Snider. All rights reserved.                 */
/*-------------------------------------------------------------------------
 * Description:  Linux Platform Device Driver for the 
 *               compFilterProcessor component
 * ------------------------------------------------------------------------
 * Authors : Ross K. Snider and Trevor Vannoy
 * Company : Montana State University
 * Create Date : November 10, 2021
 * Revision : 1.0
 * License : GPL-2.0 or MIT (opensource.org / licenses / MIT, GPL-2.0)
-------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/mutex.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
/*#include "fp_conversions.h"*/

/*-----------------------------------------------------------------------*/
/* DEFINE STATEMENTS                                                     */
/*-----------------------------------------------------------------------*/
/* Define the Component Register Offsets*/
#define REG0_delaym_OFFSET 0x0
/* #define REG1  */
#define REG1_b0_OFFSET 0x04

#define REG2_bm_OFFSET 0x08
/* #define REG3  */
#define REG3_wetdrymix_OFFSET 0x0C

/* Memory span of all registers (used or not) in the                     */
/* component compFilterProcessor                                            */
#define SPAN 0x10


/*-----------------------------------------------------------------------*/
/* compFilterProcessor device structure                                     */
/*-----------------------------------------------------------------------*/
/*
 * struct  compFilterProcessor_dev - Private compFilterProcessor device struct.
 * @miscdev: miscdevice used to create a char device 
 *           for the compFilterProcessor component
 * @base_addr: Base address of the compFilterProcessor component
 * @lock: mutex used to prevent concurrent writes 
 *        to the hps_led_pacompFilterProcessorterns component
 *
 * An compFilterProcessor_dev struct gets created for each compFilterProcessor 
 * component in the system.
 */
struct compFilterProcessor_dev {
	struct miscdevice miscdev;
	void __iomem *base_addr;
	struct mutex lock;
};



/*-----------------------------------------------------------------------*/
/* sysfs Attributes                                                      */
/*-----------------------------------------------------------------------*/
// Define sysfs attributes
static DEVICE_ATTR_RW(delaym);    // Attribute for REG0
static DEVICE_ATTR_RW(b0);    // Attribute for REG0
static DEVICE_ATTR_RW(bm);    // Attribute for REG0
static DEVICE_ATTR_RW(wetdrymix);    // Attribute for REG0
/* TODO: Add the attributes for REG1 and REG2 using register names       */


// Create an atribute group so the device core can 
// export the attributes for us.
static struct attribute *compFilterProcessor_attrs[] = {
	&dev_attr_delaym.attr,
	&dev_attr_b0.attr,
	&dev_attr_bm.attr,
	&dev_attr_wetdrymix.attr,
/* TODO: Add the attribute entries for REG1 and REG2 using register names*/


	NULL,
};
ATTRIBUTE_GROUPS(compFilterProcessor);


/*-----------------------------------------------------------------------*/
/* File Operations read()                                                */
/*-----------------------------------------------------------------------*/
/*
 * hps_led_patterns_read() - Read method for the hps_led_patterns char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value into.
 * @count: The number of bytes being requested.
 * @offset: The byte offset in the file being read from.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */
static ssize_t compFilterProcessor_read(struct file *file, char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * hps_led_patterns_dev struct. container_of returns the 
     * hps_led_patterns_dev struct that contains the miscdev in private_data.
	 */
	struct compFilterProcessor_dev *priv = container_of(file->private_data,
	                            struct compFilterProcessor_dev, miscdev);

	// Check file offset to make sure we are reading to a valid location.
	if (pos < 0) {
		// We can't read from a negative file position.
		return -EINVAL;
	}
	if (pos >= SPAN) {
		// We can't read from a position past the end of our device.
		return 0;
	}
	if ((pos % 0x4) != 0) {
		/*
		 * Prevent unaligned access. Even though the hardware
		 * technically supports unaligned access, we want to
		 * ensure that we only access 32-bit-aligned addresses
		 * because our registers are 32-bit-aligned.
		 */
		pr_warn("compFilterProcessor_read: unaligned access\n");
		return -EFAULT;
	}

	// If the user didn't request any bytes, don't return any bytes :)
	if (count == 0) {
		return 0;
	}

	// Read the value at offset pos.
	val = ioread32(priv->base_addr + pos);

	ret = copy_to_user(buf, &val, sizeof(val));
	if (ret == sizeof(val)) {
		// Nothing was copied to the user.
		pr_warn("compFilterProcessor_read: nothing copied\n");
		return -EFAULT;
	}

	// Increment the file offset by the number of bytes we read.
	*offset = pos + sizeof(val);

	return sizeof(val);
}
/*-----------------------------------------------------------------------*/
/* File Operations write()                                               */
/*-----------------------------------------------------------------------*/
/*
 * hps_led_patterns_write() - Write method for the hps_led_patterns char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value from.
 * @count: The number of bytes being written.
 * @offset: The byte offset in the file being written to.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */
static ssize_t compFilterProcessor_write(struct file *file, const char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * hps_led_patterns_dev struct. container_of returns the 
     * hps_led_patterns_dev struct that contains the miscdev in private_data.
	 */
	struct compFilterProcessor_dev *priv = container_of(file->private_data,
	                              struct compFilterProcessor_dev, miscdev);

	// Check file offset to make sure we are writing to a valid location.
	if (pos < 0) {
		// We can't write to a negative file position.
		return -EINVAL;
	}
	if (pos >= SPAN) {
		// We can't write to a position past the end of our device.
		return 0;
	}
	if ((pos % 0x4) != 0) {
		/*
		 * Prevent unaligned access. Even though the hardware
		 * technically supports unaligned access, we want to
		 * ensure that we only access 32-bit-aligned addresses
		 * because our registers are 32-bit-aligned.
		 */
		pr_warn("compFilterProcessor_write: unaligned access\n");
		return -EFAULT;
	}

	// If the user didn't request to write anything, return 0.
	if (count == 0) {
		return 0;
	}

	mutex_lock(&priv->lock);

	ret = copy_from_user(&val, buf, sizeof(val));
	if (ret == sizeof(val)) {
		// Nothing was copied from the user.
		pr_warn("compFilterProcessor_write: nothing copied from user space\n");
		ret = -EFAULT;
		goto unlock;
	}

	// Write the value we were given at the address offset given by pos.
	iowrite32(val, priv->base_addr + pos);

	// Increment the file offset by the number of bytes we wrote.
	*offset = pos + sizeof(val);

	// Return the number of bytes we wrote.
	ret = sizeof(val);

unlock:
	mutex_unlock(&priv->lock);
	return ret;
}


/*-----------------------------------------------------------------------*/
/* File Operations Supported                                             */
/*-----------------------------------------------------------------------*/
/*
 *  hps_led_patterns_fops - File operations supported by the  
 *                          hps_led_patterns driver
 * @owner: The hps_led_patterns driver owns the file operations; this 
 *         ensures that the driver can't be removed while the 
 *         character device is still in use.
 * @read: The read function.
 * @write: The write function.
 * @llseek: We use the kernel's default_llseek() function; this allows 
 *          users to change what position they are writing/reading to/from.
 */
static const struct file_operations  compFilterProcessor_fops = {
	.owner = THIS_MODULE,
	.read = compFilterProcessor_read,
	.write = compFilterProcessor_write,
	.llseek = default_llseek,
};


/*-----------------------------------------------------------------------*/
/* Platform Driver Probe (Initialization) Function                       */
/*-----------------------------------------------------------------------*/
/*
 * hps_led_patterns_probe() - Initialize device when a match is found
 * @pdev: Platform device structure associated with our 
 *        hps_led_patterns device; pdev is automatically created by the 
 *        driver core based upon our hps_led_patterns device tree node.
 *
 * When a device that is compatible with this hps_led_patterns driver 
 * is found, the driver's probe function is called. This probe function 
 * gets called by the kernel when an hps_led_patterns device is found 
 * in the device tree.
 */
static int compFilterProcessor_probe(struct platform_device *pdev)
{
	struct compFilterProcessor_dev *priv;
	int ret;

	/*
	 * Allocate kernel memory for the hps_led_patterns device and set it to 0.
	 * GFP_KERNEL specifies that we are allocating normal kernel RAM;
	 * see the kmalloc documentation for more info. The allocated memory
	 * is automatically freed when the device is removed.
	 */
	priv = devm_kzalloc(&pdev->dev, sizeof(struct compFilterProcessor_dev), GFP_KERNEL);
	if (!priv) {
		pr_err("Failed to allocate kernel memory for hps_led_pattern\n");
		return -ENOMEM;
	}

	/*
	 * Request and remap the device's memory region. Requesting the region
	 * make sure nobody else can use that memory. The memory is remapped
	 * into the kernel's virtual address space becuase we don't have access
	 * to physical memory locations.
	 */
	priv->base_addr = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(priv->base_addr)) {
		pr_err("Failed to request/remap platform device resource (compFilterProcessor)\n");
		return PTR_ERR(priv->base_addr);
	}

	// Initialize the misc device parameters
	priv->miscdev.minor = MISC_DYNAMIC_MINOR;
	priv->miscdev.name = "compFilterProcessor";
	priv->miscdev.fops = &compFilterProcessor_fops;
	priv->miscdev.parent = &pdev->dev;
	priv->miscdev.groups = compFilterProcessor_groups;

	// Register the misc device; this creates a char dev at 
    // /dev/hps_led_patterns
	ret = misc_register(&priv->miscdev);
	if (ret) {
		pr_err("Failed to register misc device for compFilterProcessor\n");
		return ret;
	}

	// Attach the hps_led_patterns' private data to the 
    // platform device's struct.
	platform_set_drvdata(pdev, priv);

	pr_info("compFilterProcessor_probe successful\n");

	return 0;
}

/*-----------------------------------------------------------------------*/
/* Platform Driver Remove Function                                       */
/*-----------------------------------------------------------------------*/
/*
 * hps_led_patterns_remove() - Remove an hps_led_patterns device.
 * @pdev: Platform device structure associated with our hps_led_patterns device.
 *
 * This function is called when an hps_led_patterns devicee is removed or
 * the driver is removed.
 */
static int compFilterProcessor_remove(struct platform_device *pdev)
{
	// Get thehps_led_patterns' private data from the platform device.
	struct compFilterProcessor_dev *priv = platform_get_drvdata(pdev);

	// Deregister the misc device and remove the /dev/hps_led_patterns file.
	misc_deregister(&priv->miscdev);

	pr_info("compFilterProcessor_remove successful\n");

	return 0;
}

/*-----------------------------------------------------------------------*/
/* Compatible Match String                                               */
/*-----------------------------------------------------------------------*/
/*
 * Define the compatible property used for matching devices to this driver,
 * then add our device id structure to the kernel's device table. For a 
 * device to be matched with this driver, its device tree node must use the 
 * same compatible string as defined here.
 */
static const struct of_device_id compFilterProcessor_of_match[] = {
    // ****Note:**** This .compatible string must be identical to the 
    // .compatible string in the Device Tree Node for hps_led_patterns
	{ .compatible = "SQ,compFilterProcessor", },  
	{ }
};
MODULE_DEVICE_TABLE(of, compFilterProcessor_of_match);

/*-----------------------------------------------------------------------*/
/* Platform Driver Structure                                             */
/*-----------------------------------------------------------------------*/
/*
 * struct hps_led_patterns_driver - Platform driver struct for the 
 *                                  hps_led_patterns driver
 * @probe: Function that's called when a device is found
 * @remove: Function that's called when a device is removed
 * @driver.owner: Which module owns this driver
 * @driver.name: Name of the hps_led_patterns driver
 * @driver.of_match_table: Device tree match table
 * @driver.dev_groups: hps_led_patterns sysfs attribute group; this
 *                     allows the driver core to create the
 *                     attribute(s) without race conditions.
 */
static struct platform_driver compFilterProcessor_driver = {
	.probe = compFilterProcessor_probe,
	.remove = compFilterProcessor_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "compFilterProcessor",
		.of_match_table = compFilterProcessor_of_match,
		.dev_groups = compFilterProcessor_groups,
	},
};

/*
 * We don't need to do anything special in module init/exit.
 * This macro automatically handles module init/exit.
 */
module_platform_driver(compFilterProcessor_driver);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Soos Qasem");  // Adapted from Trevor Vannoy's Echo Driver
MODULE_DESCRIPTION("compFilterProcessor driver");
MODULE_VERSION("1.0");
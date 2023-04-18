/* SPDX-License-Identifier: GPL-2.0 or MIT                               */
/* Copyright(c) 2021 Ross K.Snider. All rights reserved.                 */
/*-------------------------------------------------------------------------
 * Description:  Linux Platform Device Driver for the
 *               fftAnalysisSynthesisProcessor component
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
#define REG0_passthrough_OFFSET 0x00
#define REG1_filterselect_OFFSET 0x04

/* Memory span of all registers (used or not) in the                     */
/* component fftAnalysisSynthesisProcessor                                            */
#define SPAN 0x08


/*-----------------------------------------------------------------------*/
/* fftAnalysisSynthesisProcessor device structure                                     */
/*-----------------------------------------------------------------------*/
/*
 * struct  fftAnalysisSynthesisProcessor_dev - Private fftAnalysisSynthesisProcessor device struct.
 * @miscdev: miscdevice used to create a char device
 *           for the fftAnalysisSynthesisProcessor component
 * @base_addr: Base address of the fftAnalysisSynthesisProcessor component
 * @lock: mutex used to prevent concurrent writes
 *        to the fftAnalysisSynthesisProcessor component
 *
 * An fftAnalysisSynthesisProcessor_dev struct gets created for each fftAnalysisSynthesisProcessor
 * component in the system.
 */
struct fftAnalysisSynthesisProcessor_dev {
	struct miscdevice miscdev;
	void __iomem *base_addr;
	struct mutex lock;
};

/*-----------------------------------------------------------------------*/
/* REG0: passthrough register read function show()                   */
/*-----------------------------------------------------------------------*/
/*
 * passthrough_show() - Return the passthrough value
 *                          to user-space via sysfs.
 * @dev: Device structure for the fftAnalysisSynthesisProcessor component. This
 *       device struct is embedded in the fftAnalysisSynthesisProcessor' device struct.
 * @attr: Unused.
 * @buf: Buffer that gets returned to user-space.
 *
 * Return: The number of bytes read.
 */
static ssize_t passthrough_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	bool passthrough;

	// Get the private fftAnalysisSynthesisProcessor data out of the dev struct
	struct fftAnalysisSynthesisProcessor_dev *priv = dev_get_drvdata(dev);

	passthrough = ioread32(priv->base_addr + REG0_passthrough_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", passthrough);
}

/*-----------------------------------------------------------------------*/
/* REG0: passthrough register write function store()                 */
/*-----------------------------------------------------------------------*/
/**
 * passthrough_store() - Store the passthrough value.
 * @dev: Device structure for the fftAnalysisSynthesisProcessor component. This
 *       device struct is embedded in the fftAnalysisSynthesisProcessor'
 *       platform device struct.
 * @attr: Unused.
 * @buf: Buffer that contains the passthrough value being written.
 * @size: The number of bytes being written.
 *
 * Return: The number of bytes stored.
 */
static ssize_t passthrough_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	bool passthrough;
	int ret;
	struct fftAnalysisSynthesisProcessor_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a bool
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtobool(buf, &passthrough);
	if (ret < 0) {
		// kstrtobool returned an error
		return ret;
	}

	iowrite32(passthrough, priv->base_addr + REG0_passthrough_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

/*-----------------------------------------------------------------------*/
/* TODO: Add show() and store() functions for                            */
/* Registers REG1 (filterselect) and REG2 (bm)                    */
/* in component fftAnalysisSynthesisProcessor                                         */
/*-----------------------------------------------------------------------*/
/* Add here...                                                           */
static ssize_t filterselect_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u8 filterselect;
	struct fftAnalysisSynthesisProcessor_dev *priv = dev_get_drvdata(dev);

	filterselect = ioread32(priv->base_addr + REG1_filterselect_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", filterselect);
}

static ssize_t filterselect_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u8 filterselect;
	int ret;
	struct fftAnalysisSynthesisProcessor_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtou8(buf, 0, &filterselect);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(filterselect, priv->base_addr + REG1_filterselect_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

/*-----------------------------------------------------------------------*/
/* sysfs Attributes                                                      */
/*-----------------------------------------------------------------------*/
// Define sysfs attributes
static DEVICE_ATTR_RW(passthrough);    // Attribute for REG0
/* TODO: Add the attributes for REG1 and REG2 using register names       */
static DEVICE_ATTR_RW(filterselect);		// Attribute for REG1

// Create an atribute group so the device core can
// export the attributes for us.
static struct attribute *fftAnalysisSynthesisProcessor_attrs[] = {
	&dev_attr_passthrough.attr,
/* TODO: Add the attribute entries for REG1 and REG2 using register names*/
	&dev_attr_filterselect.attr,
	NULL,
};
ATTRIBUTE_GROUPS(fftAnalysisSynthesisProcessor);

/*-----------------------------------------------------------------------*/
/* File Operations read()                                                */
/*-----------------------------------------------------------------------*/
/*
 * fftAnalysisSynthesisProcessor_read() - Read method for the fftAnalysisSynthesisProcessor char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value into.
 * @count: The number of bytes being requested.
 * @offset: The byte offset in the file being read from.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */
static ssize_t fftAnalysisSynthesisProcessor_read(struct file *file, char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * fftAnalysisSynthesisProcessor_dev struct. container_of returns the
     * fftAnalysisSynthesisProcessor_dev struct that contains the miscdev in private_data.
	 */
	struct fftAnalysisSynthesisProcessor_dev *priv = container_of(file->private_data,
	                            struct fftAnalysisSynthesisProcessor_dev, miscdev);

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
		pr_warn("fftAnalysisSynthesisProcessor_read: unaligned access\n");
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
		pr_warn("fftAnalysisSynthesisProcessor_read: nothing copied\n");
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
 * fftAnalysisSynthesisProcessor_write() - Write method for the fftAnalysisSynthesisProcessor char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value from.
 * @count: The number of bytes being written.
 * @offset: The byte offset in the file being written to.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */
static ssize_t fftAnalysisSynthesisProcessor_write(struct file *file, const char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * fftAnalysisSynthesisProcessor_dev struct. container_of returns the
     * fftAnalysisSynthesisProcessor_dev struct that contains the miscdev in private_data.
	 */
	struct fftAnalysisSynthesisProcessor_dev *priv = container_of(file->private_data,
	                              struct fftAnalysisSynthesisProcessor_dev, miscdev);

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
		pr_warn("fftAnalysisSynthesisProcessor_write: unaligned access\n");
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
		pr_warn("fftAnalysisSynthesisProcessor_write: nothing copied from user space\n");
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
 *  fftAnalysisSynthesisProcessor_fops - File operations supported by the
 *                          fftAnalysisSynthesisProcessor driver
 * @owner: The fftAnalysisSynthesisProcessor driver owns the file operations; this
 *         ensures that the driver can't be removed while the
 *         character device is still in use.
 * @read: The read function.
 * @write: The write function.
 * @llseek: We use the kernel's default_llseek() function; this allows
 *          users to change what position they are writing/reading to/from.
 */
static const struct file_operations  fftAnalysisSynthesisProcessor_fops = {
	.owner = THIS_MODULE,
	.read = fftAnalysisSynthesisProcessor_read,
	.write = fftAnalysisSynthesisProcessor_write,
	.llseek = default_llseek,
};

/*-----------------------------------------------------------------------*/
/* Platform Driver Probe (Initialization) Function                       */
/*-----------------------------------------------------------------------*/
/*
 * fftAnalysisSynthesisProcessor_probe() - Initialize device when a match is found
 * @pdev: Platform device structure associated with our
 *        fftAnalysisSynthesisProcessor device; pdev is automatically created by the
 *        driver core based upon our fftAnalysisSynthesisProcessor device tree node.
 *
 * When a device that is compatible with this fftAnalysisSynthesisProcessor driver
 * is found, the driver's probe function is called. This probe function
 * gets called by the kernel when an fftAnalysisSynthesisProcessor device is found
 * in the device tree.
 */
static int fftAnalysisSynthesisProcessor_probe(struct platform_device *pdev)
{
	struct fftAnalysisSynthesisProcessor_dev *priv;
	int ret;

	/*
	 * Allocate kernel memory for the fftAnalysisSynthesisProcessor device and set it to 0.
	 * GFP_KERNEL specifies that we are allocating normal kernel RAM;
	 * see the kmalloc documentation for more info. The allocated memory
	 * is automatically freed when the device is removed.
	 */
	priv = devm_kzalloc(&pdev->dev, sizeof(struct fftAnalysisSynthesisProcessor_dev), GFP_KERNEL);
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
		pr_err("Failed to request/remap platform device resource (fftAnalysisSynthesisProcessor)\n");
		return PTR_ERR(priv->base_addr);
	}

	// Initialize the misc device parameters
	priv->miscdev.minor = MISC_DYNAMIC_MINOR;
	priv->miscdev.name = "fftAnalysisSynthesisProcessor";
	priv->miscdev.fops = &fftAnalysisSynthesisProcessor_fops;
	priv->miscdev.parent = &pdev->dev;
	priv->miscdev.groups = fftAnalysisSynthesisProcessor_groups;

	// Register the misc device; this creates a char dev at
    // /dev/fftAnalysisSynthesisProcessor
	ret = misc_register(&priv->miscdev);
	if (ret) {
		pr_err("Failed to register misc device for fftAnalysisSynthesisProcessor\n");
		return ret;
	}

		// Attach the fftAnalysisSynthesisProcessor' private data to the
    // platform device's struct.
	platform_set_drvdata(pdev, priv);

	pr_info("fftAnalysisSynthesisProcessor_probe successful\n");

	return 0;
}

/*-----------------------------------------------------------------------*/
/* Platform Driver Remove Function                                       */
/*-----------------------------------------------------------------------*/
/*
 * fftAnalysisSynthesisProcessor_remove() - Remove an fftAnalysisSynthesisProcessor device.
 * @pdev: Platform device structure associated with our fftAnalysisSynthesisProcessor device.
 *
 * This function is called when an fftAnalysisSynthesisProcessor devicee is removed or
 * the driver is removed.
 */
static int fftAnalysisSynthesisProcessor_remove(struct platform_device *pdev)
{
	// Get thefftAnalysisSynthesisProcessor' private data from the platform device.
	struct fftAnalysisSynthesisProcessor_dev *priv = platform_get_drvdata(pdev);

	// Deregister the misc device and remove the /dev/fftAnalysisSynthesisProcessor file.
	misc_deregister(&priv->miscdev);

	pr_info("fftAnalysisSynthesisProcessor_remove successful\n");

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
static const struct of_device_id fftAnalysisSynthesisProcessor_of_match[] = {
    // ****Note:**** This .compatible string must be identical to the
    // .compatible string in the Device Tree Node for fftAnalysisSynthesisProcessor
	{ .compatible = "SQ,fftAnalysisSynthesisProcessor", },
	{ }
};
MODULE_DEVICE_TABLE(of, fftAnalysisSynthesisProcessor_of_match);

/*-----------------------------------------------------------------------*/
/* Platform Driver Structure                                             */
/*-----------------------------------------------------------------------*/
/*
 * struct fftAnalysisSynthesisProcessor_driver - Platform driver struct for the
 *                                  fftAnalysisSynthesisProcessor driver
 * @probe: Function that's called when a device is found
 * @remove: Function that's called when a device is removed
 * @driver.owner: Which module owns this driver
 * @driver.name: Name of the fftAnalysisSynthesisProcessor driver
 * @driver.of_match_table: Device tree match table
 * @driver.dev_groups: fftAnalysisSynthesisProcessor sysfs attribute group; this
 *                     allows the driver core to create the
 *                     attribute(s) without race conditions.
 */
static struct platform_driver fftAnalysisSynthesisProcessor_driver = {
	.probe = fftAnalysisSynthesisProcessor_probe,
	.remove = fftAnalysisSynthesisProcessor_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "fftAnalysisSynthesisProcessor",
		.of_match_table = fftAnalysisSynthesisProcessor_of_match,
		.dev_groups = fftAnalysisSynthesisProcessor_groups,
	},
};

/*
 * We don't need to do anything special in module init/exit.
 * This macro automatically handles module init/exit.
 */
module_platform_driver(fftAnalysisSynthesisProcessor_driver);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Suhaib Qasem");  // Adapted from Trevor Vannoy's Echo Driver
MODULE_DESCRIPTION("fftAnalysisSynthesisProcessor driver");
MODULE_VERSION("1.0");
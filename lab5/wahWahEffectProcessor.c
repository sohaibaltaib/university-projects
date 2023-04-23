/* SPDX-License-Identifier: GPL-2.0 or MIT                               */
/* Copyright(c) 2021 Ross K.Snider. All rights reserved.                 */
/*-------------------------------------------------------------------------
 * Description:  Linux Platform Device Driver for the
 *               wahWahEffectProcessor component
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
#define REG0_enable_OFFSET 0x00
#define REG1_volume_OFFSET 0x04
#define REG2_damp_OFFSET 0x08
#define REG3_minf_OFFSET 0x0C
#define REG4_maxf_OFFSET 0x10
#define REG5_delta_OFFSET 0x14
#define REG6_wetDry_OFFSET 0x18

/* Memory span of all registers (used or not) in the                     */
/* component wahWahEffectProcessor                                            */
#define SPAN 0x1C

/*-----------------------------------------------------------------------*/
/* wahWahEffectProcessor device structure                                     */
/*-----------------------------------------------------------------------*/
/*
 * struct  wahWahEffectProcessor_dev - Private wahWahEffectProcessor device struct.
 * @miscdev: miscdevice used to create a char device
 *           for the wahWahEffectProcessor component
 * @base_addr: Base address of the wahWahEffectProcessor component
 * @lock: mutex used to prevent concurrent writes
 *        to the wahWahEffectProcessor component
 *
 * An wahWahEffectProcessor_dev struct gets created for each wahWahEffectProcessor
 * component in the system.
 */
struct wahWahEffectProcessor_dev {
	struct miscdevice miscdev;
	void __iomem *base_addr;
	struct mutex lock;
};

/*-----------------------------------------------------------------------*/
/* REG0: enable register read function show()                   */
/*-----------------------------------------------------------------------*/
/*
 * enable_show() - Return the enable value
 *                          to user-space via sysfs.
 * @dev: Device structure for the wahWahEffectProcessor component. This
 *       device struct is embedded in the wahWahEffectProcessor' device struct.
 * @attr: Unused.
 * @buf: Buffer that gets returned to user-space.
 *
 * Return: The number of bytes read.
 */

 static ssize_t enable_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	bool enable;

	// Get the private wahWahEffectProcessor data out of the dev struct
	struct wahWahEffectProcessor_dev *priv = dev_get_drvdata(dev);

	enable = ioread32(priv->base_addr + REG0_enable_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", enable);
}

/*-----------------------------------------------------------------------*/
/* REG0: enable register write function store()                 */
/*-----------------------------------------------------------------------*/
/**
 * enable_store() - Store the enable value.
 * @dev: Device structure for the wahWahEffectProcessor component. This
 *       device struct is embedded in the wahWahEffectProcessor'
 *       platform device struct.
 * @attr: Unused.
 * @buf: Buffer that contains the enable value being written.
 * @size: The number of bytes being written.
 *
 * Return: The number of bytes stored.
 */

 static ssize_t enable_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	bool enable;
	int ret;
	struct wahWahEffectProcessor_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a bool
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtobool(buf, &enable);
	if (ret < 0) {
		// kstrtobool returned an error
		return ret;
	}

	iowrite32(enable, priv->base_addr + REG0_enable_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

/*-----------------------------------------------------------------------*/
/* TODO: Add show() and store() functions for                            */
/* Registers REG1 (volume) and REG2 (bm)                    */
/* in component wahWahEffectProcessor                                         */
/*-----------------------------------------------------------------------*/
/* Add here...                                                           */

static ssize_t volume_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u16 volume;
	struct wahWahEffectProcessor_dev *priv = dev_get_drvdata(dev);

	volume = ioread32(priv->base_addr + REG1_volume_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", volume);
}

static ssize_t volume_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u16 volume;
	int ret;
	struct wahWahEffectProcessor_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtou16(buf, 0, &volume);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(volume, priv->base_addr + REG1_volume_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

/*-----------------------------------------------------------------------*/
/* TODO: Add show() and store() functions for                            */
/* Registers REG1 (volume) and REG2 (bm)                    */
/* in component wahWahEffectProcessor                                         */
/*-----------------------------------------------------------------------*/
/* Add here...                                                           */

static ssize_t damp_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u16 damp;
	struct wahWahEffectProcessor_dev *priv = dev_get_drvdata(dev);

	damp = ioread32(priv->base_addr + REG2_damp_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", damp);
}

static ssize_t damp_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u16 damp;
	int ret;
	struct wahWahEffectProcessor_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtou16(buf, 0, &damp);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(damp, priv->base_addr + REG2_damp_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

/*-----------------------------------------------------------------------*/
/* TODO: Add show() and store() functions for                            */
/* Registers REG1 (volume) and REG2 (bm)                    */
/* in component wahWahEffectProcessor                                         */
/*-----------------------------------------------------------------------*/
/* Add here...                                                           */

static ssize_t minf_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u16 minf;
	struct wahWahEffectProcessor_dev *priv = dev_get_drvdata(dev);

	minf = ioread32(priv->base_addr + REG3_minf_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", minf);
}

static ssize_t minf_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u16 minf;
	int ret;
	struct wahWahEffectProcessor_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtou16(buf, 0, &minf);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(minf, priv->base_addr + REG3_minf_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

/*-----------------------------------------------------------------------*/
/* TODO: Add show() and store() functions for                            */
/* Registers REG1 (volume) and REG2 (bm)                    */
/* in component wahWahEffectProcessor                                         */
/*-----------------------------------------------------------------------*/
/* Add here...                                                           */

static ssize_t maxf_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u16 maxf;
	struct wahWahEffectProcessor_dev *priv = dev_get_drvdata(dev);

	maxf = ioread32(priv->base_addr + REG4_maxf_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", maxf);
}

static ssize_t maxf_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u16 maxf;
	int ret;
	struct wahWahEffectProcessor_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtou16(buf, 0, &maxf);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(maxf, priv->base_addr + REG4_maxf_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

/*-----------------------------------------------------------------------*/
/* TODO: Add show() and store() functions for                            */
/* Registers REG1 (volume) and REG2 (bm)                    */
/* in component wahWahEffectProcessor                                         */
/*-----------------------------------------------------------------------*/
/* Add here...                                                           */

static ssize_t delta_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u16 delta;
	struct wahWahEffectProcessor_dev *priv = dev_get_drvdata(dev);

	delta = ioread32(priv->base_addr + REG5_delta_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", delta);
}

static ssize_t delta_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u16 delta;
	int ret;
	struct wahWahEffectProcessor_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtou16(buf, 0, &delta);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(delta, priv->base_addr + REG5_delta_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}


/*-----------------------------------------------------------------------*/
/* TODO: Add show() and store() functions for                            */
/* Registers REG1 (volume) and REG2 (bm)                    */
/* in component wahWahEffectProcessor                                         */
/*-----------------------------------------------------------------------*/
/* Add here...                                                           */

static ssize_t wetDry_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u16 wetDry;
	struct wahWahEffectProcessor_dev *priv = dev_get_drvdata(dev);

	wetDry = ioread32(priv->base_addr + REG6_wetDry_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", wetDry);
}

static ssize_t wetDry_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u16 wetDry;
	int ret;
	struct wahWahEffectProcessor_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtou16(buf, 0, &wetDry);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(wetDry, priv->base_addr + REG6_wetDry_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}
/*-----------------------------------------------------------------------*/
/* sysfs Attributes                                                      */
/*-----------------------------------------------------------------------*/
// Define sysfs attributes
static DEVICE_ATTR_RW(enable);    // Attribute for REG0
/* TODO: Add the attributes for REG1 and REG2 using register names       */
static DEVICE_ATTR_RW(volume);		// Attribute for REG1
static DEVICE_ATTR_RW(damp);		// Attribute for REG1
static DEVICE_ATTR_RW(minf);		// Attribute for REG1
static DEVICE_ATTR_RW(maxf);		// Attribute for REG1
static DEVICE_ATTR_RW(delta);		// Attribute for REG1
static DEVICE_ATTR_RW(wetDry);		// Attribute for REG1

// Create an atribute group so the device core can
// export the attributes for us.
static struct attribute *wahWahEffectProcessor_attrs[] = {
	&dev_attr_enable.attr,
/* TODO: Add the attribute entries for REG1 and REG2 using register names*/
	&dev_attr_volume.attr,
	&dev_attr_damp.attr,
	&dev_attr_minf.attr,
	&dev_attr_maxf.attr,
	&dev_attr_delta.attr,
	&dev_attr_wetDry.attr,
	NULL,
};
ATTRIBUTE_GROUPS(wahWahEffectProcessor);

/*-----------------------------------------------------------------------*/
/* File Operations read()                                                */
/*-----------------------------------------------------------------------*/
/*
 * wahWahEffectProcessor_read() - Read method for the wahWahEffectProcessor char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value into.
 * @count: The number of bytes being requested.
 * @offset: The byte offset in the file being read from.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */

 static ssize_t wahWahEffectProcessor_read(struct file *file, char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * wahWahEffectProcessor_dev struct. container_of returns the
     * wahWahEffectProcessor_dev struct that contains the miscdev in private_data.
	 */
	struct wahWahEffectProcessor_dev *priv = container_of(file->private_data,
	                            struct wahWahEffectProcessor_dev, miscdev);

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
		pr_warn("wahWahEffectProcessor_read: unaligned access\n");
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
		pr_warn("wahWahEffectProcessor_read: nothing copied\n");
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
 * wahWahEffectProcessor_write() - Write method for the wahWahEffectProcessor char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value from.
 * @count: The number of bytes being written.
 * @offset: The byte offset in the file being written to.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */

 static ssize_t wahWahEffectProcessor_write(struct file *file, const char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * wahWahEffectProcessor_dev struct. container_of returns the
     * wahWahEffectProcessor_dev struct that contains the miscdev in private_data.
	 */
	struct wahWahEffectProcessor_dev *priv = container_of(file->private_data,
	                              struct wahWahEffectProcessor_dev, miscdev);

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
		pr_warn("wahWahEffectProcessor_write: unaligned access\n");
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
		pr_warn("wahWahEffectProcessor_write: nothing copied from user space\n");
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
 *  wahWahEffectProcessor_fops - File operations supported by the
 *                          wahWahEffectProcessor driver
 * @owner: The wahWahEffectProcessor driver owns the file operations; this
 *         ensures that the driver can't be removed while the
 *         character device is still in use.
 * @read: The read function.
 * @write: The write function.
 * @llseek: We use the kernel's default_llseek() function; this allows
 *          users to change what position they are writing/reading to/from.
 */
static const struct file_operations  wahWahEffectProcessor_fops = {
	.owner = THIS_MODULE,
	.read = wahWahEffectProcessor_read,
	.write = wahWahEffectProcessor_write,
	.llseek = default_llseek,
};

/*-----------------------------------------------------------------------*/
/* Platform Driver Probe (Initialization) Function                       */
/*-----------------------------------------------------------------------*/
/*
 * wahWahEffectProcessor_probe() - Initialize device when a match is found
 * @pdev: Platform device structure associated with our
 *        wahWahEffectProcessor device; pdev is automatically created by the
 *        driver core based upon our wahWahEffectProcessor device tree node.
 *
 * When a device that is compatible with this wahWahEffectProcessor driver
 * is found, the driver's probe function is called. This probe function
 * gets called by the kernel when an wahWahEffectProcessor device is found
 * in the device tree.
 */
static int wahWahEffectProcessor_probe(struct platform_device *pdev)
{
	struct wahWahEffectProcessor_dev *priv;
	int ret;

/*
	 * Allocate kernel memory for the wahWahEffectProcessor device and set it to 0.
	 * GFP_KERNEL specifies that we are allocating normal kernel RAM;
	 * see the kmalloc documentation for more info. The allocated memory
	 * is automatically freed when the device is removed.
	 */
	priv = devm_kzalloc(&pdev->dev, sizeof(struct wahWahEffectProcessor_dev), GFP_KERNEL);
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
		pr_err("Failed to request/remap platform device resource (wahWahEffectProcessor)\n");
		return PTR_ERR(priv->base_addr);
	}

	// Initialize the misc device parameters
	priv->miscdev.minor = MISC_DYNAMIC_MINOR;
	priv->miscdev.name = "wahWahEffectProcessor";
	priv->miscdev.fops = &wahWahEffectProcessor_fops;
	priv->miscdev.parent = &pdev->dev;
	priv->miscdev.groups = wahWahEffectProcessor_groups;

	// Register the misc device; this creates a char dev at
    // /dev/wahWahEffectProcessor
	ret = misc_register(&priv->miscdev);
	if (ret) {
		pr_err("Failed to register misc device for wahWahEffectProcessor\n");
		return ret;
	}

		// Attach the wahWahEffectProcessor' private data to the
    // platform device's struct.
	platform_set_drvdata(pdev, priv);

	pr_info("wahWahEffectProcessor_probe successful\n");

	return 0;
}

/*-----------------------------------------------------------------------*/
/* Platform Driver Remove Function                                       */
/*-----------------------------------------------------------------------*/
/*
 * wahWahEffectProcessor_remove() - Remove an wahWahEffectProcessor device.
 * @pdev: Platform device structure associated with our wahWahEffectProcessor device.
 *
 * This function is called when an wahWahEffectProcessor devicee is removed or
 * the driver is removed.
 */
static int wahWahEffectProcessor_remove(struct platform_device *pdev)
{
	// Get thewahWahEffectProcessor' private data from the platform device.
	struct wahWahEffectProcessor_dev *priv = platform_get_drvdata(pdev);

	// Deregister the misc device and remove the /dev/wahWahEffectProcessor file.
	misc_deregister(&priv->miscdev);

	pr_info("wahWahEffectProcessor_remove successful\n");

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

 static const struct of_device_id wahWahEffectProcessor_of_match[] = {
    // ****Note:**** This .compatible string must be identical to the
    // .compatible string in the Device Tree Node for wahWahEffectProcessor
	{ .compatible = "SQ,wahWahEffectProcessor", },
	{ }
};
MODULE_DEVICE_TABLE(of, wahWahEffectProcessor_of_match);

/*-----------------------------------------------------------------------*/
/* Platform Driver Structure                                             */
/*-----------------------------------------------------------------------*/
/*
 * struct wahWahEffectProcessor_driver - Platform driver struct for the
 *                                  wahWahEffectProcessor driver
 * @probe: Function that's called when a device is found
 * @remove: Function that's called when a device is removed
 * @driver.owner: Which module owns this driver
 * @driver.name: Name of the wahWahEffectProcessor driver
 * @driver.of_match_table: Device tree match table
 * @driver.dev_groups: wahWahEffectProcessor sysfs attribute group; this
 *                     allows the driver core to create the
 *                     attribute(s) without race conditions.
 */
static struct platform_driver wahWahEffectProcessor_driver = {
	.probe = wahWahEffectProcessor_probe,
	.remove = wahWahEffectProcessor_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "wahWahEffectProcessor",
		.of_match_table = wahWahEffectProcessor_of_match,
		.dev_groups = wahWahEffectProcessor_groups,
	},
};

/*
 * We don't need to do anything special in module init/exit.
 * This macro automatically handles module init/exit.
 */
module_platform_driver(wahWahEffectProcessor_driver);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Suhaib Qasem");  // Adapted from Trevor Vannoy's Echo Driver
MODULE_DESCRIPTION("wahWahEffectProcessor driver");
MODULE_VERSION("1.0");

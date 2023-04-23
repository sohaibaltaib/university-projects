/* SPDX-License-Identifier: GPL-2.0 or MIT                               */
/* Copyright(c) 2021 Ross K.Snider. All rights reserved.                 */
/*-------------------------------------------------------------------------
 * Description:  Linux Platform Device Driver for the
 *               adc_0 component
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
#define REG0_P0_OFFSET 0x0
#define REG1_P1_OFFSET 0x04
#define REG2_P2_OFFSET 0x08
#define REG3_P3_OFFSET 0x0c
#define REG4_P4_OFFSET 0x10
#define REG5_P5_OFFSET 0x14

/* Memory span of all registers (used or not) in the                     */
/* component adc_0                                            */
#define SPAN 0x18

/*-----------------------------------------------------------------------*/
/* adc_0 device structure                                     */
/*-----------------------------------------------------------------------*/
/*
 * struct  adc_0_dev - Private adc_0 device struct.
 * @miscdev: miscdevice used to create a char device
 *           for the adc_0 component
 * @base_addr: Base address of the adc_0 component
 * @lock: mutex used to prevent concurrent writes
 *        to the adc_0 component
 *
 * An adc_0_dev struct gets created for each adc_0
 * component in the system.
 */
struct adc_0_dev {
	struct miscdevice miscdev;
	void __iomem *base_addr;
	struct mutex lock;
};

/*-----------------------------------------------------------------------*/
/* REG0: p0 register read function show()                   */
/*-----------------------------------------------------------------------*/
/*
 * p0_show() - Return the red value
 *                          to user-space via sysfs.
 * @dev: Device structure for the adc_0 component. This
 *       device struct is embedded in the adc_0' device struct.
 * @attr: Unused.
 * @buf: Buffer that gets returned to user-space.
 *
 * Return: The number of bytes read.
 */
static ssize_t p0_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 p0;

	// Get the private adc_0 data out of the dev struct
	struct adc_0_dev *priv = dev_get_drvdata(dev);

	p0 = ioread32(priv->base_addr + REG0_P0_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", p0);
}

static ssize_t p0_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 p0;
	int ret;
	struct adc_0_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtou32(buf, 0, &p0);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(p0, priv->base_addr + REG0_P0_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}


static ssize_t p1_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 p1;
	struct adc_0_dev *priv = dev_get_drvdata(dev);

	p1 = ioread32(priv->base_addr + REG1_P1_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", p1);
}

static ssize_t p1_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 p1;
	int ret;
	struct adc_0_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtou32(buf, 0, &p1);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(p1, priv->base_addr + REG1_P1_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

static ssize_t p2_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 p2;
	struct adc_0_dev *priv = dev_get_drvdata(dev);

	p2 = ioread32(priv->base_addr + REG2_P2_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", p2);
}

static ssize_t p2_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 p2;
	int ret;
	struct adc_0_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtou32(buf, 0, &p2);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(p2, priv->base_addr + REG2_P2_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

static ssize_t p3_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 p3;
	struct adc_0_dev *priv = dev_get_drvdata(dev);

	p3 = ioread32(priv->base_addr + REG3_P3_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", p3);
}

static ssize_t p3_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 p3;
	int ret;
	struct adc_0_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtou32(buf, 0, &p3);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(p3, priv->base_addr + REG3_P3_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}

static ssize_t p4_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 p4;
	struct adc_0_dev *priv = dev_get_drvdata(dev);

	p4 = ioread32(priv->base_addr + REG4_P4_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", p4);
}

static ssize_t p4_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 p4;
	int ret;
	struct adc_0_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtou32(buf, 0, &p4);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(p4, priv->base_addr + REG4_P4_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}


static ssize_t p5_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u32 p5;
	struct adc_0_dev *priv = dev_get_drvdata(dev);

	p5 = ioread32(priv->base_addr + REG5_P5_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", p5);
}

static ssize_t p5_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u32 p5;
	int ret;
	struct adc_0_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtou32(buf, 0, &p5);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(p5, priv->base_addr + REG5_P5_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}
/*-----------------------------------------------------------------------*/
/* sysfs Attributes                                                      */
/*-----------------------------------------------------------------------*/
// Define sysfs attributes
static DEVICE_ATTR_RW(p0);    // Attribute for REG0
/* TODO: Add the attributes for REG1 and REG2 using register names       */
static DEVICE_ATTR_RW(p1);		// Attribute for REG1
static DEVICE_ATTR_RW(p2);            // Attribute for REG2
static DEVICE_ATTR_RW(p3);
static DEVICE_ATTR_RW(p4);
static DEVICE_ATTR_RW(p5);

// Create an atribute group so the device core can
// export the attributes for us.
static struct attribute *adc_0_attrs[] = {
	&dev_attr_p0.attr,
/* TODO: Add the attribute entries for REG1 and REG2 using register names*/
	&dev_attr_p1.attr,
	&dev_attr_p2.attr,
	&dev_attr_p3.attr,
	&dev_attr_p4.attr,
	&dev_attr_p5.attr,
	NULL,
};
ATTRIBUTE_GROUPS(adc_0);


/*-----------------------------------------------------------------------*/
/* File Operations read()                                                */
/*-----------------------------------------------------------------------*/
/*
 * adc_0_read() - Read method for the adc_0 char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value into.
 * @count: The number of bytes being requested.
 * @offset: The byte offset in the file being read from.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */

static ssize_t adc_0_read(struct file *file, char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * adc_0_dev struct. container_of returns the
     * adc_0_dev struct that contains the miscdev in private_data.
	 */
	struct adc_0_dev *priv = container_of(file->private_data,
	                            struct adc_0_dev, miscdev);

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
		pr_warn("adc_0_read: unaligned access\n");
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
		pr_warn("adc_0_read: nothing copied\n");
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
 * adc_0_write() - Write method for the adc_0 char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value from.
 * @count: The number of bytes being written.
 * @offset: The byte offset in the file being written to.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */
static ssize_t adc_0_write(struct file *file, const char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * adc_0_dev struct. container_of returns the
     * adc_0_dev struct that contains the miscdev in private_data.
	 */
	struct adc_0_dev *priv = container_of(file->private_data,
	                              struct adc_0_dev, miscdev);

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
		pr_warn("adc_0_write: unaligned access\n");
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
		pr_warn("adc_0_write: nothing copied from user space\n");
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
 *  adc_0_fops - File operations supported by the
 *                          adc_0 driver
 * @owner: The adc_0 driver owns the file operations; this
 *         ensures that the driver can't be removed while the
 *         character device is still in use.
 * @read: The read function.
 * @write: The write function.
 * @llseek: We use the kernel's default_llseek() function; this allows
 *          users to change what position they are writing/reading to/from.
 */
static const struct file_operations  adc_0_fops = {
	.owner = THIS_MODULE,
	.read = adc_0_read,
	.write = adc_0_write,
	.llseek = default_llseek,
};

/*-----------------------------------------------------------------------*/
/* Platform Driver Probe (Initialization) Function                       */
/*-----------------------------------------------------------------------*/
/*
 * adc_0_probe() - Initialize device when a match is found
 * @pdev: Platform device structure associated with our
 *        adc_0 device; pdev is automatically created by the
 *        driver core based upon our adc_0 device tree node.
 *
 * When a device that is compatible with this adc_0 driver
 * is found, the driver's probe function is called. This probe function
 * gets called by the kernel when an adc_0 device is found
 * in the device tree.
 */
static int adc_0_probe(struct platform_device *pdev)
{
	struct adc_0_dev *priv;
	int ret;

	/*
	 * Allocate kernel memory for the adc_0 device and set it to 0.
	 * GFP_KERNEL specifies that we are allocating normal kernel RAM;
	 * see the kmalloc documentation for more info. The allocated memory
	 * is automatically freed when the device is removed.
	 */
	priv = devm_kzalloc(&pdev->dev, sizeof(struct adc_0_dev), GFP_KERNEL);
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
		pr_err("Failed to request/remap platform device resource (adc_0)\n");
		return PTR_ERR(priv->base_addr);
	}

	// Initialize the misc device parameters
	priv->miscdev.minor = MISC_DYNAMIC_MINOR;
	priv->miscdev.name = "adc_0";
	priv->miscdev.fops = &adc_0_fops;
	priv->miscdev.parent = &pdev->dev;
	priv->miscdev.groups = adc_0_groups;

	// Register the misc device; this creates a char dev at
    // /dev/adc_0
	ret = misc_register(&priv->miscdev);
	if (ret) {
		pr_err("Failed to register misc device for adc_0\n");
		return ret;
	}

	// Attach the adc_0' private data to the
    // platform device's struct.
	platform_set_drvdata(pdev, priv);

	pr_info("adc_0_probe successful\n");

	return 0;
}

/*-----------------------------------------------------------------------*/
/* Platform Driver Remove Function                                       */
/*-----------------------------------------------------------------------*/
/*
 * adc_0_remove() - Remove an adc_0 device.
 * @pdev: Platform device structure associated with our adc_0 device.
 *
 * This function is called when an adc_0 devicee is removed or
 * the driver is removed.
 */
static int adc_0_remove(struct platform_device *pdev)
{
	// Get theadc_0' private data from the platform device.
	struct adc_0_dev *priv = platform_get_drvdata(pdev);

	// Deregister the misc device and remove the /dev/adc_0 file.
	misc_deregister(&priv->miscdev);

	pr_info("adc_0_remove successful\n");

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
static const struct of_device_id adc_0_of_match[] = {
    // ****Note:**** This .compatible string must be identical to the
    // .compatible string in the Device Tree Node for adc_0
	{ .compatible = "SQ,adc_0", },
	{ }
};
MODULE_DEVICE_TABLE(of, adc_0_of_match);

/*-----------------------------------------------------------------------*/
/* Platform Driver Structure                                             */
/*-----------------------------------------------------------------------*/
/*
 * struct adc_0_driver - Platform driver struct for the
 *                                  adc_0 driver
 * @probe: Function that's called when a device is found
 * @remove: Function that's called when a device is removed
 * @driver.owner: Which module owns this driver
 * @driver.name: Name of the adc_0 driver
 * @driver.of_match_table: Device tree match table
 * @driver.dev_groups: adc_0 sysfs attribute group; this
 *                     allows the driver core to create the
 *                     attribute(s) without race conditions.
 */
static struct platform_driver adc_0_driver = {
	.probe = adc_0_probe,
	.remove = adc_0_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "adc_0",
		.of_match_table = adc_0_of_match,
		.dev_groups = adc_0_groups,
	},
};

/*
 * We don't need to do anything special in module init/exit.
 * This macro automatically handles module init/exit.
 */
module_platform_driver(adc_0_driver);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Huiwen Zhang");  // Adapted from Trevor Vannoy's Echo Driver
MODULE_DESCRIPTION("adc_0 driver");
MODULE_VERSION("1.0");
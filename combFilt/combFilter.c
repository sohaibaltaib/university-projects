/* SPDX-License-Identifier: GPL-2.0 or MIT                               */
/* Copyright(c) 2021 Ross K.Snider. All rights reserved.                 */
/*-------------------------------------------------------------------------
 * Description:  Linux Platform Device Driver for the
 *               CombFilter component
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
#define REG0_delayM_OFFSET 0x0
/* #define REG1 (Add offset for b0) */
#define REG1_b0_OFFSET 0x04

#define REG2_bM_OFFSET 0x08
/* #define REG3 (Add offset for wetDryMix) */
#define REG3_wetDryMix_OFFSET 0x0C

/* Memory span of all registers (used or not) in the                     */
/* component CombFilter                                            */
#define SPAN 0x10


/*-----------------------------------------------------------------------*/
/* CombFilter device structure                                     */
/*-----------------------------------------------------------------------*/
/*
 * struct  CombFilter_dev - Private CombFilter device struct.
 * @miscdev: miscdevice used to create a char device
 *           for the CombFilter component
 * @base_addr: Base address of the CombFilter component
 * @lock: mutex used to prevent concurrent writes
 *        to the CombFilter component
 *
 * An CombFilter_dev struct gets created for each CombFilter
 * component in the system.
 */
struct CombFilter_dev {
	struct miscdevice miscdev;
	void __iomem *base_addr;
	struct mutex lock;
};

/*-----------------------------------------------------------------------*/
/* REG0: delayM register read function show()                   */
/*-----------------------------------------------------------------------*/
/*
 * delayM_show() - Return the delayM value
 *                          to user-space via sysfs.
 * @dev: Device structure for the CombFilter component. This
 *       device struct is embedded in the CombFilter' device struct.
 * @attr: Unused.
 * @buf: Buffer that gets returned to user-space.
 *
 * Return: The number of bytes read.
 */
static ssize_t delayM_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u16 delayM;

	// Get the private CombFilter data out of the dev struct
	struct CombFilter_dev *priv = dev_get_drvdata(dev);

	delayM = ioread32(priv->base_addr + REG0_delayM_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", delayM);
}
/*-----------------------------------------------------------------------*/
/* REG0: delayM register write function store()                 */
/*-----------------------------------------------------------------------*/
/**
 * delayM_store() - Store the delayM value.
 * @dev: Device structure for the CombFilter component. This
 *       device struct is embedded in the CombFilter'
 *       platform device struct.
 * @attr: Unused.
 * @buf: Buffer that contains the delayM value being written.
 * @size: The number of bytes being written.
 *
 * Return: The number of bytes stored.
 */
static ssize_t delayM_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u16 delayM;
	int ret;
	struct CombFilter_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a bool
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtou16(buf, 0, &delayM);
	if (ret < 0) {
		// kstrtobool returned an error
		return ret;
	}

	iowrite32(delayM, priv->base_addr + REG0_delayM_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}


/*-----------------------------------------------------------------------*/
/* TODO: Add show() and store() functions for                            */
/* Registers REG1 (b0) and REG2 (wetDryMix)                    */
/* in component CombFilter                                         */
/*-----------------------------------------------------------------------*/
/* Add here...                                                           */
/*-----------------------------------------------------------------------*/
/* REG1: b0 register read function show()                      */
/*-----------------------------------------------------------------------*/
static ssize_t b0_show(struct device *dev,
        struct device_attribute *attr, char *buf)
{
        u16 b0;
        struct CombFilter_dev *priv = dev_get_drvdata(dev);

        b0 = ioread32(priv->base_addr + REG1_b0_OFFSET);

        return scnprintf(buf, PAGE_SIZE, "%u\n", b0);
}
/*-----------------------------------------------------------------------*/
/* REG1: b0 register write function store()                    */
/*-----------------------------------------------------------------------*/
static ssize_t b0_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t size)
{
        u16 b0;
        int ret;
        struct CombFilter_dev *priv = dev_get_drvdata(dev);

        // Parse the string we received as a u8
        // See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
        ret = kstrtou16(buf, 0, &b0);
        if (ret < 0) {
                // kstrtou16 returned an error
                return ret;
        }

        iowrite32(b0, priv->base_addr + REG1_b0_OFFSET);

        // Write was succesful, so we return the number of bytes we wrote.
        return size;
}

/*-----------------------------------------------------------------------*/
/* REG3: wetDryMix register read function show()                      */
/*-----------------------------------------------------------------------*/
static ssize_t wetDryMix_show(struct device *dev,
        struct device_attribute *attr, char *buf)
{
        u16 wetDryMix;
        struct CombFilter_dev *priv = dev_get_drvdata(dev);

        wetDryMix = ioread32(priv->base_addr + REG3_wetDryMix_OFFSET);

        return scnprintf(buf, PAGE_SIZE, "%u\n", wetDryMix);
}
/*-----------------------------------------------------------------------*/
/* REG3: wetDryMix register write function store()                    */
/*-----------------------------------------------------------------------*/
static ssize_t wetDryMix_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t size)
{
        u16 wetDryMix;
        int ret;
        struct CombFilter_dev *priv = dev_get_drvdata(dev);

        // Parse the string we received as a u8
        // See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
        ret = kstrtou16(buf, 0, &wetDryMix);
        if (ret < 0) {
                // kstrtou16 returned an error
                return ret;
        }

        iowrite32(wetDryMix, priv->base_addr + REG3_wetDryMix_OFFSET);

        // Write was succesful, so we return the number of bytes we wrote.
        return size;
}

/*-----------------------------------------------------------------------*/
/* REG2: bM register read function show()                           */
/*-----------------------------------------------------------------------*/
/*
 * bM_show() - Return the bM value to user-space via sysfs.
 * @dev: Device structure for the CombFilter component. This
 *       device struct is embedded in the CombFilter' platform
 *       device struct.
 * @attr: Unused.
 * @buf: Buffer that gets returned to user-space.
 *
 * Return: The number of bytes read.
 */
static ssize_t bM_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u16 bM;
	struct CombFilter_dev *priv = dev_get_drvdata(dev);

	bM = ioread32(priv->base_addr + REG2_bM_OFFSET);

	return scnprintf(buf, PAGE_SIZE, "%u\n", bM);
}
/*-----------------------------------------------------------------------*/
/* REG2: bM register write function store()                         */
/*-----------------------------------------------------------------------*/
/*
 * bM_store() - Store the bM value.
 * @dev: Device structure for the CombFilter component. This
 *       device struct is embedded in the CombFilter' platform
 *       device struct.
 * @attr: Unused.
 * @buf: Buffer that contains the  CombFilter value being written.
 * @size: The number of bytes being written.
 *
 * Return: The number of bytes stored.
 */
static ssize_t bM_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	u16 bM;
	int ret;
	struct CombFilter_dev *priv = dev_get_drvdata(dev);

	// Parse the string we received as a u8
	// See https://elixir.bootlin.com/linux/latest/source/lib/kstrtox.c#L289
	ret = kstrtou16(buf, 0, &bM);
	if (ret < 0) {
		// kstrtou16 returned an error
		return ret;
	}

	iowrite32(bM, priv->base_addr + REG2_bM_OFFSET);

	// Write was succesful, so we return the number of bytes we wrote.
	return size;
}


/*-----------------------------------------------------------------------*/
/* sysfs Attributes                                                      */
/*-----------------------------------------------------------------------*/
// Define sysfs attributes
static DEVICE_ATTR_RW(delayM);    // Attribute for REG0
/* TODO: Add the attributes for REG1 and REG2 using register names       */
static DEVICE_ATTR_RW(b0);            // Attribute for REG1
static DEVICE_ATTR_RW(wetDryMix);            // Attribute for REG2
static DEVICE_ATTR_RW(bM);            // Attribute for REG3

// Create an atribute group so the device core can
// export the attributes for us.
static struct attribute *CombFilter_attrs[] = {
	&dev_attr_delayM.attr,
/* TODO: Add the attribute entries for REG1 and REG2 using register names*/
	&dev_attr_bM.attr,
	&dev_attr_b0.attr,
        &dev_attr_wetDryMix.attr,

	NULL,
};
ATTRIBUTE_GROUPS(CombFilter);


/*-----------------------------------------------------------------------*/
/* File Operations read()                                                */
/*-----------------------------------------------------------------------*/
/*
 * CombFilter_read() - Read method for the CombFilter char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value into.
 * @count: The number of bytes being requested.
 * @offset: The byte offset in the file being read from.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */
static ssize_t CombFilter_read(struct file *file, char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * CombFilter_dev struct. container_of returns the
     * CombFilter_dev struct that contains the miscdev in private_data.
	 */
	struct CombFilter_dev *priv = container_of(file->private_data,
	                            struct CombFilter_dev, miscdev);

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
		pr_warn("CombFilter_read: unaligned access\n");
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
		pr_warn("CombFilter_read: nothing copied\n");
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
 * CombFilter_write() - Write method for the CombFilter char device
 * @file: Pointer to the char device file struct.
 * @buf: User-space buffer to read the value from.
 * @count: The number of bytes being written.
 * @offset: The byte offset in the file being written to.
 *
 * Return: On success, the number of bytes written is returned and the
 * offset @offset is advanced by this number. On error, a negative error
 * value is returned.
 */
static ssize_t CombFilter_write(struct file *file, const char __user *buf,
	size_t count, loff_t *offset)
{
	size_t ret;
	u32 val;

	loff_t pos = *offset;

	/*
	 * Get the device's private data from the file struct's private_data
	 * field. The private_data field is equal to the miscdev field in the
	 * CombFilter_dev struct. container_of returns the
     * CombFilter_dev struct that contains the miscdev in private_data.
	 */
	struct CombFilter_dev *priv = container_of(file->private_data,
	                              struct CombFilter_dev, miscdev);

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
		pr_warn("CombFilter_write: unaligned access\n");
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
		pr_warn("CombFilter_write: nothing copied from user space\n");
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
 *  CombFilter_fops - File operations supported by the
 *                          CombFilter driver
 * @owner: The CombFilter driver owns the file operations; this
 *         ensures that the driver can't be removed while the
 *         character device is still in use.
 * @read: The read function.
 * @write: The write function.
 * @llseek: We use the kernel's default_llseek() function; this allows
 *          users to change what position they are writing/reading to/from.
 */
static const struct file_operations  CombFilter_fops = {
	.owner = THIS_MODULE,
	.read = CombFilter_read,
	.write = CombFilter_write,
	.llseek = default_llseek,
};


/*-----------------------------------------------------------------------*/
/* Platform Driver Probe (Initialization) Function                       */
/*-----------------------------------------------------------------------*/
/*
 * CombFilter_probe() - Initialize device when a match is found
 * @pdev: Platform device structure associated with our
 *        CombFilter device; pdev is automatically created by the
 *        driver core based upon our CombFilter device tree node.
 *
 * When a device that is compatible with this CombFilter driver
 * is found, the driver's probe function is called. This probe function
 * gets called by the kernel when an CombFilter device is found
 * in the device tree.
 */
static int CombFilter_probe(struct platform_device *pdev)
{
	struct CombFilter_dev *priv;
	int ret;

	/*
	 * Allocate kernel memory for the CombFilter device and set it to 0.
	 * GFP_KERNEL specifies that we are allocating normal kernel RAM;
	 * see the kmalloc documentation for more info. The allocated memory
	 * is automatically freed when the device is removed.
	 */
	priv = devm_kzalloc(&pdev->dev, sizeof(struct CombFilter_dev), GFP_KERNEL);
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
		pr_err("Failed to request/remap platform device resource (CombFilter)\n");
		return PTR_ERR(priv->base_addr);
	}

	// Initialize the misc device parameters
	priv->miscdev.minor = MISC_DYNAMIC_MINOR;
	priv->miscdev.name = "CombFilter";
	priv->miscdev.fops = &CombFilter_fops;
	priv->miscdev.parent = &pdev->dev;
	priv->miscdev.groups = CombFilter_groups;

	// Register the misc device; this creates a char dev at
    // /dev/CombFilter
	ret = misc_register(&priv->miscdev);
	if (ret) {
		pr_err("Failed to register misc device for CombFilter\n");
		return ret;
	}

	// Attach the CombFilter' private data to the
    // platform device's struct.
	platform_set_drvdata(pdev, priv);

	pr_info("CombFilter_probe successful\n");

	return 0;
}

/*-----------------------------------------------------------------------*/
/* Platform Driver Remove Function                                       */
/*-----------------------------------------------------------------------*/
/*
 * CombFilter_remove() - Remove an CombFilter device.
 * @pdev: Platform device structure associated with our CombFilter device.
 *
 * This function is called when an CombFilter devicee is removed or
 * the driver is removed.
 */
static int CombFilter_remove(struct platform_device *pdev)
{
	// Get theCombFilter' private data from the platform device.
	struct CombFilter_dev *priv = platform_get_drvdata(pdev);

	// Deregister the misc device and remove the /dev/CombFilter file.
	misc_deregister(&priv->miscdev);

	pr_info("CombFilter_remove successful\n");

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
static const struct of_device_id combFilterProcessor[] = {
    // ****Note:**** This .compatible string must be identical to the
    // .compatible string in the Device Tree Node for CombFilter
	{ .compatible = "SQ,combFilterProcessor", },
	{ }
};
MODULE_DEVICE_TABLE(of, combFilterProcessor);

/*-----------------------------------------------------------------------*/
/* Platform Driver Structure                                             */
/*-----------------------------------------------------------------------*/
/*
 * struct CombFilter_driver - Platform driver struct for the
 *                                  CombFilter driver
 * @probe: Function that's called when a device is found
 * @remove: Function that's called when a device is removed
 * @driver.owner: Which module owns this driver
 * @driver.name: Name of the CombFilter driver
 * @driver.of_match_table: Device tree match table
 * @driver.dev_groups: CombFilter sysfs attribute group; this
 *                     allows the driver core to create the
 *                     attribute(s) without race conditions.
 */
static struct platform_driver CombFilter_driver = {
	.probe = CombFilter_probe,
	.remove = CombFilter_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "CombFilter",
		.of_match_table = combFilterProcessor,
		.dev_groups = CombFilter_groups,
	},
};

/*
 * We don't need to do anything special in module init/exit.
 * This macro automatically handles module init/exit.
 */
module_platform_driver(CombFilter_driver);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Soos Qasem");  // Adapted from Trevor Vannoy's Echo Driver
MODULE_DESCRIPTION("CombFilter driver");
MODULE_VERSION("1.0");

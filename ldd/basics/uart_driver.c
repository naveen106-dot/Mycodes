/* [ 3019.315164] usb 2-1: USB disconnect, device number 3
[ 3030.899486] usb 2-1: new SuperSpeed USB device number 4 using xhci_hcd
[ 3030.916688] usb 2-1: New USB device found, idVendor=0781, idProduct=5581, bcdDevice= 1.00
[ 3030.916710] usb 2-1: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[ 3030.916724] usb 2-1: Product:  SanDisk 3.2Gen1
[ 3030.916735] usb 2-1: Manufacturer:  USB
[ 3030.916745] usb 2-1: SerialNumber: 03003320091123214537
[ 3030.919540] usb-storage 2-1:1.0: USB Mass Storage device detected
[ 3030.920327] scsi host0: usb-storage 2-1:1.0
[ 3031.935992] scsi 0:0:0:0: Direct-Access      USB      SanDisk 3.2Gen1 1.00 PQ: 0 ANSI: 6
[ 3031.936538] sd 0:0:0:0: Attached scsi generic sg0 type 0
[ 3031.952811] sd 0:0:0:0: [sda] 30031250 512-byte logical blocks: (15.4 GB/14.3 GiB)
[ 3031.954015] sd 0:0:0:0: [sda] Write Protect is off
[ 3031.954028] sd 0:0:0:0: [sda] Mode Sense: 43 00 00 00
[ 3031.954595] sd 0:0:0:0: [sda] Write cache: disabled, read cache: enabled, doesn't support DPO or FUA
[ 3031.973528]  sda:
[ 3031.975094] sd 0:0:0:0: [sda] Attached SCSI removable disk
[ 3032.265324] FAT-fs (sda): Volume was not properly unmounted. Some data may be corrupt. Please run fsck.

*/


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define   VENDOR_ID 0x0781
#define   PRODUCT_ID 0x5581   


static int usb_probe(struct usb_interface *interface,
                     const struct usb_device_id *id)
{
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i;

    printk(KERN_INFO "USB Device Connected\n");

    /* Get interface descriptor */
    iface_desc = interface->cur_altsetting;

    printk(KERN_INFO "Number of endpoints = %d\n",
           iface_desc->desc.bNumEndpoints);

    /* Loop through endpoints */
    for (i = 0; i < iface_desc->desc.bNumEndpoints; i++) {

        endpoint = &iface_desc->endpoint[i].desc;

        printk(KERN_INFO "Endpoint[%d]\n", i);

        printk(KERN_INFO "Endpoint Address = 0x%x\n",
               endpoint->bEndpointAddress);

        printk(KERN_INFO "Attributes = 0x%x\n",
               endpoint->bmAttributes);

        printk(KERN_INFO "Max Packet Size = %d\n",
               endpoint->wMaxPacketSize);

        printk(KERN_INFO "Interval = %d\n",
               endpoint->bInterval);
    }

    return 0;
}

static void usb_disconnect(struct usb_interface *interface){
       printk(KERN_INFO "SanDisk USB device disconnected\n");
}


/* table of devices that work with this driver */
static struct usb_device_id usb_table [] = {
 { USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
        { }                      /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, usb_table);



static struct usb_driver usb_drv = {
        .name        = "skeleton",
        .probe       = usb_probe,
        .disconnect  = usb_disconnect,
        .id_table    = usb_table,
};


static int hello_init(void)
{
    int result;
    printk(KERN_INFO"hello world module initilized\n");
    result = usb_register(&usb_drv);
    if (result < 0) {
        printk("usb_register failed for while register");
                return -1;
        }
    return 0;
}

static void hello_exit(void)
{
     printk(KERN_INFO"hello world module exit\n");
     usb_deregister(&usb_drv);
}

module_init(hello_init);
module_exit(hello_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Naveen");
MODULE_DESCRIPTION("Simple USB Driver");

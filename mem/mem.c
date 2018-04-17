#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/vmalloc.h>
#include <linux/mm.h>

int mem_test_init(void) 
{
    char *big = NULL;

    printk(KERN_INFO "Loading Module\n");

    printk(KERN_INFO "VMALLOC_START = %08X\n"
                     "VMALLOC_END = %08X\n"
                     "VMALLOC RANGE SIZE = %08X\n", 
        (unsigned int)VMALLOC_START, 
        (unsigned int)VMALLOC_END,
        (unsigned int)VMALLOC_END - (unsigned int)VMALLOC_START);

    big = vmalloc(1024 * 1024 * 10);
    if (NULL == big) {
        printk(KERN_INFO "vmalloc failed.\n");
        return -1;
    }
    printk(KERN_INFO "big = %08X.  __pa(big) = %08X\n", 
        (unsigned int)big, (unsigned int)__pa(big));

    vfree(big);
    big = NULL;

    return 0;
}

void mem_test_exit(void) 
{
    printk(KERN_INFO "Removing Module\n");

    return;
}

module_init(mem_test_init);
module_exit(mem_test_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Mem Module");
MODULE_AUTHOR("LYJ");

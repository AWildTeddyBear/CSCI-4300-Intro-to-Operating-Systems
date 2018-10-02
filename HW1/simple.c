#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/types.h>

// This function is called when the module is loaded.
int module_init(void) {
    printk(KERN_INFO "Loading Birthday Module\n");

    // Define our struct
    struct birthday {
        int day
        int month
        int year;
        struct list_head list; // list_head defined in <linux/types.h> [next, prev]
    };

    // Declare our object to use (birthday_list)[typeof: struct list_head]
    static LIST_HEAD(birthday_list); // struct list_head birthday_list;

    // Create our first birthday object
    struct birthday *person;

    // Allocate memory
    person = kmalloc(sizeof(*person), GFP_KERNEL); // GFP_KERNEL flag [may sleep]
    if (!person) { printk(KERN_ALERT "Could not allocate memory for person! Exiting!\n"); return 0; } // Make sure our memory got allocated
    person -> day = 2;
    person -> month = 8;
    person -> year = 1995;
    
    // Initalizes list member in struct birthday (make current value the head of the linked list)
    INIT_LIST_HEAD(&person -> list); // <linux/list.h>

    // Also initalize the tail of the linked list
    list_add_tail(&person -> list, &birthday_list); // <linux/list.h>

    // We need 4 more entires into the linked list
    struct birthday *person2;
    struct birthday *person3;
    struct birthday *person4;
    struct birthday *person5;

    // Allocate memory, and make sure of it
    person2 = kmalloc(sizeof(*person2), GFP_KERNEL); if (!person2) { printk(KERN_ALERT "Could not allocate memory for person2! Exiting!\n"); return 0; }
    person3 = kmalloc(sizeof(*person3), GFP_KERNEL); if (!person3) { printk(KERN_ALERT "Could not allocate memory for person3! Exiting!\n"); return 0; }
    person4 = kmalloc(sizeof(*person4), GFP_KERNEL); if (!person4) { printk(KERN_ALERT "Could not allocate memory for person4! Exiting!\n"); return 0; }
    person5 = kmalloc(sizeof(*person5), GFP_KERNEL); if (!person5) { printk(KERN_ALERT "Could not allocate memory for person4! Exiting!\n"); return 0; }

    // Populate our new entries with some data
    person2 -> day = 3; person2 -> month = 9; person2 -> year = 1996;
    person3 -> day = 4; person3 -> month = 10; person3 -> year = 1997;
    person4 -> day = 5; person4 -> month = 11; person4 -> year = 1998;
    person4 -> day = 6; person4 -> month = 12; person4 -> year = 1999;

    // Push our new entires back into the struct (birthday_list)
    list_add_tail(&person2 -> list, &birthday_list); // <linux/list.h>
    list_add_tail(&person3 -> list, &birthday_list); // <linux/list.h>
    list_add_tail(&person4 -> list, &birthday_list); // <linux/list.h>
    list_add_tail(&person5 -> list, &birthday_list); // <linux/list.h>

    // Print out our birthday linked list
    struct birthday *ptr; // Used for traversing the list [location]
    list_for_each_entry(ptr, &birthday_list, list) { // <linux/list.h>
        printk(KERN_INFO "Added Birthday! Day: %d\tMonth: %d\tYear: %d\n", ptr -> day, ptr -> month, ptr -> year);
    }

    return 0;
}

// This function is called when the module is removed.
void module_exit(void) {
    printk(KERN_INFO "Removing Module\n");
}

// Macros for registering module entry and exit points.
module init(simple init);
module exit(simple exit);
MODULE LICENSE("GPL");
MODULE DESCRIPTION("CSCI-4300 Birthday Kernel Module");
MODULE AUTHOR("AWildTeddyBear");
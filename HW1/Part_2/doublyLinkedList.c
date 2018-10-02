#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>

// Define our circular doubly linked list
struct Birthday_List {
    int day, month, year;
    struct Birthday_List *next, *prev;
};

// Create our new list
struct Birthday_List* list = NULL;

// Function to pushBack data for the list
void pushBack(struct Birthday_List *begin, int day, int month, int year) {
    // Create a new Birthday_List
    struct Birthday_List *newList;
    
    // Let's allocate some memory for newList and make sure it was successful
    newList = kmalloc(sizeof(*newList), GFP_KERNEL); if (!newList) { printk(KERN_ALERT "Could not allocate memory for newList! Exiting!\n"); return; }

    // If our list is empty, let's make a new head with some data
    if (begin == NULL) {
        // Allocate memory
        begin = kmalloc(sizeof(*begin), GFP_KERNEL); if (!begin) { printk(KERN_ALERT "Could not allocate memory for begin! Exiting!\n"); return; }

        // Make it circular!
        newList -> next = newList;
        newList -> prev = newList;

        // Fill some data
        newList -> day = day;
        newList -> month = month;
        newList -> year = year;

        // Assign our newly made Birthday_List back
        begin = newList;
    } else {
        // List isn't empty

        // Find the last Birthday_List element
        Birthday_List *end = begin -> prev;

        // Make it circular!
        newList -> next = begin;
        newList -> prev = end;

        // Fill some data
        newList -> day = day;
        newList -> month = month;
        newList -> year = year;

        // Circular
        end -> next = newList;
        begin -> prev = newList;
    }
}

// Pops the last element out of the list (deletes)
void pop(struct Birthday_List *inputList) {
    // Set our previous node's front element to be the element in front of us
    inputList -> prev -> next = inputList -> next;
    
    // Set our next node's previous element to be the element behind us
    inputList -> next -> prev = inputList -> prev;
}

// Function for iterating fowards in the list
void print_iterate_fowards(struct Birthday_List** begin) {
    // Let's allocate some memory for newList and make sure it was successful
    begin = kmalloc(sizeof(*begin), GFP_KERNEL); if (!begin) { printk(KERN_ALERT "Could not allocate memory for begin! Exiting!\n"); return; }

    printk(KERN_INFO "Iterating Fowards!\n");
    
    // Point to the starting element in the list
    struct Birthday_List *temp = begin;

    // Iterate through our list until we hit where we started
    while (temp -> next != begin) {
        printk(KERN_INFO "Added Birthday! Day: %d\tMonth: %d\tYear: %d\t\n", temp -> day, temp -> month, temp -> year);
        
        // Make sure we go fowards
        temp = temp -> next;
    }
}

// Function for iterating backwards in the list
void deletePrint_backwards(struct Birthday_List** begin) {
    // Let's allocate some memory for newList and make sure it was successful
    begin = kmalloc(sizeof(*begin), GFP_KERNEL); if (!begin) { printk(KERN_ALERT "Could not allocate memory for begin! Exiting!\n"); return; }

    printk(KERN_INFO "Iterating Backwards!\n");

    // Point to the last element in the list as our start
    struct Birthday_List *end = begin -> prev;

    while (end -> prev != NULL) {
        printk(KERN_INFO "Deleted Birthday! Day: %d\tMonth: %d\tYear: %d\t\n", end -> day, end -> month, end -> year);

        // Pop element out
        pop(end);

        // Make sure we go backwards
        end = end -> prev;
    }
}

// This function is called when the module is loaded.
static int __init doubly_init(void) {
    printk(KERN_NOTICE "Loading Birthday Module\n");

    // Begin to insert our data now
    pushBack(&list, 2, 8, 1995);
    pushBack(&list, 3, 9, 1996);
    pushBack(&list, 4, 10, 1997);
    pushBack(&list, 5, 11, 1998);
    pushBack(&list, 6, 12, 1999);

    // Print our list!
    print_iterate_fowards(&list);

    return 0;
}

// This function is called when the module is removed.
static void __exit doubly_exit(void) {
    printk(KERN_NOTICE "Removing Birthday Module\n");

    // Print our list as we delete it
    deletePrint_backwards(&list);
}

// Macros for registering module entry and exit points.
module_init(doubly_init);
module_exit(doubly_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CSCI-4300 Birthday Kernel Module");
MODULE_AUTHOR("AWildTeddyBear");
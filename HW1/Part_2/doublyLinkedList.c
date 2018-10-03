/*
    [Proof of ownership]
    File hash of file submitted for this project: F6F0116355025F397A4FAE15D21102417FB0FAB25F248E8E63C91548043D7B8B69CF89EE7C39A4533CFF47E132CCBB498443428BCE6B95F4BC63944D1E9F57A9
    [Proof of ownership]
*/

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

// Create a global struct to use
struct Birthday_List* first = NULL;

// Function for deleting a node out of the list
void DeleteFromList(struct Birthday_List* toDelete) {
    // Make sure that neither of our inputs are empty lists
    if (!first || !toDelete) return;

    // If our first and delete struct are equal then iterate forwards once
    if (first == toDelete) first = toDelete -> next;

    // If our next exists, then unlink next
    if (toDelete -> next) {
        // Check if we reduce the list to 1 after this delete call
        if (toDelete -> next == first) {
            first -> prev = NULL;
        } else {
            toDelete -> next -> prev = toDelete -> prev;
        }
    }

    // If our prev exists, then unlink prev
    if (toDelete -> prev) {
        // Check if we reduce the list to 1 after this delete call
        if (toDelete -> prev == first) {
            first -> next = NULL;
        } else {
            toDelete -> prev -> next = toDelete -> next;
        }
    }

    // Debug info out to kernel logs
    printk(KERN_INFO "Removing Birthday Day: %d\tMonth: %d\tYear: %d\n", toDelete -> day, toDelete -> month, toDelete -> year);

    // Actually delete the struct we wanted since it was entirely isolated now
    kfree(toDelete);
    return;
}

// Function for pushing a new element node into the list
void AddToList(int day, int month, int year) {
    // Allocate memory and check
    struct Birthday_List* new_node = kmalloc( sizeof( struct Birthday_List ), GFP_KERNEL );

    if (!new_node) { printk(KERN_ALERT "Could not allocate memory for new_node! Exiting!\n"); return; }

    // Insert some data into our new struct
    new_node -> day = day;
    new_node -> month = month;
    new_node -> year = year;
    new_node -> prev = NULL;
    new_node -> next = NULL;

    // Link node with the rest of the list
    if (first) {
        new_node -> next = first; // We are at the end of the list for the new element so the next should be the start again

        if (first -> prev) {
            new_node -> prev = first -> prev; // Our previous is the former last element
            first -> prev -> next = new_node; // The former last elements next is no the beginning of the list anymore but the new last element
        } else {
            new_node -> prev = first; // In case we add the second element, they both need to link to each other
            first -> next = new_node;
        }

        first -> prev = new_node; // The new one is the end of the list now.
    } else {
        // He is the first one in the list.
        first = new_node;
    }

    // Debug info out to kernel logs
    printk(KERN_INFO "Adding Birthday Day: %d\tMonth: %d\tYear: %d\n", new_node -> day, new_node -> month, new_node -> year);
}

// This function is called when the module is loaded.
static int __init doubly_init(void) {
    printk(KERN_NOTICE "Loading Birthday Module\n");

    // Insert example data
    AddToList(2, 8, 1995);
    AddToList(3, 9, 1996);
    AddToList(4, 10, 1997);
    AddToList(5, 11, 1998);
    AddToList(6, 12, 1999);

    return 0;
}

// This function is called when the module is removed.
static void __exit doubly_exit(void) {
    printk(KERN_NOTICE "Removing Birthday Module\n");

    struct Birthday_List* currentElement = first -> prev;

    // Delete our list in reverse order, as long as we find a previous entry to go to
    while (currentElement) {
        struct Birthday_List* goToNext = currentElement -> prev;

        // Delete the previous node (as we want reverse order)
        DeleteFromList(currentElement);

        currentElement = goToNext;
    }
}

// Macros for registering module entry and exit points.
module_init(doubly_init);
module_exit(doubly_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CSCI-4300 Birthday Kernel Module Part 2");
MODULE_AUTHOR("AWildTeddyBear"); // https://github.com/AWildTeddyBear
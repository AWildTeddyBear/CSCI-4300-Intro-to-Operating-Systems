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
struct Birthday_List* head = NULL; 

// Function for deleting a node out of the list
void deleteNodeSafe(struct Birthday_List **head_ref, struct Birthday_List *del) {
    // Make sure that neither of our inputs are empty lists
    if (*head_ref == NULL || del == NULL) return;

    // If our head and delete struct are equal then iterate fowards once
    if (*head_ref == del) *head_ref = del -> next;

    // If our next exists, then unlink next
    if (del -> next != NULL) del -> next -> prev = del -> prev;

    // If our prev exists, then unlink prev
    if (del -> prev != NULL) del -> prev -> next = del -> next; 

    // Debug info out to kernel logs
    printk(KERN_INFO "Removing Birthday!\tDay: %d\tMonth: %d\tYear: %d\n", del -> day, del -> month, del -> year);

    // Actually delete the struct we wanted since it was entirely isolated now
    kfree(del);
    return; 
}      

// Function for pushing a new element node into the list
void pushBack(struct Birthday_List** head_ref, int day, int month, int year) {
    // Allocate memory and check
    struct Birthday_List* new_node = kmalloc(sizeof(struct Birthday_List), GFP_KERNEL);
    if (!new_node) { printk(KERN_ALERT "Could not allocate memory for new_node! Exiting!\n"); return; }

    // Insert some data into our new struct
    new_node -> day = day;
    new_node -> month = month;
    new_node -> year = year;

    // Make our previous pointer null
    new_node -> prev = NULL;

    // Make the next pointer the head
    new_node -> next = (*head_ref);

    // If our head exists, then we assign our new node to head's previous
    if ((*head_ref) != NULL) (*head_ref) -> prev = new_node;

    // Debug info out to kernel logs
    printk(KERN_INFO "Adding Birthday!\tDay: %d\tMonth: %d\tYear: %d\n", new_node -> day, new_node -> month, new_node -> year);

    // Link our head to our new node
    (*head_ref) = new_node;
} 

// This function is called when the module is loaded.
static int __init doubly_init(void) {
    printk(KERN_NOTICE "Loading Birthday Module\n");

    // Insert example data
    pushBack(&head, 2, 8, 1995);
    pushBack(&head, 3, 9, 1996);
    pushBack(&head, 4, 10, 1997);
    pushBack(&head, 5, 11, 1998);
    pushBack(&head, 6, 12, 1999);

    return 0;
}

// This function is called when the module is removed.
static void __exit doubly_exit(void) {
    printk(KERN_NOTICE "Removing Birthday Module\n");

    // Store the last element
    struct Birthday_List *temp;
    temp = head -> prev;

    // Cut the list
    head -> prev = NULL;

    // Delete our list in reverse order -- make sure that we don't access some pointer that's null (empty list)
    while (temp -> prev != NULL) {
        // Delete the previous node (as we want reverse order)
        deleteNodeSafe(&temp, temp);
        printk(KERN_INFO "Delete! Info Year: %d\n", temp -> year);

        // Roll back in the list
        temp = temp -> prev;
    }
}

// Macros for registering module entry and exit points.
module_init(doubly_init);
module_exit(doubly_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CSCI-4300 Birthday Kernel Module Part 2");
MODULE_AUTHOR("AWildTeddyBear");
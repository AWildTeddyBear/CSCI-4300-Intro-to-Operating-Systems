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
void deleteNode(struct Birthday_List **head_ref, struct Birthday_List *del) { 
    // Base case
    if (*head_ref == NULL || del == NULL) return;

    //
    if (*head_ref == del) *head_ref = del -> next;

    //
    if (del -> next != NULL) del -> next -> prev = del -> prev;

    //
    if (del -> prev != NULL) del -> prev -> next = del -> next; 

    printk(KERN_INFO "Removing Birthday!\tDay: %d\tMonth: %d\tYear: %d\n", del -> day, del -> month, del -> year);

    //
    kfree(del);
    return; 
}      

// Function for pushing back another node to the list with data
void pushBack(struct Birthday_List** head_ref, int day, int month, int year) {
    // Allocate memory and check
    struct Birthday_List* new_node = kmalloc(sizeof(struct Birthday_List), GFP_KERNEL);
    if (!new_node) { printk(KERN_ALERT "Could not allocate memory for new_node! Exiting!\n"); return; }

    //
    new_node -> day = day;
    new_node -> month = month;
    new_node -> year = year;

    //
    new_node -> prev = NULL;

    //
    new_node -> next = (*head_ref);

    //
    if ((*head_ref) != NULL) (*head_ref) -> prev = new_node;

    //
    printk(KERN_INFO "Adding Birthday!\tDay: %d\tMonth: %d\tYear: %d\n", new_node -> day, new_node -> month, new_node -> year);

    //
    (*head_ref) = new_node;
} 

// This function is called when the module is loaded.
static int __init doubly_init(void) {
    printk(KERN_NOTICE "Loading Birthday Module\n");

    // Begin to insert our data now
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

}

// Macros for registering module entry and exit points.
module_init(doubly_init);
module_exit(doubly_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CSCI-4300 Birthday Kernel Module");
MODULE_AUTHOR("AWildTeddyBear");
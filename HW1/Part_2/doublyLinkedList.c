#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>

// Define our circular doubly linked list
struct birthday_list {
    int day, month, year;
    struct birthday_list *next, *prev;
};

// Create our new list
struct birthday_list* list = NULL;
    
// Function to pushBack data for the list
void pushBack(struct birthday_list** begin, int day, int month, int year) {
    // If our list is empty, let's make a new head with some data
    if (*begin == NULL) {
        // Create a new birthday_list with data
        struct birthday_list* new = new birthday_list;
        new -> day = day;
        new -> month = month;
        new -> year = year;

        // Assign our newly made birthday_list back
        *begin = new;
    } else {
        // List isn't empty

        // Find the last birthday_list
        birthday_list *end = (*begin) -> prev;

        // Create our new birthday_list with data
        struct birthday_list* new = new birthday_list;
        new -> day = day;
        new -> month = month;
        new -> year = year;

        // Start will be next of new
        new -> next = *begin;

        // Make new previous of start
        (*begin) -> prev = new;

        // Make end prev of new
        new -> prev = end;

        // Make new next of old end
        end -> next = new;
    }
}

// Pops the last element out of the list (deletes)
void pop(struct birthday_list *list) {
    // Set our previous node's front element to be the element in front of us
    list -> prev -> next = list -> next;
    
    // Set our next node's previous element to be the element behind us
    list -> next -> prev = list -> prev;
}

// Function for iterating fowards in the list
void print_iterate_fowards(struct birthday_list** begin) {
    printk(KERN_INFO "Iterating Fowards!\n");
    
    // Point to the starting element in the list
    struct birthday_list *temp = begin;

    // Iterate through our list until we hit where we started
    while (temp -> next != begin) {
        printk(KERN_INFO "Added Birthday! Day: %d\tMonth: %d\tYear: %d\t\n", temp -> day, temp -> month, temp -> year);
        
        // Make sure we go fowards
        temp = temp -> next;
    }
}

// Function for iterating backwards in the list
void deletePrint_backwards(struct birthday_list** begin) {
    printk(KERN_INFO "Iterating Backwards!\n");

    // Point to the last element in the list as our start
    struct birthday_list *end = begin -> prev;

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
    print_iterate_fowards(list);

    return 0;
}

// This function is called when the module is removed.
static void __exit doubly_exit(void) {
    printk(KERN_NOTICE "Removing Birthday Module\n");

    // Print our list as we delete it
    deletePrint_backwards(list);
}

// Macros for registering module entry and exit points.
module_init(doubly_init);
module_exit(doubly_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CSCI-4300 Birthday Kernel Module");
MODULE_AUTHOR("AWildTeddyBear");
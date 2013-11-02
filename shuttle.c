#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/linkage.h>
#include <linux/export.h>

#define PROC_NAME "terminal"
MODULE_LICENSE("GPL");

extern long (*start_shuttle)(void);
extern long (*issue_request)(char p_type, int start_t, int end_t);
extern long (*stop_shuttle)(void);

#define CAPACITY 50
#define OFFLINE 0
#define RUNNING 1
#define DEACTIVATING 2

struct pass {
	char type;
	int dest;
	struct pass* next;
};

static struct pass terminal[5];
static int delivered[5];
static int shuttle_status = OFFLINE;


long m_start(void) { 
	printk( KERN_ALERT "Starting shuttle simulation\n");
	shuttle_status = RUNNING;
	return 0; 
}

long m_request(char p_type, int start_t, int end_t) {
	
	printk( KERN_ALERT "add p_type:%c to terminal:%i -> %i\n", p_type, start_t-1, end_t-1);

	if (start_t < 1 || start_t > 5 || end_t < 1 || end_t > 5 ) {
		printk( KERN_ALERT "invalid S or E term: %i, %i\n", start_t, end_t);
		return 1;
	}

	if(p_type != 'C' && p_type != 'A' && p_type != 'L') {
		printk( KERN_ALERT "invalid p_type: %c\n", p_type);
		return 1;
	}

	start_t--;
	end_t--;	

	printk( KERN_ALERT "--preparing for list traversal\n");

	struct pass *last_node = &(terminal[start_t]);

	if (last_node->next == NULL) {
		last_node->type = p_type;
		last_node->dest = end_t;
		last_node->next = kmalloc(sizeof(struct pass), GFP_KERNEL);

		last_node->next->type = '0';
		last_node->next->dest = 9;
		last_node->next->next = NULL;
		return 0;
	}

	while (last_node->next != NULL)	{	
		printk( KERN_ALERT "--next is %p\n", last_node->next);
		last_node = last_node->next;
	}

	last_node->next = kmalloc(sizeof(struct pass), GFP_KERNEL);

	if (last_node->next == NULL) {
		printk( KERN_ALERT "COULD NOT ALLOCATE PASSENGER\n");
		return 1;
	}

	last_node->next->type = p_type;
	last_node->next->dest = end_t;
	last_node->next->next = NULL;

	printk( KERN_ALERT "Saved p: %c d: %i\n\n", last_node->type, last_node->dest);

	// } else {
	// 	printk( KERN_ALERT "something unexpected has occured\n");
	// }
	return 0;
}

long m_stop(void) {
	shuttle_status = DEACTIVATING;
	int i =0;
	int j =0;
	struct pass *last_node;

	printk( KERN_ALERT "=======STOP CALLED: QUEUE OUTPUT=======\n");
	for(; i < 5; ++i) {
		printk(KERN_ALERT "terminal[%i,%i: %c, %i\n", i, j, terminal[i].type, terminal[i].dest);
		last_node = terminal[i].next;

		if (last_node == NULL){
			printk(KERN_ALERT "next was null: %p", last_node);
			continue;
		} else {
			while(last_node->next != NULL) {
				last_node = last_node->next;
				j++;
				printk(KERN_ALERT "\tterminal[%i,%i: %c, %i\n", i, j, last_node->type, last_node->dest);
			}
			j = 0;
		}
	}
	printk( KERN_ALERT "==============END OUTPUT==============\n");

	// kthread_should_stop or some shit
	return 0;
}

#include "seq_shiz.c"

int init_module(void) {
	struct proc_dir_entry *entry;
	int i = 0;
	
	entry = proc_create(PROC_NAME, 0, NULL, &my_file_ops);
	start_shuttle = m_start;
	issue_request = m_request;
	stop_shuttle = m_stop;

	for(; i < 5; ++i) {
		terminal[i].type = '0';
		terminal[i].dest = 9;
		terminal[i].next = NULL;
		delivered[i] = 0;
	}

	return 0;
}

/**
 * This function is called when the module is unloaded.
 *
 */
void cleanup_module(void) {
	start_shuttle = NULL;
	issue_request = NULL;
	stop_shuttle = NULL;
	remove_proc_entry(PROC_NAME, NULL);

}
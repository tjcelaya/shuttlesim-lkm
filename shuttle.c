#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/linkage.h>
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>

#define PROC_NAME "terminal"
MODULE_LICENSE("GPL");

extern long (*start_shuttle)(void);
extern long (*issue_request)(char p_type, int start_t, int end_t);
extern long (*stop_shuttle)(void);

#define CAPACITY 50
#define OFFLINE 0
#define DEACTIVATING 1
#define MOVING 2
#define PARKED 3
#define INITIAL_TERMINAL 2
#define INVAL_TERMINAL 9
#define CHILDSPC 1
#define ADULTSPC 2
#define LUGADSPC 4
#define CAPACITY 50
#define TEN_SEC 100
#define THREE_SEC 30
#define THRTY_SEC 300


/**passengers[terminal]*
 * CHILD ARRAY INDEX 0 *
 * ADULT ARRAY INDEX 1 *
 * A w L ARRAY INDEX 2 *
 ***********************/

struct pass {
	int type;
	int dest;
	struct pass* next;
};

typedef struct shuttle_t {
	int status;
	int passengers[5][3];
	int pos, dest, amtFilled;
} shuttle_t;

static shuttle_t shuttle;

static struct pass terminal[5];
static int delivered[5];
static int shuttle_status = OFFLINE;

static struct task_struct *shuttle_thread;

#include "simulate.c"

long m_start(void) { 
	int i, j;
	printk( KERN_ALERT "Starting shuttle simulation\n");

	if (shuttle.status == OFFLINE) {
		shuttle.status = PARKED;
		shuttle.pos = INITIAL_TERMINAL;
		shuttle.dest = -1;

		for (i = 0; i < 5; i++)
			for (j = 0; j < 3; j++) shuttle.passengers[i][j] = 0;

		shuttle_thread = kthread_run(simulate_shuttle, NULL, "shuttle");
	
		return 0;
	} else {
		printk( KERN_ALERT "shuttle not offline");
		return 1;
	}
}

long m_request(char p_type, int start_t, int end_t) {
	struct pass *last_node;
	int n_type = -1;	

	// printk( KERN_ALERT "add p_type:%c to terminal:%i -> %i\n", p_type, start_t-1, end_t-1);

	if (start_t < 1 || start_t > 5 || end_t < 1 || end_t > 5 ) {
		printk( KERN_ALERT "invalid S or E term: %i, %i\n", start_t, end_t);
		return 1;
	}

	if(p_type != 'C' && p_type != 'A' && p_type != 'L') {
		printk( KERN_ALERT "invalid p_type: %c\n", p_type);
		return 1;
	}

	if (p_type == 'C') n_type = 0;
	else if (p_type == 'A') n_type = 1;
	else if (p_type == 'L') n_type = 2;

	start_t--;
	end_t--;	

	last_node = terminal[start_t].next;

	printk( KERN_ALERT "checking if first addition\n");
	if (last_node == NULL) {
		
		
		printk( KERN_ALERT "first addition\n");
		
		terminal[start_t].next = kmalloc(sizeof(struct pass), GFP_KERNEL);

		(terminal[start_t].next)->type = n_type;
		(terminal[start_t].next)->dest = end_t;
		(terminal[start_t].next)->next = NULL;
		return 0;
	}
	printk( KERN_ALERT "it wasnt\n");

	while (last_node->next != NULL)	{	
		// printk( KERN_ALERT "--next is %p\n", last_node->next);
		last_node = last_node->next;
	}

	last_node->next = kmalloc(sizeof(struct pass), GFP_KERNEL);

	if (last_node->next == NULL) {
		printk( KERN_ALERT "COULD NOT ALLOCATE PASSENGER\n");
		return 1;
	}

	last_node->next->type = n_type;
	last_node->next->dest = end_t;
	last_node->next->next = NULL;

	// printk( KERN_ALERT "Saved p: %i d: %i\n\n", last_node->type, last_node->dest);

	return 0;
}

long m_stop(void) {
	int i =0;
	int j =0;
	struct pass *last_node;
	kthread_stop(shuttle_thread);
/*
	printk( KERN_ALERT "=======STOP CALLED: QUEUE OUTPUT=======\n");
	for(; i < 5; ++i) {
		printk(KERN_ALERT "terminal[%i,%i: %i, %i\n", i, j, terminal[i].type, terminal[i].dest);
		last_node = terminal[i].next;

		if (last_node == NULL){
			// printk(KERN_ALERT "next was null: %p", last_node);
			continue;
		} else {
			while(last_node->next != NULL) {
				last_node = last_node->next;
				j++;
				printk(KERN_ALERT "\tterminal[%i,%i: %i, %i\n", i, j, last_node->type, last_node->dest);
			}
			j = 0;
		}
	}
	printk( KERN_ALERT "==============END OUTPUT==============\n");
*/
	return 0;
}

#include "seq_shiz.c"

int init_module(void) {
	struct proc_dir_entry *entry;
	int i = 0;
	
	shuttle.status = OFFLINE;
	shuttle.amtFilled = 0;

	entry = proc_create(PROC_NAME, 0, NULL, &my_file_ops);
	start_shuttle = m_start;
	issue_request = m_request;
	stop_shuttle = m_stop;

	for(i = 0; i < 5; ++i) {
		terminal[i].type = -1;
		terminal[i].dest = -1;
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
//	struct pass *node, *temp;
//	int i =0;
	
	start_shuttle = NULL;
	issue_request = NULL;
	stop_shuttle = NULL;

	remove_proc_entry(PROC_NAME, NULL);

}

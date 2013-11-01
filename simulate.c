// #define CAPACITY 50
// #define OFFLINE 0
// #define DEACTIVATING 1
// #define MOVING 2
// #define PARKED 3
// #define INITIAL_TERMINAL 2
// #define INVAL_TERMINAL 9
// #define CHILDSPC 1
// #define ADULTSPC 2
// #define LUGADSPC 4
// #define CAPACITY 50
// #define TEN_SEC 100
// #define THREE_SEC 30
// #define THRTY_SEC 300


// *passengers[terminal]*
//  * CHILD ARRAY INDEX 0 *
//  * ADULT ARRAY INDEX 1 *
//  * A w L ARRAY INDEX 2 *
//  **********************

// struct pass {
// 	int type;
// 	int dest;
// 	struct pass* next;
// };

// typedef struct shuttle_t {
// 	int status;
// 	int passengers[5][3];
// 	int pos, dest, amtFilled;
// } shuttle_t;

// static shuttle_t shuttle;

// static struct pass terminal[5];
// static int delivered[5];
// static int shuttle_status = OFFLINE;

int calculatePriority(void) {

	return (shuttle.pos + 1) % 5;

}


void printStats(void) {
	int i = 0, j = 0;
	struct pass* last_node = NULL;
	printk( KERN_ALERT "SHUTTLE status = %i\n            pos = %i\n            cap = %i\n", shuttle.status, shuttle.pos, CAPACITY - shuttle.amtFilled);
	printk( KERN_ALERT "Passengers\n");

	for(i = 0; i < 5; ++i)
		printk( KERN_ALERT "d:%i   C %i A %i L %i\n", i, shuttle.passengers[i][0], shuttle.passengers[i][1], shuttle.passengers[i][2]);
/*
	printk( KERN_ALERT "=======STATS CALLED: QUEUE OUTPUT=======\n");
	for(i = 0; i < 5; ++i) {
		printk(KERN_ALERT "terminal[%i,%i: %i, %i\n", i, j, terminal[i].type, terminal[i].dest);
		last_node = terminal[i].next;

		if (last_node == NULL){
			// printk(KERN_ALERT "next was null: %p", last_node);
			continue;
		} else {
			while(last_node != NULL) {
				printk(KERN_ALERT "\tterminal[%i,%i: %i, %i\n", i, j, last_node->type, last_node->dest);
				last_node = last_node->next;
				j++;
			}
			j = 0;
		}
	}
	printk( KERN_ALERT "==============END OUTPUT==============\n");
*/
}

int simulate_shuttle(void *d) {

    int i, size = -1, unload = 0;
    int amtDropped = 0, amtPicked = 0;
    struct pass *cursor = NULL;
    struct pass *temp = NULL;

    printStats();
	
	int steps = 20;

	while (steps--) {
		
		if (shuttle.dest != -1)
			shuttle.pos = shuttle.dest;

		if (shuttle.status != DEACTIVATING) shuttle.status = PARKED;  //park at the terminal
		
		amtDropped = 0;

		if (shuttle.amtFilled > 0) {

			if(shuttle.passengers[shuttle.pos][0] > 0) {
				shuttle.amtFilled -= (shuttle.passengers[shuttle.pos][0] * CHILDSPC);
				amtDropped += shuttle.passengers[shuttle.pos][0];
				delivered[shuttle.pos] += shuttle.passengers[shuttle.pos][0];
				shuttle.passengers[shuttle.pos][0] = 0;
			}
			if(shuttle.passengers[shuttle.pos][1] > 0) {
				shuttle.amtFilled -= (shuttle.passengers[shuttle.pos][1] * ADULTSPC);
				amtDropped += shuttle.passengers[shuttle.pos][1];
				delivered[shuttle.pos] += shuttle.passengers[shuttle.pos][1];
				shuttle.passengers[shuttle.pos][1] = 0;
			}
			if(shuttle.passengers[shuttle.pos][2] > 0) {
				shuttle.amtFilled -= (shuttle.passengers[shuttle.pos][2] * LUGADSPC);
				amtDropped += shuttle.passengers[shuttle.pos][2];
				delivered[shuttle.pos] += shuttle.passengers[shuttle.pos][2];
				shuttle.passengers[shuttle.pos][2] = 0;
			}
	
		}

		amtPicked = 0;
	
		if (shuttle.status != DEACTIVATING) {
			//load passengers

			if(terminal[shuttle.pos].next != NULL) {
				cursor = terminal[shuttle.pos].next;
				
				if (cursor != NULL && cursor->next == NULL) {
					switch (cursor->type) {
						case 0 : size = CHILDSPC;
									 break;
						case 1 : size = ADULTSPC;
									 break;
						case 2 : size = LUGADSPC;
									 break;
						default: size = -1;
					}

					if (size != -1 && size + shuttle.amtFilled <= 50) {

						shuttle.amtFilled += size;
						shuttle.passengers[shuttle.pos][cursor->type]++;
						
						kfree(cursor);
						terminal[shuttle.pos].next = NULL;
						
						amtDropped++;
					}

				} else {
					
					while( shuttle.amtFilled < 50 && cursor->next->next != NULL) {
						while(cursor->next != NULL) { 
							//go to the end of the list
							temp = cursor;
							cursor = cursor->next;
						}

						switch (cursor->type) {
							case 0 : size = CHILDSPC;
										 break;
							case 1 : size = ADULTSPC;
										 break;
							case 2 : size = LUGADSPC;
										 break;
							default: size = -1;
						}

						if (size != -1 && size + shuttle.amtFilled <= 50) {

							shuttle.amtFilled += size;
							shuttle.passengers[cursor->dest][cursor->type]++;
							
							kfree(cursor);
							temp->next = NULL;
						}

						cursor = terminal[shuttle.pos].next;
					}
				}
			}
		}

		shuttle.dest = calculatePriority();
		printk( KERN_ALERT "--------NEXT DESTINATION = %i ----------", shuttle.dest);
		printStats();

		if ( (amtDropped + amtPicked) > 4) 
			msleep(TEN_SEC + (THREE_SEC * (amtDropped + amtPicked)));
		else msleep(TEN_SEC);

		shuttle.status = MOVING;

		msleep(abs(shuttle.pos - shuttle.dest) * THRTY_SEC);

	}



	printk( KERN_ALERT "setting offline");
	shuttle.status = OFFLINE;	



	return 0;
}

void otherFunc(void) {
	printk( KERN_ALERT "SHUTTLE status = %i and pos = %i\n", shuttle.status, shuttle.pos);
}

int simulate_shuttle(void *d) {
	otherFunc();
	return 0;
}

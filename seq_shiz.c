/**
 * This function is called at the beginning of a sequence.
 * ie, when:
 *- the /proc file is read (first time)
 *- after the function stop (end of sequence)
 *
 */

 static void *my_seq_start(struct seq_file *s, loff_t *pos) {
  static unsigned long counter = 0;

  /* beginning a new sequence ? */
  if ( *pos == 0 )
    {
      /* yes => return a non null value to begin the sequence */
      return &counter;
    }
  else
    {
      /* no => it's the end of the sequence, return end to stop reading */
      *pos = 0;
      return NULL;
    }
}

/**
 * This function is called after the beginning of a sequence.
 * It's called untill the return is NULL (this ends the sequence).
 *
 */
static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos) {
  unsigned long *tmp_v = (unsigned long *)v;
  (*tmp_v)++;
  (*pos)++;
  return NULL;
}

/**
 * This function is called at the end of a sequence
 * 
 */
static void my_seq_stop(struct seq_file *s, void *v) {
  /* nothing to do, we use a static value in start() */
}

/**
 * This function is called for each "step" of a sequence
 *
 */
static int my_seq_show(struct seq_file *s, void *v) {
  // loff_t *spos = (loff_t *) v;
  
  int total_delivered = 0, i =0, q_adultsL=0, q_adults=0, q_childs=0;
  struct pass *cursor = NULL;

  seq_printf(s, "Status: ");
  switch(shuttle.status) {
    case OFFLINE:
      seq_printf(s, "OFFLINE\n");
      break;
    case DEACTIVATING:
      seq_printf(s, "DEACTIVATING\n");
      break;
    case MOVING:
      seq_printf(s, "MOVING\n");
      break;
    case PARKED:
      seq_printf(s, "PARKED\n");
      break;
    default:
      seq_printf(s, "OFFLINE\n");
      break;
  }


  seq_printf(s, "Seats: %i used %i available\n", shuttle.amtFilled/2 , 50 - shuttle.amtFilled/2);
  
  int pC=0, pA=0, pL=0, total=0;
  
	for (i=0; i<5; ++i) {
		pC = shuttle.passengers[i][0];
		pA = shuttle.passengers[i][1];
		pL = shuttle.passengers[i][2];
		total += shuttle.passengers[i][0] + shuttle.passengers[i][1] + shuttle.passengers[i][2];
	}


  seq_printf(s, "Passengers: %i (%i adult with luggage, %i adult without luggage, %i children)\n", pC, pA, pL, total);
  seq_printf(s, "Location: %i\n", shuttle.pos+1);
  seq_printf(s, "Destination: %i\n", shuttle.dest+1);

  for(i =0; i<5; ++i) {
    total_delivered += delivered[i];
  }

  seq_printf(s, "Delivered: %i (%i adult with luggage, %i adult without luggage, %i children)\n", total_delivered, 0, 0, 0);
  seq_printf(s, "\n");
  for(i =0; i<5; ++i) {

    if (terminal[i].next != NULL) {
      cursor = terminal[i].next;

      while (cursor != NULL) {
        if (cursor->type == 0) { q_childs++;  }
        else if (cursor->type == 1) { q_adults++;  }
        else if (cursor->type == 2) { q_adultsL++;  }
        cursor = cursor->next;
      }

    }
    seq_printf(s, 
      "Terminal %i: %i adult w/ luggage, %i adult w/o luggage, %i children in queue. %i passengers delivered so far\n",
      i+1,
      q_adultsL,
      q_adults,
      q_childs,
      delivered[i]
    );
    q_childs = q_adults = q_adultsL = 0;
  }
  return 0;
}

/**
 * This structure gather "function" to manage the sequence
 *
 */
static struct seq_operations my_seq_ops = {
  .start = my_seq_start,
  .next  = my_seq_next,
  .stop  = my_seq_stop,
  .show  = my_seq_show
};

/**
 * This function is called when the /proc file is open.
 *
 */
static int my_open(struct inode *inode, struct file *file) {
  return seq_open(file, &my_seq_ops);
};

static struct file_operations my_file_ops = {
  .owner   = THIS_MODULE,
  .open    = my_open,
  .read    = seq_read,
  .llseek  = seq_lseek,
  .release = seq_release
};

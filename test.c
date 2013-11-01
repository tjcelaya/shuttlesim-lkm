#include <unistd.h>

int main() {
	syscall(352, 'C', 1, 1);
	syscall(352, 'L', 2, 1);
	syscall(352, 'L', 2, 1);
	syscall(352, 'L', 2, 1);
	syscall(352, 'L', 2, 1);
	syscall(352, 'L', 2, 1);
	syscall(352, 'L', 2, 1);
	syscall(352, 'L', 2, 1);
	syscall(352, 'L', 2, 1);
	syscall(352, 'L', 2, 1);
	syscall(352, 'L', 2, 1);
	syscall(352, 'L', 2, 1);
	syscall(352, 'A', 3, 1);
	syscall(352, 'L', 3, 1);
	syscall(352, 'C', 3, 1);
	syscall(352, 'L', 2, 1);
	syscall(352, 'C', 3, 1);
	syscall(352, 'C', 3, 1);
	syscall(352, 'C', 3, 1);
	syscall(352, 'C', 3, 1);
	syscall(352, 'C', 3, 1);
	syscall(352, 'C', 3, 1);
	syscall(352, 'C', 3, 1);
	syscall(352, 'C', 3, 1);
	syscall(352, 'C', 3, 1);
	syscall(352, 'C', 4, 1);
	syscall(352, 'A', 5, 2);
	syscall(352, 'A', 5, 2);
	syscall(352, 'A', 5, 2);
	syscall(352, 'A', 5, 2);
	syscall(352, 'A', 5, 2);
	syscall(352, 'A', 5, 2);
	syscall(352, 'A', 5, 2);
	syscall(352, 'A', 5, 2);
	syscall(352, 'A', 5, 2);
	syscall(352, 'A', 5, 2);
	syscall(352, 'A', 5, 2);
	syscall(352, 'A', 5, 2);
	syscall(352, 'A', 5, 2);
	syscall(352, 'A', 5, 2);
	syscall(352, 'L', 3, 1);
	syscall(352, 'A', 4, 4);
	syscall(352, 'C', 4, 5);
	

	syscall(351);
	// syscall(353);         <- DANGER
	sleep(5);
}

#include <unistd.h>

int main() {
	syscall(352, 'C', 3, 1);
	syscall(352, 'C', 3, 1);
	syscall(352, 'A', 3, 2);
	syscall(352, 'L', 3, 1);
	syscall(352, 'A', 3, 4);
	syscall(352, 'C', 3, 5);
	
	sleep(2);

	syscall(351);
	sleep(5);
	syscall(353);
}

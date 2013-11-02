#include <unistd.h>

int main() {
	syscall(351);
	syscall(352, 'A', 1, 2);
	syscall(352, 'C', 1, 3);
	syscall(352, 'L', 1, 4);
	syscall(352, 'A', 1, 4);
	syscall(352, 'C', 1, 4);
	syscall(352, 'L', 1, 4);
	syscall(352, 'L', 2, 4);
	syscall(352, 'L', 2, 4);
	syscall(352, 'L', 2, 4);
	syscall(352, 'A', 3, 4);
	syscall(352, 'A', 3, 4);
	syscall(352, 'A', 3, 4);
	syscall(352, 'L', 3, 4);
	syscall(353);
}

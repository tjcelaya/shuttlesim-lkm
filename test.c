#include <unistd.h>

int main() {
	syscall(351);
	syscall(352, 'L', 2, 1);
	syscall(352, 'L', 2, 3);
	syscall(352, 'L', 2, 4);
	syscall(352, 'A', 3, 1);
	syscall(352, 'C', 3, 1);
	syscall(352, 'L', 3, 1);
	syscall(353);
}

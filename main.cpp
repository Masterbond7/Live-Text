// Including general files
#include <iostream>
#include <errno.h>
#include <string.h>

// Including inotify files
#include <sys/inotify.h>

// Setting namespace
using namespace std;

// Setting template strings
const char TEMPLATE_ERROR[] = "\033[31m[ERROR]\033[0m";

// Main function
int main() {
	while (true){
	// Start an inotify instance
	int fd = inotify_init();
	if (fd == -1) {
		printf("%s Inotify initialization error\nErrno %i\n", TEMPLATE_ERROR, errno);
		return errno;
	}}

	// Exiting the program successfully
	return 0;
}

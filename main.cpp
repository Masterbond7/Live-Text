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
	// Start an inotify instance
	int fd = inotify_init(); // File descriptor
	if (fd == -1) {
		printf("%s Inotify initialization error\nErrno %i, (%s)\n", TEMPLATE_ERROR, errno, strerror(errno));
		return errno;
	}

	// Add an inotify watch
	int wd = inotify_add_watch(fd, "/home/james/Desktop/", IN_ACCESS | IN_CLOSE_WRITE | IN_CLOSE_NOWRITE | IN_CREATE | IN_DELETE | IN_MODIFY | IN_OPEN); // Watch descriptor
	if (wd == -1) {
		printf("%s Inotify watch start error\nErrno %i, (%s)\n", TEMPLATE_ERROR, errno, strerror(errno));
		return errno;
	}

	// Exiting the program successfully
	return 0;
}

// Including general files
#include <iostream>
#include <errno.h>
#include <string.h>
#include <unistd.h>

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
	int wd = inotify_add_watch(fd, "/home/james/Desktop/", IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO); // Watch descriptor 
    //(may need to add IN_CLOSE_WRITE)
	if (wd == -1) {
		printf("%s Inotify watch start error\nErrno %i, (%s)\n", TEMPLATE_ERROR, errno, strerror(errno));
		return errno;
	}

	// Read events from file descriptor
	char buf[4096];
	ssize_t len = read(fd, buf, sizeof(buf));
	const struct inotify_event *event;
	if (len == -1) {
		printf("%s Inotify read\nErrno %i, (%s)\n", TEMPLATE_ERROR, errno, strerror(errno));
	} else {
		printf("File event detected\n");
	}

	// Interpret events from buffer
	for (char *ptr = buf; ptr < buf + len; ptr += sizeof(struct inotify_event) + event->len) {
		event = (const struct inotify_event *) ptr;
		cout << event->mask << endl;
	}

	// Exiting the program successfully
	return 0;
}

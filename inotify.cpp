// Including files
#include <iostream>
#include <poll.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>

// Setting namespace
using namespace std;

// Setting template strings
const char TEMPLATE_ERROR[] = "\033[31m[ERROR]\033[0m";

// Main function
int main() {
	// Start an inotify instance
	printf("Starting inotify instance\n");
	int fd = inotify_init(); // File descriptor
	if (fd == -1) {
		printf("%s Inotify initialization error\nErrno %i, (%s)\n", TEMPLATE_ERROR, errno, strerror(errno));
		return errno;
	}

	// Add an inotify watch
	printf("Adding inotify watch\n");
	int wd = inotify_add_watch(fd, "/home/james/Desktop/", IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO); // Watch descriptor
	if (wd == -1) {
		printf("%s Inotify watch start error\nErrno %i, (%s)\n", TEMPLATE_ERROR, errno, strerror(errno));
		return errno;
	}

	// Prepare for polling
	nfds_t nfds;
	struct pollfd fds[1];

	fds[0].fd = fd;
	fds[0].events = POLLIN;
	nfds = 1;

	// Read events from file descriptor
	printf("Reading events\n");
	while (true) {
		// If an event is available, read and interpret the event
		if (poll(fds, nfds, -1)) {
			// Read the amount of bytes available
			int bytes_available;
			if (ioctl(fd, FIONREAD, &bytes_available) == -1) {
				printf("%s Polling bytes available\nErrno %i, (%s)\n", TEMPLATE_ERROR, errno, strerror(errno));
				return errno;
			}

			// Make the buffer only as large as it needs to be
			char buf[bytes_available] __attribute__ ((aligned(__alignof__(struct inotify_event)))); // Make buffer and align (for good measure)
			ssize_t len = read(fd, buf, sizeof(buf));
			if (len == -1) {
				printf("%s Inotify read\nErrno %i, (%s)\n", TEMPLATE_ERROR, errno, strerror(errno));
				return errno;
			}

			// Interpret events from buffer
			const struct inotify_event *event;
			for (char *ptr = buf; ptr < buf + len; ptr += sizeof(struct inotify_event) + event->len) {
				event = (const struct inotify_event *) ptr;
				if (event->mask & IN_CREATE) { printf("IN_CREATE: %s\n", event->name); }
				else if (event->mask & IN_DELETE) { printf("IN_DELETE: %s\n", event->name); }
				else if (event->mask & IN_MODIFY) { printf("IN_MODIFY: %s\n", event->name); }
				else if (event->mask & IN_MOVED_FROM) { printf("IN_MOVED_FROM: %s, %i\n", event->name, event->cookie); }
				else if (event->mask & IN_MOVED_TO) { printf("IN_MOVED_TO: %s %i\n", event->name, event->cookie); }
			}
		}
	}

	// Exiting the program successfully
	return 0;
}

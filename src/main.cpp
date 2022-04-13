#include <iostream>
#include <regex>
#include <filesystem>
#include <thread>
#include <poll.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <future>
#include <signal.h>

#include "config.hpp" 

// Defining global variables
int exit_code = 0;
bool running = true;
char* target_path = NULL;
char* target_ip = NULL;

// Declaring functions
void sigint_handler(int sig_num);
void fs_updates();

// Main function
int main(int argc, char **argv) {
    // Set signal handler for SIGINT
    signal(SIGINT, sigint_handler);

	// Parse inputs
	for (int i = 1; i < argc; i++) {
		// Handle -h (help) argument
		if (std::string(argv[i]) == "-h") {
			// Print help text and exit successfully
			printf("%s\n", config_header::HELP_TEXT); 
			return 0;
		}

		// Handle IP input
		else if (std::regex_match(std::string(argv[i]), config_header::REGEX_IP)) {
			// Say IP-like input was detected and check if it is valid (numbers 0-255)
			printf("IP detected (%s); ", argv[i]);
			// If target_ip was already set exit and inform the user
			if (target_ip != NULL) {
				printf("IP was already set (%s); Argument can only be set once.\n", target_ip);
				return 1;
			}
			else {
				if (std::regex_match(std::string(argv[i]), config_header::REGEX_IP_VALID)) {
					// Set IP if it is valid
					target_ip = argv[i];
					printf("IP is valid.\n");
				}
				else { printf("IP is invalid.\n"); return 1; }
			}
		}

		// Handle path input
		else if (std::regex_match(std::string(argv[i]), config_header::REGEX_PATH)) {
			// If target_path was already set exit and inform the user
			if (target_path != NULL) {
				printf("Path was already set (%s); Argument can only be set once.\n", target_path);
				return 1;
			}
			else {
				// Say a path was detected and check if it is valid
				printf("Path detected (%s); ", argv[i]);
				if (std::regex_match(std::string(argv[i]), config_header::REGEX_DIRECTORY)) {
					if (std::filesystem::is_directory(argv[i])) {
						printf("Directory is valid.\n");
						target_path = argv[i];
					}
					else { printf("Directory is invalid.\n"); return 2; }
				}
				// Handle non-directory inputs
				else {
					printf("File was provided or path root was not specified (first or last '/' may be missing)\n");
					return 3;
				}
			}
		}

		// Handle unknown arguments
		else {
			printf("Unknown argument. (%s)\n%s\n", argv[i], config_header::HELP_TEXT); 
			return 1;
		}
	}

	// Handle situation if required arguments are not given
	if (target_path == NULL || target_ip == NULL) {
		printf("Required arguments not given; ");
		if (target_path == NULL) { printf("Path uninitialized. "); }
		if (target_ip == NULL) { printf("IP uninitialized. "); }
		printf("\n");
		
		return 4;
	}

	// Start threads
	auto fs_updates_thread = std::async(fs_updates);

	// Main loop
	while (running) {
		// If an error occurs in any of the threads stop
		if (exit_code != 0) {
			running = false;
			return exit_code;
		}
	
		// Delay to not max out cpu core
		usleep(1000);
	}

	// Exit successfully
	return exit_code;
}

// Function to catch and handle SIGINT signal (Ctrl+C)
void sigint_handler(int sig_num) {
    // Clean up and exit
    running = false;

    printf("Quit successfully.\n");
    exit(9);
}

// Function to catch and handle file system updates
void fs_updates() {
	// Start an inotify instance
	printf("Starting inotify instance\n");
	int fd = inotify_init(); // File descriptor
	if (fd == -1) {
		printf("%s Inotify initialization error\nErrno %i, (%s)\n", config_header::TEMPLATE_ERROR, errno, strerror(errno));
		exit_code = 5;
	}

	// Add an inotify watch
	printf("Adding inotify watch\n");
	int wd = inotify_add_watch(fd, target_path, IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO);
	if (wd == -1) {
		printf("%s Inotify watch start error\nErrno %i, (%s)\n", config_header::TEMPLATE_ERROR, errno, strerror(errno));
		exit_code = 6;
	}

	// Prepare for polling
	nfds_t nfds;
	struct pollfd fds[1];
	fds[0].fd = fd;
	fds[0].events = POLLIN;
	nfds = 1;

	// Read events from file descriptor
	printf("Reading events\n");
	while (running) {
		// If an event is available, read and interpret the event
		if (poll(fds, nfds, -1)) {
			// Read the amount of bytes available
			int bytes_available;
			if (ioctl(fd, FIONREAD, &bytes_available) == -1) {
				printf("%s Polling bytes available\nErrno %i, (%s)\n", config_header::TEMPLATE_ERROR, errno, strerror(errno));
				exit_code = 7;
			}

			// Make the buffer only as large as it needs to be
			char buf[bytes_available] __attribute__ ((aligned(__alignof__(struct inotify_event)))); // Make buffer and align (for good measure)
			ssize_t len = read(fd, buf, sizeof(buf));
			if (len == -1) {
				printf("%s Inotify read\nErrno %i, (%s)\n", config_header::TEMPLATE_ERROR, errno, strerror(errno));
				exit_code = 8;
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

		// Delay to not max out cpu core
		usleep(1000);
	}
}
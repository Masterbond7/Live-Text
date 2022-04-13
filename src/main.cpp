#include <iostream>
#include <regex> // Potentially unnessecary (in config.hpp)
#include <filesystem>

#include "config.hpp" 

// Defining variables
char* target_path;
char* target_ip;

// Main function
int main(int argc, char **argv) {
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
            if (std::regex_match(std::string(argv[i]), config_header::REGEX_IP_VALID)) {
                // Set IP if it is valid
                target_ip = argv[i];
                printf("IP is valid.\n");
            }
            else { printf("IP is invalid.\n"); }
        }

        // Handle path input
        else if (std::regex_match(std::string(argv[i]), config_header::REGEX_PATH)) {
            // Say a path was detected and check if it is valid
            printf("Path detected (%s); ", argv[i]);
            if (std::regex_match(std::string(argv[i]), config_header::REGEX_DIRECTORY)) {
                if (std::filesystem::is_directory(argv[i])) {
                    printf("Directory is valid.\n");
                }
                else { printf("Directory is invalid.\n"); return 2; }
            }
            // Handle non-directory inputs
            else {
                printf("File was provided / path root was not specified.\n", argv[i]);
                return 3;
            }
        }

        // Handle unknown arguments
        else {
            printf("Unknown argument. (%s)\n%s\n", argv[i], config_header::HELP_TEXT); 
            return 1;
        }
    }

    // Exit successfully
    return 0;
}
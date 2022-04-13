#include <iostream>
#include <regex> // Potentially unnessecary (in config.hpp)
#include <filesystem>

#include "config.hpp" 

// Defining variables
char* target_path = NULL;
char* target_ip = NULL;

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

    // Exit successfully
    return 0;
}
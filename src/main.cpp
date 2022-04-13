#include <iostream>
#include <regex> // Potentially unnessecary (in config.hpp)

#include "config.hpp" 

// Defining variables
char* target_path;
char* target_ip;

// Main function
int main(int argc, char **argv) {
    // Parse inputs
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-h") {
            printf("%s\n", config_header::HELP_TEXT); 
            return 0;
        }
        else if (std::regex_match(std::string(argv[i]), config_header::REGEX_IP)) {
            printf("IP detected (%s); ", argv[i]);
            if (std::regex_match(std::string(argv[i]), config_header::REGEX_IP_VALID)) {
                target_ip = argv[i];
                printf("IP is valid.\n");
            }
            else { printf("IP is invalid.\n"); }
        }
        else {
            printf("Unknown argument.\n%s\n", config_header::HELP_TEXT); 
            return 1;
        }
    }

    // Exit successfully
    return 0;
}
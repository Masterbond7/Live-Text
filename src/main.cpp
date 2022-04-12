#include <iostream>

namespace config_header { 
    #include "config.h" 
}

// Defining variables
char* target_path;
char* target_ip;

// Main function
int main(int argc, char **argv) {
    // Parse inputs
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-h") { printf("%s\n", config_header::HELP_TEXT); return 0; }
        else { printf("Unknown argument.\n%s\n", config_header::HELP_TEXT); return 1; }
    }

    // Exit successfully
    return 0;
}
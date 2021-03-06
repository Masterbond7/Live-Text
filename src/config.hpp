#include <regex>
#include <sys/inotify.h>

namespace config_header { 
    const char HELP_TEXT[] = "Usage: ./run.sh <project folder path> <target ip>";
    const char TEMPLATE_ERROR[] = "\033[31m[ERROR]\033[0m";

    std::regex REGEX_IP ("([0-9]|[0-9][0-9]|[0-9][0-9][0-9])\\.([0-9]|[0-9][0-9]|[0-9][0-9][0-9])\\.([0-9]|[0-9][0-9]|[0-9][0-9][0-9])\\.([0-9]|[0-9][0-9]|[0-9][0-9][0-9])");
    std::regex REGEX_IP_VALID ("(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])");
    std::regex REGEX_PATH (".*/.*");
    std::regex REGEX_DIRECTORY ("(/|~/).+/");
}
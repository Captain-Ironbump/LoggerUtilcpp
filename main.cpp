#include "logger.hpp"

int main()
{
    Logger logger("log"); // Create logger instance

    // Example usage of the logger
    logger.log(INFO, "Program started.");
    logger.log(DEBUG, "Debugging information.");
    logger.log(WARNING, "This is a warning.");
    logger.log(ERROR, "An error occurred.");
    logger.log(CRITICAL, "Critical failure!");

    return 0;
}

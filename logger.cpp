#include "logger.hpp"
#include <iostream>
#include <ctime>
#include <ostream>
#include <regex>
#include <filesystem>
#include <cstdio>
#include <cstdarg>
#ifdef __APPLE__
    #include <sstream>
#endif
// Constructor
Logger::Logger(const std::string& baseFileName) 
{
    // filename can be without providing the .count number
    if (baseFileName.empty()) {
        // If filename is empty, don't open a log file.
        logFile = nullptr;
        std::cout << "No log file specified. Logging will be to console only." << std::endl;
    } else {
        std::string filename = getNextLogFile(baseFileName);
        // check if the opend file exeed the maxFileSize
        logFile = new std::ofstream(filename, std::ios::app);
        if (!logFile->is_open()) {
            std::cerr << "Error opening log file: " << filename << std::endl << std::flush;
        } else {
            std::cout << "Log file opened successfully: " << filename << std::endl << std::flush;
        }
    }
}


// Destructor
Logger::~Logger() 
{
    if (logFile != nullptr)
    {
        logFile->close();
        delete logFile;
    }
}

// To String
std::string Logger::levelToString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG:
        return "DEBUG";
        break;
    case LogLevel::INFO:
        return "INFO";
        break;
    case LogLevel::WARNING:
        return "WARNING";
        break;
    case LogLevel::ERROR:
        return "ERROR";
        break;
    case LogLevel::CRITICAL:
        return "CRITICAL";
        break;
    default:
        return "UNKNOWN";
        break;
    }
}

// color converter
std::string Logger::getColorForLevel(LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG:
        return "\033[0;37m";
        break;
    case LogLevel::INFO:
        return "\033[0;32m";
        break;
    case LogLevel::WARNING:
        return "\033[0;33m";
        break;
    case LogLevel::ERROR:
        return "\033[0;31m";
        break;
    case LogLevel::CRITICAL:
        return "\033[1;31m";
        break;
    default:
        return "\033[0m";
        break;
    }
}

// log functiom
void Logger::log(LogLevel level, const char* message, ...)
{
    // Get current timestamp
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

    char buffer[1024];
    
    // Pass a copy of the string (not a reference) to va_start
    va_list args;
    va_start(args, message);  // message here is passed as a non-reference

    vsnprintf(buffer, sizeof(buffer), message, args);
    va_end(args);
    // Create log entry
    std::ostringstream logEntry;
    logEntry << "[" << timestamp << "] "
            << getColorForLevel(level) << levelToString(level) << "\033[0m"  // Apply color
            << ": " << buffer
            << std::endl;

    // Output to console
    std::cout << logEntry.str() << std::flush;

    if (logFile == nullptr)
        return;
    // Output to log file
    if (logFile->is_open()) {
        *logFile << logEntry.str();
        logFile->flush(); // Ensure immediate write to file
    }
}

// Get the next log file and return name
std::string Logger::getNextLogFile(const std::string& baseName) {
    std::string pattern = baseName + R"(\.(\d+)\.txt)";
    std::regex regexPattern(pattern);

    int highestIndex = -1;
    std::string latestFile;

    // Iterate through current directory files
    for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
        std::string filename = entry.path().filename().string();
        std::smatch match;

        // Check if filename matches the pattern
        if (std::regex_match(filename, match, regexPattern)) {
            int currentIndex = std::stoi(match[1].str());

            // Update the highest index
            if (currentIndex > highestIndex) {
                highestIndex = currentIndex;
                latestFile = filename;
            }
        }
    }

    // Check if the latest file exceeds size limit
    if (!latestFile.empty() && std::filesystem::file_size(latestFile) > this->maxFileSize) {
        highestIndex++;
    }

    // Create the new log file name
    return baseName + "." + std::to_string(highestIndex + 1) + ".txt";
}

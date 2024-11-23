#include "logger.hpp"
#include <iostream>
#include <ctime>
#include <ostream>

// Constructor
Logger::Logger(const std::string& filename) 
{
    if (filename.empty()) {
        // If filename is empty, don't open a log file.
        logFile = nullptr;
        std::cout << "No log file specified. Logging will be to console only." << std::endl;
    } else {
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
void Logger::log(LogLevel level, const std::string& message, int useLogFile)
{
    // Get current timestamp
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp),
            "%Y-%m-%d %H:%M:%S", timeinfo);

    // Create log entry
    std::ostringstream logEntry;
    logEntry << "[" << timestamp << "] "
            << getColorForLevel(level) << levelToString(level) << "\033[0m"  // Apply color
            << ": " << message
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

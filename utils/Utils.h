#ifndef MULTIPART_UTILS_H
#define MULTIPART_UTILS_H

#include <mutex>
#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <sys/stat.h>
#include <vector>

namespace Utils{
    using StringList = std::vector<std::string>;
    
    /**
     * NOTE, it works to have both the declaration and definition of a function in a h file
     * if it is static. If it would not have been static the compiler would fail to compile
     * a cpp file if that cpp file includes this h file and also another h file that in turn
     * also includes this h file. In that case the compiler would see it as two functions
     * with the same name and it would fail with "multiple definitions", "first defined here". 
     */ 

    std::string getCurrentTime(const std::string& format, bool ms);

    /**
     * Returns the amount of milliseconds since the epoch 1970-01-01 until the time
     * given in the time input.
     * The input time shall be in the format given by format.
     * For example, if the the time is "2021-05-07T14:54:01Z" the format shall be
     * "%Y-%m-%dT%H:%M:%SZ".
     */
    int64_t getMillisecondsFromString(std::string time, std::string format);

    /**
     * Returns the amount of milliseconds between the given timeToCompareInMs (UTC) and now (UTC).
     * If the returned value is positive the timeToCompareInMs is later than now, and
     * if it is negative the timeToCompareInMs has already accured.
     */
    int64_t getMsBetweenTimeAndNow(int64_t timeToCompareInMs);

    /**
     * threadsafe wrapper for std::cout.
     * @param msg
     */
    void print(std::string msg);

    /**
     * Returns if a file exists or not
     */
    bool fileExist(const std::string& name);

    /**
     * Returns which path we are currently executing in
     */
    std::string currentlyExecutingPath();

    /**
     * Returns "./" if we are executing in the project folder or "../" if we are executing in the build folder
     */
    std::string getRelativePrefix();

    /**
     * @brief validate if a single file exist, prints error message if file is missing
     * @param file
     * @return false if file does not exist
     */
    [[nodiscard]] bool validateFile(const std::string &file);

    /**
     * @brief validate if a list files exist, prints error message if file is missing
     * @param files
     * @return false if one or more files is missing
     */
    [[nodiscard]] bool validateFiles(const StringList &files);

    /**
     * @brief Get the File List From dir folder, only files with extension specified in extension.
     * 
     * @param dir 
     * @param extension Should be given in the format .log, .txt, .ini etc.
     * @return std::vector<std::string> 
     */
    std::vector<std::string> getFileListFromDir(std::string dir, std::string extension);
}

#endif //MULTIPART_UTILS_H

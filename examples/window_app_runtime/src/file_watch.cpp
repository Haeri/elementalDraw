#include "file_watch.hpp"

#include <iostream>
#include <sys/stat.h>

std::atomic<bool> FileWatch::_checking = false;
std::atomic<bool> FileWatch::_shouldRun = false;
std::thread FileWatch::_timeThread;
// std::thread FileWatch::_workThread;
std::map<std::string, FileWatch::TimeStampedFile> FileWatch::_watchList;

void FileWatch::addToWatchList(std::string filePath, IReloadableFile* file)
{
    struct stat result;

    // If file exists
    if (stat(filePath.c_str(), &result) == 0)
    {
        TimeStampedFile sf = {
            result.st_mtime,
            file,
        };
        _watchList[filePath] = sf;
    }
}

void FileWatch::clearWatchList()
{
    _watchList.clear();
}

void FileWatch::startCheckInterval(unsigned int milliseconds)
{
    _shouldRun = true;
    _timeThread = std::thread([milliseconds]() {
        while (_shouldRun)
        {
            auto next = std::chrono::steady_clock::now() + std::chrono::milliseconds(milliseconds);
            checkValidity();
            std::this_thread::sleep_until(next);
        }
    });
}

void FileWatch::stopCheckInterval()
{
    _shouldRun = false;
    if (_timeThread.joinable())
        _timeThread.join();
}

void FileWatch::checkValidity()
{
    if (_checking)
        return;

    _checking = true;

    for (std::map<std::string, TimeStampedFile>::iterator it = _watchList.begin();
         it != _watchList.end(); ++it)
    {
        struct stat result;
        if (stat(it->first.c_str(), &result) == 0 && it->second.timestamp != result.st_mtime)
        {
            std::cout << "File Changed: " << it->first << std::endl;
            it->second.timestamp = result.st_mtime;
            it->second.file->onReload();
        }
    }

    _checking = false;
}
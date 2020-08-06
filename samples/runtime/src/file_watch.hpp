#ifndef FILE_WATCH_HPP
#define FILE_WATCH_HPP

#include <string>
#include <thread>
#include <map>
#include <atomic>

class IReloadableFile
{
public:
    virtual void onReload() = 0;
};

class FileWatch
{
public:
    struct TimeStampedFile
    {
        int64_t timestamp;
        IReloadableFile* file;
    };

    static void addToWatchList(std::string filePath, IReloadableFile* file);
    static void clearWatchList();
    static void startCheckInterval(unsigned int interval = 1000);
    static void stopCheckInterval();

private:
    static std::atomic<bool> _checking;
    static std::atomic<bool> _shouldRun;
    static std::thread _timeThread;
    //static std::thread _workThread;
    static std::map<std::string, TimeStampedFile> _watchList;

    static void checkValidity();
private:

};

#endif // FILE_WATCH_HPP
#include "loglib.h"
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

int main(int argc, char* argv[]) {
    if (argc < 3) return 1;
    Logger logger(argv);

    std::queue<std::pair<std::string, std::string>> messageQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    std::atomic<bool> isRunning{true};
    
    std::thread worker([&]() {
        while (isRunning) {
            std::pair<std::string, std::string> msg;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                queueCV.wait(lock, [&] { 
                    return !messageQueue.empty() || !isRunning; 
                });
                
                if (!isRunning && messageQueue.empty()) return;
                
                msg = messageQueue.front();
                messageQueue.pop();
            }
            logger.addMessage(msg.first, msg.second);
        }
    });

    std::cout << "Journal " << logger.getLoggerName() << " has been created, default level of importance - " << logger.convertLEVELToString(logger.getLevelImportance()) << std::endl;
    int choice;
    std::string message;
    std::string level;
    while (1) {
        std::cout << "Menu:\n1. Enter message to journal.\n2. Change default level of importance.\n3. Exit." << std::endl;
        std::cin >> choice;
        std::cin.ignore();
        switch (choice) {
            case 1:
                std::cout << "Enter message:" << std::endl;
                getline(std::cin, message);
                std::cout << "Enter level of importance (LOW, MIDDLE, HIGH) or press Enter to skip:" << std::endl;
                getline(std::cin, level);
                if (level.empty()) level = "NONE";
                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    messageQueue.push({message, level});
                }
                queueCV.notify_one();
                break;
            case 2:
                std::cout << "Enter level of importance (LOW, MIDDLE, HIGH):" << std::endl;
                std::cin >> level;
                logger.changeLevelImportance(level);
                break;
            case 3:
                isRunning = false;
                queueCV.notify_all();
                worker.join();
                std::cout << "Program finished.";
                return 0;
        }
    }
    
    return 0;
}
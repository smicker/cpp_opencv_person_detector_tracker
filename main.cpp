#include <iostream>
#include <fmt/format.h>
#include <thread>

#include "utils/Utils.h"
#include "main.h"
#include "core/Globals.h"
#include "video_handler.h"


int main(int argc, char *argv[]) {
    Utils::print("##############");
    Utils::print("Program starts");
    Utils::print("##############");

    if (!checkRequiredFiles()) {
        // One or several required files are missing, terminate program!!
        return -1;
    }

    Video::VideoUI video_ui;
    video_ui.run();

    // Note, you might be tempted to start the object detection and tracking in a worker thread, like
    // below. But since opencv will use QT, for displaying the video in a window, you shall not run
    // it in a worker thread. QT is handling the GUI and the GUI shall be executed in the main thread.
    // QT is not thread safe and even thought it mostly works to run it in a worker thread you might
    // end up with strange problems if you do, so avoid it!! You will always get a warning when exiting
    // your program that the Qthread was stopped from another thread "QObject::~QObject: Timers cannot
    // be stopped from another thread".
    // auto videoThread = std::thread([&video_handler](){
    //     video_handler.run();
    // });
    // videoThread.join();

    Utils::print("******* Program Ended *******");
    return 0;
}

bool checkRequiredFiles() {
    Utils::StringList required_files;
    required_files.emplace_back(Core::COCO_NAMES_FILE);
    required_files.emplace_back(Core::YOLO_CFG_FILE);
    required_files.emplace_back(Core::YOLO_WEIGHTS_FILE);

    if (!Utils::validateFiles(required_files)) {
        Utils::print("Required yolo files are missing, make sure below files exists!!");
        for (auto &file: required_files) {
            Utils::print(file);
        }
        return false;
    }

    required_files.clear();
    return true;
}

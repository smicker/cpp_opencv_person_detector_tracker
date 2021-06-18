#ifndef VIDEOUI_H
#define VIDEOUI_H

#include <string>
#include <atomic>
#include <opencv2/opencv.hpp>

namespace Video {

    /**
     * Class to hold info about mouse clicks and drags over the video window
     */
    class MouseArea {
        public:
            bool isClicked = false;
            bool isDragged = false;
            bool isLeftMouseButtonPressed = false;
            cv::Point clickedMousePoint = cv::Point(0, 0); // The x,y coordinate of the last mouse click
            cv::Point currentMousePoint = cv::Point(0, 0); // The x,y coordinate of the current mouse

            cv::Rect2d getMouseDraggedArea() {
                std::lock_guard<std::mutex> lock(mMutex);
                return mMouseDraggedArea;
            }

            void setMouseDraggedArea(int x, int y, int width, int height) {
                std::lock_guard<std::mutex> lock(mMutex);
                mMouseDraggedArea.x = x;
                mMouseDraggedArea.y = y;
                mMouseDraggedArea.width = width;
                mMouseDraggedArea.height = height;
            }

            void reset() {
                clickedMousePoint.x = -1;
                clickedMousePoint.y = -1;
                currentMousePoint.x = -1;
                currentMousePoint.y = -1;
                isClicked = false;
                isDragged = false;
                setMouseDraggedArea(0, 0, 0, 0);
            }        
        private:
            std::mutex mMutex;
            cv::Rect2d mMouseDraggedArea = cv::Rect2d(0, 0, 0, 0);
    };

    class VideoUI {
        public:
            explicit VideoUI();
            void run();
        private:
            static void onMouse(int event,int x,int y,int,void*);
            void onMouse(int event, int x, int y);

            std::atomic<bool> mRunning = false;
            std::atomic<bool> mIsTracking = false;

            MouseArea mMouseArea;
    };
}

#endif // VIDEOUI_H
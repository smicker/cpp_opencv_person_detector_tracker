#include <fmt/format.h>
#include "video_handler.h"
#include "utils/Utils.h"
#include "core/Colors.h"
#include "yolo_detection.h"
#include "csrt_tracker.h"


namespace Video {
    VideoUI::VideoUI() {
        Utils::print("Video UI initiated");
    }

    void VideoUI::run() {
        mRunning = true;
        YoloDetector detector;
        CsrtTracker tracker;

        std::optional<cv::Rect2d> selectedObject = std::nullopt;
        cv::Rect2d mRectangleToTrack;

        // Open video stream
        cv::VideoCapture capture("./resources/MickeIsabergDownhill.mp4");
        //cv::VideoCapture capture(0); // This will use the web camera instead
        if (!capture.isOpened()) {
            Utils::print("Failed to open video stream");
            return;
        }

        // Init opencv output window
        cv::namedWindow("Video output");
        cv::setMouseCallback("Video output", onMouse, this);

        while (mRunning) {
            cv::UMat frame;

            // Get a new frame from video stream
            capture >> frame;
            if (frame.empty()) {
                Utils::print("Can't receive frame (stream end?). Exiting...");
                mRunning = false;
                break;
            }

            if (!mIsTracking) {
                // Detect all objects
                std::vector<cv::Rect> foundObjects = detector.detectObjects(frame);

                if (!foundObjects.empty()) {
                    // Draw a rectangle around each found object
                    for (const auto &rectangle : foundObjects) {
                        cv::rectangle(frame, rectangle, Core::COLOR_BLUE, 2, 1);
                    }

                    // Select mouse clicked object.
                    if (!selectedObject.has_value() && mMouseArea.isClicked) {
                        for (const auto &rectangle : foundObjects) {
                            if (mMouseArea.clickedMousePoint.x > rectangle.x && mMouseArea.clickedMousePoint.x < rectangle.x + rectangle.width &&
                                mMouseArea.clickedMousePoint.y > rectangle.y && mMouseArea.clickedMousePoint.y < rectangle.y + rectangle.height) {
                                    selectedObject = rectangle;
                                    mIsTracking = true;
                                    break;
                            }
                        }
                    }
                }

                // Select to lock on the mouse dragged area
                if (mMouseArea.isDragged) {
                    cv::Rect2d mouseDraggedArea = mMouseArea.getMouseDraggedArea();
                    if (!selectedObject.has_value()
                        && mouseDraggedArea.width > 0
                        && mouseDraggedArea.height > 0) {
                        selectedObject = mouseDraggedArea;
                        mIsTracking = true;
                    }
                }

                if (mIsTracking) {
                    mRectangleToTrack = selectedObject.value();
                    tracker.initTracker(frame, mRectangleToTrack);
                }
            } else {
                // Track the object
                bool trackingObject = tracker.update(frame, mRectangleToTrack);

                if (!trackingObject) {
                    Utils::print("Lost track!!");
                    tracker.reset();
                    mIsTracking = false;
                    selectedObject = std::nullopt;
                } else {
                    // Draw the updated rectangle to track
                    cv::rectangle(frame, mRectangleToTrack, Core::COLOR_BLUE, 2, 1);
                }
            }

            // Check if user has dragged an area on the window
            if (mMouseArea.isLeftMouseButtonPressed) {
                // Draw the dragged area onto the window
                cv::rectangle(frame, mMouseArea.getMouseDraggedArea(), Core::COLOR_BLUE, 2, 1);
            }

            // Show video and listen for keypress
            cv::imshow("Video output", frame);
            int key = cv::waitKey(10);
            switch (key) {
                case 27: // esc
                    mRunning = false;
                    break;
            }
        }

        // The camera will be deinitialized automatically in VideoCapture destructor
        cv::destroyAllWindows();
        Utils::print("Video UI has shut down");
    }

    void VideoUI::onMouse(int event, int x, int y, int, void* userdata) {
        if (userdata != nullptr) {
            VideoUI* videoUi = reinterpret_cast<VideoUI*>(userdata);
            videoUi->onMouse(event, x, y);
        }
    }
    
    void VideoUI::onMouse(int event, int x, int y) {
        if (event == cv::EVENT_LBUTTONDOWN) {
            mMouseArea.reset();
            mMouseArea.isLeftMouseButtonPressed = true;
            mMouseArea.clickedMousePoint.x = mMouseArea.currentMousePoint.x = x;
            mMouseArea.clickedMousePoint.y = mMouseArea.currentMousePoint.y = y;
            mMouseArea.setMouseDraggedArea(x, y, 0, 0);
        } else if (event == cv::EVENT_MOUSEMOVE) {
            if (mMouseArea.isLeftMouseButtonPressed) {
                mMouseArea.currentMousePoint.x = x;
                mMouseArea.currentMousePoint.y = y;
                mMouseArea.setMouseDraggedArea(std::min(mMouseArea.clickedMousePoint.x, x),
                                                std::min(mMouseArea.clickedMousePoint.y, y),
                                                abs(x - mMouseArea.clickedMousePoint.x),
                                                abs(y - mMouseArea.clickedMousePoint.y));
            }
        } else if (event == cv::EVENT_LBUTTONUP) {
            mMouseArea.isLeftMouseButtonPressed = false;
            if ((abs(x - mMouseArea.clickedMousePoint.x) < 2) && (abs(y - mMouseArea.clickedMousePoint.y) < 2)) {
                // The area selected by mouse dragging is too small, treat it as a point clicked instead.
                mMouseArea.isDragged = false;
                mMouseArea.isClicked = true;
            } else {
                mMouseArea.isClicked = false;
                mMouseArea.isDragged = true;
            }
        }
    }
}
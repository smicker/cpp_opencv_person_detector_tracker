#ifndef CSRT_TRACKER_H
#define CSRT_TRACKER_H

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

namespace Video {
    class CsrtTracker{
        public:
            explicit CsrtTracker();

            /**
             * Init the CSRT tracker
             */
            void initTracker(const cv::InputOutputArray &frame, cv::Rect2d roi);

            /**
             * Calculates where the new rectangle is on the new frame and returns this in roi.
             * Returns true if tracking is successful or false if tracking is lost.
             */
            bool update(const cv::InputOutputArray &frame, cv::Rect2d &roi);

            /**
             * Resets the tracker
             */
            void reset();
        private:
            cv::Ptr<cv::Tracker> mTracker;
    };
}

#endif // CSRT_TRACKER_H
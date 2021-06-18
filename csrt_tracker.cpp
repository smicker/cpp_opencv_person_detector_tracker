#include "csrt_tracker.h"
#include "core/Colors.h"
#include "utils/Utils.h"

namespace Video {
    CsrtTracker::CsrtTracker() {}

    void CsrtTracker::initTracker(const cv::InputOutputArray &frame, cv::Rect2d roi) {
        mTracker = cv::TrackerCSRT::create();
        mTracker->init(frame, roi);
    }

    bool CsrtTracker::update(const cv::InputOutputArray &frame, cv::Rect2d &roi) {
        bool success = mTracker->update(frame, roi);
        return success;
    }

    void CsrtTracker::reset() {
        mTracker.reset();
    }

}
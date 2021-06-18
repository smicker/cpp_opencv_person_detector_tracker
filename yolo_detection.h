#ifndef YOLO_DETECTOR_H
#define YOLO_DETECTOR_H

#include <opencv2/opencv.hpp>

namespace Video {
    class YoloDetector{
        public:
            explicit YoloDetector();
            /**
             * Returns a vector with all found objects.
             */
            std::vector<cv::Rect> detectObjects(cv::UMat frame);
        private:
            /**
             * Get the found objects from the net output
             */
            std::vector<cv::Rect> postProcess(cv::InputOutputArray frame, const std::vector<cv::Mat> &layerOuts);
            cv::dnn::Net net;
            std::vector<std::string> classes;
            std::vector<cv::String> lastLayerNames;

            // Initialize the parameters
            float confThreshold = 0.5; // Confidence threshold
            float nmsThreshold = 0.4;  // Non-maximum suppression threshold
    };
}

#endif // YOLO_DETECTOR_H
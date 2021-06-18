#include <fstream>
#include <opencv2/dnn.hpp>
#include "yolo_detection.h"
#include "core/Globals.h"

namespace Video {
    YoloDetector::YoloDetector() {
        // Load all class names (object names) from coco.names
        std::string classesFile = std::string(Core::COCO_NAMES_FILE);
        std::ifstream ifs(classesFile.c_str());
        std::string line;
        while (std::getline(ifs, line)) {
            classes.push_back(line);
        }

        // Load yolo model with cfg and weights
        // For yolov3
        //net = cv::dnn::readNetFromDarknet('./resources/yolo/yolov3.cfg', './resources/yolo/yolov3.weights');
        // For yolov4
        net = cv::dnn::readNetFromDarknet(Core::YOLO_CFG_FILE, Core::YOLO_WEIGHTS_FILE);
        net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

        // Find the names of the last layer of the neural network
        lastLayerNames = net.getUnconnectedOutLayersNames();
    }

    std::vector<cv::Rect> YoloDetector::detectObjects(cv::UMat frame) {
        std::vector<cv::Rect> boxes;
        cv::UMat blob;
        std::vector<cv::Mat> layerOut;

        /* Yolo net takes the image in a different format so it needs to be converted. Every pixel
        needs to have its color range divided by 255 since yolo wants it in range 0->1. Then
        size should be 320x320 (or 416x416 or 608x608) */
        cv::dnn::blobFromImage(frame, blob, 1/255.0, cv::Size(320, 320), cv::Scalar(0, 0, 0), true, false);

        // Feed this blob image to the network
        net.setInput(blob);

        // Get the output from the neural network, which is the last layer
        net.forward(layerOut, lastLayerNames);
        /*
        layer_out is a list of 3 elements. The first element has 507 rows and each row has 85 elements.
        print(len(layer_out)) # Gives 3
        print(layer_out[0].shape) # Gives (507, 85)
        print(layer_out[1].shape) # Gives (2028, 85)
        print(layer_out[2].shape) # Gives (8112, 85)
        The first 5 elements in each row, eg layer_out[0][0][0:5] is the bounding box of the found
        object. 0=center_x, 1=center_y, 2=width, 3=height, 4=? (Those are in percentage of the image)
        After that we have 80 elements. Each of those represents one object in coco.names
        and tells the probability that it is that object. So, lets say that the 6th element is 0.6.
        That means that it is 60% probability that the found object is a person. Or if the second
        object is 0.45, it means a bicycle of 45% probability.
        */

        boxes = postProcess(frame, layerOut);

        return boxes;
    }

    std::vector<cv::Rect> YoloDetector::postProcess(cv::InputOutputArray frame, const std::vector<cv::Mat> &layerOuts) {
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;

        cv::UMat internalFrame = frame.getUMat();

        // Now look through each finding and see what objects we detected.
        for (const auto &out : layerOuts) {
            //scan all boxes, assign box class name with highest score.
            auto data = (float *) out.data;

            for (int j=0; j<out.rows; ++j, data+=out.cols) {
                // The probabilities comes after the 5th element. Get the probabilities into scores.
                cv::Mat scores = out.row(j).colRange(5, out.cols);

                cv::Point classIdPoint;
                double confidence;

                // This finds the index of the max value in the scores array and returns the value at
                // this index, stored to "confidence" here, and also returns this index in classIdPoint.
                cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);

                // Only handle objects with higher confidence than confThreashold and only persons
                if (confidence > confThreshold && classIdPoint.x < (int) classes.size() && classes[classIdPoint.x] == "person") {
                    int centerX = (int) (data[0] * internalFrame.cols);
                    int centerY = (int) (data[1] * internalFrame.rows);
                    int width = (int) (data[2] * internalFrame.cols);
                    int height = (int) (data[3] * internalFrame.rows);
                    int left = centerX - width / 2; // Store as int to get rid of decimals
                    int top = centerY - height / 2;

                    confidences.push_back((float) confidence);
                    boxes.emplace_back(left, top, width, height);
                }
            }
        }

        // Reduce overlaping boxes with lower confidence
        std::vector<int> indexes;
        std::vector<cv::Rect> returnBox;
        cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indexes);

        for (int idx : indexes) {
            returnBox.push_back(boxes[idx]);
        }
        return returnBox;
    }
}
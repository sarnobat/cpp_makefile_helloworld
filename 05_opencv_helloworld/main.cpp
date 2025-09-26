#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Create a black image
    cv::Mat img = cv::Mat::zeros(200, 400, CV_8UC3);

    // Write "Hello, OpenCV!" in green text
    cv::putText(img,
                "Hello, OpenCV!",
                cv::Point(20, 100),              // bottom-left corner of text
                cv::FONT_HERSHEY_COMPLEX,        // font face
                1.0,                             // font scale
                cv::Scalar(0, 255, 0),           // BGR color (green)
                2);                              // thickness

    // Show the image
    cv::imshow("Hello", img);

    // Wait until a key is pressed
    cv::waitKey(0);

    return 0;
}

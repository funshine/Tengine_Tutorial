#include "UltraFace.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <tengine .tmfile> [image files...]\n", argv[0]);
        return 1;
    }

    std::string tengine_path = argv[1];
    UltraFace ultraface(tengine_path, 320, 240, 4, 0.65); // config model input

    if (argc>2){
        for (int i = 2; i < argc; i++) {
            std::string image_file = argv[i];
            std::cout << "Processing " << image_file << std::endl;

            cv::Mat frame = cv::imread(image_file);
            auto start = std::chrono::steady_clock::now();
            std::vector<FaceInfo> face_info;
            ultraface.detect(frame, face_info);

            std::cout << "face_info " << face_info.size() << std::endl;

            for (auto face : face_info) {
                cv::Point pt1(face.x1, face.y1);
                cv::Point pt2(face.x2, face.y2);
                cv::rectangle(frame, pt1, pt2, cv::Scalar(0, 255, 0), 2);
            }

            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            std::cout << "all time: " << elapsed.count() << " s" << std::endl;
            // cv::imshow("UltraFace", frame);
            // cv::waitKey();
            std::string result_name = "result" + std::to_string(i) + ".jpg";
            cv::imwrite(result_name, frame);
        }
    } else 
    {
        // use camera
        cv::VideoCapture cap0(1);//1：调用usb摄像头，0:调用树莓派自带摄像头
        cap0.release();
        cv::VideoCapture cap(1);//1：调用usb摄像头，0:调用树莓派自带摄像头
     
        if(!cap.isOpened())
        {
            std::cout << "can't open this camera" << std::endl;
            return -1;
        }
        cap.set(cv::CAP_PROP_FRAME_WIDTH, 320);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, 240);

        // If you do not care about backward compatibility
        // You can use the following instead for OpenCV 3
        // double fps = cap.get(CAP_PROP_FPS);
        double fps = cap.get(cv::CAP_PROP_FPS);
        std::cout << "Frames per second using cap.get(CAP_PROP_FPS) : " << fps << std::endl;

        int num_frames = 30;
        auto start = std::chrono::steady_clock::now();

        cv::Mat frame,edges;
        while(1)
        {
            if (num_frames == 30)
            {
                //here we take the timestamp
                start = std::chrono::steady_clock::now();
            }
            cap >> frame;

            // cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);//转化为灰度图
            // cv::blur(frame, frame, cv::Size(7,7));//图像模糊去燥
            // cv::Canny(frame, edges, 3, 9, 3);//检测轮廓
            // cv::imshow("Camera Capture", edges);

            std::vector<FaceInfo> face_info;
            ultraface.detect(frame, face_info);
            // std::cout << "face_info " << face_info.size() << std::endl;
            for (auto face : face_info) {
                cv::Point pt1(face.x1, face.y1);
                cv::Point pt2(face.x2, face.y2);
                cv::rectangle(frame, pt1, pt2, cv::Scalar(0, 255, 0), 2);
            }
            cv::imshow("Camera Capture", frame);

            num_frames--;
            if (num_frames == 0)
            {
                //measure timestamp again
                auto end = std::chrono::steady_clock::now();
                //end - start is the time taken to process 1 frame, output it:
                std::chrono::duration<double> diff = end-start;
                std::cout << "Time to process last frame (seconds): " << diff.count() 
                        << " FPS: " << 30.0 / diff.count() << std::endl;
                num_frames = 30;
            }
            
            if(cv::waitKey(10)==27)
            {
                std::cout << "Exit" << std::endl;
                break;
            }
        }
    }
    return 0;
}

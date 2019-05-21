/*
// Copyright (c) 2018 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#include <utility>
#include <vector>

#include <opencv2/imgproc/imgproc.hpp>

#include "human_pose_estimator.hpp"
#include "render_human_pose.hpp"

#define PI_num 3.14159265358979323846 /* pi */
//#define DEBUG

namespace human_pose_estimation {
bool renderHumanPose(const std::vector<HumanPose>& poses, cv::Mat& image, bool starjump_flag) {
    CV_Assert(image.type() == CV_8UC3);

    const std::vector<cv::Scalar> colors = {
        cv::Scalar(255, 0, 0), cv::Scalar(255, 85, 0), cv::Scalar(255, 170, 0),
        cv::Scalar(255, 255, 0), cv::Scalar(170, 255, 0), cv::Scalar(85, 255, 0),
        cv::Scalar(0, 255, 0), cv::Scalar(0, 255, 85), cv::Scalar(0, 255, 170),
        cv::Scalar(0, 255, 255), cv::Scalar(0, 170, 255), cv::Scalar(0, 85, 255),
        cv::Scalar(0, 0, 255), cv::Scalar(85, 0, 255), cv::Scalar(170, 0, 255),
        cv::Scalar(255, 0, 255), cv::Scalar(255, 0, 170), cv::Scalar(255, 0, 85)
    };
    const std::vector<std::pair<int, int> > limbKeypointsIds = {
        {1, 2},  {1, 5},   {2, 3},
        {3, 4},  {5, 6},   {6, 7},
        {1, 8},  {8, 9},   {9, 10},
        {1, 11}, {11, 12}, {12, 13},
        {1, 0},  {0, 14},  {14, 16},
        {0, 15}, {15, 17}
    };

    const int stickWidth = 4;
    const cv::Point2f absentKeypoint(-1.0f, -1.0f);
    // int count=0;

    for (const auto& pose : poses) {
        CV_Assert(pose.keypoints.size() == HumanPoseEstimator::keypointsNumber);

        // std::cout<< "Number of keypoints: "<<pose.keypoints.size() <<std::endl;
        for (size_t keypointIdx = 0; keypointIdx < pose.keypoints.size(); keypointIdx++) {
            if (pose.keypoints[keypointIdx] != absentKeypoint) {
                // std::cout<< "My keypointsIdx:"<< keypointIdx <<std::endl;
                //Draw the numbers on the points
                cv::putText(image, std::to_string(keypointIdx), pose.keypoints[keypointIdx], cv::FONT_HERSHEY_COMPLEX, 1.1, colors[keypointIdx],2);
                cv::circle(image, pose.keypoints[keypointIdx], 4, colors[keypointIdx], -1);
                // std::cout<< pose.keypoints[0]<<std::endl;
                // std::cout<< pose.keypoints[0].x<<std::endl;
                // std::cout<< pose.keypoints[0].y<<std::endl;

                float D4to1, ang1, A4to1, D10to1, ang2, A10to1, corrected_A4to1, D7to1, ang3, A7to1, D13to1, ang4, A13to1, corrected_A7to1;
                // LEFT DATA
                D4to1 = sqrt(pow((pose.keypoints[1].x - pose.keypoints[4].x),2) + pow((pose.keypoints[1].y - pose.keypoints[4].y),2));
                ang1 = (pose.keypoints[1].y - pose.keypoints[4].y) / D4to1;
                A4to1 = copysign(acos(ang1) * 180 / PI_num, ang1);
                D10to1 = sqrt(pow((pose.keypoints[1].x - pose.keypoints[10].x),2) + pow((pose.keypoints[1].y - pose.keypoints[10].y),2));
                ang2 = (pose.keypoints[1].y - pose.keypoints[10].y) / D10to1;
                A10to1 = copysign(acos(ang2) * 180 / PI_num, ang2);
                if(A4to1 < 0)
                    A4to1 = A4to1 + 180;
                else
                    A4to1 = A4to1 - 90;
                //printf("Angle4 to Angle14: %.2f\tAngle10 to Angle14: %.2f\n", A4to14, A10to14);
                // RIGHT DATA
                D7to1 = sqrt(pow((pose.keypoints[1].x - pose.keypoints[7].x),2) + pow((pose.keypoints[1].y - pose.keypoints[7].y),2));
                ang3 = (pose.keypoints[1].y - pose.keypoints[7].y) / D7to1;
                A7to1 = copysign(acos(ang3) * 180 / PI_num, ang3);
                D13to1 = sqrt(pow((pose.keypoints[1].x - pose.keypoints[13].x),2) + pow((pose.keypoints[1].y - pose.keypoints[13].y),2));
                ang4 = (pose.keypoints[1].y - pose.keypoints[13].y) / D13to1;
                A13to1 = copysign(acos(ang4) * 180 / PI_num, ang4);
                if(A7to1 < 0)
                    A7to1 = A7to1 + 180;
                else
                    A7to1 = A7to1 - 90;
                //printf("Angle7 to Angle14: %.2f\tAngle13 to Angle14: %.2f\n", A7to14, A13to14);
                if (A4to1 <= -50 && A7to1 <= -50 && A10to1 >= -170 && A13to1 >= -170 && starjump_flag == false)
                    starjump_flag = true;	

                if (A4to1 >= -50 && A7to1 >= -50 && A10to1 <= -170 && A13to1 <= -170 && starjump_flag == true){
                    starjump_flag = false;
                    // std::cout<<"Count is incremented!"<<std::endl;
                    // count = count + 1;
                }
            #ifdef DEBUG
                std::cout<<"Flag: "<<starjump_flag<<std::endl;
                // std::cout<<"Count: "<<count<<std::endl;
                std::cout<<"A4to1: "<<A4to1<<std::endl;
                std::cout<<"A10to1: "<<A10to1<<std::endl;
                std::cout<<"A7to1: "<<A7to1<<std::endl;
                std::cout<<"A13to1: "<<A13to1<<std::endl;
            #endif

                
                

                // std::cout<< pose.keypoints[keypointIdx]<<std::endl;

                // std::cout<< "All keypoints:"<<std::endl;
                // std::cout<< pose.keypoints<<std::endl;
            }
        }
    }
    cv::Mat pane = image.clone();
    for (const auto& pose : poses) {
        for (const auto& limbKeypointsId : limbKeypointsIds) {
            std::pair<cv::Point2f, cv::Point2f> limbKeypoints(pose.keypoints[limbKeypointsId.first],
                    pose.keypoints[limbKeypointsId.second]);
            if (limbKeypoints.first == absentKeypoint
                    || limbKeypoints.second == absentKeypoint) {
                continue;
            }

            float meanX = (limbKeypoints.first.x + limbKeypoints.second.x) / 2;
            float meanY = (limbKeypoints.first.y + limbKeypoints.second.y) / 2;
            cv::Point difference = limbKeypoints.first - limbKeypoints.second;
            double length = std::sqrt(difference.x * difference.x + difference.y * difference.y);
            int angle = static_cast<int>(std::atan2(difference.y, difference.x) * 180 / CV_PI);
            std::vector<cv::Point> polygon;
            cv::ellipse2Poly(cv::Point2d(meanX, meanY), cv::Size2d(length / 2, stickWidth),
                             angle, 0, 360, 1, polygon);
            cv::fillConvexPoly(pane, polygon, colors[limbKeypointsId.second]);
        }
    }
    cv::addWeighted(image, 0.4, pane, 0.6, 0, image);
    return starjump_flag;
}
    
}  // namespace human_pose_estimation

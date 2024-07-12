// EMI ( Eye Movement Integration ).cpp : Defines the entry point for the application.
//

#include "emi.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <chrono>

using namespace cv;
using namespace std::chrono;
using namespace std;

int main()
{
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout << "Error: Could not open the camera." << endl;
		return -1;
	}
	// Variables to store thumb positions
	vector<Point> thumb_positions;
	bool stop = false;
	
	//Create a windwo to display the video
	namedWindow("ThumbMovement", WINDOW_AUTOSIZE);

	while (true) {
		auto start_time = high_resolution_clock::now();
		Mat frame;
		cap >> frame;
		if (frame.empty()) {
			cout << "Error: Could not capture frame." << endl;
			break;
		}
		auto read_frame_time = high_resolution_clock::now();
		// Convert frame to grayscale
		Mat gray;
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		auto convert_frame_time = high_resolution_clock::now();

		// Simple thresholding to detect the thumb( this is just a placeholder for actual thumb tracking logic
		Mat thresh;
		threshold(gray, thresh, 100, 255, THRESH_BINARY);
		auto process_frame_time = high_resolution_clock::now();
		
		//Find Contours
		vector<vector<Point>> contours;
		findContours(thresh, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

		// Find largest contour
		int largest_contour_index = -1;
		double largest_area = 0.0;
		for (int i = 0; i < contours.size(); i++) {
			double area = contourArea(contours[i]);
			if (area > largest_area) {
				largest_area = area;
				largest_contour_index = i;
			}
		}

		// Draw the largest contour and get the centroid
		if (largest_contour_index != -1) {
			Moments M = moments(contours[largest_contour_index]);
			int cx = int(M.m10 / M.m00);
			int cy = int(M.m01 / M.m00);
			thumb_positions.push_back(Point(cx, cy));
			//Draw the contour and centroid
			drawContours(frame, contours, largest_contour_index, Scalar(0, 255, 0), 2);
			circle(frame, Point(cx, cy), 5, Scalar(0, 0, 255), -1);
		}
		auto render_frame_time = high_resolution_clock::now();
		if (waitKey(1) == 'q') {
			stop = true;
			break;
		}
	}
	cap.release();
	destroyAllWindows();
	cout << "Hello CMake." << endl;
	return 0;
}

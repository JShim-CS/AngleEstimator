#include "AngleEstimate.h"
//@author Junhyung Shim
void AngleEstimate::detectObject() {
	cv::VideoCapture cap(0);
	cv::Mat frame;
	int minh = 0;
	int mins = 0;
	int minv = 0;

	int maxh = 179;
	int maxs = 255;
	int maxv = 255;

	cv::namedWindow("Track", (640, 400));
	cv::createTrackbar("minh", "Track", &minh,179);
	cv::createTrackbar("mins", "Track", &mins, 255);
	cv::createTrackbar("minv", "Track", &minv, 255);

	cv::createTrackbar("maxh", "Track", &maxh, 179);
	cv::createTrackbar("maxs", "Track", &maxs, 255);
	cv::createTrackbar("maxv", "Track", &maxv, 255);

	while (true) {
		cap.read(frame);
		cv::cvtColor(frame, frame, cv::COLOR_BGR2HSV);
		
		cv::Scalar lower = cv::Scalar(minh, mins, minv);
		cv::Scalar upper = cv::Scalar(maxh, maxs, maxv);
		cv::inRange(frame, lower, upper, frame);
		cv::Canny(frame, frame, 50, 150, 3);
		
		

		

		cv::imshow("result", frame);
		cv::waitKey(1);
	}



}

void AngleEstimate::runProgram() {
	cv::VideoCapture cap(0);
	cv::Mat frame;

	while (true) {
		if (GetKeyState('C') & 0x8000) {
			cap.release();
			break;
		}

		cap.read(frame);
		setUpMasks(frame);
		findCont();
		drawBoundingBoxesAndShowAngle(frame);

		cv::imshow("PRESS C TO EXIT", frame);
		cv::waitKey(1);
		//std::cout << mask.size() << "\n";
		clearData();
		
		
	
	}


}

void AngleEstimate::clearData() {
	contours.clear();
	//boundRect.clear();

}

void AngleEstimate::findCont() {
	//contours.clear();
	//is it overriding contours everytime??? --> it does
	std::vector < std::vector < cv::Point>> contours2;
	for (size_t k = 0; k < mask.size(); k++) {
		cv::findContours(mask[k], contours2, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE,
			cv::Point(0,0));
		
		for (size_t i = 0; i < contours2.size(); i++) {
			contours.push_back(contours2[i]);
		}
	}
	
	

	
	
	//boundRect.resize(contours.size());
}

void AngleEstimate::setUpMasks(cv::Mat &img) {
	cv::Mat cpy;
	cv::cvtColor(img, cpy, cv::COLOR_BGR2HSV);

	for (size_t i = 0; i < colors.size(); i++) {
		cv::Scalar lower = cv::Scalar(colors[i][0], colors[i][1], colors[i][2]);
		cv::Scalar upper = cv::Scalar(colors[i][3], colors[i][4], colors[i][5]);
		
		cv::inRange(cpy, lower, upper, mask[i]);
		cv::Canny(mask[i], mask[i], 50, 150, 3);
		//mask.push_back(cpy);
	}

}

void AngleEstimate::drawBoundingBoxesAndShowAngle(cv::Mat& img) {
	cv::Mat lineOnly = cv::Mat::zeros(img.rows,img.cols,CV_8UC1);
	std::vector<cv::Vec2f> lines;

	for (size_t i = 0; i < contours.size(); i++) {
		//cv::drawContours(img, contours, (int)i, cv::Scalar(0, 0, 255));
		if (cv::contourArea(contours[i]) > 300) {
			cv::RotatedRect r = cv::minAreaRect(contours[i]);

			cv::Point2f rect_points[4];
			r.points(rect_points);

			
			double l1 = sqrt(
				(rect_points[0].x - rect_points[1].x) * (rect_points[0].x - rect_points[1].x)
				+ (rect_points[0].y - rect_points[1].y) * (rect_points[0].y - rect_points[1].y)
			);

			double l2 = sqrt(
				(rect_points[1].x - rect_points[2].x) * (rect_points[1].x - rect_points[2].x)
				+ (rect_points[1].y - rect_points[2].y) * (rect_points[1].y - rect_points[2].y)
			);

			if (l1 - l2 > 0.001) { //l1 is bigger

				int xmid0 = (rect_points[0].x + rect_points[3].x) / 2;
				int ymid0 = (rect_points[0].y + rect_points[3].y) / 2;

				int xmid1 = (rect_points[1].x + rect_points[2].x) / 2;
				int ymid1 = (rect_points[1].y + rect_points[2].y) / 2;


				cv::Point start = cv::Point(xmid0, ymid0);
				cv::Point end = cv::Point(xmid1, ymid1);

				cv::line(img, start, end, cv::Scalar(255, 0, 0), 3);
				cv::line(lineOnly, start, end, cv::Scalar(255, 255, 255), 3);

			}else {
				int xmid0 = (rect_points[1].x + rect_points[0].x) / 2;
				int ymid0 = (rect_points[1].y + rect_points[0].y) / 2;

				int xmid1 = (rect_points[2].x + rect_points[3].x) / 2;
				int ymid1 = (rect_points[2].y + rect_points[3].y) / 2;

				cv::Point start = cv::Point(xmid0, ymid0);
				cv::Point end = cv::Point(xmid1, ymid1);

				cv::line(img, start, end, cv::Scalar(255, 0, 0), 3);
				cv::line(lineOnly, start, end, cv::Scalar(255,255,255), 3);

				
			}
			
		}
	}

	cv::HoughLines(lineOnly, lines, 1, CV_PI / 180, 50, 0, 0);
	for (size_t i = 0; i < lines.size(); i++) {
		std::cout << abs((lines[i][1]* 180)/CV_PI - 90)<< " degrees \n";
	}

}
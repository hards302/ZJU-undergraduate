#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define pi 3.1415926
#define H 720
#define W 720
#define PATH "out.avi"

VideoWriter vwriter(PATH, VideoWriter::fourcc('M', 'J', 'P', 'G'), 60, Size(W, H), true);

Mat img;

void gradual(Mat src1, Mat src2) {                        // 两张图片的渐变转换
	Mat miximg;
	for (double i = 0; i <= 1; i += 0.02) {
		addWeighted(src1, 1.0 - i, src2, i, 0, miximg);
		vwriter.write(miximg);
	}
}

void drawPoint(Point center, double radius, Scalar color) {     // 画点
	circle(img, center, radius, color, -1);
}

void drawarc(Point center, double radius, double start, double end, Scalar color, double thick) {  // 画圆弧
	Point p;
	double foot = 0.02;
	for (double r = start; r <= end; r += foot) {
		p.x = center.x + radius * cos(r);
		p.y = center.y + radius * sin(r);
		drawPoint(p, thick, color);
	}
}

void drawEarc(Point center, double radius, double start, double end, float a, float b, Scalar color, int thick, bool isx) { // 画椭圆弧，isx表示焦点是否在x轴上
	Point p;
	double foot = 0.02;
	for (double r = start; r <= end; r += foot) {
		if (isx) {
			p.x = center.x + a * cos(r);
			p.y = center.y + b * sin(r);
		}
		else {
			p.x = center.x + b * cos(r);
			p.y = center.y + a * sin(r);
		}
		drawPoint(p, thick, color);
	}
}

void drawBackground(bool iswrite) {
	rectangle(img, Point(0, 0), Point(W, H), Scalar(0, 0, 0), -1, 8);
	if(iswrite) vwriter.write(img);
}

void drawBackground_every(Mat p, bool iswrite) {
	rectangle(p, Point(0, 0), Point(W, H), Scalar(0, 0, 0), -1, 8);
	if (iswrite) vwriter.write(p);
}

void drawstars(int time) {
	if (time >= 50 && time <= 100) {
		drawPoint(Point(W / 4, H / 6), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 2, H / 7), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 2, H / 5), 2, Scalar(255, 255, 255));
		drawPoint(Point(W * 3 / 4, H / 7), 2, Scalar(255, 255, 255));
		drawPoint(Point(W * 3 / 5, H / 6), 2, Scalar(255, 255, 255));
	}
	if (time > 100 && time <= 150) {
		drawPoint(Point(W / 2, H / 5), 2, Scalar(255, 255, 255));
		drawPoint(Point(W * 3 / 4, H / 7), 2, Scalar(255, 255, 255));
		drawPoint(Point(W * 3 / 5, H / 6), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 4, H / 6), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 5, H / 7), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 4, H / 5), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 3, H / 4.5), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 2, H / 7), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 2, H / 4.5), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 7, H / 5), 2, Scalar(255, 255, 255));
	}
	if (time > 150) {
		drawPoint(Point(W / 2, H / 5), 2, Scalar(255, 255, 255));
		drawPoint(Point(W * 3 / 4, H / 7), 2, Scalar(255, 255, 255));
		drawPoint(Point(W * 3 / 5, H / 6), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 4, H / 6), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 5, H / 7), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 4, H / 5), 2, Scalar(255, 255, 255));
		drawPoint(Point(W * 2 / 3, H / 4), 2, Scalar(255, 255, 255));
		drawPoint(Point(W * 4 / 5, H / 5.5), 2, Scalar(255, 255, 255));
		drawPoint(Point(W * 4 / 5, H / 4), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 3, H / 4.5), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 2, H / 7), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 2, H / 4.5), 2, Scalar(255, 255, 255));
		drawPoint(Point(W / 7, H / 5), 2, Scalar(255, 255, 255));
	}
}

void drawmount() {
	line(img, Point(0, H / 2), Point(W / 6, H / 3), Scalar(255, 255, 255), 2);
	line(img, Point(W / 6, H / 3), Point(W / 3, H / 2), Scalar(255, 255, 255), 2);
	line(img, Point(W / 6, H * 2 / 3), Point(W * 5 / 12, H * 5 / 12), Scalar(255, 255, 255), 2);
	line(img, Point(W * 5 / 12, H * 5 / 12), Point(W * 3 / 4, H * 3 / 4), Scalar(255, 255, 255), 2);
}

void begin_() {
	Mat zjuimg, personimg, miximg, temp;
	zjuimg = imread("zju.jpg");
	personimg = imread("person.jpg");
	resize(zjuimg, zjuimg, Size(W, H));
	resize(personimg, personimg, Size(W, H));
	for (int i = 0; i < 20; i++) vwriter.write(zjuimg);
	gradual(zjuimg, personimg);
	for (int i = 0; i < 20; i++) vwriter.write(personimg);
	string name = "Name: ***", stu_id = "ID: 319*******";
	for (int i = 0; i < 50; i++) {
		drawBackground(false);
		putText(img, stu_id, Point(5 * i, H / 3), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
		putText(img, name, Point(W - 8 * i, H / 2), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
		vwriter.write(img);
	}
	for (int i = 0; i < 5; i++) vwriter.write(img);
}

void scenery() {
	for (int i = 0; i <= 300; i++) {
		drawBackground(false);
		circle(img, Point(W*i / 300, H/10 + abs(i-150) * H / 1000), 12, Scalar(255, 255, 255), -1);  // 日升日落
		drawmount();
		line(img, Point(0, H * 3 / 4), Point(W, H * 3 / 4), Scalar(255, 255, 255), 2);
		for (int j = i+300; j >= 0; j -= 20) {
			line(img, Point(W*j / 300, H * 13 / 16), Point(W*j / 300 + 10, H * 13 / 16), Scalar(255, 255, 255));
			line(img, Point(W*j / 300 + 5, H * 14 / 16), Point(W*j / 300 + 15, H * 14 / 16), Scalar(255, 255, 255));
			line(img, Point(W*j / 300, H * 15 / 16), Point(W*j / 300 + 10, H * 15 / 16), Scalar(255, 255, 255));
		}
		vwriter.write(img);
	}
	for (int i = 0; i <= 300; i++) {
		drawBackground(false);
		drawarc(Point(W*i / 300, H / 10 + abs(i - 150) * H / 1000), 12, -pi / 2, pi / 2, Scalar(255, 255, 255), 1);
		drawEarc(Point(W*i / 300, H / 10 + abs(i - 150) * H / 1000), 12, -pi / 2, pi / 2, 12, 6, Scalar(255,255,255), 1, false);// 月升月落
		drawstars(i);
		drawmount();
		line(img, Point(0, H * 3 / 4), Point(W, H * 3 / 4), Scalar(255, 255, 255), 2);
		for (int j = i+300; j >= 0; j -= 20) {
			line(img, Point(W * j / 300, H * 13 / 16), Point(W * j / 300 + 10, H * 13 / 16), Scalar(255, 255, 255));
			line(img, Point(W * j / 300 + 5, H * 14 / 16), Point(W * j / 300 + 15, H * 14 / 16), Scalar(255, 255, 255));
			line(img, Point(W * j / 300, H * 15 / 16), Point(W * j / 300 + 10, H * 15 / 16), Scalar(255, 255, 255));
		}
		vwriter.write(img);
	}
}

void end_() {
	string str = "Good Night~";
	string str2 = "OpenCV452 by mht";
	Mat last;
	last.create(H, W, CV_8UC3);
	last.setTo(Scalar(0, 0, 0));
	gradual(img, last);
	for (int i = 0; i <= 100; i++) {
		drawBackground_every(last, false);
		putText(last, str, Point(W / 3 - 50, H - H * i / 150), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar(255, 255, 255), 2);
		vwriter.write(last);
	}
	for (int i = 0; i <= 20; i++) {
		drawBackground_every(last, false);
		putText(last, str, Point(W / 3 - 50, H / 3), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar(255, 255, 255), 2);
		putText(last, str2, Point(W * i / 30, H * 7 / 8), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255), 1);
		vwriter.write(last);
	}
	for (int i = 0; i <= 20; i++) vwriter.write(last);
}

void playvideo() {
	VideoCapture cap(PATH);
	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open\n";
		return ;
	}
	Mat frame;
	double rate = cap.get(CAP_PROP_FPS);
	double delay = 1000 / rate;
	while (1) {
		if (!cap.read(frame)) {
			break;
		}
		imshow("Lab1video", frame);
		double c = waitKey(delay);
		if (c >= 0) waitKey(0);
	}
	cap.release();
}

int main() {
	img.create(H, W, CV_8UC3);
	img.setTo(Scalar(255, 255, 255));
	begin_();
	scenery();
	end_();
	playvideo();
	vwriter.release();
	waitKey(0);
}
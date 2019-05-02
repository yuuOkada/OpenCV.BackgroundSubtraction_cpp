// Opencv.BackgroundDifferenceMethod.cpp

#include "pch.h"
#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main() {
	VideoCapture cap(1);

	Mat frm, bg, dst, diff, gry;

	const char th = 20;  //閾値設定

	//背景画像を取得 スペースで背景画像取得
	while (waitKey(1) != 32) {
		cap >> frm;
		frm.copyTo(bg);
		imshow("bg", bg);
	}

	while (waitKey(1) != 27) {
		cap >> frm;

		absdiff(frm, bg, diff);  //背景画像との差分を取得
		cvtColor(diff, gry, CV_BGR2GRAY);  //差分画像をグレイスケールに
		threshold(gry, dst, th, 255, THRESH_BINARY);  //二値画像化

		imshow("dst", dst);
		imshow("frm", frm);
	}
}

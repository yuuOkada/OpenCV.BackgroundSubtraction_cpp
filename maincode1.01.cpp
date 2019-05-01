// Opencv.BackgroundDifferenceMethod.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main() {
	VideoCapture cap(1);

	Mat frm, bg, dst, diff, gry;
	Mat element = Mat::ones(3, 3, CV_8UC1);

	//テキストファイルに出力
	char filename[] = "output.txt";
	FILE *fp;
	fopen_s(&fp, filename, "w");
	int i = 0;

	//閾値
	const char th = 120;

	//背景画像を取得 スペースで背景画像取得
	while (waitKey(1) != 32) {
		cap >> frm;
		frm.copyTo(bg);
		imshow("BackGround", bg);
	}

	while (waitKey(1) != 27) {
		i++;

		cap >> frm;

		absdiff(frm, bg, diff);//背景画像との差分を取得
		cvtColor(diff, gry, CV_BGR2GRAY);//差分画像をグレイスケールに
		threshold(gry, dst, th, 255, THRESH_BINARY);//二値化画像化
		dilate(dst, dst, element, Point(-1, -1), 3);

		//２値画像から輪郭線を抽出
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		//輪郭内の面積計算
		double max_area = 0;
		int max_area_contour = -1;
		for (int j = 0; j < contours.size(); j++) {
			double area = contourArea(contours.at(j));
			if (max_area < area) {
				max_area = area;
				max_area_contour = j;
			}
		}

		//目標物と推定された物体の重心計算
		long count = contours.at(max_area_contour).size();
		double x = 0;
		double y = 0;
			for (long k = 0; k < count; k++) {
				x += contours.at(max_area_contour).at(k).x;
				y += contours.at(max_area_contour).at(k).y;
			}
			x /= count;
			y /= count;
			circle(frm, Point(x, y), 50, Scalar(0, 0, 255), 3, 4);

			fprintf(fp, "%d,%d,%d\n", i, (int)x, (int)y);

		imshow("dst", dst);
		imshow("frm", frm);

		//フレームが空か、ボタンが押された時か一周したときに出る。
		if (frm.empty() || waitKey(30) >= 0 || cap.get(CV_CAP_PROP_POS_AVI_RATIO) == 1) {
			break;
		}

		//待機時間
		waitKey(1);

	}

	fclose(fp);
	return(0);

}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します

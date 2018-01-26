#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>

using namespace std;

#define YUDO 0.5
#define IOU_OUTPUT 0.7
#define DISTANCE 200.0

class Place {
public:
	int x;
	int y;
	int width;
	int height;
	float yudo;

	int teri_num;

	Place() {
		x = y = -1;
		width = height = -1;
		yudo = 0.0;

		teri_num = -1;
	}
};

class Place_2 {
	public:
		int C_x;
		int C_y;
		int C_width;
		int C_height;
		float C_yudo;

		int F_x;
		int F_y;
		int F_width;
		int F_height;
		float F_yudo;

		int teri_num;

		Place_2() {
			C_x = C_y = -1;
			C_width = C_height = -1;
			C_yudo = 0.0;

			F_x = F_y = -1;
			F_width = F_height = -1;
			F_yudo = 0.0;

			teri_num = -1;
		}
};

float evaluation(cv::Mat RE, cv::Mat GT) {
	float Overlap = 0.0, Union = 0.0;
	for (int n = 0; n < RE.rows; n++) {
		for (int m = 0; m < RE.cols; m++) {
			if (RE.at<cv::Vec3b>(n, m) == cv::Vec3b(255, 255, 255) || GT.at<cv::Vec3b>(n, m) == cv::Vec3b(255, 255, 255)) {
				Union += 1.0;
				if (RE.at<cv::Vec3b>(n, m) == cv::Vec3b(255, 255, 255) && GT.at<cv::Vec3b>(n, m) == cv::Vec3b(255, 255, 255)) {
					Overlap += 1.0;
				}
			}
		}
	}

	return Overlap / Union;
}

void Result_IoU() {
	//テスト画像ファイル一覧メモ帳読み込み
	char GT_name[1024];
	FILE *GT_data;
	fopen_s(&GT_data, "C:/photo/GT_binary_list2.txt", "r");

	while (fgets(GT_name, 256, GT_data) != NULL) {

		string name_GT = GT_name;
		char new_GT_name[1024];
		char new_result_name[1024];
		for (int i = 0; i < name_GT.length() - 1; i++) {
			new_GT_name[i] = GT_name[i];
			new_GT_name[i + 1] = '\0';
			new_result_name[i] = GT_name[i];
			new_result_name[i + 1] = '\0';
		}
		char GT_n[1024] = "C:/photo/GT_binary/";
		char RE_n[1024] = "C:/photo/result_data_from_demo/CD-0.99_FD-0.999/result_binary/";
		strcat_s(GT_n, new_GT_name);
		strcat_s(RE_n, new_result_name);
		cv::Mat GT = cv::imread(GT_n, 1);
		cv::Mat RE = cv::imread(RE_n, 1);
		//		cv::imshow("GT", GT);
		//		cv::imshow("RE", RE);
		//		cvWaitKey(10);
		cout << new_result_name << "," << evaluation(RE, GT) << endl;

	}

	fclose(GT_data);

}

void Result_IoU_2() {
	//テキストファイルのリスト読み込み
	char List_n[1024];
	FILE *List;
	if (fopen_s(&List, "c:/photo/tmp.txt", "r") != 0) {
		cout << "not found List file" << endl;
		return ;
	}
	while (fgets(List_n, 256, List) != NULL) {
		string List_str = List_n;
		char List_name[1024];
		for (int i = 0; i < List_str.length() - 1; i++) {
			List_name[i] = List_n[i];
			List_name[i + 1] = '\0';
		}
		char GT_name[1024] = "c:/photo/GT_text/";
		strcat_s(GT_name, List_name);
		//GTファイル読み込み
		char GT_n[4][1024];
		FILE *GT;
		if (fopen_s(&GT, GT_name, "r") != 0) {
			cout << "not found GT file" << endl;
			return ;
		}
		Place place_GT[10];
		int num_G = 0;
		while (fgets(GT_n[0], 256, GT) != NULL) {	//すべて読み込み，変数に格納
			fgets(GT_n[1], 256, GT);
			fgets(GT_n[2], 256, GT);
			fgets(GT_n[3], 256, GT);

			place_GT[num_G].x = atoi(GT_n[0]);
			place_GT[num_G].y = atoi(GT_n[1]);
			place_GT[num_G].width = atoi(GT_n[2]);
			place_GT[num_G].height = atoi(GT_n[3]);
			num_G++;
		}
		fclose(GT);
		char Result_name[1024] = "C:/photo/result_data_from_demo/2018_01_04_AP/result_data/";
		strcat_s(Result_name, List_name);
		//Resultファイル読み込み
		char Result_n[5][1024];
		FILE *Result;
		if (fopen_s(&Result, Result_name, "r") != 0) {
			cout << "not found Result file" << endl;
			return ;
		}
		Place place_Result[10];
		int num_R = 0;
		while (fgets(Result_n[0], 256, Result) != NULL) {	//すべて読み込み，変数に格納
			fgets(Result_n[1], 256, Result);
			fgets(Result_n[2], 256, Result);
			fgets(Result_n[3], 256, Result);
			fgets(Result_n[4], 256, Result);

			fgets(Result_n[0], 256, Result);
			fgets(Result_n[1], 256, Result);
			fgets(Result_n[2], 256, Result);
			fgets(Result_n[3], 256, Result);
			fgets(Result_n[4], 256, Result);


			place_Result[num_R].yudo = atof(Result_n[0]);
			place_Result[num_R].x = atoi(Result_n[1]);
			place_Result[num_R].y = atoi(Result_n[2]);
			place_Result[num_R].width = atoi(Result_n[3]);
			place_Result[num_R].height = atoi(Result_n[4]);
			num_R++;
		}
		fclose(Result);

		//バイナリ画像生成
		cv::Mat GT_Binary = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3);
		cv::Mat Result_Binary = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3);

		for (int i = 0; i < num_G; i++) {
			for (int n = place_GT[i].y; n < place_GT[i].y + place_GT[i].height; n++) {
				for (int m = place_GT[i].x; m < place_GT[i].x + place_GT[i].width; m++) {
					GT_Binary.at<cv::Vec3b>(n, m) = cv::Vec3b(255, 255, 255);
				}
			}
		}

		for (int i = 0; i < num_R; i++) {
			for (int n = place_Result[i].y; n < place_Result[i].y + place_Result[i].height; n++) {
				for (int m = place_Result[i].x; m < place_Result[i].x + place_Result[i].width; m++) {
					Result_Binary.at<cv::Vec3b>(n, m) = cv::Vec3b(255, 255, 255);
				}
			}
		}
		cout << List_name << "," << evaluation(Result_Binary, GT_Binary) << endl;
	}

	fclose(List);
}

cv::Mat draw_rectangle(cv::Mat ans_im, int x, int y, int width, int height, int r, int g, int b) {
	rectangle(ans_im, cvPoint(x, y), cvPoint(x + width, y + height), CV_RGB(r, g, b), 1);
	return ans_im;
}

void Result_MR_and_FPPI(float yudo, float iou_output) {
	//変数宣言
	float miss_rate = 0;
	float fppi = 0;
	int num = 0;

	//テキストファイルのリスト読み込み
	char List_n[1024];
	FILE *List;
	if (fopen_s(&List, "c:/photo/text_list.txt", "r") != 0) {
		cout << "not found List file" << endl;
		return;
	}
	while (fgets(List_n, 256, List) != NULL) {
		string List_str = List_n;
		char List_name[1024];
		for (int i = 0; i < List_str.length() - 1; i++) {
			List_name[i] = List_n[i];
			List_name[i + 1] = '\0';
		}
		char GT_name[1024] = "c:/photo/GT_text/";
		strcat_s(GT_name, List_name);
		//GTファイル読み込み
		char GT_n[4][1024];
		FILE *GT;
		if (fopen_s(&GT, GT_name, "r") != 0) {
			cout << "not found GT file" << endl;
			return;
		}

		Place place_GT[10];
		int num_G = 0;
		while (fgets(GT_n[0], 256, GT) != NULL) {	//すべて読み込み，変数に格納
			fgets(GT_n[1], 256, GT);
			fgets(GT_n[2], 256, GT);
			fgets(GT_n[3], 256, GT);

			place_GT[num_G].x = atoi(GT_n[0]);
			place_GT[num_G].y = atoi(GT_n[1]);
			place_GT[num_G].width = atoi(GT_n[2]);
			place_GT[num_G].height = atoi(GT_n[3]);
			num_G++;
		}
		fclose(GT);

		char Result_name[1024] = "C:/photo/result_data_from_demo/2017_12_29_EP/result_data/";
		strcat_s(Result_name, List_name);
		//Resultファイル読み込み
		char Result_n[5][1024];
		FILE *Result;
		if (fopen_s(&Result, Result_name, "r") != 0) {
			cout << "not found Result file" << endl;
			return;
		}

		Place place_Result[10];
		int num_R = 0;
		while (fgets(Result_n[0], 256, Result) != NULL) {	//すべて読み込み，変数に格納
			fgets(Result_n[1], 256, Result);
			fgets(Result_n[2], 256, Result);
			fgets(Result_n[3], 256, Result);
			fgets(Result_n[4], 256, Result);

			place_Result[num_R].yudo = atof(Result_n[0]);
			place_Result[num_R].x = atoi(Result_n[1]);
			place_Result[num_R].y = atoi(Result_n[2]);
			place_Result[num_R].width = atoi(Result_n[3]);
			place_Result[num_R].height = atoi(Result_n[4]);
			num_R++;
		}
		fclose(Result);

		//2値画像作成
		//GT
		cv::Mat GT_B[10] = {
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
		};
		for (int i = 0; i < num_G; i++) {
			for (int n = place_GT[i].y; n < place_GT[i].y + place_GT[i].height; n++) {
				for (int m = place_GT[i].x; m < place_GT[i].x + place_GT[i].width; m++) {
					GT_B[i].at<cv::Vec3b>(n, m) = cv::Vec3b(255, 255, 255);
				}
			}
		}

		//Result
		cv::Mat Result_B[10] = {
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
			cv::Mat::zeros(480, 640, CV_8UC3),
		};

		for (int i = 0; i < num_R; i++) {
			if (place_Result[i].yudo < yudo) continue;
			for (int n = place_Result[i].y; n < place_Result[i].y + place_Result[i].height; n++) {
				for (int m = place_Result[i].x; m < place_Result[i].x + place_Result[i].width; m++) {
					Result_B[i].at<cv::Vec3b>(n, m) = cv::Vec3b(255, 255, 255);
				}
			}
		}
		int point[10] = { 0,0,0,0,0,0,0,0,0,0 };
		//GTとResultの中心画素距離算出
		for (int n = 0; n < num_G; n++) {
			for (int m = 0; m < num_R; m++) {
				if (abs((place_GT[n].x + place_GT[n].width / 2) - (place_Result[m].x + place_Result[m].width / 2)) <= 50
					&&
					abs((place_GT[n].y + place_GT[n].height / 2) - (place_Result[m].y + place_Result[m].height / 2)) <= 50) {

					point[n] = 1;
				}
			}
		}

		float ok = 0.0;
		//(GT&Result)/GT 算出
		//		for (int i = 0; i < num_G; i++) {
		//			float IoU = evaluation(Result_B[i],GT_B[i]);
		//			if (IoU >= iou_output && point[i]==1) {
		//				ok+=1.0;
		//			}
		//		}
		for (int i = 0; i < num_G; i++) {
			if (point[i] == 1) {
				for (int j = 0; j < num_R; j++) {
					float Union = 0, Overlap = 0;

					for (int n = 0; n < 480; n++) {
						for (int m = 0; m < 640; m++) {
							if (GT_B[i].at<cv::Vec3b>(n, m) == cv::Vec3b(255, 255, 255)) {
								Union += 1.0;
								if (Result_B[i].at<cv::Vec3b>(n, m) == cv::Vec3b(255, 255, 255)) {
									Overlap += 1.0;
								}
							}
						}
					}

					if ((Overlap / Union) >= iou_output) {
						ok += 1.0;
						break;
					}
				}
			}
		}

		//Miss Rate算出(OKの数/GTの数)
		miss_rate += 1.0 - (ok / (float)num_G);

		//FPPI算出(Resultの数-OKの数)
		fppi += (float)num_R - ok;
		//		cout << "miss rate:" << miss_rate << ",fppi:" << fppi << endl;
		num++;

		//		cout << List_name << endl;
		//		cout << "miss rate:" << miss_rate << endl;
		//		cout << "fppi" << fppi << endl;
	}
	cout << yudo << "," << miss_rate / (float)num << "," << fppi / (float)num << endl;
	//	cout << "fppi" << fppi / (float)num << endl;

	fclose(List);
}

void Merge() {
	//テキストファイルのリスト読み込み
	char List_n[1024];
	FILE *List;
	if (fopen_s(&List, "c:/photo/text_list_4.txt", "r") != 0) {
		cout << "not found List file" << endl;
		return;
	}
	while (fgets(List_n, 256, List) != NULL) {
		string List_str = List_n;
		char List_name[1024];
		for (int i = 0; i < List_str.length() - 1; i++) {
			List_name[i] = List_n[i];
			List_name[i + 1] = '\0';
		}

		char Result_name[1024] = "c:/photo/result_data_from_demo/2018_01_13_EP/result_data/";
		strcat_s(Result_name, List_name);
		//Resultファイル読み込み
		char Result_n[5][1024];
		FILE *Result;
		if (fopen_s(&Result, Result_name, "r") != 0) {
			cout << "not found Result file" << endl;
			return;
		}

		//記録用ファイル作成
		char Save_name[1024] = "c:/photo/result_data_from_demo/2018_01_13_EP/save_data/0.5_200/";
		strcat_s(Save_name, List_name);
		//Resultファイル読み込み
		char Save_n[1024];
		FILE *Save;
		if (fopen_s(&Save, Save_name, "w") != 0) {
			cout << "not found Save file" << endl;
			return;
		}

		cout << List_name << endl;

		Place place_Result[2000];
		int num_R = 0;
		//検出結果の読み込み
		while (fgets(Result_n[0], 256, Result) != NULL) {	//すべて読み込み，変数に格納
			fgets(Result_n[1], 256, Result);
			fgets(Result_n[2], 256, Result);
			fgets(Result_n[3], 256, Result);
			fgets(Result_n[4], 256, Result);
			
			place_Result[num_R].yudo = atof(Result_n[0]);
			place_Result[num_R].x = atoi(Result_n[1]);
			place_Result[num_R].y = atoi(Result_n[2]);
			place_Result[num_R].width = atoi(Result_n[3]);
			place_Result[num_R].height = atoi(Result_n[4]);

			//一定の尤度に届いていない場合，値を保存しない
			if (place_Result[num_R].yudo < YUDO) {
				place_Result[num_R].yudo = 0;
				place_Result[num_R].x = -1;
				place_Result[num_R].y = -1;
				place_Result[num_R].width = -1;
				place_Result[num_R].height = -1;

				continue;
			}
			num_R++;
		}
		fclose(Result);

		//MeanShiftのようなもの
		int territory_num = 1;
		while (1) {
			float maximam = 0.0;
			int num = 0;
			//番号の割り振られていない領域のうち最大値を探索
			for (int i = 0; i < num_R; i++) {
				if (place_Result[i].teri_num == -1) {
					if (maximam < place_Result[i].yudo) {
						maximam = place_Result[i].yudo;
						num = i;
					}
				}
			}
			if (maximam == 0.0) {
				break;
			}

			place_Result[num].teri_num = territory_num;
			territory_num++;

			for (int i = 0; i < num_R; i++) {
				if (place_Result[i].teri_num == -1 &&
					abs((place_Result[i].x + (float)place_Result[i].width / 2) - (place_Result[num].x + (float)place_Result[num].width / 2)) <= DISTANCE &&
					abs((place_Result[i].y + (float)place_Result[i].height / 2) - (place_Result[num].y + (float)place_Result[num].height / 2)) <= DISTANCE)

					place_Result[i].teri_num = place_Result[num].teri_num;
			}

			//検出結果の記録
			fprintf_s(Save, "%f", place_Result[num].yudo);
			fprintf_s(Save, "\n");
			fprintf_s(Save, "%d", place_Result[num].x);
			fprintf_s(Save, "\n");
			fprintf_s(Save, "%d", place_Result[num].y);
			fprintf_s(Save, "\n");
			fprintf_s(Save, "%d", place_Result[num].width);
			fprintf_s(Save, "\n");
			fprintf_s(Save, "%d", place_Result[num].height);
			fprintf_s(Save, "\n");

		//	printf("%f, %d, %d, %d, %d\n", place_Result[num].yudo, place_Result[num].x, place_Result[num].y, place_Result[num].width, place_Result[num].height);

		}
		fclose(Save);
	}


	fclose(List);
}

void Merge_2() {
	//テキストファイルのリスト読み込み
	char List_n[1024];
	FILE *List;
	if (fopen_s(&List, "c:/photo/text_list_4.txt", "r") != 0) {
		cout << "not found List file" << endl;
		return;
	}
	while (fgets(List_n, 256, List) != NULL) {
		string List_str = List_n;
		char List_name[1024];
		for (int i = 0; i < List_str.length() - 1; i++) {
			List_name[i] = List_n[i];
			List_name[i + 1] = '\0';
		}

		char Result_name[1024] = "c:/photo/result_data_from_demo/2018_01_15_AP/result_data/";
		strcat_s(Result_name, List_name);
		//Resultファイル読み込み
		char Result_n[10][1024];
		FILE *Result;
		if (fopen_s(&Result, Result_name, "r") != 0) {
			cout << "not found Result file" << endl;
			return;
		}

		//記録用ファイル作成
		char Save_name[1024] = "c:/photo/result_data_from_demo/2018_01_15_AP/save_data/0.5_20/";
		strcat_s(Save_name, List_name);
		//Resultファイル読み込み
		char Save_n[1024];
		FILE *Save;
		if (fopen_s(&Save, Save_name, "w") != 0) {
			cout << "not found Save file" << endl;
			return;
		}

		cout << List_name << endl;

		Place_2 place_Result[2000];
		int num_R = 0;
		//検出結果の読み込み
		while (fgets(Result_n[0], 256, Result) != NULL) {	//すべて読み込み，変数に格納
			fgets(Result_n[1], 256, Result);
			fgets(Result_n[2], 256, Result);
			fgets(Result_n[3], 256, Result);
			fgets(Result_n[4], 256, Result);

			place_Result[num_R].C_yudo = atof(Result_n[0]);
			place_Result[num_R].C_x = atoi(Result_n[1]);
			place_Result[num_R].C_y = atoi(Result_n[2]);
			place_Result[num_R].C_width = atoi(Result_n[3]);
			place_Result[num_R].C_height = atoi(Result_n[4]);

			fgets(Result_n[5], 256, Result);
			fgets(Result_n[6], 256, Result);
			fgets(Result_n[7], 256, Result);
			fgets(Result_n[8], 256, Result);
			fgets(Result_n[9], 256, Result);

			place_Result[num_R].F_yudo = atof(Result_n[5]);
			place_Result[num_R].F_x = atoi(Result_n[6]);
			place_Result[num_R].F_y = atoi(Result_n[7]);
			place_Result[num_R].F_width = atoi(Result_n[8]);
			place_Result[num_R].F_height = atoi(Result_n[9]);

			//一定の尤度に届いていない場合，値を保存しない
			if (place_Result[num_R].F_yudo < YUDO) {
				place_Result[num_R].C_yudo = 0;
				place_Result[num_R].C_x = -1;
				place_Result[num_R].C_y = -1;
				place_Result[num_R].C_width = -1;
				place_Result[num_R].C_height = -1;
				place_Result[num_R].F_yudo = 0;
				place_Result[num_R].F_x = -1;
				place_Result[num_R].F_y = -1;
				place_Result[num_R].F_width = -1;
				place_Result[num_R].F_height = -1;

				continue;
			}
			num_R++;
		}
		fclose(Result);

		//MeanShiftのようなもの
		int territory_num = 1;
		while (1) {
			float maximam = 0.0;
			int num = 0;
			//番号の割り振られていない領域のうち最大値を探索
			for (int i = 0; i < num_R; i++) {
				if (place_Result[i].teri_num == -1) {
					if (maximam < place_Result[i].F_yudo) {
						maximam = place_Result[i].F_yudo;
						num = i;
					}
				}
			}
			if (maximam == 0.0) {
				break;
			}

			place_Result[num].teri_num = territory_num;
			territory_num++;

			for (int i = 0; i < num_R; i++) {
				if (place_Result[i].teri_num == -1 &&
					abs((place_Result[i].C_x + (float)place_Result[i].C_width / 2) - (place_Result[num].C_x + (float)place_Result[num].C_width / 2)) <= DISTANCE &&
					abs((place_Result[i].C_y + (float)place_Result[i].C_height / 2) - (place_Result[num].C_y + (float)place_Result[num].C_height / 2)) <= DISTANCE)

					place_Result[i].teri_num = place_Result[num].teri_num;
			}

			//検出結果の記録
			fprintf_s(Save, "%f", place_Result[num].C_yudo);
			fprintf_s(Save, "\n");
			fprintf_s(Save, "%d", place_Result[num].C_x);
			fprintf_s(Save, "\n");
			fprintf_s(Save, "%d", place_Result[num].C_y);
			fprintf_s(Save, "\n");
			fprintf_s(Save, "%d", place_Result[num].C_width);
			fprintf_s(Save, "\n");
			fprintf_s(Save, "%d", place_Result[num].C_height);
			fprintf_s(Save, "\n");
			fprintf_s(Save, "%f", place_Result[num].F_yudo);
			fprintf_s(Save, "\n");
			fprintf_s(Save, "%d", place_Result[num].F_x);
			fprintf_s(Save, "\n");
			fprintf_s(Save, "%d", place_Result[num].F_y);
			fprintf_s(Save, "\n");
			fprintf_s(Save, "%d", place_Result[num].F_width);
			fprintf_s(Save, "\n");
			fprintf_s(Save, "%d", place_Result[num].F_height);
			fprintf_s(Save, "\n");

		//	printf("%f, %d, %d, %d, %d\n", place_Result[num].F_yudo, place_Result[num].F_x, place_Result[num].F_y, place_Result[num].F_width, place_Result[num].F_height);

		}
		fclose(Save);
	}


	fclose(List);
}


int main(int argc, char** argv) {
	Merge();
	

	return 0;
}

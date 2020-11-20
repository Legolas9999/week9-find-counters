#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

int main() {
//-------------------轮胎图片筛选圆圈---------------------------------------------
	Mat tire;			//轮胎灰度图
	Mat tire_rgb;		//轮胎rgb图
	Mat tire_erzhi;		//轮胎二值图
	Mat tire_re;		//前景和背景转换
	Mat tire_labels;	//连通域标记函数中的labels
	Mat tire_stats;		//连通域标记函数中的状态矩阵
	Mat tire_cen;		//连通域标记函数中的中心矩阵
	Mat tire_disMat;	//目标图像
	int circles;		//返回 连通域+背景 个数
	tire_rgb = imread("tire.jpg");
	tire = imread("tire.jpg",0);
	threshold(tire ,tire_erzhi , 120, 255, THRESH_OTSU);	//二值化
	
	tire_re = 255 - tire_erzhi;		//前景和背景转换
	
	circles = connectedComponentsWithStats(tire_re, tire_labels, tire_stats, tire_cen,4,CV_32S);	//连通域标记函数
	
	

	for (int i = 1; i < circles ; i++) {				//给符合要求的连通域画矩形
		if ((tire_stats.at<int>(i, 4) < 5500)&&(tire_stats.at<int>(i,4)>1500)) {	//连通域筛选条件（连通域面积）
			Rect rect;
			rect.x = tire_stats.at<int>(i,0);
			rect.y = tire_stats.at<int>(i, 1);
			rect.width= tire_stats.at<int>(i, 2);
			rect.height = tire_stats.at<int>(i, 3);
			rectangle(tire_rgb, rect, Scalar(0, 0, 255), 1, 8, 0);	//画矩形
		
		}
		
	}
	vector<vector<Point>> tire_contours;		//是一个双重向量（向量内每个元素保存了一组由连续的Point构成的点的集合的向量），每一组点集就是一个轮廓，有多少轮廓，contours就有多少元素；
	findContours(tire_re, tire_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);		//寻找边缘，生成tire_contours
	
	for (size_t i = 0; i < tire_contours.size(); i++) {			//给符合条件的连通域上色
		int len = tire_contours[i].size();						//第i个连通域的点的个数赋值给len
		if ((len>120)&&(len<500)) {								//通过len的取值来筛选需要标记的连通域
			drawContours(tire_rgb, tire_contours, i, Scalar(rand() % 255, rand() % 255, rand() % 255), -1, 0);	//给符合条件的连通域以随机颜色填充内部（-1）
		}
	}

	
	
	imshow("tire",tire_rgb);
	//------------------------------芯片图片，筛选中心部分----------------------------------------------
	Mat chip;				//芯片灰度图
	Mat chip_erzhi;			//芯片二值图
	Mat chip_rgb;			//芯片rgb图
	chip_rgb = imread("chip.jpg");
	chip = imread("chip.jpg", 0);
	threshold(chip,chip_erzhi, 120, 255, THRESH_OTSU);		//二值化
	vector<vector<Point>> chip_contours;		//是一个双重向量（向量内每个元素保存了一组由连续的Point构成的点的集合的向量），每一组点集就是一个轮廓，有多少轮廓，contours就有多少元素；
	vector<Vec4i> hierarchy;		//继承啥的。。
	

	findContours(chip_erzhi,chip_contours, hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);		//寻找边缘，并生成chip_contours

	for (size_t i = 0; i < chip_contours.size(); i++) {			
		RotatedRect rbox = minAreaRect(chip_contours[i]);					//生成rbox 为第i个连通域的最小外接矩形
		drawContours(chip_rgb, chip_contours, i, Scalar(rand()%255, rand() % 255, rand() % 255), 1, 8);	//给第i个连通域边缘上色
		Size2f ck = rbox.size;									//.size为成员函数，生成ck，包含该最小外接矩形的长宽面积等。。。
		if ((ck.height>40)&&(ck.width>40)) {					//连通域筛选条件（最小外接矩形的长和宽都要大于40）若连通域符合条件
			Point2f vtx[4];										//vtx[4]为该最小外接矩形的四个顶点
			rbox.points(vtx);									//给vtx赋值
			for (int j = 0; j < 4; j++) {					//绘制最小外接矩形
				line(chip_rgb, vtx[j], vtx[(j + 1) % 4], CV_RGB(0, 255, 50), 4);		//用line函数绘制最小外接矩形
			}
			
		}
		
	}
	
	imshow("chip",chip_rgb);
	
	waitKey(0);

}
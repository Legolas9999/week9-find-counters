#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

int main() {
//-------------------��̥ͼƬɸѡԲȦ---------------------------------------------
	Mat tire;			//��̥�Ҷ�ͼ
	Mat tire_rgb;		//��̥rgbͼ
	Mat tire_erzhi;		//��̥��ֵͼ
	Mat tire_re;		//ǰ���ͱ���ת��
	Mat tire_labels;	//��ͨ���Ǻ����е�labels
	Mat tire_stats;		//��ͨ���Ǻ����е�״̬����
	Mat tire_cen;		//��ͨ���Ǻ����е����ľ���
	Mat tire_disMat;	//Ŀ��ͼ��
	int circles;		//���� ��ͨ��+���� ����
	tire_rgb = imread("tire.jpg");
	tire = imread("tire.jpg",0);
	threshold(tire ,tire_erzhi , 120, 255, THRESH_OTSU);	//��ֵ��
	
	tire_re = 255 - tire_erzhi;		//ǰ���ͱ���ת��
	
	circles = connectedComponentsWithStats(tire_re, tire_labels, tire_stats, tire_cen,4,CV_32S);	//��ͨ���Ǻ���
	
	

	for (int i = 1; i < circles ; i++) {				//������Ҫ�����ͨ�򻭾���
		if ((tire_stats.at<int>(i, 4) < 5500)&&(tire_stats.at<int>(i,4)>1500)) {	//��ͨ��ɸѡ��������ͨ�������
			Rect rect;
			rect.x = tire_stats.at<int>(i,0);
			rect.y = tire_stats.at<int>(i, 1);
			rect.width= tire_stats.at<int>(i, 2);
			rect.height = tire_stats.at<int>(i, 3);
			rectangle(tire_rgb, rect, Scalar(0, 0, 255), 1, 8, 0);	//������
		
		}
		
	}
	vector<vector<Point>> tire_contours;		//��һ��˫��������������ÿ��Ԫ�ر�����һ����������Point���ɵĵ�ļ��ϵ���������ÿһ��㼯����һ���������ж���������contours���ж���Ԫ�أ�
	findContours(tire_re, tire_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);		//Ѱ�ұ�Ե������tire_contours
	
	for (size_t i = 0; i < tire_contours.size(); i++) {			//��������������ͨ����ɫ
		int len = tire_contours[i].size();						//��i����ͨ��ĵ�ĸ�����ֵ��len
		if ((len>120)&&(len<500)) {								//ͨ��len��ȡֵ��ɸѡ��Ҫ��ǵ���ͨ��
			drawContours(tire_rgb, tire_contours, i, Scalar(rand() % 255, rand() % 255, rand() % 255), -1, 0);	//��������������ͨ���������ɫ����ڲ���-1��
		}
	}

	
	
	imshow("tire",tire_rgb);
	//------------------------------оƬͼƬ��ɸѡ���Ĳ���----------------------------------------------
	Mat chip;				//оƬ�Ҷ�ͼ
	Mat chip_erzhi;			//оƬ��ֵͼ
	Mat chip_rgb;			//оƬrgbͼ
	chip_rgb = imread("chip.jpg");
	chip = imread("chip.jpg", 0);
	threshold(chip,chip_erzhi, 120, 255, THRESH_OTSU);		//��ֵ��
	vector<vector<Point>> chip_contours;		//��һ��˫��������������ÿ��Ԫ�ر�����һ����������Point���ɵĵ�ļ��ϵ���������ÿһ��㼯����һ���������ж���������contours���ж���Ԫ�أ�
	vector<Vec4i> hierarchy;		//�̳�ɶ�ġ���
	

	findContours(chip_erzhi,chip_contours, hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);		//Ѱ�ұ�Ե��������chip_contours

	for (size_t i = 0; i < chip_contours.size(); i++) {			
		RotatedRect rbox = minAreaRect(chip_contours[i]);					//����rbox Ϊ��i����ͨ�����С��Ӿ���
		drawContours(chip_rgb, chip_contours, i, Scalar(rand()%255, rand() % 255, rand() % 255), 1, 8);	//����i����ͨ���Ե��ɫ
		Size2f ck = rbox.size;									//.sizeΪ��Ա����������ck����������С��Ӿ��εĳ�������ȡ�����
		if ((ck.height>40)&&(ck.width>40)) {					//��ͨ��ɸѡ��������С��Ӿ��εĳ��Ϳ�Ҫ����40������ͨ���������
			Point2f vtx[4];										//vtx[4]Ϊ����С��Ӿ��ε��ĸ�����
			rbox.points(vtx);									//��vtx��ֵ
			for (int j = 0; j < 4; j++) {					//������С��Ӿ���
				line(chip_rgb, vtx[j], vtx[(j + 1) % 4], CV_RGB(0, 255, 50), 4);		//��line����������С��Ӿ���
			}
			
		}
		
	}
	
	imshow("chip",chip_rgb);
	
	waitKey(0);

}
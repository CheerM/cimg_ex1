#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include <iostream>
#include <limits.h>
#include <math.h>
#include <vector>
#include "CImg.h"

#define PI 3.1415926

using namespace cimg_library;
using namespace std;

//计算两点之间的距离
double distance(int x1, int y1, int x2, int y2) {
	double ans = pow(static_cast<double>(x1 - x2), 2) + pow(static_cast<double>(y1 - y2), 2);
	return sqrt(ans);
};

//(positionX, positionY)旋转中心位置
//angle表示逆时针旋转角度，可以为负数
//boundary表示是否用0补全边界
CImg<unsigned char> myRotate(CImg<unsigned char>& inputImage, const int positionX, const int positionY, const float angle, bool boundary) {
	unsigned int width = inputImage.width();
	unsigned int height = inputImage.height();
	unsigned int depth = inputImage.depth();
	unsigned int spectrum = inputImage.spectrum();

	//四个顶点到原点的距离
	double distancePoint[4];
	distancePoint[0] = distance(0, 0, positionX, positionY);
	distancePoint[1] = distance(static_cast<int>(width), 0, positionX, positionY);
	distancePoint[2] = distance(0, static_cast<int>(height), positionX, positionY);
	distancePoint[3] = distance(static_cast<int>(width), static_cast<int>(height), positionX, positionY);

	//四个顶点与中心的夹角（0-2PI）
	double anglePoint[4];
	anglePoint[0] = PI - atan((positionY - 0.0) / positionX) + angle / 180 * PI;
	anglePoint[1] = atan((positionY - 0.0) / (width - positionX)) + angle / 180 * PI;
	anglePoint[2] = PI + atan((height - positionY + 0.0) / positionX) + angle / 180 * PI;
	anglePoint[3] = 2 * PI - atan((height - positionY + 0.0) / (width - positionX)) + angle / 180 * PI;

	//四个顶点的初始坐标
	int origin_x[4], origin_y[4];
	origin_x[0] = 0;
	origin_y[0] = 0;
	origin_x[1] = width;
	origin_y[1] = 0;
	origin_x[2] = 0;
	origin_y[2] = height;
	origin_x[3] = width;
	origin_y[3] = height;

	//旋转后四个顶点的坐标
	//y'=x*sin(a)+y*cos(a);
     	//x'=x*cos(a)-y*sin(a);  
	int x[4], y[4];

	//得到旋转后的图片像素分布范围
	int minX = INT_MAX, maxX = INT_MIN, minY = INT_MAX, maxY = INT_MIN;
	for (int i = 0; i < 4; i ++) {
		x[i] = positionX + static_cast<int>(floor(cos(anglePoint[i]) * distancePoint[i]));
		y[i] = positionY - static_cast<int>(floor(sin(anglePoint[i]) * distancePoint[i]));
		minX = minX > x[i] ? x[i] : minX;
		maxX = maxX < x[i] ? x[i] : maxX;
		minY = minY > y[i] ? y[i] : minY;
		maxY = maxY < y[i] ? y[i] : maxY;
	}

	//新建两张图，一张不补边，一张补边
	CImg<unsigned char> img(width, height, depth, spectrum);
	CImg<unsigned char> imgBoundary(maxX - minX, maxY - minY, depth, spectrum);
	img.fill(0);
	imgBoundary.fill(0);

	for (int xx = 0; xx < width; xx ++)
		for (int yy = 0; yy < height; yy ++) {
			//对于每一个像素，算出它到中心的距离，以及XY轴的直线距离
			//然后求出一个角度，在根据像素所在的象限来具体求出偏角
			double distPoint = distance(xx, yy, positionX, positionY);
			double distX = abs(static_cast<double>(xx - positionX));
			double distY = abs(static_cast<double>(yy - positionY));
			double angPoint;
			if (xx < positionX && yy < positionY) {
				angPoint = PI - atan(distY / distX) + angle / 180 * PI;
			}
			else if (xx >= positionX && yy <= positionY) {
				angPoint = atan(distY / distX) + angle / 180 * PI;
			}
			else if (xx <= positionX && yy >= positionY) {
				angPoint = PI + atan(distY / distX) + angle / 180 * PI;
			}
			else {
				angPoint = 2 * PI - atan(distY / distX) + angle / 180 * PI;
			}
			
			//为了解决抗锯齿的问题，对每个浮点数向上向下取证，所以可以得到四个邻近点，填充这四个点————解决空洞问题
			int newPointX1 = positionX + static_cast<int>(floor(cos(angPoint) * distPoint));
			int newPointY1 = positionY - static_cast<int>(floor(sin(angPoint) * distPoint));
			int newPointX2 = positionX + static_cast<int>(ceil(cos(angPoint) * distPoint));
			int newPointY2 = positionY - static_cast<int>(ceil(sin(angPoint) * distPoint));
			
			if(boundary == 0) {
				if (newPointY1 < height && newPointY1>= 0 && newPointX1 < width && newPointX1>=0
				&& newPointY2 < height && newPointY2>= 0 && newPointX2 < width && newPointX2>=0) {
					for (int zz = 0; zz < spectrum; zz ++) {
						img.atXYZC(newPointX1, newPointY1, 1, zz) = inputImage.atXYZC(xx, yy, 1, zz);
						img.atXYZC(newPointX1, newPointY2, 1, zz) = inputImage.atXYZC(xx, yy, 1, zz);
						img.atXYZC(newPointX2, newPointY1, 1, zz) = inputImage.atXYZC(xx, yy, 1, zz);
						img.atXYZC(newPointX2, newPointY2, 1, zz) = inputImage.atXYZC(xx, yy, 1, zz);
					}
				}
			}
			else {
				if (newPointY1 < maxY && newPointY1 >= minY && newPointX1 < maxX && newPointX1 >= minX
				&& newPointY2 < maxY && newPointY2 >= minY && newPointX2 < maxX && newPointX2 >= minX) {
					for (int zz = 0; zz < spectrum; zz ++) {
						imgBoundary.atXYZC(newPointX1 - minX, newPointY1 - minY, 1, zz) = inputImage.atXYZC(xx, yy, 1, zz);
						imgBoundary.atXYZC(newPointX1 - minX, newPointY2 - minY, 1, zz) = inputImage.atXYZC(xx, yy, 1, zz);
						imgBoundary.atXYZC(newPointX2 - minX, newPointY1 - minY, 1, zz) = inputImage.atXYZC(xx, yy, 1, zz);
						imgBoundary.atXYZC(newPointX2 - minX, newPointY2 - minY, 1, zz) = inputImage.atXYZC(xx, yy, 1, zz);
					}
				}
			}
			
		}

	if (boundary == 0)return img;
	else return imgBoundary;
};

//interpolation表示缩放方法(true为双线性插值，false为邻近插值)
CImg<unsigned char> myScale(CImg<unsigned char>& inputImage, const float scale, bool interpolation) {

	unsigned int width = static_cast<unsigned int>(inputImage.width() * scale);
	unsigned int height = static_cast<unsigned int>(inputImage.height() * scale);
	unsigned int depth = inputImage.depth();
	unsigned int spectrum = inputImage.spectrum();
	CImg<unsigned char> img(width, height, depth, spectrum);

	if (interpolation == 0) {
		//邻近插值
		for (int x = 0; x < width; x ++)
			for (int y = 0; y < height; y ++)
				for (int z = 0; z < spectrum; z ++) {
					int srcX = static_cast<int>(x / scale);
					int srcY = static_cast<int>(y / scale);
					img.atXYZC(x, y, 1, z) = inputImage.atXYZC(srcX, srcY, 1, z);
				}
		return img;
	}
	else if (interpolation == 1) {
		//双线性插值
		//f(x,y)=f(0,0)(1-x)(1-y)+f(0,1)(1-x)y+f(1,1)xy+f(1,0)x(1-y)
		for (int x = 0; x < width; x ++)
			for (int y = 0; y < height; y ++)
				for (int z = 0; z < spectrum; z ++) {
					int srcX1 = static_cast<int>(ceil(x / scale));
					int srcX2 = static_cast<int>(floor(x / scale));
					int srcY1 = static_cast<int>(ceil(y / scale));
					int srcY2 = static_cast<int>(floor(y / scale));
					unsigned char Point_X1Y1 = inputImage.atXYZC(srcX1, srcY1, 1, z);
					unsigned char Point_X1Y2 = inputImage.atXYZC(srcX1, srcY2, 1, z);
					unsigned char Point_X2Y1 = inputImage.atXYZC(srcX2, srcY1, 1, z);
					unsigned char Point_X2Y2 = inputImage.atXYZC(srcX2, srcY2, 1, z);
					img.atXYZC(x, y, 1, z) = static_cast<unsigned char>(Point_X1Y1 * 0.3 * 0.3 + Point_X1Y2 * 0.3 * 0.7 + Point_X2Y2 * 0.7 * 0.7 + Point_X2Y1 * 0.3 * 0.7);
				}
		return img;
	}
	else 
		return inputImage;
};

//只需要确定对角两点的坐标、颜色、实心与否，就可以得到一个具体的矩形
void drawRec(CImg<unsigned char>& inputImage, vector<int>& position, vector<int>& color, bool solid) {
	unsigned int width = inputImage.width();
	unsigned int height = inputImage.height();
	unsigned int depth = inputImage.depth();
	unsigned int spectrum = inputImage.spectrum();

	//算法优化，尽量地减少遍历范围
	int minX = position[0] > position[2] ? position[2] : position[0];
	int maxX = position[0] > position[2] ? position[0] : position[2];
	int minY = position[1] > position[3] ? position[3] : position[1];
	int maxY = position[1] > position[3] ? position[1] : position[3];
	
	//遍历判断
	for (int x = minX; x <= maxX; x ++) {
		for (int y = minY; y <= maxY; y ++) {
			if (y >= 0 && y < height && x >= 0 && x < width) {
				for (int z = 0; z < spectrum; z ++){
					if (solid) {
						inputImage.atXYZC(x, y, 1, z) = static_cast<unsigned char>(color[z]);
					}
					else {
						if (x == minX || x == maxX || y == minY || y == maxY) {
							inputImage.atXYZC(x, y, 1, z) = static_cast<unsigned char>(color[z]);
						}
					}
				}
			}
		}
	}
};

//一个三角形，需要依靠三个顶点来确立，同样还需要颜色以及空心与否等条件
void drawTri(CImg<unsigned char>& inputImage,  vector<int>& position, vector<int>& color, bool solid ) {
	unsigned int width = inputImage.width();
	unsigned int height = inputImage.height();
	unsigned int depth = inputImage.depth();
	unsigned int spectrum = inputImage.spectrum();

	////算法优化，尽量地减少遍历范围
	int minX = (position[0] > position[2] ? position[2] : position[0]) > position[4] ? position[4] : (position[0] > position[2] ? position[2] : position[0]);
	int maxX = (position[0] > position[2] ? position[0] : position[2]) < position[4] ? position[4] : (position[0] > position[2] ? position[2] : position[0]);
	int minY = (position[1] > position[3] ? position[3] : position[1]) > position[5] ? position[5] : (position[1] > position[3] ? position[3] : position[1]);
	int maxY = (position[1] > position[3] ? position[1] : position[3]) < position[5] ? position[5] : (position[1] > position[3] ? position[3] : position[1]);

	//三条直线方程
	double k1, b1, k2, b2, k3, b3;
	k1 = (position[3] - position[1]) / (position[2] - position[0] + 0.0);
	b1 = position[1] - k1 * position[0];
	k2 = (position[5] - position[3]) / (position[4] - position[2] + 0.0);
	b2 = position[3] - k2 * position[2];
	k3 = (position[5] - position[1]) / (position[4] - position[0] + 0.0);
	b3 = position[1] - k3 * position[0];

	//遍历范围增大，防止边界无法检测 +2, 此处不需要担心超出边界，因为在最后赋值的时候会再判断一次
	for (int x = minX - 2; x <= maxX + 2; x ++) {
		for (int y = minY - 2; y <= maxY + 2; y ++) {
			if (y >= 0 && y < height && x >= 0 && x < width) {
				//按照三角形绘制算法，我们需要判断某一点落在正确的一边就需要依赖三角形中的第三个点
				//P1P2形成直线L1，判断（X， Y）是否落在正确的一边，就把P3和该点一起带入直线方程，同时大于0，或者同时小于0则为真（即有效）
				//判断某一点是否对于三条直线方程都符合条件（都为真，则该点落在三角形内）
				int ans1 = static_cast<int>(position[5] - (position[4] * k1 + b1));
				int ans2 = static_cast<int>(y - (x * k1 + b1));
				int ans3 = static_cast<int>(position[1] - (position[0] * k2 + b2));
				int ans4 = static_cast<int>(y - (x * k2 + b2));
				int ans5 = static_cast<int>(position[3] - (position[2] * k3 + b3));
				int ans6 = static_cast<int>(y - (x * k3 + b3));
				bool line1 = false, line2 = false, line3 = false;
				if ((ans1 > 0 && ans2 >= -1) || (ans1 < 0 && ans2 <= 1))line1 = true;
				if ((ans3 > 0 && ans4 >= -1) || (ans3 < 0 && ans4 <= 1))line2 = true;
				if ((ans5 > 0 && ans6 >= -1) || (ans5 < 0 && ans6 <= 1))line3 = true;
				if (line1 && line2 && line3) {
					for (int z = 0; z < spectrum; z ++){
						if (solid) {
							inputImage.atXYZC(x, y, 1, z) = static_cast<unsigned char>(color[z]);
						}
						else {
							if (ans2 == 0 || ans4 == 0 ||ans6 == 0) {
								inputImage.atXYZC(x, y, 1, z) = static_cast<unsigned char>(color[z]);
							}
						}
					}
				}
				
			}
		}
	}
};

//画圆只需要确定圆心坐标、半径、颜色和是否填充就可以了
void drawCir(CImg<unsigned char>& inputImage, int positionX, int positionY, int radius, vector<int>& color, bool solid) {
	unsigned int width = inputImage.width();
	unsigned int height = inputImage.height();
	unsigned int depth = inputImage.depth();
	unsigned int spectrum = inputImage.spectrum();

	////算法优化，尽量地减少遍历范围
	int minX = positionX - radius;
	int maxX = positionX + radius;
	int minY = positionY - radius;
	int maxY = positionY + radius;

	for (int x = minX - 2; x <= maxX + 2; x ++) {
		for (int y = minY - 2; y <= maxY + 2; y ++) {
			if (y >= 0 && y < height && x >= 0 && x < width) {
				for (int z = 0; z < spectrum; z ++){
					if (solid) {
						if (abs(static_cast<int>(distance(x, y, positionX, positionY)))<= radius)
							inputImage.atXYZC(x, y, 1, z) = static_cast<unsigned char>(color[z]);
					}
					else  {
						if (abs(static_cast<int>(distance(x, y, positionX, positionY)) - radius) <= 1)
							inputImage.atXYZC(x, y, 1, z) = static_cast<unsigned char>(color[z]);
					}
				}
			}
		}
	}
};

#endif
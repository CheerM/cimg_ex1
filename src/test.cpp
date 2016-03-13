#include "myFunction.h"


int main(int argc, char* argv[]) {
	while (1) {
		string inputImageName;
		cout << "0. exit\n1. scale\n2. rotate\n3. draw" << endl;
		cin >> inputImageName;
		if (inputImageName == "0")break;
		else if (inputImageName == "1"){
			cout << "Please input your image name:";
			cin >> inputImageName;
			inputImageName = "images/" + inputImageName;
			CImg<unsigned char> srcImage1(inputImageName.data());
			float scale;
			bool way;
			cout << "the scale: " << endl;
			cin >> scale;
			cout << "0. Nearest Neighbor Interpolation\n1. Bilinear Interpolation" << endl;
			cin >> way;
			CImg<unsigned char> outputImage1 = myScale(srcImage1, scale, way);
			outputImage1.display("Scale");
		}
		else if (inputImageName == "2") {
			cout << "Please input your image name:";
			cin >> inputImageName;
			inputImageName = "images/" + inputImageName;
			CImg<unsigned char> srcImage2(inputImageName.data());

			unsigned int positionX, positionY;
			float angle;
			bool boundary;
			cout << "positionX: " << endl;
			cin >> positionX;
			cout << "positionY: " << endl;
			cin >> positionY;
			cout << "angle: " << endl;
			cin >> angle;
			cout << "0. do nothing\n1. fill with 0 : " << endl;
			cin >> boundary;
			CImg<unsigned char> outputImage2 = myRotate(srcImage2, positionX, positionY, angle, boundary);
			outputImage2.display("Rotate");
		}
		else if (inputImageName == "3") {
			CImg<unsigned char> outputImage3(1024, 1024, 1, 3);
			outputImage3.fill(0);
			while(1) {
				int type;
				cout << "0. exit\n1. Rectangle\n2. Triangle\n3. Circle" << endl;
				cin >> type;
				if (type == 0)break;
				else if (type == 1) {
					int p;
					vector<int> position1;
					cout << "positionX1:\n";
					cin >> p;
					position1.push_back(p);
					cout << "positionY1:\n";
					cin >> p;
					position1.push_back(p);
					cout << "positionX2:\n";
					cin >> p;
					position1.push_back(p);
					cout << "positionY2:\n";
					cin >> p;
					position1.push_back(p);

					int R, G, B;
					vector<int> color1;
					cout << "(R, G, B) = " << endl;
					cin >> R;
					cin >> G;
					cin >> B;
					color1.push_back(R);
					color1.push_back(G);
					color1.push_back(B);

					int solid;
					cout << "0. hollow\n1. solid\n";
					cin >> solid;
					drawRec(outputImage3, position1, color1, solid);
					outputImage3.display("Draw");
				}
				else if (type == 2) {
					int p;
					vector<int> position2;
					cout << "positionX1:\n";
					cin >> p;
					position2.push_back(p);
					cout << "positionY1:\n";
					cin >> p;
					position2.push_back(p);
					cout << "positionX2:\n";
					cin >> p;
					position2.push_back(p);
					cout << "positionY2:\n";
					cin >> p;
					position2.push_back(p);
					cout << "positionX3:\n";
					cin >> p;
					position2.push_back(p);
					cout << "positionY3:\n";
					cin >> p;
					position2.push_back(p);

					int R, G, B;
					vector<int> color2;
					cout << "(R, G, B) = " << endl;
					cin >> R;
					cin >> G;
					cin >> B;
					color2.push_back(R);
					color2.push_back(G);
					color2.push_back(B);

					int solid;
					cout << "0. hollow\n1. solid\n";
					cin >> solid;
					drawTri(outputImage3, position2, color2, solid);
					outputImage3.display("Draw");
				}
				else if (type == 3) {
					int positionX, positionY, radius;
					cout << "positionX:\n";
					cin >> positionX;
					cout << "positionY:\n";
					cin >> positionY;
					cout << "radius:\n";
					cin >> radius;

					int R, G, B;
					vector<int> color3;
					cout << "(R, G, B) = " << endl;
					cin >> R;
					cin >> G;
					cin >> B;
					color3.push_back(R);
					color3.push_back(G);
					color3.push_back(B);

					int solid;
					cout << "0. hollow\n1. solid\n";
					cin >> solid;
					drawCir(outputImage3, positionX, positionY, radius, color3, solid);
					outputImage3.display("Draw");
				}
				else {
					cout << "Please input the correct numbers!!\n";
				}
			}
			
			
		}
		else {
			cout << "Please input the correct numbers!!\n";
		}
	}
  	return 0;
}
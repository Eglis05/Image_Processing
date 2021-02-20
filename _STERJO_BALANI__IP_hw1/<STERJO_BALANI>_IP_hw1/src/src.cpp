#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <string.h>

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui.hpp>

using namespace std;
using namespace cv;


void write_into_file(string filename, vector< vector<int> > cp){
	fstream fout;

	fout.open(filename, fstream::out);
	if(!fout.good()){
		cerr << "Sth wrong with the output filestream\n";
		exit(1);
	}

	for(unsigned int i = 0; i < cp.size(); i++){
		for(unsigned int j = 0; j < cp[0].size()-1; j++){
			fout << cp[i][j] << ", ";
		}
		fout << cp[i][cp[0].size()-1];
		if(i < cp.size()-1){
			fout << endl;
		}

	}
	fout.close();
}

vector< vector<int> > reflect(vector< vector<int> > matrix){
	unsigned int N = matrix.size();
	unsigned int M = matrix[0].size();
	int temp;

	for(unsigned int i = 0; i < (unsigned int) (N/2); i++){
		for(unsigned int j = 0; j < M; j++){

			//reflective swap
			temp = matrix[i][j];
			matrix[i][j] = matrix[N-i-1][M-j-1];
			matrix[N-i-1][M-j-1] = temp;
		}
	}

	for(unsigned int i = 0; i< (unsigned int) (M/2); i++){
		temp = matrix[(unsigned int) (N/2)][i];
		matrix[(unsigned int) (N/2)][i] = matrix[(unsigned int) (N/2)][M-1-i];
		matrix[(unsigned int) (N/2)][M-1-i] = temp;
	}

	return matrix;
}

vector< vector<int> > pad_array(vector< vector<int> > img, vector< vector<int> > se, char opt){

	vector< vector<int> > v( img.size() + se.size()-1 );

	for(unsigned int i = 0; i < v.size(); i++){
		for(unsigned int j = 0; j < img[0].size() + se[0].size()-1; j++){
			if(i >= (unsigned int) (se.size()/2) && i < img.size() + (unsigned int) (se.size()/2) && 
			   j >= (unsigned int) (se[0].size()/2) && j < img[0].size() + (unsigned int) (se[0].size()/2) ){
				v[i].push_back( img[i-(unsigned int)(se.size()/2)][j-(unsigned int)(se[0].size()/2)] );
			}
			else{
				if(opt == 'd'){
					v[i].push_back(0);
				}
				else if(opt == 'e'){
					v[i].push_back(255);
				}
				else{
					cout << "Unsupported functionality\n";
					exit(1);
				}
			}
		}
	}

	return v;
}

void print_image(vector< vector<int> > cp){
	for(unsigned int i = 0; i < cp.size(); i++){
		for(unsigned int j=0; j < cp[0].size(); j++){
			cout << cp[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

vector< vector<int> > dilation(vector< vector<int> > img, vector< vector<int> > se){
	int max = -1;
	
	vector< vector<int> > v = pad_array(img, se, 'd');
	vector< vector<int> > cp = img;
	
	
	///Erosion Process
	for(unsigned int i = 0; i < img.size(); i++){
		for(unsigned int j = 0; j < img[0].size(); j++){
			max = -1;
			for(unsigned int k = 0; k < se.size(); k++){
				for(unsigned int l = 0; l < se[0].size(); l++){
					if(se[k][l] == 1 && v[i+k][j+l] > max){
						max = v[i+k][j+l];
					}
				}
			}

			cp[i][j] = max;
		}
	}
	return cp;
}

vector< vector<int> > erosion(vector< vector<int> > img, vector< vector<int> > se){
	int min = 256;

	vector< vector<int> > v = pad_array(img, se, 'e');
	vector< vector<int> > cp = img;
	
	
	///Erosion Process
	for(unsigned int i = 0; i < img.size(); i++){
		for(unsigned int j = 0; j < img[0].size(); j++){
			min = 256;
			for(unsigned int k = 0; k < se.size(); k++){
				for(unsigned int l = 0; l < se[0].size(); l++){
					if(se[k][l] == 1 && v[i+k][j+l] < min){
						min = v[i+k][j+l];
					}
				}
			}

			cp[i][j] = min;
		}
	}

	return cp;
}

vector< vector<int> > opening(vector< vector<int> > img, vector< vector<int> > se){
	return dilation( erosion(img,se) , reflect(se) );
}

vector< vector<int> > closing(vector< vector<int> > img, vector< vector<int> > se){
	return erosion( dilation(img,se) , reflect(se) );
}

vector< vector<int> > load_data(string filename){
	int n, index=0;
	vector< vector<int> > a;
	ifstream in;

	in.open(filename);
	if(!in.is_open()){
		cerr << "Error opening file\n";
		exit(1);
	}

	
	a.push_back(vector<int>());
	while(in >> n){
		a[index].push_back(n);

		if(in.peek() == '\n'){
			index++;
			a.push_back(vector<int>());
		}
		in.ignore();
	}

	in.close();

	return a;
}

void write_into_image(string filename, vector< vector<int> > cp){
	vector<uint8_t> vec;
	for(unsigned int i = 0; i < cp.size(); i++){
		for(unsigned int j = 0; j < cp[0].size(); j++){
			vec.push_back(cp[i][j]);
		}
	}
	cv::Mat grayImage(cp.size(), cp[0].size(), CV_8U, vec.data()); //check reverse of size position
	filename.replace(filename.length() - 3, filename.length(), "png");
	cv::imwrite(filename, grayImage);
}

vector< vector<int> > load_image(string filename){
	cv::Mat img = imread(filename, IMREAD_GRAYSCALE);
	int index=0;
	vector< vector<int> > a;
	
	for(int i = 0; i < img.rows; i++, index++){
		a.push_back(vector<int>());
		for(int j = 0; j < img.cols; j++){
			a[index].push_back(img.at<uchar>(i,j));
		}

	}

	return a;
}

int main(int argc, const char *argv[]) {
	vector< vector<int> > SE, image;


	SE = load_data(argv[2]);
	if (strcmp(argv[3], "../input/noise.png") == 0 || strcmp(argv[3], "../input/gaps.png") == 0  || strcmp(argv[3], "../input/separate.png") == 0){
		image = load_image(argv[3]);
	}
	else{
		image = load_data(argv[3]);
	}
	
	switch(argv[1][0]){
		case 'e':
			image = erosion (image, SE);
			break;
		case 'd':
			image = dilation(image, SE);
			break;
		case 'o':
			image = opening (image, SE);
			break;
		case 'c':
			image = closing (image, SE);
			break;
		default:
			cout << "No optionality matches\nExiting...\n";
			exit(1);
	}

	write_into_file(argv[4], image);
	if(strcmp(argv[3], "../input/f3.txt") == 0 || strcmp(argv[3], "../output/ef3_e3.txt") == 0 || strcmp(argv[3], "../output/ef3_e4.txt") == 0 || strcmp(argv[3], "../input/noise.png") == 0 || strcmp(argv[3], "../input/gaps.png") == 0 || strcmp(argv[3], "../input/separate.png") == 0){
		write_into_image(argv[4], image);
	}

	return 0;
}
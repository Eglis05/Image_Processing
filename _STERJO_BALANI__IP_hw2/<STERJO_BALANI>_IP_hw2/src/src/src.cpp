#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <string.h>
#include <queue> 
#include <algorithm>
#include <functional>
#include <array>

#include </usr/local/include/opencv4/opencv2/opencv.hpp>
#include </usr/local/include/opencv4/opencv2/highgui.hpp>

using namespace std;
using namespace cv;

#define mask -2
#define wshed 0
#define init -1
#define inqueue -3
#define MAXIMUM 2147483647 //largest integer

struct pixel{
	int value;
	int x;
	int y;

	pixel(int v = 0, int a = 0, int b = 0){
		value = v;
		x = a;
		y = b;
	}

	bool operator< (const pixel& p) const{
        return (value < p.value);
    }
};


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


vector< vector<int> > dilation(vector< vector<int> > img, vector< vector<int> > se){
	int max = 0;
	
	vector< vector<int> > v = pad_array(img, se, 'd');
	vector< vector<int> > cp = img;
	
	
	///Erosion Process
	for(unsigned int i = 0; i < img.size(); i++){
		for(unsigned int j = 0; j < img[0].size(); j++){
			max = 0;
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
	int min = 255;

	vector< vector<int> > v = pad_array(img, se, 'e');
	vector< vector<int> > cp = img;
	
	
	///Erosion Process
	for(unsigned int i = 0; i < img.size(); i++){
		for(unsigned int j = 0; j < img[0].size(); j++){
			min = 255;
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




int hmin = MAXIMUM;

void print_image(vector< vector<int> > cp){
	for(unsigned int i = 0; i < cp.size(); i++){
		for(unsigned int j=0; j < cp[i].size(); j++){
			cout << cp[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

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

		if(in.peek() == '\n' || in.peek() == '\r'){
			hmin = min(*std::min_element(a[index].begin(), a[index].end()), hmin);
			index++;
			a.push_back(vector<int>());
		}
		in.ignore();
	}
	// hmin = min(*std::min_element(a[index].begin(), a[index].end()), hmin); //not sure if needed
	// In case of negative pixels
	if (hmin < 0){
		for(int i = 0; i < index; i++){
			for(unsigned int j = 0; j < a[i].size(); j++){
				a[i][j] -= hmin; //now it is >= 0
			}
		}
		hmin = 0;
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
	cv::Mat grayImage(cp.size(), cp[0].size(), CV_8U, vec.data());
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

vector< vector<int> > initialize_f0(vector< vector<int> > f1){
	vector< vector<int> > f0;
	for(unsigned int i = 0; i < f1.size()-1; i++){
		f0.push_back(vector<int>());
		for(unsigned int j = 0; j < f1[i].size(); j++){
			f0[i].push_back(init);
		}
	}
	return f0;
}

vector< pixel > neighborhood(pixel p, int g, int max_x, int max_y){
	vector< pixel > neighbors;
	int x[3] = {-1, 0, 1}, y[3] = {-1, 0, 1};
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			if (g == 8 || i == 1 || j == 1){
				pixel n_p(0, p.x + x[i], p.y + y[j]); //value not needed here just in sorting
				if (n_p.x >= 0 && n_p.x < max_x && n_p.y >= 0 && n_p.y < max_y){
					neighbors.push_back(n_p);
				}
			}
		}
	}
	return neighbors;
}

vector< pixel > sort_pixels(vector< vector<int> > f1){
	vector< pixel > image;

	for(int i = 0; i < (int) f1.size(); i++){
		for(int j = 0; j < (int) f1[i].size(); j++){
			pixel p = {f1[i][j], i, j};
			image.push_back(p);
		}
	}

	std::sort(image.begin(), image.end());
	return image;
}

vector< vector<int> > watershed_alg(vector< vector<int> > f1, int g){
	vector< vector<int> > f0 = initialize_f0(f1);
	int current_label = 0;
	bool flag;
	queue< pixel > fifo_queue;

	vector< pixel > sorted_pixels = sort_pixels(f1);

	for(int j = 0, h = hmin; j < (int) sorted_pixels.size(); h = sorted_pixels[min(j, (int) sorted_pixels.size()-1)].value){
		for(int i = j; i < (int) sorted_pixels.size() && sorted_pixels[i].value == h; i++){
			pixel p = sorted_pixels[i];
			f0[p.x][p.y] = mask;
			vector< pixel > neighbors = neighborhood(p, g, f1.size()-1, f1[0].size());
			for(int j = 0; j < (int) neighbors.size(); j++){
				pixel pp = neighbors[j];
				if(f0[pp.x][pp.y] > 0 || f0[pp.x][pp.y] == wshed){
					f0[p.x][p.y] = inqueue;
					fifo_queue.push(p);
				}
			}
		}

		while (!fifo_queue.empty()) {
			pixel p = fifo_queue.front();
			fifo_queue.pop();
			vector< pixel > neighbors = neighborhood(p, g, f1.size()-1, f1[0].size());
			for(int i = 0; i < (int) neighbors.size(); i++){
				pixel pp = neighbors[i];
				if(f0[pp.x][pp.y] > 0){
					if(f0[p.x][p.y] == inqueue || (f0[p.x][p.y] == wshed && flag == true)){
						f0[p.x][p.y] = f0[pp.x][pp.y];
					}
					else if(f0[p.x][p.y] > 0 && f0[p.x][p.y] != f0[pp.x][pp.y]){
						f0[p.x][p.y] = wshed;
						flag = false;
					}
				}
				else if(f0[pp.x][pp.y] == wshed){
					if(f0[p.x][p.y] == inqueue){
						f0[p.x][p.y] = wshed;
						flag = true;
					}
				}
				else if(f0[pp.x][pp.y] == mask){
					f0[pp.x][pp.y] = inqueue;
					fifo_queue.push(pp);
				}
			}
		}

		for(; j < (int) sorted_pixels.size() && sorted_pixels[j].value == h; j++){
			pixel p = sorted_pixels[j];
			if(f0[p.x][p.y] == mask){
				current_label += 1;
				fifo_queue.push(p);
				f0[p.x][p.y] = current_label;
				while (!fifo_queue.empty()) {
					pixel pp = fifo_queue.front();
					fifo_queue.pop();
					vector< pixel > neighbors = neighborhood(pp, g, f1.size()-1, f1[0].size());
					for(int k = 0; k < (int) neighbors.size(); k++){
						pixel ppp = neighbors[k];
						if(f0[ppp.x][ppp.y] == mask){
							fifo_queue.push(ppp);
							f0[ppp.x][ppp.y] = current_label;
						}
					}
				}
			}
		}

	}


	return f0;
}

int main(int argc, const char *argv[]) {
	vector< vector<int> > image;


	if(argc==5){
		image = load_image(argv[1]);
		vector< vector<int> > SE = {{1,1,1},{1,1,1},{1,1,1}};
		//Applying opening
		image = erosion(dilation(image, SE), SE);
	}
	else{
		image = load_data(argv[1]);
	}

	int g = atoi(argv[2]);

	image = watershed_alg(image, g);

	write_into_file(argv[3], image);

	return 0;
}
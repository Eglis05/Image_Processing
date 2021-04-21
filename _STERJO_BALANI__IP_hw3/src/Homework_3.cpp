#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <iterator>

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui.hpp>

#define NIL pmf.end()
#define rows_fig (int)fig1.size()
#define cols_fig (int)fig1[0].size()

using namespace std;
using namespace cv;

typedef pair<float, float> x_y;


map< pair<float,float>, float > joint_pmf_2D(vector< vector<float> > fig1, vector< vector<float> > fig2, int bin_size){
	map< pair<float,float>, float > pmf;
	map< pair<float,float>, float >::iterator it;

	float size_total = rows_fig*cols_fig;

	for(int i = 0; i < rows_fig; i++){
		for(int j = 0; j < cols_fig; j++){
			x_y p( (int) fig1[i][j]/bin_size, (int) fig2[i][j]/bin_size);

			if( (it = pmf.find(p)) == NIL){
				pmf.insert(make_pair(p, 1/size_total));
			}
			else{
				(it->second) += 1/size_total;
			}

		}

	}

	return pmf;
}


map<float, float> pmf_1D(vector< vector<float> > fig1, int bin_size){
	map<float, float> pmf;
	map<float, float>::iterator it;

	float size_total = rows_fig*cols_fig;

	for(int i = 0; i < rows_fig; i++){
		for(int j = 0; j < cols_fig; j++){
			float p = (int) fig1[i][j]/bin_size;

			if( (it = pmf.find(p)) == NIL){
				pmf.insert(pair<float, float> (p, 1/size_total) );
			}
			else{
				(it->second) += 1/size_total;
			}
		}
	}

	return pmf;
}


vector< vector<float> > crop_left_right(int pixel_no, vector< vector<float> > img){
	int beg = pixel_no;
	int fin = img[0].size() - beg;
	vector< vector<float> > out;

	if(beg > fin){cerr<<"Impossible cropping\n"; exit(1);}

	for(int i = 0; i < (int)img.size(); i++){
		out.push_back( vector<float>() );
		for(int j = beg; j < fin; j++){
			out[i].push_back(img[i][j]);
		}
	}

	return out;
}



float mutual_information(map< pair<float,float>, float > joint_pmf, map<float, float> pmf1, map<float, float> pmf2){
	map< pair<float,float>, float >::iterator it;
	float MI = 0;
	float p_1, p_2;

	for(it = joint_pmf.begin(); it != joint_pmf.end(); it++){
		
		p_1 = (pmf1.find((it->first).first))->second;
		p_2 = (pmf2.find((it->first).second))->second;

		MI += it->second * log10(it->second/(p_2*p_1));
		// cout<<"p1 of "<< (it->first).first <<": "<<p_1<<endl;
		// cout<<"p2 of "<< (it->first).second <<": "<<p_2<<endl;
		// cout<<"MI: "<<MI<<endl;

	}


	return MI;
}


vector<float> MI_n_steps(int steps, int bin_size, vector< vector<float> > img1, vector< vector<float> > img2){
	vector<float> results;
	int i = 0;

	vector< vector<float> > cp;

	cp.resize(img2.size());

	for(int index = 0; index != (int) cp.size(); index++){
		cp[index].resize(img2[0].size());
	}


	

	while(i != steps){


		for(int j = 0; j < (int) img2.size(); j++){
			for(int k = 0; k < (int) img2[0].size(); k++){
				cp[j][k] = img1[j][k+i];
			}
		}
		// for(int h=0;h<cp1.size();h++){
		// 	for(int u=0;u<cp1[0].size();u++){
		// 		cout<<i<<"("<<cp1[h][u]<<" "<<cp2[h][u]<<")";
		// 	}
		// 	cout<<endl;
		// }


		map< pair<float,float>, float > joint_pmf = joint_pmf_2D(cp, img2, bin_size);
		map<float, float> pmf1 = pmf_1D(cp, bin_size);
		map<float, float> pmf2 = pmf_1D(img2, bin_size);


		results.push_back( mutual_information(joint_pmf, pmf1, pmf2) );

		i++;
	}

	return results;

}

vector< vector<float> > load_image_color(string filename, int choice){
	cv::Mat img = imread(filename, cv::IMREAD_COLOR);

	cv::Mat bgr[3];
	cv::split(img, bgr);

	img = bgr[choice];

	int index = 0;
	vector< vector<float> > a;
	
	for(int i = 0; i < img.rows; i++, index++){
		a.push_back(vector<float>());
		for(int j = 0; j < img.cols; j++){
			a[index].push_back(img.at<uchar>(i,j));
		}

	}

	return a;
}

int main(int argc, const char *argv[]) {

	vector< vector<float> > g, r;

	g = load_image_color(argv[1], 1);
	g = crop_left_right(1,g);
	r = load_image_color(argv[1], 2);

	int b = atoi(argv[2]);

	std::ofstream output_file(argv[3]);
    std::ostream_iterator<float> output_iterator(output_file, "\n");

	vector<float> result = MI_n_steps(41, b, r, g);

    copy(result.begin(), result.end(), output_iterator);

	return 0;
}

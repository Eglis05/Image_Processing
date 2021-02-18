#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <string.h>

using namespace std;


void write_into_file(string filename, vector< vector<int> > cp){
	fstream fout;

	fout.open(filename,fstream::out);
	if(!fout.good()){
		cerr<<"Sth wrong with the output filestream\n";
		exit(1);
	}

	for(int i=0;i<cp.size();i++){
		for(int j=0;j<cp[0].size()-1;j++){
			fout<<cp[i][j]<< ", ";
		}
		fout<<cp[i][cp[0].size()-1];
		if(i<cp.size()-1){
			fout<<endl;
		}

	}
	fout.close();
}

vector< vector<int> > reflect(vector< vector<int> > matrix){
	int N = matrix.size();
	int M = matrix[0].size();
	int temp;
	

	for(int i=0;i<(int)(N/2);i++){
		for(int j=0;j<M;j++){

			//reflective swap
			temp = matrix[i][j];
			matrix[i][j] = matrix[N-i-1][M-j-1];
			matrix[N-i-1][M-j-1] = temp;
		}
	}

	for(int i=0;i<(int) (M/2);i++){
		temp = matrix[(int) (N/2)][i];
		matrix[(int) (N/2)][i] = matrix[(int) (N/2)][M-1-i];
		matrix[(int) (N/2)][M-1-i] = temp;
	}

	return matrix;
}

vector< vector<int> > pad_array(vector< vector<int> > img, vector< vector<int> > se, char opt){

	vector< vector<int> > v( img.size() + (int)se.size()-1 );

	for(int i=0;i<v.size();i++){
		for(int j=0;j<img[0].size()+se[0].size()-1;j++){
			if(i >= (int) (se.size()/2) && i < img.size() + (int) (se.size()/2) && 
			   j >= (int) (se[0].size()/2) && j < img[0].size() + (int) (se[0].size()/2) ){
				v[i].push_back( img[i-(int)(se.size()/2)][j-(int)(se[0].size()/2)] );
			}
			else{
				if(opt == 'd'){
					v[i].push_back(-1);
				}
				else if(opt == 'e'){
					v[i].push_back(256);
				}
				else{
					cout<<"Unsupported functionality\n";
					exit(1);
				}
			}
		}
	}

	return v;
}

void print_image(vector< vector<int> > cp){
	for(int i=0;i<cp.size();i++){
		for(int j=0;j<cp[0].size();j++){
			cout<<cp[i][j]<< " ";
		}
		cout<<endl;
	}
	cout<<endl;

	// cout<<cp.size()<<" "<<cp[0].size()<<endl;
}

vector< vector<int> > dilation(vector< vector<int> > img, vector< vector<int> > se){
	int max = -1;
	
	vector< vector<int> > v = pad_array(img,se,'d');
	vector< vector<int> > cp = img;
	
	
	///Erosion Process
	for(int i=0;i<img.size();i++){
		for(int j=0;j<img[0].size();j++){
			max=-1;
			for(int k=0;k<se.size();k++){
				for(int l=0;l<se[0].size();l++){
					if( se[k][l] == 1 && v[i+k][j+l]>max){
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

	vector< vector<int> > v = pad_array(img,se,'e');
	vector< vector<int> > cp = img;
	
	
	///Erosion Process
	for(int i=0;i<img.size();i++){
		for(int j=0;j<img[0].size();j++){
			min=256;
			for(int k=0;k<se.size();k++){
				for(int l=0;l<se[0].size();l++){
					if( se[k][l] == 1 && v[i+k][j+l]<min){
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

vector< vector<int> > load_data(string filename){
	int n,index=0;
	vector< vector<int> > a;
	ifstream in;

	in.open(filename);
	if(!in.is_open()){
		cerr<<"Error opening file\n";
		exit(1);
	}

	
	a.push_back(vector<int>());
	while(in>>n){
		a[index].push_back(n);

		if(in.peek() == '\n'){
			index++;
			a.push_back(vector<int>());
			in.ignore();
		}
		else{
			in.ignore();
		}
	}

	in.close();

	return a;
}



int main(int argc, const char *argv[]) {
	vector< vector<int> > SE, image;


	SE = load_data(argv[2]);
	image = load_data(argv[3]);

	// print_image(SE);
	// print_image(image);
	
	
	
	if(strcmp(argv[1],"e")==0){
		write_into_file(argv[4], erosion(image,SE));
	}
	else if(strcmp(argv[1],"d")==0){
		write_into_file(argv[4], dilation(image,SE));
	}
	else if(strcmp(argv[1],"o")==0){
		write_into_file(argv[4], opening(image,SE));
	}
	else{
		cout<<"No optionality matches\nExiting...\n";
		exit(1);
	}

	return 0;
}
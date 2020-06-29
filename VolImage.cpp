//Mbaliyethemba Shangase
//18 March 2018
//VolImage class for Assignment 2

#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ios>
#include <cmath>
#include "VolImage.h"
#include <math.h>

using namespace std;

int width, height, num;
vector<unsigned char**>slices;

SHNMBA004::VolImage::VolImage(){
	this->width = 0;
	this->height = 0;
	}
	
SHNMBA004::VolImage::~VolImage(){
	for(unsigned int i = 0; i < slices.size(); i++){
		for(int j = 0; j < height; j++){
			delete[] slices[i][j];
		}
		delete slices[i];
	}
}

// populate the object with images in stack
bool SHNMBA004::VolImage::readImages(string baseName){
	ifstream infile;
	infile.open(baseName+".dat");
	if(infile.is_open()){
		string width, height, nums, line;
		getline(infile, line);
		istringstream ss(line);
		getline(ss,width,' ');
		getline(ss,height,' ');
		getline(ss,nums,' ');
		ss.str(std::string());
		this->width = stoi(width);
		this->height = stoi(height);
		this->num = stoi(nums);
		this->slices.reserve(num);
		
		infile.close();
		
		int i = 0;
		while(i < num){
			infile.open(baseName + to_string(i) + ".raw", ios::binary);
			infile>>noskipws;
			unsigned char ** rows = new unsigned char * [this->height];
			unsigned char c;
			int w = 0;
			while(w < this-> height){
				unsigned char * linechar = new unsigned char[this->width];
				int r = 0;
				while(r < this->width){
					infile >> c;
					linechar[r] = c;
					r++;
				}
				rows[w] = linechar;
				w++;
			
		}
		infile.close();
		this->slices.push_back(rows);
		i++;
	}
	return true;
}
	return false;
}

// compute difference map and write out;
void SHNMBA004::VolImage::diffmap(int sliceI, int sliceJ, std::string output_prefix){
	ofstream outfile;
	outfile.open(output_prefix + ".raw");
	if(this->width != 0 && this->height != 0){
		int t = 0;
		while(t < height){
			int y = 0;
			while(y < width){
				outfile << (unsigned char)(fabs((float)slices[sliceI][t][y] - (float)slices[sliceJ][t][y])/2);
				y++;
			}
			t++;
		}
	}
	outfile.close();
}

// extract slice sliceId and write to output
void SHNMBA004::VolImage::extract(int sliceId, std::string output_prefix){
	ofstream outfile;
	outfile.open(output_prefix + ".dat");
	string s = to_string(this->width) + " " + to_string(this->height) + " 1";
	outfile << s << endl;
	outfile.close();
	outfile.open(output_prefix + ".raw", ios::binary);
	int i = 0;
	while ( i < this->height){
		int p = 0;
		while(p < this->width){
			outfile << this->slices[sliceId][i][p];
			p++;
		}
		i++;
	}	
	outfile.close();
}

int SHNMBA004::VolImage::volImageSize(void){
	if((this->width > 0) and (this->height > 0)){
		return (slices.size()*(this->height)*(this->width)) + (sizeof(int*)*slices.size());
	}
	return 0;
}

int SHNMBA004::VolImage::volSize(void){
	return this->num;
}

void SHNMBA004::VolImage::extractRow(int rowId, std::string output_prefix){
	string header = output_prefix + ".data";
	ofstream head(header.c_str(), ios::out);
		if(head.is_open()){
			head<< width<< " "<< height<< " "<< 1<< endl;
			cout<< "Created header file "<<header<<endl;
			head.close();
		}else{
			cout<<"Unable to open file"<<header<<endl;
		}

	string outName = output_prefix + ".raw";
	ofstream out(outName.c_str(), ios::binary);
	if(out.is_open()){
   
    for(unsigned int i=0; i< slices.size(); i++){
        char* byte = (char*)slices[i][rowId];
        out.write(byte,width);
      }
      out.close();
   }
}





























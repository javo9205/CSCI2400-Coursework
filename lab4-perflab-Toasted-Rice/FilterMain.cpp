#include <stdio.h>
#include <immintrin.h>
#include "cs1300bmp.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Filter.h"

using namespace std;

#include "rdtsc.h"

//
// Forward declare the functions
//
Filter * readFilter(string filename);
double applyFilter(Filter *filter, cs1300bmp *input, cs1300bmp *output);

int main(int argc, char **argv)
{

	if ( argc < 2) {
		fprintf(stderr,"Usage: %s filter inputfile1 inputfile2 .... \n", argv[0]);
	}

	//
	// Convert to C++ strings to simplify manipulation
	//
	string filtername = argv[1];

	//
	// remove any ".filter" in the filtername
	//
	string filterOutputName = filtername;
	string::size_type loc = filterOutputName.find(".filter");
	if (loc != string::npos) {
		//
		// Remove the ".filter" name, which should occur on all the provided filters
		//
		filterOutputName = filtername.substr(0, loc);
	}

	Filter *filter = readFilter(filtername);

	double sum = 0.0;
	int samples = 0;

	for (int inNum = 2; inNum < argc; inNum++) {
		string inputFilename = argv[inNum];
		string outputFilename = "filtered-" + filterOutputName + "-" + inputFilename;
		struct cs1300bmp *input = new struct cs1300bmp;
		struct cs1300bmp *output = new struct cs1300bmp;
		int ok = cs1300bmp_readfile( (char *) inputFilename.c_str(), input);

		if ( ok ) {
			double sample = applyFilter(filter, input, output);
			sum += sample;
			samples++;
			cs1300bmp_writefile((char *) outputFilename.c_str(), output);
		}
		delete input;
		delete output;
	}
	fprintf(stdout, "Average cycles per sample is %f\n", sum / samples);

}

class Filter *readFilter(string filename)
{
	ifstream input(filename.c_str());

	if ( ! input.bad() ) {
		int size = 0;
		input >> size;
		Filter *filter = new Filter(size);
		int div;
		input >> div;
		filter -> setDivisor(div);
		for (int i=0; i < size; i++) {
			for (int j=0; j < size; j++) {
	int value;
	input >> value;
	filter -> set(i,j,value);
			}
		}
		return filter;
	} else {
		cerr << "Bad input in readFilter:" << filename << endl;
		exit(-1);
	}
}


double applyFilter(class Filter *filter, cs1300bmp *input, cs1300bmp *output)
{
	long long cycStart, cycStop;
	cycStart = rdtscll();

	int image_width  = input -> width;
	int image_height = input -> height; 
	int image_size = image_width * image_height;
	output -> width = image_width;
	output -> height = image_height;;
	output -> color[0] = new unsigned char[image_size];
	output -> color[1] = new unsigned char[image_size];
	output -> color[2] = new unsigned char[image_size];
	
	int color_value;
	int x_limit = image_width  - 1;
	int y_limit = image_height - 1;
	int scan_length = filter-> getSize();
	int scan_last_index = scan_length-1;
	int scan_offset = (scan_last_index) >> 1;
	unsigned char *scan_pointer[scan_length];
	
	
	int divisor = filter -> getDivisor();
	int weight[scan_length][scan_length];
	
	for (int x=0; x<scan_length; x++)
	for (int y=0; y<scan_length; y++)
		weight[x][y] = filter -> get(x,y);
	
	for (int channel=0; channel < 3; channel++)
	{
		scan_pointer[1] = &(input->color[channel][0]);
		scan_pointer[2] = &(input->color[channel][image_width]);
		
		for (int y=scan_offset; y<y_limit; y++)
		{
			scan_pointer[0] = scan_pointer[1];
			scan_pointer[1] = scan_pointer[2];
			scan_pointer[2] =  &(input->color[channel][image_width * (y+scan_offset)]);
			int y_offset = y * image_width;
			
			for (int x=scan_offset; x<x_limit; x++)
			{
				int x1=x-1, x2=x+1;
				int acc1 = scan_pointer[0][x1] * weight[0][0];
				int acc2 = scan_pointer[0][x ] * weight[0][1];
				int acc3 = scan_pointer[0][x2] * weight[0][2];
				color_value = acc1 + acc2 + acc3;
				acc1 = scan_pointer[1][x1] * weight[1][0];
				acc2 = scan_pointer[1][x ] * weight[1][1];
				acc3 = scan_pointer[1][x2] * weight[1][2];
				color_value += acc1 + acc2 + acc3;
				acc1 = scan_pointer[2][x1] * weight[2][0];
				acc2 = scan_pointer[2][x ] * weight[2][1];
				acc3 = scan_pointer[2][x2] * weight[2][2];
				color_value += acc1 + acc2 + acc3;
				color_value /= divisor;
				
				output -> color[channel][y_offset+x] =
					(color_value > 255) ? 255 :
					(color_value < 0  ) ? 0   :
					color_value;
			}
		}
	}

	cycStop = rdtscll();
	double diff = cycStop - cycStart;
	double diffPerPixel = diff / (output -> width * output -> height);
	fprintf(stderr, "Took %f cycles to process, or %f cycles per pixel\n",
		diff, diff / (output -> width * output -> height));
	return diffPerPixel;
}

//============================================================================
// Name        : ova.cpp
//============================================================================

#include <iostream>
//To read files.
#include <fstream>
//To parse strings.
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
//For random numbers.
#include <ctime>
//For sleep function.
#include <unistd.h>
//Power function.
#include <cmath>

using namespace std;

#define E 2.71828182845904523536

void parseArff(string &s);
void replaceStr( string &s, const string &search, const string &replace );
vector <string> splitStr(string &s, char delim);
void printWeights(vector <vector <float> > weight, uint rows, uint cols);

int main() {
	string file;	//To store the file name.

	cout<<"******************************************"<<endl;
	cout<<"Simple perceptron clasifier"<<endl;
	cout<<"******************************************"<<endl;
	cout << "Enter .arff file name to read" << endl;
	cin>>file;	//Read file.
	cout<<"Reading: "<<file<<".arff"<<endl;
	file+=".arff";	//Add the .arff extension.

	//Read the file and store in string contents.
	ifstream in(file.c_str());
	string contents((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
	for(uint i=0;i<contents.size();i++)
		contents[i] = tolower(contents[i]);
	//Parse to find all classes.
	string clsseek = "@attribute class";
	//Now seek and save the data.
	clsseek = "@data";
	string dd = contents.substr(contents.find(clsseek));
	parseArff(dd);
	replaceStr(dd, "@data", "");
	//For some reason with iris.arff we need to leave the line breaks.
	replaceStr(dd, "\n", "");

	//We split the data by rows to work with each of them.
	vector<string> data = splitStr(dd, '\r');
	cout<<"Data size: "<<data.size()<<endl;
	uint colSize = 0;
	for(uint i=1;i<data.size();i++){
		//cout<<"i: "<<i<<" data: "<<data[i]<<endl;
		vector<string> ddd = splitStr(data[i], ',');
		if(ddd.size()>colSize)
			colSize = ddd.size();
	}
	//Now split by commas
	float io[data.size()][colSize];
	for(uint i=1;i<data.size();i++){
		vector<string> ddd = splitStr(data[i], ',');
		for(uint j=0;j<ddd.size();j++){
			io[i][j] = atof(ddd[j].c_str());
		}
	}
	/*for(uint i=1;i<data.size();i++){
		//We use colSize-1 due to the last column being the class.
		for(uint j=0;j<colSize-1;j++)
			cout<<"i:"<<i<<" j:"<<j<<" --- "<<io[i][j]<<"\t";
		cout<<endl;
	}*/

	//Feed the perceptron and start the algorithm.
	srand(time(NULL));
	vector <vector <float> > weight(4, vector<float>(3, 0.0));
	//Select weights randomly.
	for(uint i=0;i<weight.size();i++){
		for(uint j=0;j<weight[0].size();j++){
			weight[i][j] = ((float)rand() / (RAND_MAX));
		}
	}
	vector<float> bias(3, 0.0);
	for(uint i=0;i<bias.size();i++){
		bias[i] = ((float) rand() / (RAND_MAX));
	}
	float factor_aprendizaje = 0.01;
	int counttoexit, counttoexit1, counttoexit2;
	counttoexit = counttoexit1 = counttoexit2 = 0;
	float sum, sum1, sum2;
	sum = sum1 = sum2 = 0.0;

	cout<<"******************************************"<<endl;
	cout<<"Initializing Perceptron"<<endl;
	cout<<"******************************************"<<endl;

	/*for(uint i=0;i<bias.size();i++){
		cout<<"Bias["<<i<<"] --- "<<bias[i]<<endl;
	}*/
	float output1[data.size()], output2[data.size()], output3[data.size()];
	for(uint i=1;i<data.size();i++){
		if(i<4)
			output1[i] = 1;
		else
			output1[i] = 0;
		//cout<<output1[i]<<endl;
	}
	for(uint i=1;i<data.size();i++){
		if(i<7)
			output2[i] = 0;
		else
			output2[i] = 1;
		//cout<<output2[i]<<endl;
	}
	for(uint i=1;i<data.size();i++){
		if(i>3 && i < 7)
			output3[i] = 1;
		else
			output3[i] = 0;
		//cout<<output3[i]<<endl;
	}
	cout<<"******************************************"<<endl;
	//Initialize perceptron training.
	while((counttoexit < data.size()-1) || (counttoexit1 < data.size()-1) || (counttoexit2 < data.size()-1)){
		for(uint i=1;i<data.size();i++){
			sum = 0;
			//Multiply input with weight.
			for(uint j=0;j<colSize-1;j++){
				sum += io[i][j] * weight[j][0];
				sum1 += io[i][j] * weight[j][1];
				//cout<<"Input: "<<io[i][j]<<"*"<<weight[j][0]<<"="<<io[i][j] * weight[j][1]<<endl;
			}
			//Add the bias.
			sum += bias[0];
			sum1 += bias[1];
			/*cout<<"Sum: "<<sum<<endl;
			cout<<"Sum1: "<<sum1<<endl;*/
			//Evaluate using hardlim function.
			if(sum >= 0)
				sum = 1;
			else
				sum = 0;
			if(sum1 >= 0)
				sum1 = 1;
			else
				sum1 = 0;

			//Neuron's a & b output is c input.
			//We only need 2 weights as its only 2 inputs.
			sum2 = sum * weight[0][2] + sum1 * weight[1][2];
			//Add the bias.
			sum2 += bias[2];
			//Apply hardlim.
			if(sum2 >= 0)
				sum2 = 1;
			else
				sum2 = 0;

			//Compare with value of desired input.
			if(sum != output1[i]){
				//Readjust weights.
				float error = (output1[i] - sum);
				float nweight = weight[0][0] + error * io[i][0] * factor_aprendizaje;
				weight[0][0] = nweight;
				nweight = weight[1][0] + error * io[i][1] * factor_aprendizaje;
				weight[1][0] = nweight;
				nweight = weight[2][0] + error * io[i][2] * factor_aprendizaje;
				weight[2][0] = nweight;
				nweight = weight[3][0] + error * io[i][3] * factor_aprendizaje;
				weight[3][0] = nweight;
				float nbias = bias[0] + error;
				bias[0] = nbias;
				counttoexit = 0;
				cout<<"Desired0: "<<output1[i]<<" \t Obtained0: "<<sum<<endl;
				cout<<"Error0: "<<error<<" \t Resetting weights"<<endl;
			}else{
				float error = (output1[i] - sum);
				cout<<"Desired0: "<<output1[i]<<" \t Obtained0: "<<sum<<endl;
				cout<<"Error0: "<<error<<" \t Found!"<<endl;
				counttoexit++;
				/*if(counttoexit>data.size())
					break;*/
			}
			if(sum1 != output2[i]){
				//Readjust weights.
				float error = (output2[i] - sum1);
				float nweight = weight[0][1] + error * io[i][1] * factor_aprendizaje;
				weight[0][1] = nweight;
				nweight = weight[1][1] + error * io[i][1] * factor_aprendizaje;
				weight[1][1] = nweight;
				nweight = weight[2][1] + error * io[i][2] * factor_aprendizaje;
				weight[2][1] = nweight;
				nweight = weight[3][1] + error * io[i][3] * factor_aprendizaje;
				weight[3][1] = nweight;
				float nbias = bias[1] + error;
				bias[1] = nbias;
				counttoexit1 = 0;
				cout<<"Desired1: "<<output2[i]<<" \t Obtained1: "<<sum1<<endl;
				cout<<"Error1: "<<error<<" \t Resetting weights"<<endl;
			}
			else{
				float error = (output2[i] - sum1);
				cout<<"Desired1: "<<output2[i]<<" \t Obtained1: "<<sum1<<endl;
				cout<<"Error1: "<<error<<" \t Found!"<<endl;
				counttoexit1++;
				/*if(counttoexit1>data.size()-1)
					break;*/
			}
			//Compare with value of desired input.
			if(sum2 != output3[i]){
				//Readjust weights.
				float error = (output3[i] - sum2);
				float nweight = weight[0][2] + error * sum * factor_aprendizaje;
				weight[0][2] = nweight;
				nweight = weight[1][2] + error * sum1 * factor_aprendizaje;
				weight[1][2] = nweight;
				float nbias = bias[2] + error;
				bias[2] = nbias;
				counttoexit2 = 0;
				cout<<"Desired2: "<<output3[i]<<" \t Obtained2: "<<sum2<<endl;
				cout<<"Error2: "<<error<<" \t Resetting weights"<<endl;
			}else{
				float error = (output3[i] - sum2);
				cout<<"Desired2: "<<output3[i]<<" \t Obtained2: "<<sum2<<endl;
				cout<<"Error2: "<<error<<" \t Found!"<<endl;
				counttoexit2++;
			}
			cout<<"Weights"<<endl;
			cout<<weight[0][0]<<"\t"<<weight[1][0]<<"\t"<<weight[2][0]<<"\t"<<weight[3][0]<<endl;
			cout<<weight[0][1]<<"\t"<<weight[1][1]<<"\t"<<weight[2][1]<<"\t"<<weight[3][1]<<endl;
			cout<<weight[0][2]<<"\t"<<weight[1][2]<<endl;
			cout<<"******************************************"<<endl;
			//sleep(1);
		}
	}
	cout<<"Finished"<<endl;
	return 0;
}

void printWeights(vector <vector <float> > weight, uint rows, uint cols){
	for(uint k=0;k<rows;k++){
		for(uint l=0;l<cols;l++){
			cout<<"Weight["<<k<<"]["<<l<<"] --- "<<weight[k][l]<<"\t";
		}
		cout<<endl;
	}
}

void parseArff(string &s){
	replaceStr(s, " ", "");
	replaceStr(s, "\t", "");
	replaceStr(s, "{", "");
	replaceStr(s, "}", "");
}

vector <string> splitStr(string &s, char delim){
	stringstream ss(s);
	string item;
	vector<string> tokens;
	while(getline(ss, item, delim)){
		tokens.push_back(item);
	}
	return tokens;
}

void replaceStr( string &s, const string &search, const string &replace ){
    for( size_t pos = 0; ; pos += replace.length() )
	{
        pos = s.find( search, pos );
        if( pos == string::npos ) break;

        s.erase( pos, search.length() );
        s.insert( pos, replace );
    }
}

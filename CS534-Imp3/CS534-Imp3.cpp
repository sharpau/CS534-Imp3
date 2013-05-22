// CS534-Imp3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	vector<pair<int, vector<int>>> trainingData;
	// read in data
	ifstream trainingFile("SPECT-train.csv");
	// advance to data
	string line;
	trainingFile >> line;
	while(line != "@data") {
		trainingFile >> line;
	}

	while(trainingFile.good()) {
		// get class
		int y;
		trainingFile >> y;
		// get features
		vector<int> x(22, 0);
		for(int i = 0; i < 22; i++) {
			trainingFile.get();
			trainingFile >> x[i];
		}
		trainingData.push_back(make_pair(y, x));
	}

	return 0;
}

/* input: 
		bagging:
			examples only
			[use equal weights]
		boosting:
			examples
			weights on examples
 output: hypothesis
 */
void decisionStump(void) {

}

void boost(void) {

}

void bag(void) {

}
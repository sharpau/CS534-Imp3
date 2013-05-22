// CS534-Imp3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>

using namespace std;

/* input: 
		bagging:
			examples only
			[use equal weights]
		boosting:
			examples
			weights on examples
 output: hypothesis [feature index and pos / neg]
 */
pair<int, bool> decisionStump(vector<pair<int, vector<int>>> examples, vector<double> weights) {
	assert(examples.size() == weights.size());

}

// inputs: test, training, ensemble size??
// output: error
void boost(void) {

}

// inputs: test, training, ensemble size??
// output: error (weighted or simple count??)
void bag(void) {

}

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

	vector<pair<int, vector<int>>> testData;
	// read in data
	ifstream testFile("SPECT-test.csv");
	// advance to data
	testFile >> line;
	while(line != "@data") {
		testFile >> line;
	}

	while(testFile.good()) {
		// get class
		int y;
		testFile >> y;
		// get features
		vector<int> x(22, 0);
		for(int i = 0; i < 22; i++) {
			testFile.get();
			testFile >> x[i];
		}
		testData.push_back(make_pair(y, x));
	}

	return 0;
}
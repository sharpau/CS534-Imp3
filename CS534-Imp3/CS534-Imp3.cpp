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
	
	double maxProbAdjusted = -1000000000;
	int bestFeature = 0;
	bool inverse = false;

	for(int i = 0; i < examples[1].second.size(); i++){
		// if feature true, increment trueClass[i] where i is class label.  Otherwise, increment falseClass[i].
		// index 0 = false, index 1 = true
		vector<int> trueClass(2,0);
		vector<int> falseClass(2,0);

		for(int j = 0; j < examples.size(); j++){
			if(examples[j].second[i] == 0){
				// factor in weights too?
				falseClass[examples[j].first] += weights[j];
			}
			else{
				trueClass[examples[j].first] += weights[j];
			}
		}
		// calculate probabilities - posProbs for direct correlation, invProbs for inverse correlation
		double posProbs = 0;
		double invProbs = 0;
		// note to self -- make sure we're doing double division, not int.
		
		// calculate posProbs first -- remember factors to prevent division by zero!
		posProbs = (((double)falseClass[0]) / ((double)falseClass[1] + (double)falseClass[0])) * (((double)trueClass[1]) / ((double)trueClass[0] + (double)trueClass[1]));

		// calculate invProbs
		invProbs = (((double)falseClass[1]) / ((double)falseClass[1] + (double)falseClass[0])) * (((double)trueClass[0]) / ((double)trueClass[0] + (double)trueClass[1]));

		if(posProbs >= invProbs && posProbs > maxProbAdjusted){
			maxProbAdjusted = posProbs;
			bestFeature = i;
			inverse = false;
		}
		else if(invProbs > posProbs && invProbs > maxProbAdjusted){
			maxProbAdjusted = invProbs;
			bestFeature = i;
			inverse = true;
		}
	}
	return make_pair(bestFeature, inverse);
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
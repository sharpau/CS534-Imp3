// CS534-Imp3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <time.h>

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
pair<int, bool> decisionStump(vector<pair<int, vector<int>>> examples, vector<double> weights, double &error) {
	assert(examples.size() == weights.size());
	
	double maxProbAdjusted = -1000000000;
	int bestFeature = 0;
	bool inverse = false;

	for(int i = 0; i < examples[1].second.size(); i++){
		// if feature true, increment trueClass[i] where i is class label.  Otherwise, increment falseClass[i].
		// index 0 = false, index 1 = true
		vector<double> trueClass(2,0);
		vector<double> falseClass(2,0);

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
		posProbs = ((falseClass[0]) + (trueClass[1])) / (falseClass[1] + falseClass[0] + trueClass[0] + trueClass[1]);

		// calculate invProbs
		invProbs = ((falseClass[1]) + trueClass[0]) / (trueClass[0] + trueClass[1] + falseClass[1] + falseClass[0]);

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
	
	error = 1.0 - maxProbAdjusted;
	return make_pair(bestFeature, inverse);
}

// inputs: training, ensemble size
// output: ensemble of hypothesis-weight pairs
pair<vector<pair<int, bool>>, vector<double>> boost(vector<pair<int, vector<int>>> trainingData, int ensembleSize) {
	/*
		for 0 to ensemble size
			decision stump
			get error
			update weights
			normalize weights
	*/
	vector<pair<int, bool>> vote;
	vector<double> voteWeight;
	vector<double> weights(trainingData.size(), 1.0 / trainingData.size());
	pair<int, bool> result;
	double alpha;
	for(int i = 0; i < ensembleSize; i++) {
		double error = 0;
		result = decisionStump(trainingData, weights, error);
		alpha = 0.5*log((1 - error) / error);

		// add to vote ensemble
		vote.push_back(result);
		voteWeight.push_back(alpha);

		// update weights
		double sum = 0;
		for(int j = 0; j < trainingData.size(); j++) {
			// check whether prediction was correct
			if(!result.second) {
				// we predict using this feature
				if(trainingData[j].first == trainingData[j].second[result.first]) {
					// correct prediction
					weights[j] *= exp(-1.0 * alpha);
				}
				else {
					// incorrect
					weights[j] *= exp(alpha);
				}
			}
			else {
				// predict using inverse of feature
				if(trainingData[j].first == !trainingData[j].second[result.first]) {
					// correct prediction
					weights[j] *= exp(-1.0 * alpha);
				}
				else {
					// incorrect
					weights[j] *= exp(alpha);
				}
			}
			sum += weights[j];
		}
		// normalize weights
		for(int j = 0; j < weights.size(); j++) {
			weights[j] /= sum;
		}
	}
	return make_pair(vote, voteWeight);

	// TODO: MOVE THIS CODE TO classify()
	//vector<double> voteTotals(trainingData[1].second.size(), 0);
	//vector<double> voteInverseTotals(trainingData[1].second.size(), 0);
	//
	//// tally votes
	//for(int i = 0; i < vote.size(); i++){
	//	voteTotals[vote[i].first] += voteWeight[i];
	//	// is this inverted?
	//	if(vote[i].second == true){
	//		voteInverseTotals[vote[i].first] += voteWeight[i];
	//	}
	//}
	//
	//// variables for vote results
	//double currentMax = 0;
	//int bestFeature = 0;
	//bool inverse = false;
	//// get vote results
	//for(int i = 0; i < voteTotals.size(); i++){
	//	if(voteTotals[i] > currentMax){
	//		currentMax = voteTotals[i];
	//		bestFeature = i;
	//	}
	//}
	//// is the true/false classification inverted?
	//if((double) voteInverseTotals[bestFeature] / (double) voteTotals[bestFeature] > 0.5){
	//	inverse = true;
	//}

	//return make_pair(bestFeature, inverse);
}

// inputs: test, ensemble size
// output: ensemble of hypotheses, all weighted equally
pair<vector<pair<int, bool>>, vector<double>> bag(vector<pair<int, vector<int>>> trainingData, int ensembleSize) {
	/* for 0 to ensemble size
			for 0 to example set size
				pick random number from 0 to example set size
				copy that example to new set
			decision stump
		majority vote of ensemble
	*/
	vector<pair<int, vector<int>>> samples;
	vector<vector<pair<int, vector<int>>>> ensemble(ensembleSize, samples);
	vector<pair<int, bool>> vote;
	vector<double> weights(trainingData.size(), 1.0/trainingData.size());
	
	for(int i = 0; i < ensembleSize; i++){
		// create bootstrap sample
		for(int j = 0; j < trainingData.size(); j++){
			ensemble[i].push_back(trainingData[rand() % trainingData.size()]);
		}
		// run learning algorithm (decision stump)
		double error;
		vector<double> dummyWeights(trainingData.size(), 1.0 / trainingData.size());
		vote.push_back(decisionStump(ensemble[i], dummyWeights, error));
	}

	vector<double> dummyVoteWeights(vote.size(), 1.0f/vote.size());
	return make_pair(vote, dummyVoteWeights);
	
	// TODO: MOVE THIS CODE TO classify()
	//vector<int> voteTotals(trainingData[1].second.size(), 0);
	//vector<int> voteInverseTotals(trainingData[1].second.size(), 0);
	
	// tally votes
	//for(int i = 0; i < vote.size(); i++){
	//	voteTotals[vote[i].first]++;
	//	// is this inverted?
	//	if(vote[i].second == true){
	//		voteInverseTotals[vote[i].first]++;
	//	}
	//}
	
	//// variables for vote results
	//int currentMax = 0;
	//int bestFeature = 0;
	//bool inverse = false;
	//// get vote results
	//for(int i = 0; i < voteTotals.size(); i++){
	//	if(voteTotals[i] > currentMax){
	//		currentMax = voteTotals[i];
	//		bestFeature = i;
	//	}
	//}
	//// is the true/false classification inverted?
	//if((double) voteInverseTotals[bestFeature] / (double) voteTotals[bestFeature] > 0.5){
	//	inverse = true;
	//}

	//return make_pair(bestFeature, inverse);
	
}

// TODO: update 'hypothesis' input to be an actual ensemble (ie set of hypothesis-weight pairs)
double classify(vector<pair<int, vector<int>>> testData, pair<int, bool> classifier){

	double testError;
	int testCorrect = 0;
	
	// classify data

	if(classifier.second == false){
		// non-inverted
		for(int i = 0; i < testData.size(); i++){
			if(testData[i].first == testData[i].second[classifier.first]){
				testCorrect++;
			}
		}
	}
	else{
		// inverted
		for(int i = 0; i < testData.size(); i++){
			if(testData[i].first != testData[i].second[classifier.first]){
				testCorrect++;
			}
		}
	}
	
	return (1.0 - (double)testCorrect / (double)testData.size());
}

int _tmain(int argc, _TCHAR* argv[])
{
	// init random seed
	srand (time(NULL));
	
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

	// TODO: update classify
	vector<double> boostTrainError;
	vector<double> boostTestError;
	vector<double> bagTrainError;
	vector<double> bagTestError;
	for(int size = 5; size < 35; size += 5) {
		auto boostH = boost(trainingData, size);
		//boostTrainError.push_back(classify(trainingData, boostH));
		//boostTestError.push_back(classify(testData, boostH));
		double bagTotalTrainError = 0;
		double bagTotalTestError = 0;
		for(int i = 0; i < 5; i++) {
			auto bagH = bag(trainingData, size);
			//bagTotalTrainError += classify(trainingData, bagH);
			//bagTotalTestError += classify(testData, bagH);
		}
		bagTrainError.push_back(bagTotalTrainError / 5.0);
		bagTestError.push_back(bagTotalTestError / 5.0);
	}

	return 0;
}
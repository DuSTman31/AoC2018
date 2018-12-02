#include "Lib.h"
#include <iostream>
using std::cout;
#include <algorithm>
using std::sort;

bool hasRepeated(string &a, unsigned int seqLength) {
	bool hasPrev = false;
	char prev = '\0';
	unsigned int prevOccur = 0;
	
	for(char &c : a) {
		if(!hasPrev) {
			hasPrev = true;
			prev = c;
			prevOccur = 1;
		}
		else {
			if(prev == c) {
				prevOccur++;
			}
			else {
				if(prevOccur == seqLength) {
					return true;
				}
				else {
					prev = c;
					prevOccur = 1;
				}
			}
		}
	}

	if(hasPrev && prevOccur == seqLength) {
		return true;
	}
	else {
		return false;
	}	
}

bool singleCharDifference(string &a, string &b) {
	if(a.length() != b.length()) {
		return false;
	}

	auto aIter = a.begin();
	auto bIter = b.begin();
	int difference = 0;

	while((aIter!=a.end()) && (bIter!=b.end())) {
		if(*aIter != *bIter) {
			difference++;
			if(difference > 1) {
				return false;
			}
		}

		aIter++;
		bIter++;
	}
		
	if(difference == 1) {
		return true;
	}
	else {
		return false;
	}
}

int main() {
	try{
		vector<string> input = readFileAsLines("day2.txt");

		int twoRepeats =  0;
		int threeRepeats = 0;
		for(auto &i : input) {
			string sortedString = i;
			sort(sortedString.begin(), sortedString.end());
			if(hasRepeated(sortedString, 2)) {
				twoRepeats++;
			}
			if(hasRepeated(sortedString, 3)) {
				threeRepeats++;
			}
		}

		cout << "Part 1 \n";
		cout << "Two repeated chars: " << twoRepeats << "\n";
		cout << "Three repeated chars: " << threeRepeats << "\n";
		cout << "Checksum: " << twoRepeats * threeRepeats << "\n";

		cout << "\nPart 2 \n";
		for(unsigned int i=0 ; i!=input.size() ; i++) {
			for(unsigned int j=i+1 ; j!=input.size() ; j++) {
				if(i!=j) {
					if(singleCharDifference(input[i], input[j])) {
						cout << input[i] << " " << input[j] << "\n";
					}
				}
			}
		}
		
	}
	catch(FileReadException &e){
		cout << "Could not read input file.\n";
	}

}

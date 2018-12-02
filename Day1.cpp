#include "Lib.h"
#include <iostream>
using std::cout;
#include <map>
using std::map;

class FormatException{};

int parseLine(string &in) {
	bool neg = false;
	if(in[0] == '-') {
		neg = true;
	}
	else if (in[0] != '+') {
		throw FormatException();
	}

	unsigned int consume = 0;
	int op = decodeInt(in, 1, &consume);
	
	if(neg) {
		return (op * -1);
	}
	else {
		return op;
	}
}

int partOne(vector<int> &fe) {
	int freq = 0;

	for(int i : fe) {
		freq += i;
	}
	return freq;
}

int partTwo(vector<int> &fe) {
	map<int, bool> frequencyMap;
	int no = 0;
	
	int freq = 0;
	while(true){
		for(int i : fe) {
			freq += i;
			if(frequencyMap.count(freq) != 0) {
			  cout << no << "\n";
				return freq;
			}
			else {
				frequencyMap[freq] = true;
				no++;
			}	
		}
	}
}

int main() {
	try{
		vector<string> input = readFileAsLines("day1.txt");
		vector<int> freqAdjustments;
		freqAdjustments.reserve(input.size());

		for(string &l : input){
			int a = parseLine(l);
			freqAdjustments.push_back(a);
		}
		
		int p1 = partOne(freqAdjustments);

		cout << "Part one: " << p1 << "\n";

		int p2 = partTwo(freqAdjustments);
		cout << "Part two: " << p2 << "\n";

	}
	catch(FileReadException &e){
		cout << "Could not read input file.\n";
	}

}

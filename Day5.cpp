#include "Lib.h"
#include <iostream>
using std::cout;
#include <algorithm>
using std::sort;

class FormatException{};

bool isUpperCase(char a) {
	return ((a >= 'A') && (a <= 'Z'));
}

bool isLowerCase(char a) {
	return ((a >= 'a') && (a <= 'z'));
}

bool isAlphabetic(char a) {
	return (isUpperCase(a) || isLowerCase(a));
}

char upperToLower(char a) {
	return (a+('a' - 'A'));
}

char LowerToUpper(char a) {
	return (a-('a' - 'A'));
}

bool doCharsDestruct(char a, char b){
	if(isLowerCase(a)) {
		if((a-('a' - 'A')) == b) {
			return true;
		}
		else {
			return false;
		}
	}
	else if(isUpperCase(a)) {
		if((a+('a' - 'A')) == b) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

class stack {
public:
	bool isPrevDestructive(char a) {
		if(data.size() == 0) {
			return false;
		}
		else if(doCharsDestruct(a, data[data.size() - 1])) {
			return true;
		}
		else {
			return false;
		}
		
	};
	void push(char a) {
		if(isPrevDestructive(a)) {
			data.pop_back();
		}
		else {

			data.push_back(a);
		}
	};
	unsigned int getSize() {
		return data.size();
	};
private:
	vector<char> data;
};

bool isSameType(char a, char b) {
	if(isLowerCase(a)) {
		if(b == a) {
			return true;
		}
		else if(isUpperCase(b)) {
			if((a-('a' - 'A')) == b) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	else if(isUpperCase(a)) {
		if(b == a) {
			return true;
		}
		else if (isLowerCase(b)) {
			if(a+('a' - 'A') == b) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}

	}
	return false;
	

}

string removeType(string &polymer, char a){
	string op;

	for(char c : polymer) {
		if(!isSameType(a, c)) {
			op.push_back(c);
		}
	}
	return op;
}


int main() {
	try{
		string input = readWholeFile("day5.txt");
		stack s;
		
		for(char a : input) {
			if(isAlphabetic(a)) {
				s.push(a);
			}
		}

		cout << "str length: " << s.getSize() << "\n";

		bool hasShortest = false;
		unsigned int shortestSize = 0;
		char shortestVariant = 0;
		for(char c = 'a' ; c<='z'; c++) {
			string trimmed = removeType(input, c);
			stack cs;

			for(char a : trimmed) {
				if(isAlphabetic(a)) {
					cs.push(a);
				}
			}

			if(!hasShortest) {
				hasShortest = true;
				shortestVariant = c;
				shortestSize = cs.getSize();
			}
			else if(cs.getSize() < shortestSize) {
				shortestSize = cs.getSize();
				shortestVariant = c;
			}
		}

		cout << "Shortest variant is " << shortestVariant << " with size " << shortestSize << "\n"; 

	}
	catch(FileReadException &e){
		cout << "Could not read input file.\n";
	}

}

#include "Lib.h"
#include <iostream>
using std::cout;
#include <algorithm>
using std::sort;

class FormatException{};

struct coords{
  unsigned int x;
  unsigned int y;
};

struct claim{
  int claimNo = 0;
  coords start;
  coords size;
};

claim parseLine(const string &line) {
    claim result;
    unsigned int i = 0;
    if(line[i] != '#') {
        throw FormatException();
    }
    i++;
    
    unsigned int consumed = 0;
    result.claimNo = decodeInt(line, i, &consumed);
    i += consumed;
    
    if(!matchString(string(" @ "), line, i)) {
        throw FormatException();
    }
    i += 3;
    
    result.start.x = decodeInt(line, i, &consumed);
    i += consumed;
    
    if(line[i] != ',') {
        throw FormatException();        
    }
    i += 1;
    
    result.start.y = decodeInt(line, i, &consumed);
    i += consumed;
    
    if(!matchString(string(": "), line, i)) {
        throw FormatException();
    }
    i += 2;
    
    result.size.x = decodeInt(line, i, &consumed);
    i += consumed;
    
    if(line[i] != 'x') {
        throw FormatException();
    }
    i += 1;
    
    result.size.y = decodeInt(line, i, &consumed);
    i += consumed;
    
    return result;
}

int main() {
	try{
        vector<string> input = readFileAsLines("day3.txt");
        vector<claim> claims;
        vector<vector<int> > claimMap(1000, vector<int>(1000, 0));
        

        for(auto &line : input) {
            claim c = parseLine(line);
            claims.push_back(c);
            
            for(unsigned int x = 0 ; x!=c.size.x ; x++) {
                for(unsigned int y=0 ; y!=c.size.y ; y++) {
                    claimMap[c.start.x + x][c.start.y + y]++;
                }
            }

        }
        unsigned int total = 0;
        for(unsigned int x = 0 ; x!=1000 ; x++) {
            for(unsigned int y=0 ; y!=1000 ; y++) {
                if(claimMap[x][y] > 1) {
                    total++;
                }
            }
        }       
        cout << total << " square inches fabric contested.\n";
        
        for(auto &claim : claims) {
            bool hasConflict = false;
            for(unsigned int x = 0 ; !hasConflict && x!=claim.size.x ; x++) {
                for(unsigned int y=0 ; !hasConflict && y!=claim.size.y ; y++) {
                    if(claimMap[claim.start.x + x][claim.start.y + y] > 1) {
                        hasConflict = true;
                    }
                }
            }                        
            
            if(!hasConflict) {
                cout << "Claim no: " << claim.claimNo << " has no conflict\n";                
            }
        }
	}
	catch(FileReadException &e){
		cout << "Could not read input file.\n";
	}

}

#include "Lib.h"
#include <iostream>
using std::cout;
#include <algorithm>
using std::sort;
#include <map>
using std::map;

class FormatException{};

struct timestamp{
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int minute;
};

enum class event{fallAsleep, wakeUp, beginShift};

struct logEntry{
	event ev;
	timestamp ts;
	unsigned int guardNo;
};

bool operator<(const timestamp &a, const timestamp &b) {
	if(a.year != b.year) {
		return (a.year < b.year);
	}
	else if(a.month != b.month) {
		return (a.month < b.month);
	}
	else if(a.day != b.day) {
		return (a.day < b.day);
	}
	else if(a.hour != b.hour) {
		return (a.hour < b.hour);
	}
	else if(a.minute != b.minute) {
		return (a.minute < b.minute);
	}
	else {
		return false;
	}
};

bool compareTS(const logEntry &a, const logEntry &b) {
	return (a.ts < b.ts);
}

timestamp parseTime(const string &in, unsigned int offset, unsigned int *consume_) {
	timestamp ts;
	unsigned int i=0;
	if(in[offset+i] != '[') {
		throw FormatException();
	}
	i++;

	unsigned int consume = 0;
	ts.year = decodeUInt(in, offset+i, &consume);
	i += consume;

	if(in[offset+i] != '-') {
		throw FormatException();
	}
	i++;

	ts.month = decodeUInt(in, offset+i, &consume);
	i += consume;
	
	if(in[offset+i] != '-') {
		throw FormatException();
	}
	i++;

	ts.day = decodeUInt(in, offset+i, &consume);
	i += consume;

	if(in[offset+i] != ' ') {
		throw FormatException();
	}
	i++;

	ts.hour = decodeUInt(in, offset+i, &consume);
	i += consume;

	if(in[offset+i] != ':') {
		throw FormatException();
	}
	i++;

	ts.minute = decodeUInt(in, offset+i, &consume);
	i += consume;
	
	if(in[offset+i] != ']') {
		throw FormatException();
	}
	i++;

	*consume_ = i;
	return ts;
}

logEntry parseLine(const string &line) {
	logEntry l;
	unsigned int i=0;
	unsigned int consume = 0;
	l.ts = parseTime(line, i, &consume);
	i += consume;
	
	if(line[i] != ' ') {
		throw FormatException();
	}
	i++;
	
	bool fallAsleep = matchString("falls asleep", line, i);
	if(fallAsleep) {
		l.ev = event::fallAsleep;
		return l;
	}
	bool wakeUp = matchString("wakes up", line, i);
	if(wakeUp) {
		l.ev = event::wakeUp;
		return l;
	}
	bool startShift = matchString("Guard #", line, i);
	if(startShift) {
		l.ev = event::beginShift;
		i += 7;

		l.guardNo = decodeUInt(line, i, &consume);
		i += consume;

		if(matchString(" begins shift", line, i) == false) {
			throw FormatException();
		}
		i += 13;
		
		return l;
	}
	throw FormatException();
}

int main() {
	try{
		vector<string> input = readFileAsLines("./data/day4.txt");
		vector<logEntry> log;

		for(auto &l : input) {
			logEntry le = parseLine(l);
			log.push_back(le);
		}

		std::sort(log.begin(), log.end(), compareTS);


		map<unsigned int, vector<int> > timeSlept;
		
		unsigned int guard = 0;
		timestamp sleepStart;
		for(auto &l : log) {
			if(l.ev == event::beginShift) {
				guard = l.guardNo;
				if(timeSlept.count(guard) == 0) {
					timeSlept[guard] = vector<int>(60, 0);
				}
			}
			if(l.ev == event::fallAsleep) {
				sleepStart = l.ts;
			}
			if(l.ev == event::wakeUp) {
				unsigned int minute = sleepStart.minute;
				while(minute < l.ts.minute) {
					timeSlept[guard][minute]++;
					//cout << "Guard " << guard << " was asleep at " << l.ts.year << "-" << l.ts.month << "-" << l.ts.day << " " << l.ts.hour << ":" << minute << "\n";
					minute++;
				}
			}
				
		}

		unsigned int sleepiestGuard = 0;
		unsigned int sleepiestGuardMinsSlept = 0;
		bool haveSleepiestGuard = false;
		for(auto &g : timeSlept) {
			unsigned int minsSlept = 0;
			for(int a : g.second) {
				minsSlept += a;
			}

			if(!haveSleepiestGuard) {
				haveSleepiestGuard = true;
				sleepiestGuard = g.first;
				sleepiestGuardMinsSlept = minsSlept;
			}
			else if(minsSlept > sleepiestGuardMinsSlept) {
				sleepiestGuard = g.first;
				sleepiestGuardMinsSlept = minsSlept;
			}
				
			
		}

		cout << "Part 1:\n";
		cout << "Guard " << sleepiestGuard << " is sleepiest with " << sleepiestGuardMinsSlept << " minutes zzz\n";
		
		unsigned int sleepiestMinute = 0;
		unsigned int sleepiestMinuteMinsSlept = 0;
		bool haveSleepiestMinute = false;
		for(unsigned int i=0 ; i!=60 ; i++) {
			if(!haveSleepiestMinute) {
				haveSleepiestMinute = true;
				sleepiestMinute = i;
				sleepiestMinuteMinsSlept = timeSlept[sleepiestGuard][i];
			}
			else if(timeSlept[sleepiestGuard][i] > sleepiestMinuteMinsSlept) {
				sleepiestMinute = i;
				sleepiestMinuteMinsSlept = timeSlept[sleepiestGuard][i];
			}
		}

		cout << " minute " << sleepiestMinute << " is his sleepiest.\n";
		cout << " overall: " << sleepiestGuard * sleepiestMinute << "\n";

		cout << "Part 2: \n";

		unsigned int maxSleepMinute = 0;
		unsigned int maxSleepMinuteVal = 0;
		bool haveMaxSleepMinute = false;
		unsigned int maxSleepMinuteGuard = 0;
		for(auto &g : timeSlept) {
			for(unsigned int i=0 ; i!=60 ; i++) {
				if(!haveMaxSleepMinute) {
					haveMaxSleepMinute = true;
					maxSleepMinute = i;
					maxSleepMinuteVal = g.second[i];
					maxSleepMinuteGuard = g.first;
				}
				else if(g.second[i] > maxSleepMinuteVal) {
					maxSleepMinute = i;
					maxSleepMinuteVal = g.second[i];
					maxSleepMinuteGuard = g.first;
				}

			}
					
		}
		cout << "Guard " << maxSleepMinuteGuard << " was sleepiest, at " << maxSleepMinute << "\n";
		cout << " overall " << maxSleepMinuteGuard * maxSleepMinute << "\n";

		
	}
	catch(FileReadException &e){
		cout << "Could not read input file.\n";
	}

}

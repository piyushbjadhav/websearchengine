#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h> 
#include <fstream>
#include <ctime>
#include <unordered_map>
#include <vector>
#include "parser-revised-again.h"

using namespace std;

int main(){

	std::vector<posting> p;
	p.reserve(10240000);
	long int pcount = 0;
	std::unordered_map<std::string,long int> lexicon;
	std::vector<string> urltable;
	long int count = 1; // For Assigining word Id
	int i=0;
	clock_t begin = clock();
	std::ifstream file("temp.wet");
	std::string line;
	while (i<18){
	 	getline(file,line);
		i++;
	}
	int parsecount;
	string garbage,url;
	int length;
	while(!file.eof()){
		parsecount = 0;

		while(parsecount < 3){
			getline(file,line);
			parsecount++;
		}
		istringstream iss(line);
		iss >> garbage;
		iss >> url;
		urltable.push_back(url);
		//cout<<url<<endl;
		while(parsecount < 9){
			getline(file,line);
			parsecount++;
		}

		iss.str(line);
		iss >> garbage;
		iss >> length;
		//cout<<length<<endl;
		getline(file,line);
		char* buffer = new char[length];
		file.read(buffer,length);
		parser((char*)url.c_str(), buffer,length,count ,lexicon,p,pcount);
		//cout << buffer << endl;
		delete buffer;
		getline(file,line);
		getline(file,line);
		count++;
	}

	std::ofstream output_file("./urltable.txt");
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    std::copy(urltable.begin(), urltable.end(), output_iterator);
    std::ofstream output_file_lexicon("./lexicon.txt");
	for(const auto & word_pair : lexicon ) {
   		output_file_lexicon << word_pair.first << ": ";
   		output_file_lexicon << word_pair.second;
   		output_file_lexicon << std::endl;
	}

	clock_t end = clock();
	cout << endl << "Execution time :: " <<(end-begin)/CLOCKS_PER_SEC << " Seconds"<<endl;
	return 0;
}
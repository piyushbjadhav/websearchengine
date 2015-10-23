#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <ctime>

struct doc{
	long int docid;
	int frequency;
	double bm25;
};

struct term{
	long int wordid;
	long int length;
	long int docfrequency;
	long int offset;
};

typedef struct term term;
typedef struct doc doc;

class readposting{

public:
	char *list;
	long int ptr;
	long int wordid;
	long int length;
	long int prev;
	readposting(long int offset , int length){
		prev = 0;
		length = length;
		ptr = 0;
		list = new char[length];
		std::ifstream file("index.bin");
		file.seekg(offset);
		file.read(list,length);

		this->wordid = readnext();
	}

long int readnext(){

	unsigned long int num = 0;
    char temp;
    const char mask = (char)128;
    unsigned int multiplier = 1;

	while(ptr<=length){
        temp = list[ptr];
        ptr++;
        if(temp & mask){
             unsigned int val = (unsigned int)(temp & ~mask);
            num +=  val * multiplier;
            return num;
        }
        else{
            num += (int)temp * multiplier;
            multiplier*=128;
        }
    }

    return -1;
}

doc nextdoc(){

	doc document;
	document.docid = readnext();
	document.frequency = readnext();
	document.docid+=prev;
	prev = document.docid;
	return document;
}

doc nextdoc (long int docid){

	doc next = nextdoc();
	while(next.docid < docid)
		next = nextdoc();

	return next;
}

};


int main(){

	std::cout << "Initializing Query Processor ..." << std::endl;
	std::cout << "Loading Lexicon ..." << std::endl;
	clock_t begin = clock();
    std::unordered_map<std::string,term> lexicon;
    std::ifstream lexfile("templexicon.txt"); 
    std::string lexline;
    while( std::getline( lexfile, lexline ) ){
        std::stringstream lexss(lexline);
        term temp;
        std::string key;
        lexss >> key;
        lexss >> temp.wordid;
        lexss >> temp.wordid;
        lexss >> temp.offset;
        lexss >> temp.docfrequency;
        lexss >> temp.length;
        lexicon[key] = temp;
    }

    clock_t end = clock();
    std::cout << "Done." << std::endl;
    std::cout << std::endl << "Time :: " <<(end-begin)/CLOCKS_PER_SEC << " Seconds"<<std::endl;

    std::string query;
    std::cout << "Enter Query ::" << std::endl;
    std::getline(std::cin, query);
    //std::cin >> query;
    // std::cout << query << std::endl;
    term t[10];
    std::istringstream queryss(query);
    int numberofqueryterms = 0;
    std::string q;
    while(queryss >> q){
    	std::cout << q<<std::endl;
    	t[numberofqueryterms++] = lexicon[q];
    }

    std::cout << t[0].wordid << std::endl;
    std::cout << t[1].wordid << std::endl;

	// readposting *post = new readposting(17721000,208);
	// std::cout << post->wordid <<std::endl;
	// doc next = post->nextdoc(15000);
	// std::cout << next.docid << std::endl;
	// std::cout << next.frequency << std::endl;

	return 0;
}
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include <cmath>

#define TOTAL_DOCUMENTS 51045
#define AVG_LENGTH 694
#define K1 1.2
#define B 0.75

struct doc{
	long int docid;
	int frequency;
};

struct term{
	long int wordid;
	long int length;
	long int docfrequency;
	long int offset;
};

struct url{
	std::string url;
	int wordcount;
};

struct result{
	std::string url;
	double bm25;
};

typedef struct term term;
typedef struct doc doc;
typedef struct url url;
typedef struct result result;

bool term_sorter(term const&lhs , term const& rhs){

	return lhs.docfrequency < rhs.docfrequency;
}

bool result_sorter(result const&lhs , result const& rhs){

	return lhs.bm25 < rhs.bm25;
}

double getbm25(long int ft,int fdt,int dlen){
	double konstant = K1 * ((1 - B) + B * (double)dlen/AVG_LENGTH);
	return log(( (TOTAL_DOCUMENTS - ft + 0.5)/(ft+0.5) ) * (K1+1)*fdt/(konstant+fdt));
}

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
        //avg+=(double)docfrequency/TOTAL_DOCUMENTS
        lexss >> temp.length;
        lexicon[key] = temp;
    }

    //clock_t end = clock();
    std::cout << "Done." << std::endl;
    std::cout << "Loading URL Table ..." << std::endl;
    std::vector<url> urltable;
    std::ifstream urlfile("urltable.txt"); 
    std::string urlline;
    long long int avg = 0;
    long int doccount =0;
    url garbage;
    urltable.push_back(garbage);
    while( std::getline( urlfile, urlline ) ){
        std::stringstream urlss(urlline);
        url tempurl;
        urlss >> tempurl.url;
        urlss >> tempurl.wordcount;
        avg+=tempurl.wordcount;
        urltable.push_back(tempurl);
        doccount++;
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

    std::sort(t,t + numberofqueryterms,term_sorter);

    readposting* post[10]; 

    for(int k=0;k<numberofqueryterms;k++)
    	post[k] = new readposting(t[k].offset,t[k].length);
    
    doc d,tempdoc;
    doc doclist[10];
    tempdoc.docid = 0;
    std::vector<result> results;
    while(d.docid <= TOTAL_DOCUMENTS){

    	d = post[0]->nextdoc(tempdoc.docid);
    	doclist[0] = d;
    	for (int i = 1; i < numberofqueryterms ;i++){
    		tempdoc = post[i]->nextdoc(d.docid);
    		doclist[i] = tempdoc;
    		if(tempdoc.docid != d.docid)
    			break;
    	}

    	if(tempdoc.docid == d.docid){
    		//doc contains all words
    		std::cout << d.docid << std::endl;
    		double bm25 = 0.0;
    		for(int l =0 ; l<numberofqueryterms ;l++ ){

    			bm25+= getbm25(t[l].docfrequency,doclist[l].frequency,urltable[d.docid].wordcount);
    		}

    		result r;
    		r.url = urltable[d.docid].url;
    		r.bm25 = bm25;
    		results.push_back(r);
    	}



    }

    std::sort(results.begin(),results.end(),result_sorter);

    for(result r: results)
    	std::cout<<r.url<<std::endl;

    std::cout<<results.size()<<std::endl;

	// readposting *post = new readposting(17721000,208);
	// std::cout << post->wordid <<std::endl;
	// doc next = post->nextdoc(15000);
	// std::cout << next.docid << std::endl;
	// std::cout << next.frequency << std::endl;

	return 0;
}
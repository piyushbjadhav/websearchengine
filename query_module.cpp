#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include "posting_reader.cpp"

#define TOTAL_DOCUMENTS 51045
#define AVG_LENGTH 694
#define K1 1.2
#define B 0.75


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
typedef struct url url;
typedef struct result result;

bool term_sorter(term const&lhs , term const& rhs){

	return lhs.docfrequency < rhs.docfrequency;
}

bool result_sorter(result const&lhs , result const& rhs){

	return lhs.bm25 > rhs.bm25;
}

double getbm25(long int ft,int fdt,int dlen){
	double konstant = K1 * ((1 - B) + B * (double)dlen/AVG_LENGTH);
	return log(( (TOTAL_DOCUMENTS - ft + 0.5)/(ft+0.5) ) * (K1+1)*fdt/(konstant+fdt));
}


class query_module{
public:

    std::unordered_map<std::string,term> lexicon;
    std::vector<url> urltable;

    query_module(){

	   // std::cout << "Initializing Query Processor ..." << std::endl;
	   // std::cout << "Loading Lexicon ..." << std::endl;
	   // clock_t begin = clock();
    
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
        // std::cout << "Done." << std::endl;
        // std::cout << "Loading URL Table ..." << std::endl;
    
        std::ifstream urlfile("urltable.txt"); 
        std::string urlline;
        /*    long long int avg = 0;
                long int doccount =0;*/
        url garbage;
        urltable.push_back(garbage);
        while( std::getline( urlfile, urlline ) ){
            std::stringstream urlss(urlline);
            url tempurl;
            urlss >> tempurl.url;
            urlss >> tempurl.wordcount;
            // avg+=tempurl.wordcount;
            urltable.push_back(tempurl);
            /*doccount++;*/
        }
    }

    // clock_t end = clock();
    // std::cout << "Done." << std::endl;
  


    // std::cout << std::endl << "Time :: " <<(end-begin)/CLOCKS_PER_SEC << " Seconds"<<std::endl;

    // std::string query;
    // std::cout << "Enter Query ::" << std::endl;
    // std::getline(std::cin, query);
    //std::cin >> query;
    // std::cout << query << std::endl;
    void getresults(std::string query){
        term t[10];
        std::istringstream queryss(query);
        int numberofqueryterms = 0;
        std::string q;
        while(queryss >> q && numberofqueryterms<10){
    	
        //std::cout << q << std::endl;
        std::cout<<std::flush;
    	   t[numberofqueryterms] = lexicon[q];
        numberofqueryterms++;

        }

        std::cout <<std::flush;
        std::sort(t,t + numberofqueryterms,term_sorter);

        readposting* post[10]; 

        for(int k=0;k<numberofqueryterms;k++){
            //std::cout << "offset ::"<<t[k].offset<< "Length::" << t[k].length << std::endl; 
    	    post[k] = new readposting(t[k].offset,t[k].length);
            //std::cout << "wordid ::"<<post[k]->wordid <<std::endl;
        }

        //std::cout << "Number of Query Terms::"<<numberofqueryterms <<std::endl;
    
        doc d,tempdoc,prev;
        doc doclist[10];
        d.docid = 0;

        std::vector<result> results;
        while(d.docid <= TOTAL_DOCUMENTS){
            //std::cout<<"Here !!"<<std::endl;
            //std::cout<<d.docid<<std::endl;

            prev = d;
    	   d = post[0]->nextdoc(tempdoc.docid,t[0].length);

            //std::cout<<d.docid<<std::endl;
            if(d.docid == -1)
                break;
    	   doclist[0] = d;
            //std::cout<<"First Doc :"<<d.docid;
    	   for (int i = 1; i < numberofqueryterms ;i++){
    	       tempdoc = post[i]->nextdoc(d.docid,t[i].length);
    		  doclist[i] = tempdoc;
    		  if(tempdoc.docid != d.docid)
    		      break;
            }
    	   if(numberofqueryterms == 1 || tempdoc.docid == d.docid){
    		  //doc contains all words
                //std::cout<<d.docid<<std::endl;
    		  std::cout << std::flush;
                //long int temp = d.docid;
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

        //std::cout << "Exited While"<<std::endl;

        std::sort(results.begin(),results.end(),result_sorter);

        for (unsigned i = 0; i<results.size();i++ ){
    	   std::cout<<results[i].url<<std::endl;
    	   if(i==9)
    		  break;
        }

        for(int i=0;i<numberofqueryterms;i++)
            delete post[i];
    }

};
// int main(){
//     intialize();
//     getresults("sachin tendulkar");
//     return 0;
// }
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h> 
#include <unordered_map>
#include <ctime>


unsigned long int posn;
// struct entry{
//     long int word;
//     long unsigned int offset;
//     long int count;
// };

typedef struct entry entry;
// Writes Integer Into File as varbyte
void writevarbyte(long int x){

    static std::ofstream file("index.bin",std::ios::binary);

    char r;
    if( x == 0){
        r = x;
        posn++;
        file.write((char *) &r , sizeof(char));
        return;
    }

    while(x>0){
        r = x%128;
        x = x/128;
        if(x==0){
            r |= 1 << 7;
        }
        posn++;
        file.write((char *) &r , sizeof(char));
    }

    if(x==-1){
        file.close();
    }
}

int main(){
    // Zips throgh the sorted posting list , calculates docid frequency
    //Calculates difference between docid and compresses them using varbyte

    //loading lexicon
    std::cout<<"Loading Lexicon ...."<<std::endl;
    clock_t begin = clock();
    //std::unordered_map<long int,entry> lexicon;
    std::ofstream lexfile("wordidtable.txt"); 

    // while( std::getline( lexfile, lexline ) ){
    //     std::istringstream lexss(lexline);
    //     entry temp;
    //     lexss >> entry.word;
    //     long int wordid;
    //     lexss >> wordid;
    //     temp.count = 0;
    //     temp.offset = 0;
    //     lexicon[wordid] = temp;
    // }

    clock_t end = clock();
    std::cout << std::endl << "Time :: " <<(end-begin)/CLOCKS_PER_SEC << " Seconds"<<std::endl;
    //Opens Sorted Postings List
    std::ifstream file( "sorted.txt" );
    std::string line;

    char* myBuffer;

    long int distance;
    long int count = 1;
    long int prevword = 0;
    long int numberofdocs = 0;
    long int prevdocid = -1;
    long int word = 0;
    long int docid = 0;
    std::getline( file, line );
    std::istringstream iss(line);
    iss >> prevword;
    iss >> prevdocid;

    // lexicon[prevword].offset = posn;
    numberofdocs = 1;
    bool wordflag = 0;
    long int prevToBeWritten = 0;
    long int current = prevword;
    distance = posn;
    writevarbyte(prevword);
    while( std::getline( file, line ) )
	{
        std::istringstream iss(line);
		
		if (word == current){

		}else{
			if(wordflag == 1){
                distance = posn;
                //std::cout<<distance<<std::endl;
                writevarbyte(word);
			}
		
			current = word;
		}
        
        iss >> word;
        iss >> docid;

        if(word == prevword && docid == prevdocid ){
        	count ++;
        }
        else if(word == prevword){
            numberofdocs++;
            writevarbyte(prevdocid - prevToBeWritten);
            writevarbyte(count);
            prevToBeWritten = prevdocid;
            
        	count = 1;
        }
        else{
            lexfile << prevword << " " << distance <<" "<<numberofdocs << " "<<posn - distance + 2 <<std::endl;
            
            numberofdocs = 1;
            // lexicon[prevword].count = numberofdocs;
            // lexicon[word].offset = count;
            writevarbyte(prevdocid - prevToBeWritten);
            writevarbyte(count);
            prevToBeWritten = 0;
        	count = 1;
        	
        	wordflag = 1;
        }


        prevword = word;
        prevdocid = docid;  
	}

    lexfile << word << " " << distance <<" "<<numberofdocs << " "<<posn-distance <<std::endl;
    writevarbyte(docid - prevToBeWritten);
    writevarbyte(count);
	return 0;
}
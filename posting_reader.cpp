
#include <fstream>
#include <iostream>
#include <string>

struct doc{
	long int docid;
	int frequency;
};

typedef struct doc doc;

class readposting{

public:
	char *list ;
	long int ptr;
	long int wordid;
	//long int length;
	long int prev;
	readposting(long int offset , int length){
		prev = 0;
		//length = length;
		ptr = 0;
        //std::cout<<"Length ::"<<length<<std::endl;
		list = new char[length];
		std::ifstream file("index.bin");
		file.seekg(offset);
		file.read(list,length);
        file.close();
		this->wordid = readnext(length);
        //std::cout<<"WORD ::"<<wordid<<std::endl;
	}

long int readnext(int length){

    long int num = 0;
    char temp = 0;
    const char mask = (char)128;
    unsigned int multiplier = 1;
	while(ptr<length){
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
    //delete list;
    return -1;
}

doc nextdoc(int length){
	doc document;
	document.docid = readnext(length);
	document.frequency = readnext(length);
    if(document.docid != -1)
	   document.docid+=prev;
	prev = document.docid;
	return document;
}

doc nextdoc (long int docid,int length){

	doc next = nextdoc(length);
	while(next.docid < docid && next.docid>0){
		next = nextdoc(length);
	}

	return next;
}

};
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <locale>
#include <vector>

#define xl_isascii(c) (((c) >= 0) && ((c) <= 255))
#define xl_isdigit(c) (((c) >= '0') && ((c) <= '9'))
#define xl_islower(c) (((c) >= 'a') && ((c) <= 'z'))
#define xl_isupper(c) (((c) >= 'A') && ((c) <= 'Z'))
#define xl_isindexable(c) (xl_isdigit(c) || xl_islower(c) || xl_isupper(c))
#define xl_tolower(c) ((c) += 'a' - 'A')


struct posting{

	long int wordid;
	long int docid;
	char tag;
};

typedef struct posting posting;

bool posting_sorter(posting const&lhs , posting const& rhs){
	if(lhs.wordid != rhs.wordid)
		return lhs.wordid < rhs.wordid;
	return lhs.docid < rhs.docid;
}
int getlength(long int digit){
	int digitlength = 0;

	if(digit == 0)
		return 0;
	while(digit!=0){
		digit/=10;
		digitlength++;
	}

	return digitlength;

}



unsigned long int parser(char* url, char* doc, int maxlen,long int docid ,std::unordered_map<std::string,long int> &lex,std::vector<posting> &postings,long int &pcount)
{
	char *p, *purl, *word;
	char ch;
	unsigned int wordcount = 0;
	long int wordid;
	char fileloc[30];
	static int file_number = 1;
	static std::ofstream outputfile;
	p = doc;
	if (p == NULL)
		return 0;
/* parsing URL */
	// purl = url;
	// while (*purl != '\0')
	// {
	// 	if (!xl_isindexable(*purl))
	// 	{
	// 		purl++;
	// 		continue;
	// 	}

	// 	word = purl;
	// 	while (xl_isindexable(*purl))
	// 	{
	// 		if (xl_isupper(*purl))
	// 			xl_tolower(*purl);
	// 		purl++;
	// 	}

	// 	ch = *purl;
	// 	*purl = '\0';


		
	// 	// Assigns WordID
	// 	if(lex[word] == 0){
	// 		wordid = lex.size();
	// 		lex[word] = lex.size();

	// 	}else{

	// 		wordid = lex[word];
	// 	}

	// 	//if Vector full , sort and dump to file
	// 	if(pcount == 10240000){
	// 		pcount = 0;
	// 		std::sort(postings.begin(),postings.begin() + 10240000,&posting_sorter);
	// 		sprintf(fileloc,"temp/index%d",file_number++);
	// 		outputfile.open(fileloc);

	// 		for(long int i = 0 ; i< 10240000;i++)
	// 			outputfile << postings[i].wordid << " "<<postings[i].docid <<"\n";
	// 		outputfile.close();
	// 	}
	// 	postings[pcount].wordid = wordid;
	// 	postings[pcount].docid = docid;
	// 	pcount++;

	// 	*purl = ch;
	// }

/* parsing page */

	while ((p - doc) < maxlen)
	{
		
		if (!xl_isindexable(*p))
		{
			p++;
			continue;
		}

		word = p;
		while (xl_isindexable(*p))
		{
			if (xl_isupper(*p))
				xl_tolower(*p);
			if((p - doc) >= maxlen - 1)
				break;
			p++;
		}

		*p = '\0';

		if((p - doc) >= maxlen - 1)
				break;

		p++;

		if(*word == ' ')
			continue;
		//Assiging Word ID
		if(lex[word] == 0){
			wordid = lex.size();
			lex[word] = lex.size();

		}else{

			wordid = lex[word];
		}
	
		// If Vector Full , Sort and Dump to File
		if(pcount == 10240000){
			pcount = 0;
			std::sort(postings.begin(),postings.begin() + 10240000,&posting_sorter);
			sprintf(fileloc,"temp/index%d",file_number++);
			outputfile.open(fileloc);

			for(long int i = 0 ; i< 10240000;i++)
				outputfile << postings[i].wordid << " "<<postings[i].docid <<"\n";
			outputfile.close();
		}
			//std::cout << word <<" " << docid<< " "<<std::endl;
			postings[pcount].wordid = wordid;
			postings[pcount].docid = docid;
			pcount++;
			wordcount++;
	}

	return wordcount;
}

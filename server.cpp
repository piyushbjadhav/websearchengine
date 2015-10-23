#include "query_module.cpp"

int main(){
	initialize();
	getresults("fast forward");
	getresults("fast forward");
	getresults("fast forward");
	getresults("fast forward");
	getresults("fast forward");
	getresults("fast forward");
	getresults("fast forward");
	getresults("fast forward");
	getresults("fast forward");
	
	std::string query;
	while(1){
	std::cout << "Enter Query ::";
	std::getline(std::cin, query);
	clock_t begin = clock();
	getresults(query);
	clock_t end = clock();
	std::cout << std::endl << "Time :: " <<(end-begin)/CLOCKS_PER_SEC << " Seconds"<<std::endl;
	}

}
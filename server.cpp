#include "query_module.cpp"



int main(){
	std::string query;

	query_module *q = new query_module( );
	while(1){
		std::cout << "Enter Query ::";
		std::getline(std::cin, query);
		clock_t begin = clock();
		q->getresults(query);
		clock_t end = clock();
		std::cout << std::endl << "Time :: " <<(double)(end-begin)/CLOCKS_PER_SEC << " Seconds"<<std::endl;
		//delete q;
		//q = new query_module();
	}

}

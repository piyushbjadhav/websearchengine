#include <fstream>
#include <iostream>
void writevarbyte(long int x){
    static std::ofstream file("test.hex",std::ios::binary);
    char r;
    if( x == 0){
        r = x;
        file.write((char *) &r , sizeof(char));
        return;
    }

    while(x > 0){
        r = x%128;
        x = x/128;
        if(x==0){
            r |= 1 << 7;
        }
        file.write((char *) &r , sizeof(char));
    }
    if(x==-1){
        file.close();
    }
}

long int readvarbyte(long int offset){
    static std::ifstream file("index.bin",std::ios::binary);
    file.seekg(offset);
    unsigned long int num = 0;
    char temp;
    const char mask = (char)128;
    unsigned int multiplier = 1;
    while(!file.eof()){
        file.read(&temp,sizeof(char));
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
int main(){

    long int x = readvarbyte(23162411);
    std::cout<<x<<" ";
       
    

    return 0;
}
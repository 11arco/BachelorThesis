#include <iostream>
#include <string>

#include <bitset>
#include <cstdint>

using namespace std;
typedef unsigned int uint32; //actually 32 bit uint 



int md5comporess()
{
    return 5;
}

int get_msb_pos(uint32 u)
{
    int counter =0;
    int u2 = u;
    while (u2 > 1)
    {       
        counter +=1; 
        u2 = u2>>counter;       
        std::cout << to_string(u2) +"|";
        std::cout << to_string(counter) << endl;

    }
    return counter;
}

uint32 md5(uint32 input)
{    
    //init block
    uint32 block[16]; // 16*32=512 bit block
    int msb_pos = 0;
    int wordlen = 31;
    msb_pos = get_msb_pos(input);//get input len (act. used)
    std::cout << "msbpos :" + to_string(msb_pos)<< endl;
    std::cout << "input  :" + to_string(input)<< endl;

    input = input<<1;   //shift left with 1
    input +=1;          //set last bit 1 by adding 1
    std::cout << "input  after shifting 1:" + to_string(input)<< endl;

    std::cout << "to shift  :" + to_string(wordlen- (msb_pos +1))<< endl;
    input = input<< wordlen-(msb_pos +1); // fill max len with 0
    
    std::cout << "check:" +to_string(input )<< endl;
    std::cout << "check:" +to_string(input >>(wordlen- (msb_pos )))<< endl;


  

#



    return input; //output
}



int main()
{
    string val = "abc";
    //uint32 t = 18446744073709551615;
    md5(5);
    //std::cout << md5(5);
    return 0;
}
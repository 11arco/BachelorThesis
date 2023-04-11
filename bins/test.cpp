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
        //std::cout << to_string(u2) +"|";
        //std::cout << to_string(counter) << endl;

    }
    return counter;
}


uint32 shifting_word(uint32 input)
{
    
    int msb_pos =get_msb_pos(input);
    int wordlen = 31;


    //cout << "msbpos :" + to_string(msb_pos)<< endl;
    //cout << "input  :" + to_string(input)<< endl;

    input = input<<1;   //shift left with 1
    input +=1;          //set last bit 1 by adding 1
    //cout << "input  after shifting 1:" + to_string(input)<< endl;

    //cout << "to shift  :" + to_string(wordlen- (msb_pos +1))<< endl;
    input = input<< wordlen-(msb_pos +1); // fill max len with 0
    
    //cout << "check:" +to_string(input )<< endl;
    //cout << "check:" +to_string(input >>(wordlen- (msb_pos )))<< endl;

    return input; //output
}

uint32* pad(uint32* msg)
{
if((sizeof(msg)/sizeof(*msg)) != 16) //check len of 
{
    cout << "msg len not 16" << endl;
}

    int msg_end = (sizeof(msg)/sizeof(*msg));
    int pos_mbs = get_msb_pos(msg[msg_end]);  // 0000000001001
                                                //          ^  ^  => 9
                                                //    pos(msb)    => 3
    msg[msg_end] = shifting_word(msg[msg_end]);


    return msg;
}

uint32* partition( uint32 msg [])//#todo
{
    const int N = 1; // N blocks  each block contains 32bit uint
    uint32 blocks[N][16];
    return 0;
}

uint32* md5_compress(uint32* ihv, uint32 block [])//#todo   
{
    return 0;
}

uint32* process( uint32 msg[]) //#todo
{
    uint32 ihv[4] = {0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476} ; //(67452301,EFCDAB89,98BADCFE,10325476);
    uint32 block [16];
    
    return md5_compress(ihv,block);
}

uint32* md5( uint32 input [])
{
    uint32 output[16];

    input = pad(input);
    input = partition(input);

    return process(input);




return output;
}



int main()
{
    string val = "abc";
    //uint32 t = 18446744073709551615;
    //#todo put vals in block
    int blocks_len = (val.length()/512);
    
    int block[16]; //16*32 =512
    //md5(block);
    //std::cout << md5(5);
    return 0;
}
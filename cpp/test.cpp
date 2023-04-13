#include <iostream>
#include <string>

#include <bitset>
#include <cstdint>

#include <climits>
#include <sstream>
#include <string.h>

#include <bitset>

using namespace std;
typedef unsigned int uint32; //actually 32 bit uint 



//MD5 implementation
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

string pad(string msg)
{
    cout << msg +"|";
    cout << msg.length() << endl;
    

    char l []= {msg.length()};

    bitset<64> x(msg.length());
    cout <<  "msg len as bit: ";
    cout <<  x << '\n';

    int offset = 8 - get_msb_pos(msg.length()+1);



    
    char zero =0; //8bit
    char one = 1<<7; //8bit
    msg = msg + one;

    while (msg.length() % 64 != 56) //8bit * 64 = 512 bit
    {

        msg = msg + zero;
        cout << msg +"|";
        cout << msg.length() << endl;
    }
    


    msg = msg + x;



    cout << "final:" + msg +"|";
    cout << msg.length() << endl;
 //   char len = cast msg_len4;


    //int pos_mbs = get_msb_pos(msg[msg_end]);  // 0000000001001
                                                //          ^  ^  => 9
                                                //    pos(msb)    => 3
   // msg[msg_end] = shifting_word(msg[msg_end]);


    return msg;
}



uint32* md5_compress(uint32 ihv [4], uint32 block [16])//#todo   
{
    return ihv;
}

uint32* process( uint32 msg[], int size) //#todo
{
    uint32 ihv[4] = {0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476} ; //(67452301,EFCDAB89,98BADCFE,10325476);
    uint32* ihvN;
    const int N = size * 32; //N is the size of the massage 
    uint32 block [16];// N blocks each block contains 32bit uint 16 * 32 = 512
    uint32 block_pos;


        block_pos = 0;
        for(int j = 0; j < 16; j++)
        {
            block[j] = msg[block_pos +j];
        }


        ihvN = md5_compress(ihv,block);


    for (int i = 1; i < N + 1; i++) //partitioning
    {
        block_pos = i * 32;
        for(int j = 0; j < 16; j++)
        {
            block[j] = msg[block_pos +j];
        }


        ihvN = md5_compress(ihvN,block);
    }
    
    return 0;
    
    
    return md5_compress(ihv,block);
}

uint32* md5( uint32 input [])
{
    uint32 output[16];
    int size;
    //input = pad(input);
    // input = partition(input);

    return process(input, size);

return output;
}

//MD5 attack implementation

int* attack_md5()
{



}


int main()
{
    // input is string
    // string is char*
    // char ist 8 bit

    string val = "abc";
    //uint32 t = 18446744073709551615;
    //#todo put vals in block
    int blocks_len = (val.length()/512);
    
    int block[16]; //16*32 =512
    //md5(block);
    //std::cout << md5(5);
    // cout << "abc"<< endl;
    string test = "abc";
    
    //char test2;
    //test2 =1<<7;
    //test = test + test2;


    cout << (test);

    cout << pad(test)<< endl;


    char a = 2;
    bitset<8> x(a);
    cout << x << '\n';

    return 0;
}
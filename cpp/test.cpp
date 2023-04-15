#include <iostream>
#include <string>

#include <bitset>
#include <cstdint>

#include <climits>
#include <sstream>
#include <string.h>

#include <bitset>

#include <thread>

using namespace std;
typedef unsigned int uint32; //actually 32 bit uint 



int get_msb_pos(uint32 u)
{
    int counter =0;
    uint32 u2 = u;
    while (u2 > 1)
    {       
        counter +=1; 
        u2 = u2>>counter;       
        std::cout << to_string(u2) +"|";
        std::cout << to_string(counter) << endl;

    }
    return counter;
}


//MD5 implementation
int md5comporess()
{
    return 5;
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

    
    int l = msg.length();
    int offset = 1+(l / (2^32)) ;
    bitset<64> x(l);
    string length_for_adding = "";
    length_for_adding = l;

    cout << length_for_adding.length() <<endl;
    cout << "offset :"  <<endl;
    cout << offset << endl;

    cout << "input: " + msg  << endl;
    cout << "length:" ;
    cout << l << endl;

    cout <<  "msg len as bit: ";
    cout <<  x << '\n';
    cout << ""  << endl;




    
    char zero = 0; //8bit
    char one = 1 << 7; //8bit
    msg = msg + one;

    while (msg.length() % 64 != 64 - offset) //8bit * 64 = 512 bit
    {
        msg = msg + zero;
        cout << msg +"|";
        cout << msg.length() << endl;
    }
    
    //int msb_l = get_msb_pos(l);


    msg = msg + length_for_adding; // #todo !!!!



    cout << "final:" + msg +"|";
    cout << msg.length() << endl;
    cout << bitset<64> (msg.length()) << endl;
    cout << ""  << endl;
 //   char len = cast msg_len4;


    //int pos_mbs = get_msb_pos(msg[msg_end]);  // 0000000001001
                                                //          ^  ^  => 9
                                                //    pos(msb)    => 3
   // msg[msg_end] = shifting_word(msg[msg_end]);


    return msg;
}

void print_step(int step)
{
    string calc = "calculating ";
    string end = "th block";

    cout << calc + to_string(step) + end << endl;
}


uint32* md5_compress(uint32 ihv [4], uint32 block [16])//#todo   
{
    return ihv;
}

string process( string input) //#todo
{
    cout << "staring" << endl;
    string output;
    string padded_input = pad(input);
    cout << "padding comlete" << endl;

    int size = padded_input.size();

    stringstream a;
    stringstream b;
    stringstream c;
    stringstream d;

    uint32 ihv[4] = {0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476} ; //(67452301,EFCDAB89,98BADCFE,10325476);
    uint32* ihvN;
    uint32 msg_block [16];// N blocks each block contains 32bit uint 16 * 32 = 512
    uint32 block_pos = 0;

    cout << "calculating first block" << endl;

    for(int j = 0; j < 16; j++) //first run. Block 16*32 = 512
    {
        msg_block[j] = padded_input[j];    
    }


        ihvN = md5_compress(ihv,msg_block);


    for (int h = 1; h <= size; h++) 
    {   
        print_step(h);
        for(int j = 0; j < 16; j++)
        {
            msg_block[j] = padded_input[h +j];
        }


        ihvN = md5_compress(ihvN,msg_block);


    }

    a << std::hex << ihvN[0];
    b << std::hex << ihvN[1];
    c << std::hex << ihvN[2];
    d << std::hex << ihvN[3];
        
    output = a.str() + b.str() + c.str() + d.str(); 
    
    return output;
}



//MD5 attack implementation

int attack_md5()
{


    return 0;
}


int main()
{
    // input is string
    // string is char*
    // char ist 8 bit

    string val = "abcdefghijk";//len 50
    string large = val + val + val + val + val; // len 250
    string xl = large + large + large + large + large; // len 1250
    string xxl = xl + xl + xl + xl + xl; // 6.250
    string xl3 = xxl + xxl + xxl + xxl + xxl + xxl + xxl + xxl + xxl + xxl + xxl + xxl +xxl +xxl +xxl +xxl +xxl +xxl +xxl +xxl +xxl +xxl +xxl +xxl +xxl  ;
    //uint32 t = 18446744073709551615;
    
    string test = val;
    


    cout << process(test)<< endl;



    return 0;
}
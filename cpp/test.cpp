#include <iostream>
#include <string>

#include <bitset>
#include <cstdint>

#include <climits>
#include <sstream>
#include <string.h>

#include <bitset>

#include <thread>


#include <cmath>

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

void print_step(int step)
{
    string calc = "calculating ";
    string end = "th block";

    cout << calc + to_string(step + 1) + end << endl;
}

//MD5 implementation


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
    bitset<32> x(l);
    string length_for_adding = "";
    length_for_adding = l;

    cout << length_for_adding.length() <<endl;

    cout << "input: " + msg  << endl;
    cout << "length:" ;
    cout << l << endl;

    cout <<  "msg len as bit: ";
    cout <<  x << '\n';
    cout << ""  << endl;

    
    char zero = 0; //8bit
    char one = 1 << 7; //8bit
    //cout << bitset<8> (one) << endl;

    msg = msg + one;

    while (msg.length() % 64 != 63) //8bit * 64 = 512 bit // 16 *32 = 512
    {
       // cout<< "msg.length-offset: " + to_string(msg.length()) + "-" + to_string(offset) + " " << endl;;
        msg = msg + zero;
        cout << msg +"|";
        cout << msg.length() << endl;
    }
    
    //int msb_l = get_msb_pos(l);


    msg = msg + length_for_adding; // 



    cout << "final:" + msg +"|";
    cout << msg.length() << endl;
    cout << bitset<64> (msg.at(msg.length() -1)) << endl;

    cout << ""  << endl;
 //   char len = cast msg_len4;


    //int pos_mbs = get_msb_pos(msg[msg_end]);  // 0000000001001
                                                //          ^  ^  => 9
                                                //    pos(msb)    => 3
   // msg[msg_end] = shifting_word(msg[msg_end]);


    return msg;
}


uint32 f_t(uint32 X, uint32 Y, uint32 Z, int t ) 
{ 
    
    if (t < 16)
    {
    //  cout << "Case 0 : ";
    //  cout << t << endl;
        return (X & Y) ^ ((~X) & Z);
    }
    else if (t < 32)
    {
    //    cout << "Case 1 : " ; 
    //    cout << t << endl;
        return (Z & X) ^ ((~Z) & Y);
    }
    else if (t < 48)
    {  
    //    cout << "Case 2 : " ; 
    //    cout << (X ^ Y ^ Z) << endl;

        return (X ^ Y ^ Z);
    }
    else if (t < 64)
    {
    //    cout << "Case 3 : " ; 
    //    cout << t << endl;
        return (Y ^(X | (~Z)));
    }
    cout << "f_t faild. t not in scope.";
    return 0; // something went wrong
}

uint32 W ( uint32 m [16], int t) // m is the actual massage block
{ 
    int pos=-1;
    if (t < 16) pos = t ;
    else if (t < 32){ 
        pos = (1+(5*t)) % 16;

    }
    else if (t < 48){ 
        pos = (5+(3*t)) % 16;

    }
    else if (t < 64){ 
        pos = (7*t) % 16;

    }

    //cout << t;
    //cout << "-pos" + to_string(pos) + " ";
   // cout << to_string(t) +": "+ to_string(m[pos]) + " at " + to_string(pos) << endl;     
    if (pos == -1) cout << "something went wrong:" + to_string(t)<< endl;
    return m[pos]; 

}

uint32 RC (int t) // only 0 if somethin went wrong
{
    uint32 arr[] = {7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};
    //cout << "RC: " + to_string(t) + " | " + to_string( arr[t] ) << endl;

    return arr[t];
}



uint32 RL (uint32 T, int RC) // shifting being cyclict
{
    uint32 temp = T;
    //cout <<"rotating ";
    //cout << bitset<32>(T);
    //cout << " by " + to_string(RC)+ " gives: ";

    T = T << RC;
    temp = temp >> (32-RC);
    T = T + temp;
    //cout << bitset<32>(T)<<endl;
    return T;
}


uint32* md5_compress(uint32 ihv [4], uint32 block [16])//#todo   
{
    uint32 a = ihv [0];
    uint32 b = ihv [1];
    uint32 c = ihv [2];
    uint32 d = ihv [3];
    

    //stringstream x; 
    //x << std::hex << block;
    for (int i = 0; i< 16 ; i++)
    {
        cout << " | ";
        cout << bitset<32> (block[i]);
    }
    cout << " | " << endl;


    //cout << x.str() << endl;

    uint32 F;
    uint32 T_;
    uint32 R;
    uint32 AC;

    const int size = 68; //64 + 0 init for Q(t-1) Q(t-2) Q(t-3) and for Q(t+1) at the end

    uint32 Q[size] = {b,c,d,a};
//    cout << "Q[0]: " + to_string(Q[0]) + " - " + " b: " + to_string(b) << endl;
//    cout << "Q[1]: " + to_string(Q[1]) + " - " + " c: " + to_string(c) << endl;
//    cout << "Q[2]: " + to_string(Q[2]) + " - " + " d: " + to_string(d) << endl;
//    cout << "Q[3]: " + to_string(Q[3]) + " - " + " a: " + to_string(a) << endl;
//      
    for ( int t = 3; t < size - 1; t++)   
    {
        // x << std::hex << Q[t];
        // cout << "|" + to_string(t) + "|" + x.str() + "|" + to_string(RC(t)) + "|"<< endl;

        //cout <<"Q_t:" + to_string(Q[t]) + "|" ;


        AC = floor(pow(2,32) * abs(sin( (t-3) + 1)));
        F = f_t( Q[t], Q[t-1], Q[t-2], (t-3));
        T_ = F + Q [t-3] + AC + W( block, (t-3));
        R = RL(T_, RC((t-3))) ;
        //cout << to_string(T_) + " | " + to_string(R) << endl;;
    	
        Q[t+1] = Q[t] + R;
            
    }
 
    ihv[0] = a + Q[61 + 3];
    ihv[1] = b + Q[64 + 3];
    ihv[2] = c + Q[63 + 3];
    ihv[3] = d + Q[62 + 3];

    
    return ihv;
}


string process( string input) //#todo
{
    cout << "staring" << endl;
    string output;
    string padded_input = pad(input);
    cout << "padding comlete" << endl;

    int size = padded_input.size();

    for (int i = 0 ; i < size; i++)
    {
     //   cout << "padi. at " + to_string(i) + " :" + padded_input.at(i) << endl;
    }

    stringstream a;
    stringstream b;
    stringstream c;
    stringstream d;
    int shift = 0;

    uint32 ihv[4] = {0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476} ; //(67452301,EFCDAB89,98BADCFE,10325476);
    uint32* ihvN;
    uint32 msg_block [16] ;// N blocks each block contains 32bit uint 16 * 32 = 512

    cout << "calculating first block" << endl;

    for(int j = 0; j < 16; j++) //first run. Block 16*32 = 512
    {
        for (int i = 0; i < 4; i++)
        {
            shift = (8);
            msg_block[j] =  (msg_block[j] << shift);
            msg_block[j] = msg_block[j] + padded_input.at((j * 4) + i) ;
            //cout << (to_string(j*4+i)+ ".: " + to_string(msg_block[j] ))+ " vs " + to_string(padded_input.at((j * 4) + i)) << endl;

        }
      //  cout << "translate"+ to_string(msg_block[j]) << endl;;    
    
    }
        //cout << " msg in block vs paded input: " + to_string(msg_block[15]) + " vs " + to_string(padded_input.at(63)) << endl;


        ihvN = md5_compress(ihv,msg_block);


    for (int h = 1; h*64 < size ; h++) 
    {   
        print_step(h);
      
        for(int j = 0; j < 16; j++) //first run. Block 16*32 = 512
        {
            for (int i = 0; i < 4; i++)
            {
                shift = (8);
                msg_block[j] =  msg_block[j] << shift;

                msg_block[j] = msg_block[j] + padded_input.at((j * 4) + i) ;
            }
        //  cout << "translate"+ to_string(msg_block[j]) << endl;;    
        
        }

        ihvN = md5_compress(ihvN,msg_block);


    }
     cout << "last block calculated" << endl;


    a << std::hex << ihvN[0];
    b << std::hex << ihvN[1];
    c << std::hex << ihvN[2];
    d << std::hex << ihvN[3];
        
  
    
    return a.str() + b.str() + c.str() + d.str();
}



//MD5 attack implementation


int find_block_1 ()
{
    // choose Q_1,Q_3,..,Q_16 fullfiliing condidtions
    // calculate m_0,m_6,..,m_15
    // 
    return 0;
}

int attack_md5()
{
    // find block 1
    // find block 2
    // NCBSA
    //

    return 0;
}


int main()
{
    // input is string
    // string is char*
    // char ist 8 bit

    string val = "abc";//len 50
    string large = val + val + val + val + val; // len 250
    string xl = large + large + large + large + large; // len 1250
    string xxl = xl + xl + xl + xl + xl; // 6.250
    string xl3 = xxl + xxl + xxl + xxl + xxl + xxl + xxl + xxl + xxl + xxl + xxl + xxl +xxl +xxl +xxl +xxl +xxl +xxl +xxl +xxl +xxl +xxl +xxl +xxl +xxl  ;
    //uint32 t = 18446744073709551615;
    
    string test = val;
    

    cout << process(test)<< endl;
    //****************************


    return 0;
}
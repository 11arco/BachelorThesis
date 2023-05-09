#include <iostream>
#include <string>

#include <bitset>
#include <cstdint>

#include <climits>
#include <sstream>
#include <string.h>

#include <bitset>

#include <thread>

#include <ctime>

#include <cmath>

using namespace std;
typedef unsigned int uint32; //actually 32 bit uint 

uint32 ihv[4] = {0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476} ;     // (67452301,EFCDAB89,98BADCFE,10325476)


int get_msb_pos(uint32 u) // returns the msb from a uint32
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

string from_64(uint64_t input) // converts a uint64 into a string
{
    string output ="";
    output += (char) (input);
    output += (char) (input >> 8);
    output += (char) (input >> 16);
    output += (char) (input >> 24);
    output += (char) (input >> 32);
    output += (char) (input >> 40);
    output += (char) (input >> 48);
    output += (char) (input >> 56);
    return output;
}


string to_hex(uint32 u) // converts a uint32 into a hexadecimal vlaue
{   
    stringstream temp_0 ;
    temp_0.str("");
    if (((u << 24) >> 24) < 0x10 ) (temp_0 <<  '0');  
    temp_0 << hex << ((u << 24) >> 24);
    if (((u << 16) >> 24) < 0x10 ) (temp_0 <<  '0');  
    temp_0 << hex << ((u << 16) >> 24);
    if (((u << 8) >> 24) < 0x10 ) (temp_0 <<  '0');  
    temp_0 << hex << ((u << 8) >> 24);
    if (((u << 0) >> 24) < 0x10 ) (temp_0 <<  '0');  
    temp_0 << hex << ((u << 0) >> 24);
    
    //temp << hex << u;

    return temp_0.str();
}

void print_step(int step)
{
    string calc = "calculating block ";

    cout << calc + to_string(step + 1)  << endl;
}

//MD5 implementation


uint32 shifting_word(uint32 input)
{
    
    int msb_pos =get_msb_pos(input);
    int wordlen = 31;

    input = input<<1;   //shift left with 1
    input +=1;          //set last bit 1 by adding 1

    input = input<< wordlen-(msb_pos +1); // fill max len with 0

    return input; //output
}

string pad(string msg)
{
   // msg+= "\n"; // Wichtig! Endline wir immer mit verabreitet

    int l = msg.length();

    int counter = 0;
    bitset<32> x(l);
    int  length_for_adding = l*8 ;

    cout << "input: " + msg  << endl;
    cout << "length: " ;
    cout << l << endl;
    
    char end = 0; //8bit
    char one = 1 << 7; //8bit

    msg +=one; // maybe?
    
    while ( (msg.length()*8 % (64*8)) != 448 ) //8bit * 64 = 512 bit // 16 * 32 = 512 // 8 * 56 = 448 // depends on the maybes
    {
       // cout<< "msg.length-offset: " + to_string(msg.length()) + "-" + to_string(offset) + " " << endl;;
        msg = msg + end;
        cout << msg.length() << endl;
        counter++;
    }

    msg += from_64(length_for_adding); //maybe

    cout << "final:" + msg + "|";
    cout << msg.length() << endl;


    return msg;
}


uint32 f_t(uint32 X, uint32 Y, uint32 Z, int t ) 
{ 
    uint32 out;
    if (t < 16)
    {
        if (false)
        {
            cout << bitset<32>(X);
            cout << "" << endl;
            cout << bitset<32>(0);
            cout << " | " << endl;
            cout << "________________________________" << endl;

            cout << bitset<32>(X | 0);
            cout << ""<< endl;
            cout << ""<< endl;
        }
        out = (X & Y) ^ ((~X) & Z); // out = 
    }
    else if (t < 32)
    {

        out = (Z & X) ^ ((~Z) & Y);
    }
    else if (t < 48)
    {  

         out = (X ^ Y ^ Z);
    }
    else if (t < 64)
    {

         out = (Y ^ (X | (~Z)));
    }
    //cout <<"f_t:" + to_string(out) << endl;
    if (t<0 || t>63) cout << "f_t faild. t not in scope." + to_string(t) << endl;
    return out; // something went wrong
}

uint32 W ( uint32 m [16], int t) // 16 blocks each 32bit uints
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

    //cout << to_string( m[t] ) << endl;
    //cout << "-pos" + to_string(pos) + " ";
    //cout << to_string(t) +": "+ to_string(m[pos]) + " at " + to_string(pos) << endl;     
    if (pos <= -1) cout << "something went wrong un W. t is:" + to_string(t)<< endl;
    if (pos >= 16) cout << "something went wrong in W. t is:" + to_string(t)<< endl;

    return m[pos]; 

}

uint32 RC (int t) // return the roataion constat
{
    uint32 c ;
    uint32 RC0[4] ={7,12,17,22};
    uint32 RC1[4] ={5,9,14,20};
    uint32 RC2[4] ={4,11,16,23};
    uint32 RC3[4] ={6,10,15,21};

    if (t < 16)
    {
        c = RC0[t % 4];
    }
    else if (t<32)
    {
        c = RC1[t % 4];
    }
    else if (t<48)
    {
        c = RC2[t % 4];
    }
    else if (t<64)
    {
        c = RC3[t % 4];
    }
    
    if (t > 64 || t < 0) (cout << "RC: something went wrong. c = " + to_string(c));
    //cout << "t: " + to_string(t) + " RC" + to_string(c) << endl;
    return c;
}



uint32 RL (uint32 T, int RC) // shifting being cyclict
{
    uint32 temp = T;
    //cout <<"rotating "; //cout << bitset<32>(T);//cout << " by " + to_string(RC)+ " gives: ";
    T = T << RC;
    temp = temp >> (32-RC);
    T = T | temp;
    //cout << bitset<32>(T); //cout << " - " + to_string(RC) <<endl;
    return T;
}

uint32 RR (uint32 T, int RC) // shifting being cyclict
{
    uint32 temp = T;
    //cout <<"rotating ";//cout << bitset<32>(T);//cout << " by " + to_string(RC)+ " gives: ";

    T = T >> RC;
    temp = temp << (32-RC);
    T = T | temp;
    //cout << bitset<32>(T)<<endl;
    return T;
}


uint32 AC(uint32 t)
{
    uint32 result ;
    double sin_t =abs(sin(t+1));
    double pow_2 = pow(2,32);
    result = floor(sin_t * pow_2);
    return result;
}


void md5_compress( uint32 block [16])//#todo   
{
    uint32 a = ihv [0];
    uint32 b = ihv [1];
    uint32 c = ihv [2];
    uint32 d = ihv [3];

    uint32 help = 0;

    uint32 F;
    uint32 T_;
    uint32 R;
    uint32 AC_t;
    uint32 two_pow32= (uint32) pow(2,32);
    uint32 absin;

    int s = 0;

    uint32 Q[68];
    fill_n(Q,68,0);
    Q[0] = a;
    Q[1] = d;
    Q[2] = c;
    Q[3] = b;
    
    for (int i = 0; i< 16 ; i++) //shows msg block as bin
    {
        if (i>0&&(i%4)==0) cout << " | " << endl;

        cout << " | ";
        cout << bitset<32> (block[i]);
        
    }
    cout << " | " << endl;
    if(false) //show msg block as dec
    { 
        for (int i = 0; i< 16 ; i++)
        {
            if (i>0&&(i%4)==0) cout << " | " << endl;

            cout << " | ";
            cout << to_string(block[i]);
            
        }
        cout << " | " << endl;
    }
    //cout << x.str() << endl;

    for ( int t = 3; t < 67 ; t++)   // t = s + 3
    {   
        AC_t = AC(s);
        F = f_t( Q[t], Q[t-1], Q[t-2], s);
        T_ = F + Q [t-3] + AC_t + W( block, (s));
        R = RL(T_, RC(s)) ;        
        Q[t+1] = Q[t] + R; //altering the state of Q[t+1]
        s++;
    }

    ihv[0] = a + Q[61 + 3];
    ihv[1] = b + Q[64 + 3];
    ihv[2] = c + Q[63 + 3];
    ihv[3] = d + Q[62 + 3];
    
    return;
}


string process( string input) //#todo
{
    cout << "staring" << endl;
    string output;
    uint32 l = input.length();

    string padded_input = pad(input);
    cout << "padding complete" << endl;
    int size = padded_input.size();

    uint32 msg_block [16] ;// N blocks each block contains 32bit uint 16 * 32 = 512
    cout << "calculating first block (16 x 32bit):" << endl;

    for(int j = 0; j < 16; j++) //first run. Block 16*32 = 512
    {
        msg_block[j] = 0;
        for (int i = 0; i < 4; i++)
        {
            msg_block[j] += uint32( (unsigned char) (padded_input.at((j * 4) + i)) << (i*8)) ;
            //cout << (to_string(j*4+i)+ .: " + to_string(msg_block[j] ))+ " vs " + to_string(padded_input.at((j * 4) + i)) << endl;
        }
        //  cout << "translate"+ to_string(msg_block[j]) << endl;    
    }
    //cout << " msg in block vs paded input: " + to_string(msg_block[15]) + " vs " + to_string(padded_input.at(63)) << endl;
    md5_compress(msg_block);


    for (int h = 1; h*64 < size ; h++) //64 *8 = 512, after an iteration we look at the next 64 8bit chars and passing them into a block
    {   
        print_step(h);      
        for(int j = 0; j < 16; j++) //first run. Block 16*32 = 512
        {   
            msg_block[j] = 0;
            for (int i = 0; i < 4; i++)
            {
                msg_block[j] += uint32( (unsigned char) (padded_input.at((h * 64 ) + (j * 4) + i)) << (i*8)) ;
            }     
        }
        md5_compress(msg_block);
    }
    cout << "last block calculated" << endl;
    cout << endl;
    return to_hex(ihv[0]) + to_hex(ihv[1]) + to_hex(ihv[2]) + to_hex(ihv[3]);
}



//MD5 attack implementation

void reverse_md5(uint32 md5 [4] )
{
    uint32 Q[19] = {md5[0], md5[1], md5[2], md5[3]};
    int k;
    uint32 F_t;
    uint32 m [16];
    int s = 3;
    for (int t = 0; t < 16; t++)
    {   
        F_t = f_t( Q[s], Q[s - 1], Q[s - 2], t);
        m[t] = RR(Q[s+1] - Q[s], RC(t)) - F_t - Q[s-3] + AC(t); // Q[t+1] - Q[t] = R_t =? RL(T_t, RC_(t)
        s++;
    }

    return;
}

int collsion_search_algorithm()
{
    srand(std::time(nullptr));
    uint32 M_0 = rand() ^ rand();

    cout << M_0;

    return 0;
}


int find_block_1 ()
{
    // choose Q_1,Q_3,..,Q_16 fullfiliing condidtions
    // calculate m_0,m_6,..,m_15
    
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


void test_RL()
{   
    uint32 test = 17;

    if (RL(test,1) == 34) cout << "RL test 1 succes" << endl;
    if (RL(test,31) == 8 + pow(2,31)) cout << "RL test 2 succes" << endl;
    if (RL(test,32) == 17) cout << "RL test 3 succes" << endl;

    return;
}

void test_to_hex()
{
    uint32 val = 0xF2345678 ; 
    cout << to_string(val) + " " << to_hex(val) << endl;

    return;
}

int main()
{
    string val_stevens = "abc\n";
    string val ="(Fast täglich lesen wir in den Nachrichten von Datenschutz-Skandalen oder Fällen von Datendiebstahl. Heute speichern wir gerne unsere persönlichen Daten in der Cloud.)";
    
    string test = val;
        
    cout << process(test)<< endl;

   // collsion_search_algorithm();

    return 0;
}
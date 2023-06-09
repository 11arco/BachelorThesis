#include <iostream>
#include <string>

#include <bitset>

#include <climits>

#include <ctime>

#include <cmath>
#include <vector>
#include <string.h>

#include <sstream>

using namespace std;
typedef unsigned int uint32; //actually u32 
uint32 ihv_g[4] = {0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476} ;     // (67452301,EFCDAB89,98BADCFE,10325476)
uint32 Q[67];   // core to algorythm and collf.


// can't handle äöü

void show_bits(uint32 * block, int len) // uese carefully
{
    for (int i = 0; i< (len) ; i++) //shows msg block as bin
    {
        if (i > 0 && (i % 4) == 0) cout << " | " << endl;

        cout << " | ";
        cout << bitset<32> (block[i]);
        
    }
    cout << " | " << endl;
}

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

void test_to_hex()
{
    uint32 val = 0xF2345678 ; 
    cout << to_string(val) + " " << to_hex(val) << endl;

    return;
}

void print_step(int step)
{
    string calc = "calculating block ";

    cout << calc + to_string(step )  << " currect IHV: ";

    cout << to_hex(ihv_g[0]) << to_hex(ihv_g[1]) << to_hex( ihv_g[2]) << to_hex( ihv_g[3]) << endl;
}


uint32 shifting_word(uint32 input)
{
    
    int msb_pos =get_msb_pos(input);
    int wordlen = 31;

    input = input<<1;   //shift left with 1
    input +=1;          //set last bit 1 by adding 1

    input = input<< wordlen-(msb_pos +1); // fill max len with 0

    return input; //output
}

string pad(string msg) // pad an imput massage into the correct lgenth + correct format (adds a 1 at the start and length at the end)
{
   // msg+= "\n"; // Wichtig! Endline wir immer mit verabreitet (bei Stevens). Enweder hier, oder vorhermitgeben.
    int l = msg.length();

    int counter = 0;
    bitset<32> x(l);
    int  length_for_adding = l*8 ;

    cout << "input: " + msg  << endl;
    cout << "length: " ;
    cout << l << endl;
    
    char end = 0; //8bit
    char one = 1 << 7; //8bit

    msg += one; // stevens does not do this part of padding
    
    
    while ( (msg.length()*8 % (64*8)) != 448 ) //8bit * 64 = 512 bit // 16 * 32 = 512 // 8 * 56 = 448 // depends on the maybes
    {
        msg = msg + end;
        counter++;
    }

    msg += from_64(length_for_adding);  // stevens does not do this part of padding

    cout << "final:" + msg + "|";
    cout << msg.length() << endl;

    return msg;
}

string pad_stv(string msg) // pad an imput massage into the correct lgenth + correct format (adds a 1 at the start and length at the end)
{
    msg+= "\n"; // Wichtig! Endline wir immer mit verabreitet (bei Stevens). Enweder hier, oder vorhermitgeben.
    int l = msg.length();

    int counter = 0;
    //int  length_for_adding = 0;

    cout << "input: " + msg  << endl;
    cout << "length: " ;
    cout << l << endl;
    
    char end = 0; //8bit
    char one = 1 << 7; //8bit

    //msg += one; // stevens does not do this part of padding
    
    
    while ( ((msg.length()*8 % (64*8)) != 0) || (counter  == 0 )) //8bit * 64 = 512 bit // 16 * 32 = 512 // 8 * 56 = 448 // depends on the maybes
    {
        msg = msg + end;
        counter++;
    }

    //msg += from_64(length_for_adding);  // stevens does not do this part of padding

    cout << "stv: " <<"final:" + msg + "|";
    cout << msg.length() << endl;

    return msg;
}




uint32 f_t(uint32 X, uint32 Y, uint32 Z, int t ) 
{ 
    uint32 out;

    if (t < 16)
    {
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
    if (t<0 || t>63) cout << "f_t faild. t not in scope:" + to_string(t) << endl;
    return out; // something went wrong
}

uint32 W ( uint32 m [16], int t) // 16 blocks each 32bit uints
{ 
        if (t <= -1 || t >= 64)
        {
             cout << "something went wrong in W. t is:" + to_string(t)<< endl;
            return -1;     
        }

    //int pos=-1;
    /*
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
    */

    //easy speed up
    int t_pos[64] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
                     1,6,11,0,5,10,15,4,9,14,3,8,13,2,7,12,
                     5,8,11,14,1,4,7,10,13,0,3,6,9,12,15,2,
                     0,7,14,5,12,3,10,1,8,15,6,13,4,11,2,9};



    return m[t_pos[t]]; 

}

uint32 RC (int t) // return the roataion constat
{
    uint32 c ;
    uint32 RC0[4] ={7,12,17,22};
    uint32 RC1[4] ={5,9,14,20};
    uint32 RC2[4] ={4,11,16,23};
    uint32 RC3[4] ={6,10,15,21};
    if (t > 64 || t < 0) (cout << " RC: something went wrong. t = " << t << endl);

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
    
    return c;
}



uint32 RL (uint32 T, unsigned int RC) // shifting being cyclict
{
    uint32 temp = T;
    T = T << RC;
    temp = temp >> (32-RC);
    return T | temp;
}

void test_RL()
{   
    uint32 test = 17;

    if (RL(test,1) == 34) cout << "RL test 1 succes" << endl;
    if (RL(test,31) == 8 + pow(2,31)) cout << "RL test 2 succes" << endl;
    if (RL(test,32) == 17) cout << "RL test 3 succes" << endl;

    return;
}

uint32 RR (uint32 T, unsigned int RC) // shifting being cyclict
{
    uint32 temp = T;

    T = T >> RC;
    temp = temp << (32-RC);
    return  T | temp;
}


uint32 AC(uint32 t)
{
    uint32 result ;
    double sin_t = abs(sin(t+1));
    result = floor(sin_t * (0x100000000));

    return result;
}



uint32 reverse_md5(uint32 block [16], uint32 t, uint32 AC, uint32 RC )
{  
    uint32 offset = 3;

    uint32 help = Q[offset + t + 1] - Q[offset + t];
	help = RR(help, RC) - f_t(Q[offset + t], Q[offset + t - 1], Q[offset + t - 2], t) - Q[offset + t - 3] - AC ;

    return help;
}


uint32 precise_step_foward(uint32 t, uint32 r, uint32 b, uint32 c, uint32 d, uint32 w_t,uint32 ac, uint32 rc )
{
    r += f_t(b, c, d, t) + ac + w_t;
    r = RL(r, rc) + b;

    return r;
}
         
uint32 step_foward( uint32 t, uint32 w_t) //if Q[] global => less 
{
    uint32 R = Q[t - 3];
    uint32 offset = 3;

    R = precise_step_foward(t - offset,R, Q[t - 0], Q[t - 1], Q[t - 2], w_t, AC(t - offset), RC(t - offset));

    return  R; //altering the state of Q[t+1]
}


void md5_compress_f( uint32 block [16],uint32 IHV[4])   
{
    //uint32cout << "compress" << endl;
    uint32 a = IHV [0];
    uint32 b = IHV [1];
    uint32 c = IHV [2];
    uint32 d = IHV [3];

    fill_n(Q,67,0);
    Q[0] = a; // !
    Q[1] = d; // !
    Q[2] = c; // !
    Q[3] = b; // !
    for ( int t = 3; t < 67; t++)  
    {   
        Q[t + 1] = step_foward(t, W(block, t-3 ));
    }
    IHV[0] += Q[3 + 61 ];
    IHV[1] += Q[3 + 64 ];
    IHV[2] += Q[3 + 63 ];
    IHV[3] += Q[3 + 62 ];
 
    return ;
}

void md5_compress (uint32 block [16])
{
    md5_compress_f(block,ihv_g);
    std::cout << to_hex(ihv_g[0]) << " ";
    std::cout << to_hex(ihv_g[1]) << " ";
    std::cout << to_hex(ihv_g[2]) << " ";
    std::cout << to_hex(ihv_g[3]) << std::endl;

    return ;
}


string process( string input)
{
    cout << "starting" << endl;
    string output;
    uint32 l = input.length();
    bool stv = true; // enable padding like it is in Steven's code
    string padded_input;
    if(stv)
    {
        padded_input = pad_stv(input);
    } 
    else 
    {        
        padded_input = pad(input);

    }

    int size = padded_input.size();

    uint32 msg_block [16] ;// N blocks each block contains 32bit uint 16 * 32 = 512

    for (int h = 0; h*64 < size ; h++) //64 *8 = 512, after an iteration we look at the next 64 8bit chars and passing them into a block
    {   
        print_step(h);      
        for(int j = 0; j < 16; j++) //first run. Block 16*32 = 512
        {   
            msg_block[j] = 0;
            for (int i = 0; i < 4; i++)
            {
                msg_block[j] += uint32( (unsigned char) (padded_input.at((h * 64 ) + (j * 4) + i)) << (i * 8)) ;
            }     
        }
        show_bits(msg_block,16);
        md5_compress(msg_block);
    }
    cout << "last block calculated" << endl;
    cout << endl;

    return to_hex(ihv_g[0]) + to_hex(ihv_g[1]) + to_hex(ihv_g[2]) + to_hex(ihv_g[3]);
}

uint32* to_block( string input, uint32 msg_block [16]) //only for msgs. with len < 17
{
    string output;
    uint32 l = input.length();
    string padded_input = pad(input);
    int size = padded_input.size();

    //uint32 msg_block [16] ;// N blocks each block contains 32bit uint 16 * 32 = 512

    for (int h = 0; h*64 < size ; h++) //64 *8 = 512, after an iteration we look at the next 64 8bit chars and passing them into a block
    {   
        print_step(h);      
        for(int j = 0; j < 16; j++) //first run. Block 16*32 = 512
        {   
            msg_block[j] = 0;
            for (int i = 0; i < 4; i++)
            {
                msg_block[j] += uint32( (unsigned char) (padded_input.at((h * 64 ) + (j * 4) + i)) << (i * 8)) ;
            }     
        }
    }
    
    //cout << to_hex(ihv[0]) + to_hex(ihv[1]) + to_hex(ihv[2]) + to_hex(ihv[3]) << endl;
    return msg_block;
}

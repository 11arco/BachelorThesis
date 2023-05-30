#include <iostream>
#include <string>

#include <bitset>

#include <climits>
#include <sstream>
#include <string.h>

<<<<<<< HEAD:cpp/test.cpp
#include <thread>

=======
>>>>>>> 54122fed10d9c71b6e354036624392af6cd738fa:cpp/MD5.cpp
#include <ctime>

#include <cmath>

using namespace std;
typedef unsigned int uint32; //actually u32 

uint32 ihv[4] = {0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476} ;     // (67452301,EFCDAB89,98BADCFE,10325476)
uint32 Q[68];   // core to algorythm and collf.

void show_bits(uint32 block [16])
{
    for (int i = 0; i< 16 ; i++) //shows msg block as bin
    {
        if (i>0&&(i%4)==0) cout << " | " << endl;

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

    cout << calc + to_string(step + 1)  << " currect IHV: ";

    cout << to_hex(ihv[0]) + to_hex(ihv[1]) + to_hex(ihv[2]) + to_hex(ihv[3]) << endl;
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

    if (pos <= -1 || pos >= 16) cout << "something went wrong un W. t is:" + to_string(t)<< endl;

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
    return c;
}



uint32 RL (uint32 T, int RC) // shifting being cyclict
{
    uint32 temp = T;
    T = T << RC;
    temp = temp >> (32-RC);
    T = T | temp;

    return T;
}

void test_RL()
{   
    uint32 test = 17;

    if (RL(test,1) == 34) cout << "RL test 1 succes" << endl;
    if (RL(test,31) == 8 + pow(2,31)) cout << "RL test 2 succes" << endl;
    if (RL(test,32) == 17) cout << "RL test 3 succes" << endl;

    return;
}

uint32 RR (uint32 T, int RC) // shifting being cyclict
{
    uint32 temp = T;

    T = T >> RC;
    temp = temp << (32-RC);
    T = T | temp;

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



void reverse_md5(uint32 block [16], uint32 t, uint32 AC, uint32 RC )
{  
    uint32 offset = 3;

    block[t] = Q[offset + t + 1] - Q[offset + t];
	block[t] = RR(block[t], RC) - f_t(Q[offset + t], Q[offset + t - 1], Q[offset + t - 2], t) - Q[offset + t - 3] - AC ;
}




uint32 step_foward( uint32 t, uint32 W_t) //if Q[] gloab => less 
{
    uint32 F;
    uint32 T_;
    uint32 R;
    uint32 AC_t;
    uint32 two_pow32 = (uint32) 4294967296;
    uint32 absin;
    uint32 offset = 3;

    AC_t = AC(t - offset );
    F = f_t( Q[t], Q[t - 1], Q[t - 2], (t - offset));
    T_ = F + Q[t - 3] + AC_t + W_t;
    R = RL(T_, RC(t - offset)) ;        

    if(t<10) cout<<" ";                     //debug
    cout << " R_" + to_string(t) + ": ";    //debug
    cout << bitset<32>(R) << endl;          //debug

    return  R + (uint32) Q[t]; //altering the state of Q[t+1]

}

void md5_compress( uint32 block [16])   
{
    uint32 a = ihv [0];
    uint32 b = ihv [1];
    uint32 c = ihv [2];
    uint32 d = ihv [3];

    uint32 help = 0;

    fill_n(Q,68,0);
    Q[0] = a;
    Q[1] = d;
    Q[2] = c;
    Q[3] = b;

    for ( int t = 3; t < 67; t++)  
    {   
        Q[t+1] = step_foward((t ),W(block, t-3 ));

        if(t<9) cout<<" ";                         //debug
        cout << "Q[" + to_string(t) + "]: ";    //debug
        cout << bitset<32>(Q[t]) << endl;       //debug
        if(t<9) cout<<" ";                         //debug
        cout << "Q[" + to_string(t + 1) + "]: ";    //debug
        cout << bitset<32>(Q[t + 1]) << endl;       //debug
        cout << endl;
    }
    ihv[0] = a + Q[61 + 3];
    ihv[1] = b + Q[64 + 3];
    ihv[2] = c + Q[63 + 3];
    ihv[3] = d + Q[62 + 3];
    
    return;
}


string process( string input)
{
    cout << "starting" << endl;
    string output;
    uint32 l = input.length();

    string padded_input = pad(input);
    cout << "padding complete" << endl;
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
        show_bits(msg_block);
        md5_compress(msg_block);
    }
    cout << "last block calculated" << endl;
    cout << endl;

    return to_hex(ihv[0]) + to_hex(ihv[1]) + to_hex(ihv[2]) + to_hex(ihv[3]);
}


int main()
{
    string val_stevens = "abc\n";
    string val ="(Fast täglich lesen wir in den Nachrichten von Datenschutz-Skandalen oder Fällen von Datendiebstahl. Heute speichern wir gerne unsere persönlichen Daten in der Cloud.)";
    string val_2 ="Every day millions of people rely on our free all-in-one privacy solution. The DuckDuckGo app includes our Private Search, Web Tracking Protection, Smarter Encryption, Email Protection, Android App Tracking Protection, and more.";
    string test = val;
    cout << "Please enter a string" << endl;
    getline(cin, test); 
    
    if ( test == "r") cout << process(to_string(rand() * 3.2)  )<< endl;
    else cout << process(test)<< endl;

   // collsion_search_algorithm();

    return 0;
}
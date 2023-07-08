#include <iostream>
#include <string>

#include <bitset>

#include <climits>

#include <ctime>

#include <cmath>
#include <vector>
#include <string.h>

#include <sstream>


typedef unsigned int uint32; //actually u32 

/* rng seeds
	seed32_1 = uint32(time(NULL));
	seed32_2 = 0x12345678;
 */

uint32 x32 = 314159265;
//uint32 x32 = 123456789;
uint32 randX() //for 32 bits
{
    //x32 = 0xf000f000; //11110000000000001111000000000000
    x32 ^= x32 << 13;
    x32 ^= x32 >> 17;
    x32 ^= x32 << 5;

    return x32 ;
}
#include <iostream>
#include <string>

#include <bitset>
#include <string.h>

#include <bitset>

#include <cmath>

#include "MD5.cpp"


int collsion_search_algorithm()
{
    srand(std::time(nullptr));
    uint32 M_0 = rand() ^ rand();

    cout << M_0;

    return 0;
}


uint32* find_block0(uint32 block [16], uint32 IHV[4] ) // MD5 is the IV or IHV, the names are not correct yet
{
    /* 
    * Stevens works a lot with voids, which overwrites varibale on an upper, or even global level.
    * This causes confusion, since we do not know which vale is alterd.
    * There could be multiple values which get alterd.
    * We try to actually return the value we work with for clarity.
    *
    * Here, we get two empty uint32-array handed.
    * Since we don't want to return local adreses, but also do not want to alter global values,
    * we fill the first after we found the first 'block', with this 'block'.
    * 
    * There is a confusion with the block numbering in the paper and the code.
    * In the code the block are number from 0 to 1.
    * In the paper the numbering starts with 1 and end with 2
    * 
    */

    uint32 offset = 3; //offset is 3 because the "last" pos for calculation is -3 (+3 = 0) 
    uint32 Q [68] = {IHV[0], IHV[1], IHV[2], IHV[3]};
    srand(std::time(nullptr));

    uint32 word = rand();
    bool conds [4] = {false, false ,false, false}; //21 - 17 = 4 conditions (not final)
    bool conds_2 [63];
    fill_n(conds_2, 63, false); // maybe conds[0] = conds[1] & conds[2] & ... & conds[64] => size = 63 + 1 = 64  

    while(true)
    {
        // choose Q_1 ...
        // clac m0 ...

        /* 
        * Bsp für Stevens Werte für Q_t mit t = 3 
        *   01010101 00110011 00001111 00000000  some Example
        * & 11111110 10000111 10111100 00111111  0xfe87bc3f
        * _________________________________________________  
        *   01010100 00000011 00001100 00000000  zwischen Ergebniss	
        * | 00000001 01111000 01000001 11000000  0x017841c0
        * _________________________________________________
        * = 01010101 01111011 01001101 11000000  Ergebniss
        *   .......1 .1111... .1....01 11......  new bit conds für t = 3
        *   ........ ....0... ....0... .0......  old bit conds für t = 3
        * 
        *   00000000 00001111 00110011 01010101  some other Example
        * & 11111110 10000111 10111100 00111111  0xfe87bc3f
        * _________________________________________________  
        *   00000000 00000111 00110000 00010101  zwischen Ergebniss	
        * | 00000001 01111000 01000001 11000000  0x017841c0
        * _________________________________________________
        * = 00000001 01111111 01110001 11010101  Ergebniss
        *   ........ ....0... ....0... .0......  bit conds für t = 3
        *   .......1 .1111... .1....01 11...... new bit conds
        *
        * => matches with the new bit conds
        *    Stevens is building a u32 with the bits that has to be one and or it with the val after he ands the oposite value:
        *    & 11111110 10000111 10111100 00111111  0xfe87bc3f
        *    | 00000001 01111000 01000001 11000000  0x017841c0
        * 
        *   the & flips the 0 correct, the | flips the 1 correct
        * 
        */


       // cant do lots
        Q[offset + 1] = rand(); //?
        Q[offset + 2]; //?
        Q[offset + 3] = ( rand() & 0xfe98bc3f ) | 0x017841c0; // 0xfe98bc3f=11111110 10000111 10111100 00111111, 0x017841c0=00000001 01111000 01000001 11000000
        Q[offset + 4] = ( rand() & 0x44000033 ) | 0x000002c0 | ( Q[offset + 3] & 0x0287bc00 );  // 
        Q[offset + 5] = 0x41fffc8 | ( Q[offset + 4] & 0x04000033 );
        Q[offset + 6] = 0xb84b82d6;
        Q[offset + 7] = ( rand() & 0x68000084 ) | 0x02401b43;;
        Q[offset + 8] = ( rand() & 0x2b8f6e04) | 0x005090d3 | (~Q[offset + 7] & 0x40000000 );
        Q[offset + 9] = 0x20040068 | ( Q[offset + 8] & 0x00020000 ) | ( ~Q[offset + 8] & 0x40000000 );
        Q[offset + 10]= ( rand() & 0x40000000 ) | 0x1040b089;
        Q[offset + 11]= ( rand() & 0x10408008 ) | 0x0fbb7f16 | ( ~Q[offset + 10] & 0x40000000 );
        Q[offset + 12]= ( rand() & 0x1ed9df7f ) | 0x00022080 | ( ~Q[offset + 11] & 0x40200000 );
        Q[offset + 13]= ( rand() & 0x5efb4f77 ) | 0x20049008; 
        Q[offset + 14]= ( rand() & 0x1fff5f77 ) | 0x0000a088 | ( ~Q[offset + 13] & 0x40000000 );
        Q[offset + 15]= ( rand() & 0x5efe7ff7 ) | 0x80008000 | ( ~Q[offset + 14] & 0x00010000 );
        Q[offset + 16]= ( rand() & 0x1ffdffff ) | 0xa0000000 | ( ~Q[offset + 15] & 0x40020000 );

        while(!conds[0] && !conds[1] && !conds[2] && !conds[3]) // as long as we do nor fulfil all bitconds for Q_17 - Q_21
        {
            // choos Q_17
            // calc m_1 at t = 16 with reversestep pp.
            // calc Q_2 => m_2,m_3,m_4,m_5
            // calc Q_18, .. Q_64 ( Q_64 is at pos Q[64 + offset] = 67, since Q_1 is at Q[0 ... offset ] is the IHV ,the MD5 input )

        }

        while (conds_2[0] ==  true)
        {
            // use tunnels
            // calc m_8, ...
            // clac Q_22, ...
            // Verify conds
        }

    }
    return block;
}



void find_coll(uint32 md5[4]) // MD5 is the IV or IHV, the names are not correct yet
{

    // find block 0
    //  -compress
    // find blcok 1
    //  -compress
    // manipulate





}
    // determine bitconditions ( q_i )|_ i = -3 ^ 0
    // generate partial lower diff path 
    // generate partial upper diff path
    // connet both diff paths
    

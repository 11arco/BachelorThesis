#include <iostream>
#include <string>

#include <bitset>
#include <string.h>

#include <bitset>

#include <cmath>

#include "MD5.cpp"



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
    

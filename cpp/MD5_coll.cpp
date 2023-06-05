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


        // The first 16 Qs can be choosen abretary, as long as we fullfill the conditions.
        // Stevens does this by generating really good ramdom values.
        // After this he alters the random values so thy fullfill conditions.
        // We follow Stevens appoache but genereate "normal" random vlaues and alter these so they fullfill the condidtions
        // Improvement for rng is possible.
        Q[offset + 1] = rand(); //?
        Q[offset + 2]; //?
        Q[offset + 3] = ( rand() & 0xfe98bc3f ) | 0x017841c0; // 0xfe98bc3f=11111110 10000111 10111100 00111111, 0x017841c0=00000001 01111000 01000001 11000000
        Q[offset + 4] = ( rand() & 0x44000033 ) | 0x000002c0 | ( Q[offset + 3] & 0x0287bc00 );  // 
        Q[offset + 5] = 0x41fffc8 | ( Q[offset + 4] & 0x04000033 );
        Q[offset + 6] = 0xb84b82d6;
        Q[offset + 7] = ( rand() & 0x68000084 ) | 0x02401b43;
        Q[offset + 8] = ( rand() & 0x2b8f6e04) | 0x005090d3 | (~Q[offset + 7] & 0x40000000 );
        Q[offset + 9] = 0x20040068 | ( Q[offset + 8] & 0x00020000 ) | ( ~Q[offset + 8] & 0x40000000 );
        Q[offset + 10]= ( rand() & 0x40000000 ) | 0x1040b089;
        Q[offset + 11]= ( rand() & 0x10408008 ) | 0x0fbb7f16 | ( ~Q[offset + 10] & 0x40000000 );
        Q[offset + 12]= ( rand() & 0x1ed9df7f ) | 0x00022080 | ( ~Q[offset + 11] & 0x40200000 );
        Q[offset + 13]= ( rand() & 0x5efb4f77 ) | 0x20049008; 
        Q[offset + 14]= ( rand() & 0x1fff5f77 ) | 0x0000a088 | ( ~Q[offset + 13] & 0x40000000 );
        Q[offset + 15]= ( rand() & 0x5efe7ff7 ) | 0x80008000 | ( ~Q[offset + 14] & 0x00010000 );
        Q[offset + 16]= ( rand() & 0x1ffdffff ) | 0xa0000000 | ( ~Q[offset + 15] & 0x40020000 );

        // we now claculate the massage m for t =0,6,...,15. For this we use the reverse md5 fuction.
		reverse_md5(block, 0, AC(0), RC(0)); // reverse_md5 is a void altering the input: block
		reverse_md5(block, 6, AC(6), RC(6)); // stevens hard codes the AC and the RC
	    reverse_md5(block, 7, AC(7), RC(7));
		reverse_md5(block, 11, AC(11), RC(11)); 
		reverse_md5(block, 14, AC(14), RC(14)); 
		reverse_md5(block, 15, AC(15), RC(15)); 

        //preparation for next calculations
        uint32 q_1;
        uint32 q_2;
        uint32 q_17;
        uint32 q_18;
        uint32 q_19;
        uint32 q_20;
        uint32 q_21;

        uint32 m_0;
        uint32 m_1;

        bool progress = false;

        while (!progress) // as long as we do nor fulfil all bitconds for Q_17 - Q_21
        {
            // we try to pick a Q_17 that Q18 ..Q21 can be calculated with Q17 and fulfill their conidtions
            // we can't use stepforward since we havent past the vlaues in Q[t]
            q_1 = (0x02020801 | (Q[offset + 0] & 0x80000000) ) | (rand() & 0x7dfdf7be);
            m_1 = Q[offset + 2] - q_1;
            q_17 = f_t(Q[offset + 16], Q[offset + 15], Q[offset + 14], 16) + Q[offset +13] + AC(16)+ m_1;
            q_17 = RL(Q[17], RC(17));
            q_17 += Q[offset + 16];
            if(true);
            q_18 = f_t(q_17, Q[offset + 16], Q[offset + 15], 17) + Q[offset + 14] + AC(17) + block[6];
            q_18 = RL(q_18, RC(18));
            q_18 += q_17; 
    	    if (0x00020000 != ((q_18 ^ q_17) & 0xa0020000)){ // nesting probably better than: while (true) - maybe not ..
                q_19 = f_t( Q[offset + 18], Q[offset + 17],Q[offset + 16], 18) + Q[offset + 15] + AC(18) +  block[11];
                q_19 = RL(q_19, RC(19));
                q_19 += q_18;
                if (0x80000000 != (q_19 & 0x80020000)){
                    m_0 = Q[1 + offset] -Q[0 + offset];
                    m_0 = RR(m_0, RC(0)) - f_t(Q[0 + offset],Q[offset - 1], Q[ offset - 2], 0) + Q[offset - 3] + AC(0);
                    q_20 = f_t(q_19, q_18,q_17,19) + Q[16 + offset] + AC(19) + m_0;
                    q_20 = RL(q_20,RC(20));
                    q_20 += q_19;
                    if (0x00040000 != ((q_20^q_19) & 0x80040000)){
                        progress = true; // go on
                    }                    
                }
            }      
            if(progress)
            {
                block[1] = q_17 - Q[16]; 
                block[1] = RR(block[1], AC(1)); 
                block[1] -=f_t(Q[offset + 16], Q[offset + 15], Q[offset + 14], 16) + Q[offset + 13] + AC(16); 
                q_2 = block[1] + f_t(Q[offset + 1], Q[offset + 0], Q[offset - 1], 1) + Q[offset - 2] + AC(1); 
                q_2 = RL(q_2, RC(2)); 
                q_2 += Q[offset + 1];
                block[5] = RR(Q[offset + 6] - Q[offset + 5], 12) - f_t(Q[offset + 5], Q[offset + 4], Q[offset + 3],5) - AC(5) - q_2;

                Q[2 + offset] = q_2;
                Q[17 + offset] = q_17;
                Q[18 + offset] = q_18;
                Q[19 + offset] = q_19;
                Q[20 + offset] = q_20;

                reverse_md5(block, 2, AC(2), RC(2));
            
                // block m_1
                /*block[0] = m_0;
                *block[1] = m_1;
                *
                *reverse_md5(block,5,AC(5),RC(5));
                // calc Q_21
                *reverse_md5(block, 5, AC(5),RC(5));
                *q_21 = f_t(q_20, q_19,q_18,20) + Q[17 + offset] + AC(20) + block[5];
                *q_21 = RL(21,RC(21));
                *q_21 += q_20;
                *if (0 != ((q_21 ^ Q[offset + 20]) & 0x80020000)){
                *    Q[offset + 21] = q_21;	
                *}
                *else
                *{
                *    progress = false;
                *}
                */
            }
        }
        //preparation for next calculations

        while (true)
        {
            // calc m_8, ..., m_13
            // clac Q_22, ..., Q_64
            // Verify conds on Q_22, .., Q_64, T_22, T_34 and IHV-conds for netxt block
            // stop if aöö conds are sat and a near coll. is verified
        }

    }
    return block;
}



void find_coll(uint32 md5[4]) // MD5 is the IV or IHV, the names are not correct yet
{   
    uint32 block_0[16];
    uint32 block_1[16];


    find_block0(block_0, ihv);
    //  -compress
    // find blcok 1
    //  -compress
    // manipulate





}
    // determine bitconditions ( q_i )|_ i = -3 ^ 0
    // generate partial lower diff path 
    // generate partial upper diff path
    // connet both diff paths
    

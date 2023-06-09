#include <iostream>
#include <string>

#include <bitset>
#include <string.h>

#include <bitset>

#include <cmath>
#include <vector>

#include "MD5.cpp"


int collsion_search_algorithm()
{
    srand(std::time(nullptr));
    uint32 M_0 = rand() ^ rand();

    cout << M_0;

    return 0;
}
uint32* find_block1_Wang(uint32 block[16], uint32 IHV[4] )
{
    bool progress = false;
    uint32 offset = 3; //offset is 3 because the "last" pos for calculation is -3 (+3 = 0) 
    uint32 Q [68] = {IHV[0], IHV[1], IHV[2], IHV[3]};

	vector<uint32> q9mask2(1<<10);
    for (unsigned k = 0; k < q9mask2.size(); ++k)
    {
		q9mask2[k] = ((k<<1) ^ (k<<7) ^ (k<<14) ^ (k<<15) ^ (k<<22)) & 0x6074041c;
    }
    while (true) //meh
	{
		uint32 aa = Q[offset] & 0x80000000;
		uint32 bb = 0x80000000 ^ aa;

		Q[offset + 2] = (rand() & 0x71de7799) | 0x0c008840 | bb;
		Q[offset + 3] = (rand() & 0x01c06601) | 0x3e1f0966 | (Q[offset + 2] & 0x80000018);
		Q[offset + 4] = 0x3a040010 | (Q[offset + 3] & 0x80000601);
		Q[offset + 5] = (rand() & 0x03c0e000) | 0x482f0e50 | aa;
		Q[offset + 6] = (rand() & 0x600c0000) | 0x05e2ec56 | aa;
		Q[offset + 7] = (rand() & 0x604c203e) | 0x16819e01 | bb | (Q[offset + 6] & 0x01000000);
		Q[offset + 8] = (rand() & 0x604c7c1c) | 0x043283e0 | (Q[offset + 7] & 0x80000002);
		Q[offset + 9] =  (rand() & 0x00002800) | 0x1c0101c1 | (Q[offset + 8] & 0x80001000);
		Q[offset + 10] = 0x078bcbc0 | bb;
		Q[offset + 11] = (rand() & 0x07800000) | 0x607dc7df | bb;
		Q[offset + 12] = (rand() & 0x00f00f7f) | 0x00081080 | (Q[offset + 11] & 0xe7000000);
		Q[offset + 13] = (rand() & 0x00701f77) | 0x3f0fe008 | aa;
		Q[offset + 14] = (rand() & 0x00701f77) | 0x408be088 | aa;
		Q[offset + 15] = (rand() & 0x00ff3ff7) | 0x7d000000;
		Q[offset + 16] = (rand() & 0x4ffdffff) | 0x20000000 | (~Q[offset + 15] & 0x00020000);
	    
		reverse_md5(block,5, AC(5), RC(5));
		reverse_md5(block,6, AC(6), RC(6));
		reverse_md5(block,7, AC(22), RC(7));
		reverse_md5(block,11, AC(11), RC(11));
		reverse_md5(block,14, AC(14), RC(14));
		reverse_md5(block,15, AC(15), RC(15));

        // prerparing next values for active work
        uint32 q_1;
        uint32 q_2;
        uint32 q_17;
        uint32 q_18;
        uint32 q_19;
        uint32 q_20;
        uint32 q_21;

        uint32 m_0;
        uint32 m_1;

        while(!progress)
        {
            // we try to pick a Q_17 that Q18 ..Q21 can be calculated with Q17 and fulfill their conidtions
            // we can't use stepforward since we havent past the vlaues in Q[t]
            q_1 = (0x01c0e71f | (Q[offset + 2] & 0xf01e1080) ) | (rand() & 0x01c0e71f); 
            m_1 = Q[offset + 2] - q_1;
            m_1 = RR(m_1,RC(1)) - f_t(Q[offset + 1], Q[offset + 0], Q[offset -1], 1) - Q[offset -2] + AC(1);

            q_17 = f_t(Q[offset + 16], Q[offset + 15], Q[offset + 14], 16) + Q[offset +13] + AC(16)+ m_1;
            q_17 = RL(Q[17], RC(17));
            q_17 += Q[offset + 16];
            if (0x40000000 != ((q_17 ^ Q[16]) & 0xc0008008)) 
            {// nesting probably better than: while (true) - maybe not ..
                if (0 != (q_17 & 0x00020000)) 
                {
                    q_18 = f_t( Q[offset + 17], Q[offset + 16],Q[offset + 15], 17) + Q[offset + 14] + AC(17) +  block[6];
                    q_18 = RL(q_18, RC(18));
                    q_18 += q_17;

                    if (0x00020000 != ((q_18 ^ q_17) & 0xa0020000))
                    { 
                        q_19 = f_t( Q[offset + 18], Q[offset + 17],Q[offset + 16], 18) + Q[offset + 15] + AC(18) +  block[11];
                        q_19 = RL(q_19, RC(19));
                        q_19 += q_18;
                        if (0 != (q_19 & 0x80020000))
                        {
                            m_0 = Q[1 + offset] -Q[0 + offset];
                            m_0 = RR(m_0, RC(0)) - f_t(Q[0 + offset],Q[offset - 1], Q[ offset - 2], 0) + Q[offset - 3] + AC(0);
                            q_20 = f_t(q_19, q_18,q_17,19) + Q[16 + offset] + AC(19) + m_0;
                            q_20 = RL(q_20,RC(20));
                            q_20 += q_19;
                            if (0x00040000 != ((q_20^q_19) & 0x80040000))
                            {
                                progress = true; // go on
                            }                   
                        }
                    }
                }
            }

            if (progress) // assures this only happens in the last iteration
            {
                Q[2 + offset] = q_2;
                Q[17 + offset] = q_17;
                Q[18 + offset] = q_18;
                Q[19 + offset] = q_19;
                Q[20 + offset] = q_20;
                
                block[0] = m_0;
                block[1] = m_1;
                reverse_md5(block, 2, AC(2),RC(1));
            }
        }        
    
        reverse_md5(block, 2, AC(2), RC(2));
        // prerparing next values for active work
        uint32 q_4 = Q[4];
        uint32 q_9 = Q[9];
        uint32 q_10 = Q[10];
        uint32 m_10;

        progress = false; // recycling the progress value from the it before

        // In original equivalent to the following code we Steven's works a lot wit htemp and helper values to minimize the amount of calcuations (I guess), but it also helps to keep an overvie
        // so we try to find a good midling way, which is quite compact but also understandable without messing up the entire performance.
        // Some easy understandable helper values were mentioned before (e.g. q_4). Without these the clacuation wouldn't be possible or wrong results were givien as an output.
        // The following helper values are just only to help in case of overview, perfornace and clarity in code.
        // Stevens calls the helper Values like aa, bb, tt9, etc. This is hard to read.
        // We may call the values by there function, at least we try.
        uint32 help_cond_Q_21 = Q[offset + 21]; // aa
        uint32 help_cond_Q_20 = Q[offset + 20]; // bb
        uint32 help_cond_M_10 = f_t(Q[offset + 21], Q[offset + 20], Q[offset + 19],21) + Q[offset + 18] + AC(21); // dd // m_10 somewhere between Q_22 and Q_21
        help_cond_M_10 = RL(help_cond_M_10, RC(21)) + help_cond_Q_21; // for t = 21 
        uint32 help_cond_Q_22;//cc
        uint32 t;

        while(progress)
        {   
            if (0 != (help_cond_M_10 & 0x80000000))
            {
                help_cond_Q_22 = Q[offset + 19] + AC(22) + block[15] + f_t(help_cond_M_10, help_cond_Q_21, help_cond_Q_20, 22); // t = 22 //  cc

                if (0 != (help_cond_Q_22 & 0x20000)) 
                {
                    help_cond_Q_22 = RL(help_cond_Q_22, RC(22)) + help_cond_M_10;
                    if (0 != (help_cond_Q_22 & 0x80000000)) 
                    {
                        help_cond_Q_20 = Q[offset + 20] + AC(23) + block[4] + f_t(help_cond_Q_22, help_cond_M_10, help_cond_Q_21, 22); 
                        help_cond_Q_20 = RL(help_cond_Q_20, 20) + help_cond_Q_22;
                        if (0 == (help_cond_Q_20 & 0x80000000)) 
                        {
                            block[10] = m_10;
                            Q[offset + 9] = q_9;
                            Q[offset + 10] = q_10;
                            reverse_md5(block,13, AC(13), RC(13));

                        }
                        for (uint32 k9 = 0; k9 < 1024;)
				        {       
                            uint32 IV[4] ={help_cond_Q_21,help_cond_Q_20,help_cond_Q_22,help_cond_M_10};
                            Q[9] = q_9 ^ q9mask2[k9] ;
                            k9++;
                            reverse_md5(block, 8, AC(8), RC(80));
                            reverse_md5(block, 9, AC(9), RC(9));
                            reverse_md5(block, 12, AC(12), RC(12));

                            //doing steps for t \in {24,...,33}
                            t =24;
                            precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                            t =25;
                            precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                            t =26;
                            precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                            t =27;
                            precise_step_foward(t,IV[1],IV[2],IV[3],IV[4],W(block,t),AC(t),RC(t));
                            t =28;
                            precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                            t =29;
                            precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                            t =30;
                            precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                            t =31;
                            precise_step_foward(t,IV[1],IV[2],IV[3],IV[4],W(block,t),AC(t),RC(t));
                            t =32;
                            precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                            t =33;
                            precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));

                            IV[2] += f_t(IV[3],IV[0],IV[1],34); //t = 34

                            if((IV[2] & (1 << 15)) == 0) // if bit at pos 15 is zero
                            {
                                IV[2] = RL(IV[2],16) + IV[3];

                                //doing steps for t \in {35,...,47}
                                t =35;
                                precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));
                                t =36;
                                precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                                t =37;
                                precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                                t =38;
                                precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                                t =39;
                                precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));
                                t =40;
                                precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                                t =41;
                                precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                                t =42;
                                precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                                t =43;
                                precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));
                                t =44;
                                precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                                t =45;
                                precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                                t =46;
                                precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                                t =47;
                                precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));

                                if((IV[1] ^IV[3]) & 0x80000000 != 1)//checks wether IV[1] or IV[3] is 1 at pos.s of 0x80000000 
                                {   // dunno how to sove this nicely just copyed it .. for pregress
                                    // for t = 48 ... 63
                                    t = 48;
                                    precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[0] ^ IV[3]) >> 31)) continue;
                                    t = 49;
                                    precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));

                                    if (0 == ((IV[1] ^ IV[3]) >> 31)) continue;
                                    t = 50;
                                    precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[0] ^ IV[3]) >> 31)) continue;
                                    t = 51;
                                    precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[1] ^ IV[3]) >> 31)) continue;
                                    t = 52;
                                    precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[0] ^ IV[2]) >> 31)) continue;
                                    t = 53;
                                    precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[1]^IV[3]) >> 31)) continue;
                                    t = 54;
                                    precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
    
                                    if (0 != ((IV[0] ^ IV[2]) >> 31)) continue;
                                    t = 55;
                                    precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[1] ^ IV[3]) >> 31)) continue;
                                    t = 56;
                                    precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[0] ^ IV[2]) >> 31)) continue;
                                    t = 57;
                                    precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[1] ^ IV[3]) >> 31)) continue;
                                    t = 58;
                                    precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[0] ^ IV[2]) >> 31)) continue;
                                    t = 59;
                                    precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));

                                    if (0 == ((IV[1] ^ IV[3]) >> 31)) continue;
                                    t = 60;
                                    precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[0] ^ IV[2]) >> 31)) continue;
                                    t = 61;
                                    precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[1] ^ IV[3]) >> 31)) continue;
                                    t = 62;
                                    precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[0]^IV[2]) >> 31)) continue;
                                    t = 63;
                                    precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));


                                    cout <<"." << flush;

                                    uint32 block2[16];
                                    uint32 IV1[4];
                                    uint32 IV2[4];
                                    for (int t = 0; t < 4; ++t)
                                    {
                                        IV1[t] = IV[t];
                                        IV2[t] = IV[t] + (1 << 31);
                                    }
                                    IV2[1] += (1 << 25);
                                    IV2[2] += (1 << 25);
                                    IV2[3] += (1 << 25);

                                    for (int t = 0; t < 16; ++t)
                                    {
                                        block2[t] = block[t];
                                    }
                                    block2[4] += 1<<31;
                                    block2[11] -= 1<<15;
                                    block2[14] += 1<<31;

                                    ihv[0] = IV1[0];
                                    ihv[1] = IV1[1];
                                    ihv[2] = IV1[2];
                                    ihv[3] = IV1[3];
                                    md5_compress(block);

                                    ihv[0] = IV2[0];
                                    ihv[1] = IV2[1];
                                    ihv[2] = IV2[2];
                                    ihv[3] = IV2[3];
                                    md5_compress(block2);
                                    if (IV2[0]==IV1[0] && IV2[1]==IV1[1] && IV2[2]==IV1[2] && IV2[3]==IV1[3])
                                        cout << "success" << endl;
                                        return 0;

                                    if (IV2[0] != IV1[0])
                                        cout << "!" << flush;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

uint32* find_block1_00 (uint32 block [16], uint32 IHV[4] ) // Stevens Style
{//MD5 is the IV or IHV, the names are not correct yet

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
        * Exp. for Stevens vlaues for Q_t with t = 3 
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


    find_block1_Wang(block_0, ihv);
    //  -compress
    // find blcok 1
    //  -compress
    // manipulate





}
    // determine bitconditions ( q_i )|_ i = -3 ^ 0
    // generate partial lower diff path 
    // generate partial upper diff path
    // connet both diff paths
    

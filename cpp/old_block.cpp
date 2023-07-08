#include <iostream>
#include <string>

#include <bitset>

#include <climits>

#include <ctime>

#include <cmath>
#include <vector>
#include <string.h>

#include <sstream>

#include "MD5.cpp"

#include "rng.cpp"
uint32* find_block1_Wang(uint32 block[16], uint32 IHV[4]) //does not work
{
    fill_n(Q,68,0); // !!!!!!!!!!!!
    uint32 offset = 3; //offset is 3 because the "last" pos for calculation is -3 (+3 = 0) 
    Q[0] = IHV[0]; 
    Q[1] = IHV[3];
    Q[2] = IHV[2];
    Q[3] = IHV[1];

	std::vector<uint32> q4mask(1<<6);
	for (unsigned k = 0; k < q4mask.size(); ++k)
		q4mask[k] = ((k<<13) ^ (k<<19)) & 0x01c0e000;

	std::vector<uint32> q9mask(1<<5), q10mask(1<<5);
	for (unsigned k = 0; k < q9mask.size(); ++k)
	{
		uint32 msk = (k<<5) ^ (k<<13) ^ (k<<17) ^ (k<<24);
		q9mask[k] = msk &  0x00084000;
		q10mask[k] = msk & 0x18000020;
	}
	
	std::vector<uint32> q9mask2(1<<10);
	for (unsigned k = 0; k < q9mask2.size(); ++k)
		q9mask2[k] = ((k<<1) ^ (k<<7) ^ (k<<14) ^ (k<<15) ^ (k<<22)) & 0x6074041c;

    uint32 t = 0;

    while (true) //meh
	{
        uint32 help_0 = Q[offset + 0] & 0x80000000;
		uint32 help_01 = 0x80000000 ^ help_0;

		Q[offset + 2] = (randX() & 0x71de7799) | 0x0c008840 | help_01;
		Q[offset + 3] = (randX() & 0x01c06601) | 0x3e1f0966 | (Q[offset + 2] & 0x80000018);
		Q[offset + 4] = 0x3a040010 | (Q[offset +3 ] & 0x80000601);
		Q[offset + 5] = (randX() & 0x03c0e000) | 0x482f0e50 | help_0;
		Q[offset + 6] = (randX() & 0x600c0000) | 0x05e2ec56 | help_0;
		Q[offset + 7] = (randX() & 0x604c203e) | 0x16819e01 | help_01 | (Q[offset + 6] & 0x01000000);
		Q[offset + 8] = (randX() & 0x604c7c1c) | 0x043283e0 | (Q[offset + 7] & 0x80000002);
		Q[offset + 9] = (randX() & 0x00002800) | 0x1c0101c1 | (Q[offset + 8] & 0x80001000);
		Q[offset + 10] = 0x078bcbc0 | help_01;
		Q[offset + 11] = (randX() & 0x07800000) | 0x701FD040 | help_01;// changed 701FD040
		Q[offset + 12] = (randX() & 0x00f00f7f) | 0x00081080 | (Q[offset + 11] & 0xe7000000);
		Q[offset + 13] = (randX() & 0x00701f77) | 0x3f0fe008 | help_0;
		Q[offset + 14] = (randX() & 0x00701f77) | 0x408be088 | help_0;
		Q[offset + 15] = (randX() & 0x00ff3ff7) | 0x7d000000;
		Q[offset + 16] = (randX() & 0x1ffdffff) | 0x20000000 | (~Q[offset + 15] & 0x40020000); // changed 0x4ffdffff and 0x00020000

/* 
        std::cout << "Q[10]: " << bitset<32>(Q[offset + 10]) << std::endl;
        std::cout << "       0111....0..111111101...001....00" << std::endl;
        std::cout << "Q[11]: " << bitset<32>(Q[offset + 11]) << std::endl;
        std::cout << "       0010........00011100...011....10" << std::endl;
        std::cout << "Q[12]: " << bitset<32>(Q[offset + 12]) << std::endl;
        std::cout << "       000...^^....10000001...10......." << std::endl;


        std::cout << "Q[13]: " << bitset<32>(Q[offset + 13]) << std::endl;
        std::cout << "       0.0...00....1011111....11...1..." << std::endl;

        std::cout << "Q[14]: " << bitset<32>(Q[offset + 14]) << std::endl;
        std::cout << "       0.1...01.......01...........0..." << std::endl;

        std::cout << "Q[15]: " << bitset<32>(Q[offset + 15]) << std::endl;
        std::cout << "       0!1...........!................." << std::endl;

        std::cout << "Q[16]: " << bitset<32>(Q[offset + 16]) << std::endl;
        std::cout << "_____________________________ " << std::endl; */


		block[5] = reverse_md5(block,5, AC(5), RC(5));
		block[6] = reverse_md5(block,6, AC(6), RC(6));
		block[7] = reverse_md5(block,7, AC(22), RC(7));
		block[11] = reverse_md5(block,11, AC(11), RC(11));
		block[14] = reverse_md5(block,14, AC(14), RC(14));
		block[15] = reverse_md5(block,15, AC(15), RC(15));

        // prerparing next values for active work

		const uint32 t_17 = f_t(Q[offset + 16], Q[offset + 15], Q[offset + 14],16) + Q[offset + 13] + AC(16);
		const uint32 t_18 = Q[offset + 14] + AC(17) + block[6];
		const uint32 t_19 = Q[offset + 15] + AC(18) + block[11];

		const uint32 t_0 = f_t(Q[offset + 0], Q[offset - 1], Q[offset - 2],0) + Q[offset - 3] + AC(0);
		const uint32 t_1 = Q[offset - 2] + AC(1);		

		const uint32 q_a = 0x04200040 | (Q[offset + 2] & 0xf01e1080);
		

	    unsigned int counter = 0;
		while (counter < (1 << 12))
        {

            ++counter;
            // we try to pick a Q_17 that Q18 ..Q21 can be calculated with Q17 and fulfill their conidtions
            // we can't use stepforward since we havent past the vlaues in Q[t]
            uint32 q_1 = q_a | (randX() & 0x01c0e71f);
            uint32 m_1 = Q[offset + 2] - q_1;
            m_1 = RR(m_1,RC(1)) - f_t(q_1, Q[offset + 0], Q[offset -1], 1) - t_1;

            const uint32 q_16 = Q[offset + 16];
            uint32 q_17 = t_17 +  m_1;
            q_17 = RL(q_17, RC(16)) + q_16;
            
         /*    std::cout << "Q15 " << bitset<32>(Q[15]) << std::endl;
            std::cout << "Q16 " << bitset<32>(Q[16]) << std::endl;
            std::cout << "Q17 " << bitset<32>(q_17) << std::endl; */
 
            if (0x40000000 != ((q_17 ^ q_16) & 0xc0008008))continue; //!! changed to 0x40008008

            if (0 != (q_17 & 0x00020000)) continue; //!! changed to 0x80020000
            
            uint32 q_18 = f_t( q_17, q_16,Q[offset + 15], 17) + t_18;
            q_18 = RL(q_18, RC(17));
            q_18 += q_17;

            if (0x00020000 != ((q_18 ^ q_17) & 0xa0020000)) continue;

            uint32 q_19 = f_t( q_18, q_17,q_16, 18) + t_19;
            q_19 = RL(q_19, RC(18));
            q_19 += q_18;
            if (0 != (q_19 & 0x80020000)) continue;
            
            uint32 m_0 = q_1 - Q[0 + offset];
            m_0 = RR(m_0, RC(0)) - t_0;
            uint32 q_20 = f_t(q_19, q_18,q_17,19) + q_16 + AC(19) + m_0;
            q_20 = RL(q_20,RC(19));
            q_20 += q_19;

            if (0x00040000 != ((q_20 ^ q_19) & 0x80040000)) continue;
            
            Q[offset + 1 ] = q_1;
            Q[offset + 17] = q_17;
            Q[offset + 18] = q_18;
            Q[offset + 19] = q_19;
            Q[offset + 20] = q_20;


            block[0] = m_0;
            block[1] = m_1;
            block[2] = reverse_md5(block, 2, AC(2),RC(2));

            counter = 0;

			break;                      
        }            
        if (counter != 0) continue;
        // prerparing next values for active work 
        // backup values
        const uint32 q_4b = Q[offset + 4];
        const uint32 q_9b = Q[offset + 9];
        const uint32 q_10b = Q[offset + 10];
        const uint32 t_21 = f_t(Q[offset + 20], Q[offset + 19], Q[offset + 18],20) + Q[offset + 17] + AC(20);



        // In original equivalent to the following code we Steven's works a lot wit htemp and helper values to minimize the amount of calcuations (I guess), but it also helps to keep an overvie
        // so we try to find a good midling way, which is quite compact but also understandable without messing up the entire performance.
        // Some easy understandable helper values were mentioned before (e.g. q_4). Without these the clacuation wouldn't be possible or wrong results were givien as an output.
        // The following helper values are just only to help in case of overview, perfornace and clarity in code.
        // Stevens calls the helper Values like aa, bb, tt9, etc. This is hard to read.
        // We may call the values by there function, at least we try.
        //uint32 help_cond_Q_21 = Q[offset + 21]; // aa
        //uint32 help_cond_Q_20 = Q[offset + 20]; // bb
        //uint32 help_cond_M_10 = f_t(Q[offset + 21], Q[offset + 20], Q[offset + 19],21) + Q[offset + 18] + AC(21); // dd // m_10 somewhere between Q_22 and Q_21
        //help_cond_M_10 = RL(help_cond_M_10, RC(21)) + help_cond_Q_21; // for t = 21 
        //uint32 help_cond_Q_22;//cc
        //uint32 t;

		counter = 0;

		while (counter < (1<<6))
		{
            // std::cout << "i" << std::flush;

			Q[offset + 4] = q_4b ^ q4mask[counter];
			++counter;
			block[5] = reverse_md5(block,5, AC(5), RC(5));
			uint32 q_21 = t_21 + block[5];
			q_21 = RL(q_21, RC(20)); 
            q_21 += Q[offset + 20];
			if (0 != ((q_21 ^ Q[offset + 20]) & 0x80020000)) continue;

			Q[offset + 21] = q_21;
			block[3] = reverse_md5(block,3, AC(3), RC(3));
			block[4] = reverse_md5(block, 4, AC(4), RC(4));
			block[7] = reverse_md5(block, 7, AC(7), RC(7));

			const uint32 t_10 = Q[offset + 7] + AC(10);
			const uint32 t_22 = f_t(Q[offset + 21], Q[offset + 20], Q[offset + 19],21) + Q[offset + 18] + AC(21);
			const uint32 t_23 = Q[offset + 19] + AC(22) + block[15];
			const uint32 t_24 = Q[offset + 20] + AC(23) + block[4];
	 
			unsigned counter2 = 0;
			while (counter2 < (1<<5))
			{
                uint32 q_10 = q_10b ^ q10mask[counter2];
				uint32 m_10 = RR(Q[offset + 11] - q_10,RC(10));
				uint32 q_9 = q_9b ^ q9mask[counter2];
				++counter2;

				m_10 -= f_t(q_10, q_9, Q[offset + 8],10) + t_10;

                // named that help_vlaues are in corred order
				uint32 help_21 = Q[offset + 21];//aa
				uint32 help_22 = t_22 + m_10; //dd
                help_22 = RL(help_22, RC(21)) + help_21;//dd
				if (0 != (help_22 & 0x80000000)) continue;			

				uint32 help_20 = Q[offset + 20];//bb
				uint32 help_23 = t_23 + f_t(help_22, help_21, help_20,22);//cc 

				if (0 != (help_23 & 0x20000)) continue;

				help_23 = RL(help_23, RC(22)) + help_22;
				if (0 != (help_23 & 0x80000000)) continue;

				uint32 help_24 = t_24 + f_t(help_23, help_22, help_21,23);//bb 
                help_24 = RL(help_24, RC(23)) + help_23;
				if (0 == (help_24 & 0x80000000)) continue;
				block[10] = m_10;
				Q[offset + 9] = q_9;
				Q[offset + 10] = q_10;
				block[13] = reverse_md5(block, 13, AC(13), RC(13));

				for (unsigned k9 = 0; k9 < (1<<10);)
				{                         
                    Q[offset + 21] = help_21;
                    Q[offset + 22] = help_22;
                    Q[offset + 23] = help_23;
                    Q[offset + 24] = help_24;
                    /*
                    std::cout << bitset<32>(Q[offset + 21]) << "Q[21]" << std::endl;
                    std::cout << bitset<32>(Q[offset + 22]) << "Q[22]" << std::endl;
                    std::cout << bitset<32>(Q[offset + 23]) << "Q[23]" << std::endl;
                    std::cout << bitset<32>(Q[offset + 24]) << "Q[24]" << std::endl;
                    */

                    Q[offset + 9] = q_9 ^ q9mask2[k9] ;
                    ++k9;
                    block[8] = reverse_md5(block, 8, AC(8), RC(8));
                    block[9] = reverse_md5(block, 9, AC(9), RC(9));
                    block[12] = reverse_md5(block, 12, AC(12), RC(12));
                    //doing steps for t \in {24,...,33}
                    Q[offset + 25] = step_foward(offset + 24,W(block,24));
                    Q[offset + 26] = step_foward(offset + 25,W(block,25));
                    Q[offset + 27] = step_foward(offset + 26,W(block,26));
                    Q[offset + 28] = step_foward(offset + 27,W(block,27));
                    Q[offset + 29] = step_foward(offset + 28,W(block,28));
                    Q[offset + 30] = step_foward(offset + 29,W(block,29));
                    Q[offset + 31] = step_foward(offset + 30,W(block,30));  
                    Q[offset + 32] = step_foward(offset + 31,W(block,31));
                    Q[offset + 33] = step_foward(offset + 32,W(block,32));
                    Q[offset + 34] = step_foward(offset + 33,W(block,33));

                    //t=34
                    Q[offset + 35] = Q[offset + 31] + f_t(Q[offset + 34],Q[offset + 33],Q[offset + 32],34) + AC(34) + W(block,34); 

                    if(( Q[offset + 35] & (1 << 15)) == 0) continue; // if bit at pos 15 is zero
                    Q[offset + 35] = ((Q[offset + 35] << 16) | (Q[offset + 35] >> 16)) + Q[offset + 34];

                    //doing steps for t \in {35,...,47}
                    Q[offset + 36] = step_foward(offset + 35,W(block,35));
                    Q[offset + 37] = step_foward(offset + 36,W(block,36));
                    Q[offset + 38] = step_foward(offset + 37,W(block,37));
                    Q[offset + 39] = step_foward(offset + 38,W(block,38));
                    Q[offset + 40] = step_foward(offset + 39,W(block,39));
                    Q[offset + 41] = step_foward(offset + 40,W(block,40));
                    Q[offset + 42] = step_foward(offset + 41,W(block,41));
                    Q[offset + 43] = step_foward(offset + 42,W(block,42));
                    Q[offset + 44] = step_foward(offset + 43,W(block,43));
                    Q[offset + 45] = step_foward(offset + 44,W(block,44));
                    Q[offset + 46] = step_foward(offset + 45,W(block,45));
                    Q[offset + 47] = step_foward(offset + 46,W(block,46));
                    Q[offset + 48] = step_foward(offset + 47,W(block,47));//t=47

                    //checks wether IV[1] or IV[3] is 1 at pos.s of 0x80000000 
                    // do not know how to sove this nicely just copyed it .. for pregress
                    // for t = 48 ... 63

	                if (0 != ((Q[offset + 48] ^ Q[offset + 46]) & 0x80000000))continue;
                    // for t = 48 ... 63


                    t = 48; // calculations are always for Q at t+1, this is quite confusing right now. Fix inc.
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue; // we wrote at pos t+1 last set, tow bevore is t-1

                    t = 49;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 == ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;//!

                    t = 50;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;
                    
                    t = 51;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                    t = 52;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                    t = 53;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;
					
                    t = 54;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                    t = 55;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;                    

                    t = 56;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                    t = 57;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                    t = 58;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                    t = 59;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 == ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue; //!

                    t = 60;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t)); // Q[61] = ..
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                    t = 61;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));// Q[61] = ..
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                    t = 62;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));//Q[63] = ..
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;     

                    t = 63;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t)); //Q[64] = ..

					std::cout << "." << std::flush;

					uint32 block2[16];
					uint32 IV1[4];
                    uint32 IV2[4];

                    for(int i = 0; i < 4 ; i)
                    {
						IV1[i] = IHV[i];
                        IV2[i] = IHV[i] + (1 << 31);
                    }
					
                    IV2[1] += (1 << 25);
					IV2[2] += (1 << 25);
					IV2[3] += (1 << 25);

                    for(int i =0; i<16 ; i++)
                    {
						block2[i] = block[i];
                    }                    

					block2[4] += 1<<31;
					block2[11] -= 1<<15;
					block2[14] += 1<<31;

                    md5_compress_f(block,IV1);      //changes the given IV1
                    md5_compress_f(block2,IV2);     //changes the given IV2


                    if (IV2[0]==IV1[0] && IV2[1]==IV1[1] && IV2[2]==IV1[2] && IV2[3]==IV1[3])
                    {
                        return 0;
                    }
                    if (IV2[0] != IV1[0])
                        std::cout << "!" << std::endl;
                    
                }  
            }
        }      
    }
}



//##########################################################################################################################################
//############################################### end wang start steven ####################################################################
//##########################################################################################################################################


uint32* find_block1_00 (uint32 block [16], uint32 IHV[4] ) // Stevens Style
{//MD5 is the IV or IHV, the names are not correct yet

    /* 
    * Stevens works a lot with voids, which overwrites varibale on an upper, or even global level.
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
    fill_n(Q,68,0); // !!!!!!!!!!!!

    //uint32 Q [68] = {IHV[0], IHV[1], IHV[2], IHV[3]};


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
    Q[0] = IHV[0]; 
    Q[1] = IHV[3];
    Q[2] = IHV[2];
    Q[3] = IHV[1];

	std::vector<uint32> q9q10mask(1<<3);

	for (unsigned k = 0; k < q9q10mask.size(); ++k)

		q9q10mask[k] = ((k<<5) ^ (k<<12) ^ (k<<25)) & 0x08002020;

	

	std::vector<uint32> q9mask(1<<9);

	for (unsigned k = 0; k < q9mask.size(); ++k)

		q9mask[k] = ((k<<1) ^ (k<<3) ^ (k<<6) ^ (k<<8) ^ (k<<11) ^ (k<<14) ^ (k<<18)) & 0x04310d12;

    uint32 t = 0;

    while (true) //meh
	{
        uint32 help_0 = Q[offset + 0] & 0x80000000;

		Q[offset + 2] = (randX() & 0x49a0e73e) | 0x221f00c1 | help_0;
		Q[offset + 3] = (randX() & 0x0000040c) | 0x3fce1a71 | (Q[offset + 2] & 0x8000e000);
		Q[offset + 4] = (randX() & 0x00000004) | (0xa5f281a2 ^ (Q[offset + 3] & 0x80000008));
		Q[offset + 5] = (randX() & 0x00000004) | 0x67fd823b;
		Q[offset + 6] = (randX() & 0x00001044) | 0x15e5829a;
		Q[offset + 7] = (randX() & 0x00200806) | 0x950430b0;
		Q[offset + 8] = (randX() & 0x60050110) | 0x1bd29ca2 | (Q[offset + 7] & 0x00000004);
		Q[offset + 9] = (randX() & 0x40044000) | 0xb8820004;
		Q[offset + 10] = 0xf288b209| (Q[offset + 9] & 0x00044000);
        Q[offset + 11] = (randX() & 0x12888008) | 0x85712f57;
		Q[offset + 12] = (randX() & 0x1ed98d7f) | 0xc0023080 | (~Q[offset + 11] & 0x00200000);
		Q[offset + 13] = (randX() & 0x0efb1d77) | 0x1000c008;
		Q[offset + 14] = (randX() & 0x0fff5d77) | 0xa000a288;
		Q[offset + 15] = (randX() & 0x0efe7ff7) | 0xe0008000 | (~Q[offset + 14] & 0x00010000);
		Q[offset + 16] = (randX() & 0x0ffdffff) | 0xf0000000 | (~Q[offset + 15] & 0x00020000); // 0x00020000


		block[5] = reverse_md5(block,5, AC(5), RC(5));
		block[6] = reverse_md5(block,6, AC(6), RC(6));
		block[7] = reverse_md5(block,7, AC(22), RC(7));
		block[11] = reverse_md5(block,11, AC(11), RC(11));
		block[14] = reverse_md5(block,14, AC(14), RC(14));
		block[15] = reverse_md5(block,15, AC(15), RC(15));
/* 
            std::cout << "Q14 " << bitset<32>(Q[14]) << std::endl;

            std::cout << "Q15 " << bitset<32>(Q[15]) << std::endl;
            std::cout << "Q16 " << bitset<32>(Q[16]) << std::endl;
 */
        // prerparing next values for active work

		const uint32 t_17 = f_t(Q[offset + 16], Q[offset + 15], Q[offset + 14],16) + Q[offset + 13] + AC(16);
		const uint32 t_18 = Q[offset + 14] + AC(17) + block[6];
		const uint32 t_19 = Q[offset + 15] + AC(18) + block[11];

		const uint32 t_0 = f_t(Q[offset + 0], Q[offset - 1], Q[offset - 2],0) + Q[offset - 3] + AC(0);
		const uint32 t_1 = Q[offset - 2] + AC(1);		

		const uint32 q_a = 0x02020801 | (Q[offset + 0] & 0x80000000);
		

	    unsigned int counter = 0;
		while (counter < (1 << 12))
        {
       
            ++counter;
            // we try to pick a Q_17 that Q18 ..Q21 can be calculated with Q17 and fulfill their conidtions

            uint32 q_1 = q_a | (randX() & 0x7dfdf7be);
            uint32 m_1 = Q[offset + 2] - q_1;
            m_1 = RR(m_1,RC(1)) - f_t(q_1, Q[offset + 0], Q[offset -1], 1) - t_1;

            const uint32 q_16 = Q[offset + 16];
            uint32 q_17 = t_17 +  m_1;
            q_17 = RL(q_17, RC(16)) + q_16;

 
            if (0x80000000 != ((q_17 ^ q_16) & 0x80008008))continue; //!! changed to 0x40008008

            if (0 != (q_17 & 0x00020000)) continue; //!! changed to 0x80020000
            
            uint32 q_18 = f_t( q_17, q_16,Q[offset + 15], 17) + t_18;
            q_18 = RL(q_18, RC(17));
            q_18 += q_17;

            if (0x80020000 != ((q_18 ^ q_17) & 0xa0020000)) continue;

            uint32 q_19 = f_t( q_18, q_17,q_16, 18) + t_19;
            q_19 = RL(q_19, RC(18));
            q_19 += q_18;
			if (0x80000000 != (q_19 & 0x80020000)) continue;

            uint32 m_0 = q_1 - Q[0 + offset];
            m_0 = RR(m_0, RC(0)) - t_0;
            uint32 q_20 = f_t(q_19, q_18,q_17,19) + q_16 + AC(19) + m_0;
            q_20 = RL(q_20,RC(19));
            q_20 += q_19;

            if (0x00040000 != ((q_20 ^ q_19) & 0x80040000)) continue;
            
            Q[offset + 1 ] = q_1;
            Q[offset + 17] = q_17;
            Q[offset + 18] = q_18;
            Q[offset + 19] = q_19;
            Q[offset + 20] = q_20;


            block[0] = m_0;
            block[1] = m_1;

			block[5] = (block, 5, 0x4787c62a, 12);
            uint32 q_21 = f_t(Q[offset + 20], Q[offset + 19], Q[offset + 18],20) + Q[offset + 17] + AC(20) + block[5];
			q_21 = RL(q_21, 5); 
            q_21 += Q[offset + 20];
			if (0 != ((q_21^Q[offset + 20]) & 0x80020000)) continue;

			Q[offset + 21] = q_21;
            counter = 0;

			break;                      
        }            
        if (counter != 0) continue;
        // prerparing next values for active work 
        // backup values
		const uint32 q_9b = Q[offset + 9];
		const uint32 q_10b = Q[offset + 10];


		block[2] = reverse_md5(block, 2, AC(2), 17);
		block[3] = reverse_md5(block, 3, AC(3), 22);
		block[4] = reverse_md5(block, 4, AC(4), 7);
		block[7] = reverse_md5(block, 7, AC(7), 22);


		const uint32 t_10 = Q[offset + 7] + AC(10);
		const uint32 t_22 = f_t(Q[offset + 21], Q[offset + 20], Q[offset + 19],21) + Q[offset + 18] + AC(21);
		const uint32 t_23 = Q[offset + 19] + AC(22) + block[15];
		const uint32 t_24 = Q[offset + 20] + AC(23) + block[4];

        for (unsigned k10 = 0; k10 < (1<<3); ++k10)			
        {
            uint32 q_10 = q_10b | (q9q10mask[k10]&0x08000020);
            uint32 m_10 = RR(Q[offset + 11] - q_10,RC(10));
            uint32 q_9 = q_9b | (q9q10mask[k10]&0x00002000);
            
            m_10 -= f_t(q_10, q_9, Q[offset + 8],10) + t_10;

            // named that help_vlaues are in corred order
            uint32 help_21 = Q[offset + 21];//aa
            uint32 help_22 = t_22 + m_10; //dd
            help_22 = RL(help_22, RC(21)) + help_21;//dd
            if (0 == (help_22 & 0x80000000)) continue;			

            uint32 help_20 = Q[offset + 20];//bb
            uint32 help_23 = t_23 + f_t(help_22, help_21, help_20,22);//cc 

            if (0 != (help_23 & 0x20000)) continue;

            help_23 = RL(help_23, RC(22)) + help_22;
            if (0 != (help_23 & 0x80000000)) continue;

            uint32 help_24 = t_24 + f_t(help_23, help_22, help_21,23);//bb 
            help_24 = RL(help_24, RC(23)) + help_23;

            if (0 == (help_24 & 0x80000000)) continue;

            block[10] = m_10;
            Q[offset + 9] = q_9;
            Q[offset + 10] = q_10;
            block[13] = reverse_md5(block, 13, AC(13), RC(13));

            for (unsigned k9 = 0; k9 < (1<<9); ++k9)
            {                         
                Q[offset + 21] = help_21;
                Q[offset + 22] = help_22;
                Q[offset + 23] = help_23;
                Q[offset + 24] = help_24;
                /*
                std::cout << bitset<32>(Q[offset + 21]) << "Q[21]" << std::endl;
                std::cout << bitset<32>(Q[offset + 22]) << "Q[22]" << std::endl;
                std::cout << bitset<32>(Q[offset + 23]) << "Q[23]" << std::endl;
                std::cout << bitset<32>(Q[offset + 24]) << "Q[24]" << std::endl;
                */

				Q[offset + 9] = q_9 ^ q9mask[k9];

                block[8] = reverse_md5(block, 8, AC(8), RC(8));
                block[9] = reverse_md5(block, 9, AC(9), RC(9));
                block[12] = reverse_md5(block, 12, AC(12), RC(12));
                //doing steps for t \in {24,...,33}
                Q[offset + 25] = step_foward(offset + 24,W(block,24));
                Q[offset + 26] = step_foward(offset + 25,W(block,25));
                Q[offset + 27] = step_foward(offset + 26,W(block,26));
                Q[offset + 28] = step_foward(offset + 27,W(block,27));
                Q[offset + 29] = step_foward(offset + 28,W(block,28));
                Q[offset + 30] = step_foward(offset + 29,W(block,29));
                Q[offset + 31] = step_foward(offset + 30,W(block,30));  
                Q[offset + 32] = step_foward(offset + 31,W(block,31));
                Q[offset + 33] = step_foward(offset + 32,W(block,32));
                Q[offset + 34] = step_foward(offset + 33,W(block,33));

                //t=34
                Q[offset + 35] = Q[offset + 31] + f_t(Q[offset + 34],Q[offset + 33],Q[offset + 32],34) + AC(34) + W(block,34); 

                if(( Q[offset + 35] & (1 << 15)) != 0) continue; // if bit at pos 15 is zero
                Q[offset + 35] = ((Q[offset + 35] << 16) | (Q[offset + 35] >> 16)) + Q[offset + 34];

                //doing steps for t \in {35,...,47}
                Q[offset + 36] = step_foward(offset + 35,W(block,35));
                Q[offset + 37] = step_foward(offset + 36,W(block,36));
                Q[offset + 38] = step_foward(offset + 37,W(block,37));
                Q[offset + 39] = step_foward(offset + 38,W(block,38));
                Q[offset + 40] = step_foward(offset + 39,W(block,39));
                Q[offset + 41] = step_foward(offset + 40,W(block,40));
                Q[offset + 42] = step_foward(offset + 41,W(block,41));
                Q[offset + 43] = step_foward(offset + 42,W(block,42));
                Q[offset + 44] = step_foward(offset + 43,W(block,43));
                Q[offset + 45] = step_foward(offset + 44,W(block,44));
                Q[offset + 46] = step_foward(offset + 45,W(block,45));
                Q[offset + 47] = step_foward(offset + 46,W(block,46));
                Q[offset + 48] = step_foward(offset + 47,W(block,47));//t=47

                //checks wether IV[1] or IV[3] is 1 at pos.s of 0x80000000 
                // do not know how to sove this nicely just copyed it .. for pregress
                // for t = 48 ... 63

                if (0 != ((Q[offset + 48] ^ Q[offset + 46]) & 0x80000000))continue;
                // for t = 48 ... 63


                t = 48; // calculations are always for Q at t+1, this is quite confusing right now. Fix inc.
                Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue; // we wrote at pos t+1 last set, tow bevore is t-1

                t = 49;
                Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                if (0 == ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;//!

                t = 50;
                Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;
                
                t = 51;
                Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                t = 52;
                Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                t = 53;
                Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;
                
                t = 54;
                Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                t = 55;
                Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;                    

                t = 56;
                Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                t = 57;
                Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                t = 58;
                Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                t = 59;
                Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                if (0 == ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue; //!

                t = 60;
                Q[offset + t + 1] = step_foward(t + offset,W(block,t)); // Q[61] = ..
                if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                t = 61;
                Q[offset + t + 1] = step_foward(t + offset,W(block,t));// Q[61] = ..
                if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                t = 62;
                Q[offset + t + 1] = step_foward(t + offset,W(block,t));//Q[63] = ..
                if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;     

                t = 63;
                Q[offset + t + 1] = step_foward(t + offset,W(block,t)); //Q[64] = ..

                std::cout << "." << std::flush;

                uint32 block2[16];
                uint32 IV1[4];
                uint32 IV2[4];

                for(int i = 0; i < 4 ; i++)
                {
                    IV1[i] = IHV[i];
                    IV2[i] = IHV[i] + (1 << 31);
                }
                
			    IV2[1] -= (1 << 25);
				IV2[2] -= (1 << 25);
				IV2[3] -= (1 << 25);


                for(int i =0; i<16 ; i++)
                {
                    block2[i] = block[i];
                }                    

                block2[4] += 1<<31;
                block2[11] += 1<<15;
                block2[14] += 1<<31;

                md5_compress_f(block,IV1);      //changes the given IV1
                md5_compress_f(block2,IV2);     //changes the given IV2
                std::cout << bitset<32>(IV1[0])<<std::endl;
                std::cout << bitset<32>(IV2[0])<<std::endl;

                if (IV2[0]==IV1[0] && IV2[1]==IV1[1] && IV2[2]==IV1[2] && IV2[3]==IV1[3])
                {
                    std::cout << "^^" << std::flush;

                    return 0;
                }
                if (IV2[0] != IV1[0])
                    std::cout << "!" << std::endl;
                
            }  
        }
            
    }
}



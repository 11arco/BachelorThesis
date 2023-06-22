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


typedef unsigned int uint32; //actually u32 




uint32 randX() //for 32 bits
{
    uint32 x = rand();
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;

  return  x ;
}

uint32* find_block0(uint32 block[16], const uint32 IHV[4])
{
    bool progress = false;
    uint32 offset = 3; //offset is 3 because the "last" pos for calculation is -3 (+3 = 0) 
   //uint32 Q[68];
    fill_n(Q,68,0); // !!!!!!!!!!!!
    

    Q[0] = IHV[0]; 
    Q[1] = IHV[3];
    Q[2] = IHV[2];
    Q[3] = IHV[1];

    std::vector<uint32> q4mask(1<<4);
	for (unsigned k = 0; k < q4mask.size(); ++k)
		q4mask[k] = ((k<<2) ^ (k<<26)) & 0x38000004;


	std::vector<uint32> q9mask2(1<<10);
    for (unsigned k = 0; k < q9mask2.size(); ++k)
    {

		q9mask2[k] = ((k<<1) ^ (k<<7) ^ (k<<14) ^ (k<<15) ^ (k<<22)) & 0x6074041c;

    }
    std::vector<uint32> q9mask(1<<16);
	for (unsigned k = 0; k < q9mask.size(); ++k)
		q9mask[k] = ((k<<1) ^ (k<<2) ^ (k<<5) ^ (k<<7) ^ (k<<8) ^ (k<<10) ^ (k<<11) ^ (k<<13)) & 0x0eb94f16;

    std::vector<uint32> q9q10mask(1<<3);
	for (unsigned k = 0; k < q9q10mask.size(); ++k)
		q9q10mask[k] = ((k<<13) ^ (k<<4)) & 0x2060;

    while (true) //meh
	{   
		Q[offset + 1] = randX();
		Q[offset + 3] = (randX() & 0xfe87bc3f) | 0x017841c0;
		Q[offset + 4] = (randX() & 0x44000033)| 0x000002c0 | (Q[offset + 3] & 0x0287bc00);
		Q[offset + 5] =  0x41ffffc8 | (Q[offset + 4] & 0x04000033);
		Q[offset + 6] = 0xb84b82d6;
		Q[offset + 7] = (randX() & 0x68000084) | 0x02401b43;
		Q[offset + 8] = (randX() & 0x2b8f6e04) | 0x005090d3 | (~Q[offset + 7] & 0x40000000);
		Q[offset + 9] =  0x20040068 | (Q[offset + 8] & 0x00020000) | (~Q[offset + 8] & 0x40000000);
		Q[offset + 10] = (randX() & 0x40000000) | 0x1040b089;
		Q[offset + 11] = (randX() & 0x10408008) | 0x0fbb7f16 | (~Q[offset + 10] & 0x40000000);
		Q[offset + 12] = (randX() & 0x1ed9df7f) | 0x00022080 | (~Q[offset + 11] & 0x40200000);
		Q[offset + 13] = (randX() & 0x5efb4f77) | 0x20049008;
		Q[offset + 14] = (randX() & 0x1fff5f77) | 0x0000a088 | (~Q[offset + 13] & 0x40000000);
		Q[offset + 15] = (randX() & 0x5efe7ff7) | 0x80008000 | (~Q[offset + 14] & 0x00010000);
		Q[offset + 16] = (randX() & 0x1ffdffff) | 0xa0000000 | (~Q[offset + 15] & 0x40020000);
	    
        block = reverse_md5(block,0, AC(0), RC(0));
		block = reverse_md5(block,6, AC(6), RC(6));
		block = reverse_md5(block,7, AC(22), RC(7));
		block = reverse_md5(block,11, AC(11), RC(11));
		block = reverse_md5(block,14, AC(14), RC(14));
		block = reverse_md5(block,15, AC(15), RC(15));

        
        //###DEBUG ONLY###
        //All conditions are from ~ Table B-6: New first block conditions
/*      
        std::cout << ".......1.1111....1....0111...... condition on Q[3]" << std::endl;
        std::cout << bitset<32>(Q[offset + 3]) <<" Q[3]" <<std::endl;
        std::cout << "0.....^0^0000^^^^0^^^^1011..0... condition on Q[4]" << std::endl;
        std::cout << bitset<32>(Q[offset + 4]) << std::endl;
        std::cout << "01...^01111111111111111111^^1.^^ condition on Q[5]"  << std::endl;
        std::cout << bitset<32>(Q[offset + 5]) << std::endl;
        std::cout << "10.1.000010010111000001011010.10 condition on Q[6]" << std::endl;
        std::cout << bitset<32>(Q[offset + 6]) << std::endl;
        std::cout << "0..0.0100100000000011011.1000.11 condition on Q[7]" << std::endl;
        std::cout << bitset<32>(Q[offset + 7]) << std::endl;
        std::cout << "0!.0.0...101....1..1...011010.11 condition on Q[8]" << std::endl;
        std::cout << bitset<32>(Q[offset + 8]) << std::endl;
        std::cout << "0!10...0.0...1^.0..0....011.1..0 condition on Q[9]" << std::endl;
        std::cout << bitset<32>(Q[offset + 9]) << "Q [9]" <<std::endl;
        std::cout << "0.01...0.1...00.1..1....1...1..1 condition on Q[10]" << std::endl;
        std::cout << bitset<32>(Q[offset + 10]) << std::endl;
        std::cout << "0!0....1.....01...^1....00.....0 condition on Q[11]" << std::endl;
        std::cout << bitset<32>(Q[offset + 11]) << std::endl;
        std::cout << "0!0....0..!..01...1.....1....... condition on Q[12]" << std::endl;
        std::cout << bitset<32>(Q[offset + 12]) << std::endl;
        std::cout << "0.1....0.....1..1.01....0...1... condition on Q[13]" << std::endl;
        std::cout << bitset<32>(Q[offset + 13]) << std::endl;
        std::cout << "0!0.............1.1.....1...1... condition on Q[14]" << std::endl;
        std::cout << bitset<32>(Q[offset + 14]) << std::endl;
        std::cout << "1.0....0.......!1...........0... condition on Q[15]" << std::endl;
        std::cout << bitset<32>(Q[offset + 15]) << std::endl;
        std::cout << "1!1...........!................. condition on Q[16]" << std::endl;
        std::cout << bitset<32>(Q[offset + 16]) << std::endl;


        std::cout << "__________________________________" << std::endl;
          */
        
        // prepare some helper values which are not necassery, but useful
        // we refer to the T_t state of the MD5 sum without the word (W_t), when speaking about t_t
        // sometimes the helper value is having the word (W-t) but not Funktion (f_t)
        // it apperars to be seölected arbritary, how the t_t values are selected. We may proofe this later and find a solution that applies genreally.
        // F_t = f_t(Q[t]], Q[t−1], Q[t−2], t) ,
        // T_t = F_t + Q[t−3} + AC_t + W_t ,
        // in general
        uint32 t_1 = f_t(Q[offset + 1], Q[offset + 0], Q[offset - 1], 1) + AC(1);
        uint32 t_5 = RR(Q[offset + 6] - Q[offset + 5], RC(5)) - f_t(Q[offset + 5], Q[offset + 4], Q[offset + 3], 5) - AC(5); // RC(6) = 12 but why?

        uint32 t_17 = f_t(Q[offset + 16], Q[offset + 15], Q[offset + 14], 16) + Q[offset + 13] + AC(16);
        uint32 t_18 = Q[offset + 14] + AC(17) + W(block, 17);// why called t_18 t = 17 ?
        uint32 t_19 = Q[offset + 15] + AC(18) + W(block, 18);// why called t_19 t = 18 ?
        uint32 t_20 = Q[offset + 16] + AC(19) + W(block, 19);// why called t_20 t = 19 ?

        // prerparing next values for active work
        uint32 q_2 = 0;
        uint32 q_16 = 0;
        uint32 q_17 = 0;
        uint32 q_18 = 0;
        uint32 q_19 = 0;
        uint32 q_20 = 0;
        uint32 q_21 = 0;
 
        unsigned counter = 0;
        // t=16 - t=21
	    while (counter < (1 << 7))        
        {
            q_16 = Q[offset + 16];
			q_17 = ((randX() & 0x3ffd7ff7) | (q_16 & 0xc0008008)) ^ 0x40000000;
            counter ++;
			q_18 = f_t(q_17, q_16, Q[offset + 15],17) + t_18;
			q_18 = RL(q_18, RC(18)); 
            q_18 += q_17;
			if (0x00020000 != ((q_18 ^ q_17)&0xa0020000))
				continue;
            //std::cout << "16" << endl;

        
            q_19 = f_t(q_18, q_17, q_16, 18) + t_19;
            q_19 = RL(q_19, 14); 
            q_19 += q_18;
            if (0x80000000 != (q_19 & 0x80020000))
                continue;
    
            q_20 = f_t(q_19, q_18, q_17, 19) + t_20;
            q_20 = RL(q_20, RC(19)); 
            q_20 += q_19;
            if (0x00040000 != ((q_20 ^ q_19) & 0x80040000))
                continue;
            
            block[1] = q_17 - q_16;
            block[1] = RR(block[1], 5);
            block[1] -= t_17;
            q_2 = block[1] + t_1;
            q_2 = RL(q_2, 12); 
            q_2 += Q[offset + 1];
            block[5] = t_5 - q_2;
        
            Q[offset + 2] = q_2;
            Q[offset + 17] = q_17;
            Q[offset + 18] = q_18;
            Q[offset + 19] = q_19;
            Q[offset + 20] = q_20;
            reverse_md5(block,2,AC(2),RC(2));
       /*      
            for (int i = 0; i < 4; i++)
            {
                std:: cout << "Q[" + to_string(i + 17) + "] = " + to_string(Q[offset + i + 17]) << std::endl;
            }
            std::cout << "___________" << std::endl;
         */
	        counter = 0;
			break;

		}

		if (counter != 0)

			continue;

        // iterate over possible changes of q4 
		// while keeping all conditions on q1-q20 intact
		// this changes m3, m4, m5 and m7

        uint32 q4 = Q[offset + 4];
        const uint32 q9backup = Q[offset + 9]; // so we do not overwrite accidently
        uint32 t_21 = f_t(Q[offset + 20], Q[offset + 19], Q[offset + 18],20) + Q[offset + 17] + AC(20);

        unsigned counter2 = 0;

        while(counter2 < (1<<4))  // I choose this bad name intentional to force to rework this entire thing after it actually works, becuase I am afraid that I may leave the while (true)'s just as they are
        {            
            Q[offset + 4] = q4 ^ q4mask[counter2];
			++counter2;

            //Q[offset + 4] = q4 ^ q4mask[counter2]; TBD
			block = reverse_md5(block,5, AC(5), RC(5));
			q_21 = t_21 + W(block,20);
			q_21 = RL(q_21,RC(20));
            q_21 += Q[offset + 20];
            /*            
            std::cout << to_string(q_21) + (": ");
            std::cout << bitset<32>(q_21) << endl;
            
            std::cout << to_string(Q[20]) + (": ");
            std::cout << bitset<32>(Q[20]) << endl; 
             */


			if (0 != ((q_21 ^ Q[offset + 20]) & 0x80020000))
				continue;

			Q[offset + 21] = q_21;
			block = reverse_md5(block, 3, AC(3), RC(3));
			block = reverse_md5(block, 4, AC(4), RC(4));
			block = reverse_md5(block, 7, AC(7), RC(7));
			
			uint32 t_22 = f_t(Q[offset + 21], Q[offset + 20], Q[offset + 19],21) + Q[offset + 18] + AC(21);
			uint32 t_23 = Q[offset + 19] + AC(22) + W(block,22);
			uint32 t_24 = Q[offset + 20] + AC(23) + W(block,23);

			uint32 t_9 = Q[offset + 6] + AC(9);
			uint32 t_10 = Q[offset + 7] + AC(10);
			uint32 t_8 = f_t(Q[offset + 8], Q[offset + 7], Q[offset + 6], 8) + Q[offset + 5] + AC(8);		
			uint32 t_12 = RR(Q[offset + 13] - Q[offset + 12],RC(12)) - AC(12); 
			uint32 t_13 = RR(Q[offset + 14] - Q[offset + 13],RC(13)) - f_t(Q[offset + 13], Q[offset + 12], Q[offset + 11], 13) - AC(13);

			// iterate over possible changes of q9 and q10
			// while keeping conditions on q1-q21 intact
			// this changes m8, m9, m10, m12 and m13 (and not m11!)
			// the possible changes of q9 that also do not change m10 are used below
            // more helper values (for know) 
            // notation close to Stevens
            uint32 q_10;
            uint32 m_10;
			for (int counter3 = 0; counter3 < (1<<3);)
			{
                // std::cout << "for counter3 " << endl;

                q_10 = Q[offset + 10] ^ (q9q10mask[counter3] & 0x60);
				Q[offset + 9] = q9backup ^ (q9q10mask[counter3] & 0x2000);
				++counter3;
				m_10 = RR(Q[offset + 11]- q_10,RC(10));
				m_10 -= f_t(q_10, Q[offset + 9], Q[offset + 8],10) + t_10;

	             			
                uint32 help_21 = Q[offset + 21];  //aa                   
				uint32 help_22 = t_22 + m_10;      //dd
                help_22 = RL(help_22, RC(21)) + help_21;      //
                uint32 help_20 = Q[offset + 20];  //bb
				uint32 help_23 = t_23 + f_t(help_22, help_21, help_20,22);  //cc
                uint32 t;

                
				if (0x80000000 != (help_22 & 0x80000000)) continue;


				if (0 != (help_23 & 0x20000)) continue;
				help_23 = RL(help_23, RC(22)) + help_22;

				if (0 != (help_23 & 0x80000000)) continue;

				help_20 = t_24 + f_t(help_23, help_22, help_21, 23); 
                help_20 = RL(help_20, RC(19)) + help_23;
				if (0 == (help_20 & 0x80000000)) continue;

				block[10] = m_10;
				block[13] = t_13 - q_10;

				// iterate over possible changes of q9
				// while keeping intact conditions on q1-q24
				// this changes m8, m9 and m12 (but not m10!)

                //more helper values ..
                uint32 q_9;
                uint32 m_8;
                uint32 m_9;
				for (unsigned counter4 = 0; counter4 < (1<<16); ++counter4)
				{

                    q_9 = Q[offset + 9] ^ q9mask[counter4];
					block[12] = t_12 - f_t(Q[offset + 12], Q[offset + 11], q_10,12) - q_9;
					m_8 = q_9 - Q[offset + 8];
					block[8] = RR(m_8, 7) - t_8; 					
                    m_9 = q_10 - q_9;
					block[9] = RR(m_9, 12) - f_t(q_9, Q[offset + 8], Q[offset + 7],9) - t_9; 

					//uint32 a = aa, b = bb, c = cc, d = dd;
                    //uint32 IV[4] ={help_21,help_20,help_23,help_22};

                    Q[20] = help_20;
                    Q[21] = help_21;
                    Q[22] = help_22;
                    Q[23] = help_23;
/*                      
                    //### DEBUG ONLY ###
                    std::cout << "             ..." << std::endl;
                    std::cout << "1............!.................. condition on Q[20]" << std::endl;
                    std::cout << bitset<32>(Q[offset + 20]) << " Q[20]" << std::endl;
                    std::cout << "1.............^................. condition on Q[21]" << std::endl;
                    std::cout << bitset<32>(Q[offset + 21]) << " Q[21]" << std::endl;
                    std::cout << "0............................... condition on Q[22]" << std::endl;
                    std::cout << bitset<32>(Q[offset + 22]) << std::endl;
                    std::cout << "1............................... condition on Q[23]" << std::endl;
                    std::cout << bitset<32>(Q[offset + 23]) << std::endl;
 */

                    //doing steps for t \in {24,...,33}
                    /*          
                    t =24;
                    IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                    t =25;
                    IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                    t =26;
                    IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                    t =27;
                    IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[4],W(block,t),AC(t),RC(t));
                    t =28;
                    IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                    t =29;
                    IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                    t =30;
                    IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                    t =31;
                    IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[4],W(block,t),AC(t),RC(t));
                    t =32;
                    IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                    t =33;
                    IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                    */
                    
                    Q[24] = step_foward(24,W(block,24));
                    Q[25] = step_foward(24,W(block,25));
                    Q[26] = step_foward(24,W(block,26));
                    Q[27] = step_foward(24,W(block,27));
                    Q[28] = step_foward(24,W(block,28));
                    Q[29] = step_foward(24,W(block,29));
                    Q[30] = step_foward(24,W(block,30));
                    Q[31] = step_foward(24,W(block,31));
                    Q[32] = step_foward(24,W(block,32));
                    Q[33] = step_foward(24,W(block,33));

                    //std::cout << " for counter2: " + to_string(counter2) + " for counter3: " + to_string(counter3) + " for counter4: " + to_string(counter4) << endl;

                    t=34;    
					Q[t] += f_t(Q[33], Q[32], Q[31],t) + W(block,t) + AC(t);
					if (0 != (Q[t-4] & (1 << 15))) 
						continue;
					Q[30] = (Q[30] << 16 | Q[30]>>16) + Q[t]; // RL(16) bzw RR(16)
                    //cout << "progress 1 " + to_string(counter4) << endl;


                    //doing steps for t \in {35,...,47}
                    /*
                    t =35;
                    IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));
                    t =36;
                    IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                    t =37;
                    IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                    t =38;
                    IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                    t =39;
                    IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));
                    t =40;
                    IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                    t =41;
                    IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                    t =42;
                    IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                    t =43;
                    IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));
                    t =44;
                    IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                    t =45;
                    IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                    t =46;
                    IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                    t =47;
                    IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));

                    */

                    Q[35] = step_foward(35,W(block,35));
                    Q[36] = step_foward(36,W(block,36));
                    Q[37] = step_foward(37,W(block,37));
                    Q[38] = step_foward(38,W(block,38));
                    Q[39] = step_foward(39,W(block,39));
                    Q[40] = step_foward(40,W(block,40));
                    Q[41] = step_foward(41,W(block,41));
                    Q[42] = step_foward(42,W(block,42));
                    Q[43] = step_foward(43,W(block,43));
                    Q[44] = step_foward(44,W(block,44));
                    Q[45] = step_foward(45,W(block,45));
                    Q[46] = step_foward(46,W(block,46));
                    Q[47] = step_foward(47,W(block,47));


	                if (0 != ((Q[47]^Q[45]) & 0x80000000))
						continue;
                    // for t = 48 ... 63

                    t = 48;
                    //IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                    Q[t] = step_foward(t,W(block,t));

                    if (0 != ((Q[t] ^ Q[t-2]) >> 31)) continue;

                    t = 49;
                    //IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                    Q[t] = step_foward(t,W(block,t));

                    if (0 == ((Q[t-2] ^ Q[t]) >> 31)) continue;

                    t = 50;
                    //IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                    Q[t] = step_foward(t,W(block,t));

                    if (0 != ((Q[t-2] ^ Q[t]) >> 31)) continue;

                    t = 51;
                    //IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));
                    Q[t] = step_foward(t,W(block,t));

                    if (0 != ((Q[t-2] ^ Q[t]) >> 31)) continue;

                    t = 52;
                    //IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                    Q[t] = step_foward(t,W(block,t));

                    if (0 != ((Q[t-2] ^ Q[t]) >> 31)) continue;

                    t = 53;
                    //IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                    Q[t] = step_foward(t,W(block,t));

                    if (0 != ((Q[t-2] ^ Q[t]) >> 31)) continue;

                    t = 54;
                    //IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                    Q[t] = step_foward(t,W(block,t));

                    if (0 != ((Q[t-2] ^ Q[t]) >> 31)) continue;

                    t = 55;
                    //IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));
                    Q[t] = step_foward(t,W(block,t));

                    if (0 != ((Q[t-2] ^ Q[t]) >> 31)) continue;

                    t = 56;
                    //IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                    Q[t] = step_foward(t,W(block,t));

                    if (0 != ((Q[t-2] ^ Q[t]) >> 31)) continue;

                    t = 57;
                    //IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                    Q[t] = step_foward(t,W(block,t));

                    if (0 != ((Q[t-2] ^ Q[t]) >> 31)) continue;

                    t = 58;
                    //IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                    Q[t] = step_foward(t,W(block,t));

                    if (0 != ((Q[t-2] ^ Q[t]) >> 31)) continue;

                    t = 59;
                    //IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));
                    Q[t] = step_foward(t,W(block,t));

                    if (0 != ((Q[t-2] ^ Q[t]) >> 31)) continue;

                    t = 60;
                    //IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                    Q[t] = step_foward(t,W(block,t));

                    if (0 != ((Q[t-2] ^ Q[t]) >> 31)) continue;

                    t = 61;
                    //IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                    Q[t] = step_foward(t,W(block,t));

                    if (0 != ((Q[t-2] ^ Q[t]) >> 31)) continue;

                    t = 62;
                    Q[t] = step_foward(t,W(block,t));
                    //IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));

                    if (0 != ((Q[t-2] ^ Q[t]) >> 31)) continue;
                                      
                    t = 63;
                    Q[t] = step_foward(t,W(block,t));
                    //IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t)); 

                    //cout << "b0t" << endl;
					uint32 IHV1 = Q[63] + IHV[1];
					uint32 IHV2 = Q[62] + IHV[2];
					uint32 IHV3 = Q[61] + IHV[3];
					bool wang = true;

                   // std::cout << "x" << std::flush;
					if (0x02000000 != ((IHV2^IHV1) & 0x86000000)) wang = false;
					if (0 != ((IHV1^IHV3) & 0x82000000)) wang = false;
					if (0 != (IHV1 & 0x06000020)) wang = false;

					bool stevens = true;

					if ( ((IHV1^IHV2)>>31)!=0 || ((IHV1^IHV3)>>31)!= 0 ) stevens = false;
					if ( (IHV3&(1<<25))!=0 || (IHV2&(1<<25))!=0 || (IHV1&(1<<25))!=0 || ((IHV2^IHV1)&1)!=0) stevens = false;

										

					if (!(wang || stevens)) continue;


					std::cout << "." << std::flush;

					uint32 block2[16];
					uint32 IV1[4];
                    uint32 IV2[4];

                    for(int i = 0; i < 4 ; i++)
                    {
						IV1[i] = IHV[i];
                        IV2[i] = IHV[i];
                    }

                    for(int i =0; i<16 ; i++)
                    {
						block2[i] = block[i];
                    }                    

					block2[4] += 1<<31;
					block2[11] -= 1<<15;
					block2[14] += 1<<31;


                    md5_compress_f(block,IV1);    
                    md5_compress_f(block2,IV2);


        
                    std::cout << std::endl;
                    show_bits(IV1,4);
                    show_bits(IV2,4);

                    /*          
                    show_bits(IV1,4);
                    show_bits(IV2,4);

                    std::cout << std::to_string(IV2[0]) + " - " + std::to_string(IV2[1]) + " - " + std::to_string(IV2[2]) + " - " + std::to_string(IV2[3]) << std::endl;
                    std::cout << std::to_string(IV1[0] + (1<<31)) + " - " + std::to_string(IV1[1]+ (1<<31) +(1<<25)) + " - " + std::to_string(IV[2]+ (1<<31) + (1<<25)) + " - " + std::to_string(IV[3]+ (1<<31) + (1<<25)) << std::endl;
                    std::cout << "__________________________________________________" << endl;  
                    */
                

                    if (	   (IV2[0] == IV1[0] + (1<<31))
							&& (IV2[1] == IV1[1] + (1<<31) + (1<<25))
							&& (IV2[2] == IV1[2] + (1<<31) + (1<<25))
							&& (IV2[3] == IV1[3] + (1<<31) + (1<<25)))
                    {      
                        return block;
                    }

					if (IV2[0] != IV1[0] + (1<<31))
						std::cout << "!" << std::flush;

                }
            }         
        }
    }
}


uint32* find_block1_Wang(uint32 block[16], uint32 IHV[4])
{
    bool progress = false;
    uint32 offset = 3; //offset is 3 because the "last" pos for calculation is -3 (+3 = 0) 
    uint32 Q [68] = {IHV[0], IHV[1], IHV[2], IHV[3]};

	std::vector<uint32> q9mask2(1<<10);
    for (unsigned k = 0; k < q9mask2.size(); ++k)
    {
		q9mask2[k] = ((k<<1) ^ (k<<7) ^ (k<<14) ^ (k<<15) ^ (k<<22)) & 0x6074041c;
        std::cout << "q9mask :" << std::endl;
        show_bits(&q9mask2[k],q9mask2.size());
    }

    while (true) //meh
	{
		Q[offset + 2] = (randX() & 0x71de7799) | 0x0c008840 | (0x80000000 ^ Q[offset] & 0x80000000);
		Q[offset + 3] = (randX() & 0x01c06601) | 0x3e1f0966 | (Q[offset + 2] & 0x80000018);
		Q[offset + 4] = 0x3a040010 | (Q[offset + 3] & 0x80000601);
		Q[offset + 5] = (randX() & 0x03c0e000) | 0x482f0e50 | (Q[offset] & 0x80000000);
		Q[offset + 6] = (randX() & 0x600c0000) | 0x05e2ec56 | (Q[offset] & 0x80000000);
		Q[offset + 7] = (randX() & 0x604c203e) | 0x16819e01 | (0x80000000 ^ Q[offset] & 0x80000000) | (Q[offset + 6] & 0x01000000);
		Q[offset + 8] = (randX() & 0x604c7c1c) | 0x043283e0 | (Q[offset + 7] & 0x80000002);
		Q[offset + 9] =  (randX() & 0x00002800) | 0x1c0101c1 | (Q[offset + 8] & 0x80001000);
		Q[offset + 10] = 0x078bcbc0 | (0x80000000 ^ Q[offset] & 0x80000000);
		Q[offset + 11] = (randX() & 0x07800000) | 0x607dc7df | (0x80000000 ^ Q[offset] & 0x80000000);
		Q[offset + 12] = (randX() & 0x00f00f7f) | 0x00081080 | (Q[offset + 11] & 0xe7000000);
		Q[offset + 13] = (randX() & 0x00701f77) | 0x3f0fe008 | (Q[offset] & 0x80000000);
		Q[offset + 14] = (randX() & 0x00701f77) | 0x408be088 | (Q[offset] & 0x80000000);
		Q[offset + 15] = (randX() & 0x00ff3ff7) | 0x7d000000;
		Q[offset + 16] = (randX() & 0x4ffdffff) | 0x20000000 | (~Q[offset + 15] & 0x00020000);
	    
		block = reverse_md5(block,5, AC(5), RC(5));
		block = reverse_md5(block,6, AC(6), RC(6));
		block = reverse_md5(block,7, AC(22), RC(7));
		block = reverse_md5(block,11, AC(11), RC(11));
		block = reverse_md5(block,14, AC(14), RC(14));
		block = reverse_md5(block,15, AC(15), RC(15));

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
            q_1 = (0x01c0e71f | (Q[offset + 2] & 0xf01e1080) ) | (randX() & 0x01c0e71f); 
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
                block =reverse_md5(block, 2, AC(2),RC(1));
            }
        }        
    
        block= reverse_md5(block, 2, AC(2), RC(2));
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
                            block = reverse_md5(block,13, AC(13), RC(13));

                        }
                        for (uint32 k9 = 0; k9 < 1024;)
				        {       
                            uint32 IV[4] ={help_cond_Q_21,help_cond_Q_20,help_cond_Q_22,help_cond_M_10};
                            Q[9] = q_9 ^ q9mask2[k9] ;
                            k9++;
                            block = reverse_md5(block, 8, AC(8), RC(80));
                            block = reverse_md5(block, 9, AC(9), RC(9));
                            block = reverse_md5(block, 12, AC(12), RC(12));

                            //doing steps for t \in {24,...,33}
                            t =24;
                            IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                            t =25;
                            IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                            t =26;
                            IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                            t =27;
                            IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[4],W(block,t),AC(t),RC(t));
                            t =28;
                            IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                            t =29;
                            IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                            t =30;
                            IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                            t =31;
                            IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[4],W(block,t),AC(t),RC(t));
                            t =32;
                            IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                            t =33;
                            IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));

                            IV[2] += f_t(IV[3],IV[0],IV[1],34); //t = 34

                            if((IV[2] & (1 << 15)) == 0) // if bit at pos 15 is zero
                            {
                                IV[2] = RL(IV[2],16) + IV[3];

                                //doing steps for t \in {35,...,47}
                                t =35;
                                IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));
                                t =36;
                                IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                                t =37;
                                IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                                t =38;
                                IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                                t =39;
                                IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));
                                t =40;
                                IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                                t =41;
                                IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                                t =42;
                                IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                                t =43;
                                IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));
                                t =44;
                                IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));
                                t =45;
                                IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));
                                t =46;
                                IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
                                t =47;
                                IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));

                                if((IV[1] ^IV[3]) & 0x80000000 != 1)//checks wether IV[1] or IV[3] is 1 at pos.s of 0x80000000 
                                {   // do not know how to sove this nicely just copyed it .. for pregress
                                    // for t = 48 ... 63
                                    t = 48;
                                    IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[0] ^ IV[2]) >> 31)) continue;
                                    t = 49;
                                    IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));

                                    if (0 == ((IV[1] ^ IV[3]) >> 31)) continue;
                                    t = 50;
                                    IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[0] ^ IV[2]) >> 31)) continue;
                                    t = 51;
                                    IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[1] ^ IV[3]) >> 31)) continue;
                                    t = 52;
                                    IV[1] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[0] ^ IV[2]) >> 31)) continue;
                                    t = 53;
                                    IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[1]^IV[3]) >> 31)) continue;
                                    t = 54;
                                    IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));
    
                                    if (0 != ((IV[0] ^ IV[2]) >> 31)) continue;
                                    t = 55;
                                    IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[1] ^ IV[3]) >> 31)) continue;
                                    t = 56;
                                    IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[0] ^ IV[2]) >> 31)) continue;
                                    t = 57;
                                    IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[1] ^ IV[3]) >> 31)) continue;
                                    t = 58;
                                    IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[0] ^ IV[2]) >> 31)) continue;
                                    t = 59;
                                    IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));

                                    if (0 == ((IV[1] ^ IV[3]) >> 31)) continue;
                                    t = 60;
                                    IV[0] = precise_step_foward(t,IV[0],IV[1],IV[2],IV[3],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[0] ^ IV[2]) >> 31)) continue;
                                    t = 61;
                                    IV[3] = precise_step_foward(t,IV[3],IV[0],IV[1],IV[2],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[1] ^ IV[3]) >> 31)) continue;
                                    t = 62;
                                    IV[2] = precise_step_foward(t,IV[2],IV[3],IV[0],IV[1],W(block,t),AC(t),RC(t));

                                    if (0 != ((IV[0]^IV[2]) >> 31)) continue;
                                    t = 63;
                                    IV[1] = precise_step_foward(t,IV[1],IV[2],IV[3],IV[0],W(block,t),AC(t),RC(t));

                                    //cout << "b1t" << endl;

                                    std::cout <<"." << std::flush;

                                    uint32 block2[16];
                                    uint32* IV1;
                                    uint32* IV2;
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
                                    
                                    md5_compress_f(block,IV1);
                                    md5_compress_f(block2,IV2);
                                    if (IV2[0]==IV1[0] && IV2[1]==IV1[1] && IV2[2]==IV1[2] && IV2[3]==IV1[3])
                                        return 0;

                                    if (IV2[0] != IV1[0])
                                        std::cout << "!" << std::flush;
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
    fill_n(Q,68,0); // !!!!!!!!!!!!

    //uint32 Q [68] = {IHV[0], IHV[1], IHV[2], IHV[3]};


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
        Q[offset + 1] = randX(); //?
        Q[offset + 2]; //?
        Q[offset + 3] = ( randX() & 0xfe98bc3f ) | 0x017841c0; // 0xfe98bc3f=11111110 10000111 10111100 00111111, 0x017841c0=00000001 01111000 01000001 11000000
        Q[offset + 4] = ( randX() & 0x44000033 ) | 0x000002c0 | ( Q[offset + 3] & 0x0287bc00 );  // 
        Q[offset + 5] = 0x41fffc8 | ( Q[offset + 4] & 0x04000033 );
        Q[offset + 6] = 0xb84b82d6;
        Q[offset + 7] = ( randX() & 0x68000084 ) | 0x02401b43;
        Q[offset + 8] = ( randX() & 0x2b8f6e04) | 0x005090d3 | (~Q[offset + 7] & 0x40000000 );
        Q[offset + 9] = 0x20040068 | ( Q[offset + 8] & 0x00020000 ) | ( ~Q[offset + 8] & 0x40000000 );
        Q[offset + 10]= ( randX() & 0x40000000 ) | 0x1040b089;
        Q[offset + 11]= ( rand() & 0x10408008 ) | 0x0fbb7f16 | ( ~Q[offset + 10] & 0x40000000 );
        Q[offset + 12]= ( randX() & 0x1ed9df7f ) | 0x00022080 | ( ~Q[offset + 11] & 0x40200000 );
        Q[offset + 13]= ( randX() & 0x5efb4f77 ) | 0x20049008; 
        Q[offset + 14]= ( randX() & 0x1fff5f77 ) | 0x0000a088 | ( ~Q[offset + 13] & 0x40000000 );
        Q[offset + 15]= ( randX() & 0x5efe7ff7 ) | 0x80008000 | ( ~Q[offset + 14] & 0x00010000 );
        Q[offset + 16]= ( randX() & 0x1ffdffff ) | 0xa0000000 | ( ~Q[offset + 15] & 0x40020000 );

        // we now claculate the massage m for t =0,6,...,15. For this we use the reverse md5 fuction.
		block = reverse_md5(block, 0, AC(0), RC(0)); // reverse_md5 is a void altering the input: block
		block = reverse_md5(block, 6, AC(6), RC(6)); // stevens hard codes the AC and the RC
	    block = reverse_md5(block, 7, AC(7), RC(7));
		block = reverse_md5(block, 11, AC(11), RC(11)); 
		block = reverse_md5(block, 14, AC(14), RC(14)); 
		block = reverse_md5(block, 15, AC(15), RC(15)); 

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
            q_1 = (0x02020801 | (Q[offset + 0] & 0x80000000) ) | (randX() & 0x7dfdf7be);
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



void find_coll(std::string input,std::string input_2) // MD5 is the IV or IHV, the names are not correct yet
{   
    std::string a = input;
    std::string b = input_2;

    uint32 block_10[16];
    uint32 block_11[16];
    uint32* block_20;
    uint32* block_21;



    std::cout << "try to find collsions. IHV: " << std::flush;
    std::cout << to_hex(ihv[0]) + to_hex(ihv[1]) + to_hex(ihv[2]) + to_hex(ihv[3]) << std::endl;
    std::cout << "Block0: " << std::endl;
	find_block0(block_10, ihv);
    std::cout << "Block0 ... " << std::endl;
	md5_compress(block_10);
    std::cout << "Block1: " << std::endl;
	find_block1_Wang(block_11, ihv);
    std::cout << "Block1 ... " << std::endl;


	for (int t = 0; t < 16; ++t) {
		block_20[t] = block_10[t];
		block_21[t] = block_11[t];
	}
	block_20[4] += 1 << 31; 
    block_20[11] += 1 << 15; 
    block_20[14] += 1 << 31;
	block_21[4] += 1 << 31; 
    block_21[11] -= 1 << 15; 
    block_21[14] += 1 << 31;

    std::cout << "finish" << std::endl;
}

    // determine bitconditions ( q_i )|_ i = -3 ^ 0
    // generate partial lower diff path 
    // generate partial upper diff path
    // connet both diff paths
int main()
{
    srand(std::time(nullptr));

    std::string val_stevens = "abc\n";
    std::string val ="(Fast täglich lesen wir in den Nachrichten von Datenschutz-Skandalen oder Fällen von Datendiebstahl. Heute speichern wir gerne unsere persönlichen Daten in der Cloud.)";
    std::string val_2 ="Every day millions of people rely on our free all-in-one privacy solution. The DuckDuckGo app includes our Private Search, Web Tracking Protection, Smarter Encryption, Email Protection, Android App Tracking Protection, and more.";
    std::string test = val;
    std::string test_2 = val_2;

    std::cout << "Please enter a string for collision finding:" << std::endl;


    getline(std::cin, test); 
    if( test == "r" ) 
    {
        std::cout << "initilize  finding" << std::endl;
        std::cout << process(test) << std::endl;
        find_coll(to_string(randX()),to_string(randX()));  
      }
    else if (test == "md5") 
    {   
        std::cout << "Input for md5 sum" << std::endl;
        getline(std::cin, test);         
        std::cout << process(test) << std::endl;

    }
    else
    {
        std::cout << "second input:" << std::endl;
        getline(std::cin, test_2); 
        std::cout << "initilize  finding" << std::endl;
        process(test);
        find_coll(test,test_2);
    }



    return 0;
}
    

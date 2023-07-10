#include <iostream>
#include <string>

#include <bitset>

#include <climits>

#include <ctime>

#include <cmath>
#include <vector>
#include <string.h>

#include <sstream>

#include "old_block.cpp"

typedef unsigned int uint32; //actually u32 

uint32* find_block0(uint32 block[16], const uint32 IHV[4])
{
    fill_n(Q,68,0); // !!!!!!!!!!!!
    uint32 offset = 3; //offset is 3 because the "last" pos for calculation is -3 (+3 = 0) 
   //uint32 Q[68];

    Q[0] = IHV[0]; 
    Q[1] = IHV[3];
    Q[2] = IHV[2];
    Q[3] = IHV[1];

    std::vector<uint32> q4mask(1<<4);
	for (unsigned k = 0; k < q4mask.size(); ++k)
    {
		q4mask[k] = ((k<<2) ^ (k<<26)) & 0x38000004;
    }

    std::vector<uint32> q9q10mask(1<<3);
	for (unsigned k = 0; k < q9q10mask.size(); ++k)
    {
        q9q10mask[k] = ((k<<13) ^ (k<<4)) & 0x2060;
    }

    std::vector<uint32> q9mask(1<<16);
	for (unsigned k = 0; k < q9mask.size(); ++k)
    {
		q9mask[k] = ((k<<1) ^ (k<<2) ^ (k<<5) ^ (k<<7) ^ (k<<8) ^ (k<<10) ^ (k<<11) ^ (k<<13)) & 0x0eb94f16;
    }


    while (true) //meh
	{   
		Q[offset + 1] = randX();
        
        //std:: cout << bitset<32>(Q[offset + 1]) << " Q_1" << endl;
 		Q[offset + 3] = (randX() & 0xfe87bc3f) | 0x017841c0;
		Q[offset + 4] = (randX() & 0x44000033) | 0x000002c0 | (Q[offset + 3] & 0x0287bc00);
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
	    
        block[0] = reverse_md5(block,0, AC(0), RC(0));
		block[6] = reverse_md5(block,6, AC(6), RC(6));
		block[7] = reverse_md5(block,7, AC(22), RC(7));
		block[11] = reverse_md5(block,11, AC(11), RC(11));
		block[14] = reverse_md5(block,14, AC(14), RC(14));
		block[15] = reverse_md5(block,15, AC(15), RC(15));

        
        // prepare some helper values which are not necassery, but useful
        // we refer to the T_t state of the MD5 sum without the word (W_t), when speaking about t_t
        // sometimes the helper value is having the word (W-t) but not Funktion (f_t)
        // it apperars to be seölected arbritary, how the t_t values are selected. We may proofe this later and find a solution that applies genreally.
        // F_t = f_t(Q[t]], Q[t−1], Q[t−2], t) ,
        // T_t = F_t + Q[t−3} + AC_t + W_t ,
        // in general

        const uint32 t_1 = f_t(Q[offset + 1], Q[offset + 0], Q[offset - 1], 1) + Q[offset - 2] + AC(1);

        const uint32 t_17 = f_t(Q[offset + 16], Q[offset + 15], Q[offset + 14], 16) + Q[offset + 13] + AC(16);
        const uint32 t_18 = Q[offset + 14] + 0xc040b340 + block[6];
        const uint32 t_19 = Q[offset + 15] + 0x265e5a51 + block[11];
        const uint32 t_20 = Q[offset + 16] + 0xe9b6c7aa + block[0];
        const uint32 t_5 = RR(Q[offset + 6] - Q[offset + 5], RC(5)) - f_t(Q[offset + 5], Q[offset + 4], Q[offset + 3], 5) - AC(5); // RC(6) = 12 but why?


        // prerparing next values for active work

 
        unsigned counter = 0;
        // t=16 - t=21
	    while (counter < (1 << 7))        
        {
            const uint32 q_16 = Q[offset + 16];

			uint32 q_17 = ((randX() & 0x3ffd7ff7) | (q_16 & 0xc0008008)) ^ 0x40000000;
            counter ++;

			uint32 q_18 = f_t(q_17, q_16, Q[offset + 15],17) + t_18;
			q_18 = RL(q_18, RC(17)); 
            q_18 += q_17;
			if (0x00020000 != ((q_18 ^ q_17) & 0xa0020000))
				continue;
        
            uint32 q_19 = f_t(q_18, q_17, q_16, 18) + t_19;
            q_19 = RL(q_19, RC(18)); 
            q_19 += q_18;
            if (0x80000000 != (q_19 & 0x80020000))
                continue;
    
            uint32 q_20 = f_t(q_19, q_18, q_17, 19) + t_20;
            q_20 = RL(q_20, RC(19)); 
            q_20 += q_19;
			if (0x00040000 != ((q_20 ^ q_19) & 0x80040000)) // xor q_20 and q_19 gives where they are diffrent, take bit 18 with &0x00040000, if this is not set on 1 continue the while loop and do not process further
                continue;
            
/*              if (0x80000000 != (q_20 & 0x80000000))
                continue; 
 */
            block[1] = q_17 - q_16;
            block[1] = RR(block[1], 5); //RC(16) = 5
            block[1] -= t_17;
            uint32 q_2 = block[1] + t_1;
            q_2 = RL(q_2, 12); 
            q_2 += Q[offset + 1];
            block[5] = t_5 - q_2;
        
            Q[offset + 2] = q_2;
            Q[offset + 17] = q_17;
            Q[offset + 18] = q_18;
            Q[offset + 19] = q_19;
            Q[offset + 20] = q_20;
                             
            block[2] = reverse_md5(block,2,AC(2),RC(2));

	        counter = 0;
			break;

		}

		if (counter != 0)continue;

        // iterate over possible changes of q4 
		// while keeping all conditions on q1-q20 intact
		// this changes m3, m4, m5 and m7

        const uint32 q4 = Q[offset + 4];
        const uint32 q9backup = Q[offset + 9]; // so we do not overwrite accidently
        const uint32 t_21 = f_t(Q[offset + 20], Q[offset + 19], Q[offset + 18],20) + Q[offset + 17] + AC(20);

        unsigned counter2 = 0;

        while(counter2 < (1<<4))  // I choose this bad name intentional to force to rework this entire thing after it actually works, becuase I am afraid that I may leave the while (true)'s just as they are
        {            
            Q[offset + 4] = q4 ^ q4mask[counter2];
			++counter2;

            block[5] = reverse_md5(block,5, AC(5), RC(5));
			uint32 q_21 = t_21 + block[5];
			q_21 = RL(q_21,RC(20));
            q_21 += Q[offset + 20];

            if (0 != ((q_21 ^ Q[offset + 20]) & 0x80020000)) continue;

			Q[offset + 21] = q_21;

			block[3] = reverse_md5(block, 3, AC(3), RC(3));
			block[4] = reverse_md5(block, 4, AC(4), RC(4));
			block[7] = reverse_md5(block, 7, AC(7), RC(7));

			const uint32 t_22 = f_t(Q[offset + 21], Q[offset + 20], Q[offset + 19],21) + Q[offset + 18] + AC(21);
			const uint32 t_23 = Q[offset + 19] + AC(22) + W(block,22);
			const uint32 t_24 = Q[offset + 20] + AC(23) + W(block,23);

			const uint32 t_9 = Q[offset + 6] + AC(9);
			const uint32 t_10 = Q[offset + 7] + AC(10);

            const uint32 t_8 = f_t(Q[offset + 8], Q[offset + 7], Q[offset + 6], 8) + Q[offset + 5] + AC(8);		
			const uint32 t_12 = RR(Q[offset + 13] - Q[offset + 12], RC(12)) - AC(12); 
			const uint32 t_13 = RR(Q[offset + 14] - Q[offset + 13], RC(13)) - f_t(Q[offset + 13], Q[offset + 12], Q[offset + 11], 13) - AC(13);

			// iterate over possible changes of q9 and q10
			// while keeping conditions on q1-q21 intact
            
			// this changes m8, m9, m10, m12 and m13 (and not m11!)
			// the possible changes of q9 that also do not change m10 are used below
            // more helper values (for know) 
            // notation close to Stevens

			for (int counter3 = 0; counter3 < (1<<3);)
			{

            // block[0] is initial corrcet (line 89), but gets wrong over the code. My version does not change at all, Steven's does.
    
                uint32 q_10 = Q[offset + 10] ^ (q9q10mask[counter3] & 0x60);
				Q[offset + 9] = q9backup ^ (q9q10mask[counter3] & 0x2000);
				++counter3;
				uint32 m_10 = RR(Q[offset + 11] - q_10, RC(10));
				m_10 -= f_t(q_10, Q[offset + 9], Q[offset + 8],10) + t_10;

	             			
                uint32 help_21 = Q[offset + 21];  //aa                   
				uint32 help_22 = t_22 + m_10;     //dd
                help_22 = RL(help_22, RC(21)) + help_21;     

				if (0x80000000 != (help_22 & 0x80000000)) continue;

                uint32 help_20 = Q[offset + 20];  //bb
				uint32 help_23 = t_23 + f_t(help_22, help_21, help_20,22);  //cc
                uint32 t;

				if (0 != (help_23 & 0x20000)) continue;
				help_23 = RL(help_23, RC(22)) + help_22;

				if (0 != (help_23 & 0x80000000)) continue;

				uint32 help_24 = t_24 + f_t(help_23, help_22, help_21, 23); // help_20 becomes help_24, maybe rename (renamed)
                help_24 = RL(help_24, RC(23)) + help_23;

				if (0 == (help_24 & 0x80000000)) continue; //

	            block[10] = m_10;
				block[13] = t_13 - q_10;
				// iterate over possible changes of q9
				// while keeping intact conditions on q1-q24
				// this changes m8, m9 and m12 (but not m10!)

                //more helper values ..
                                            
		        for (unsigned counter4 = 0; counter4 < (1<<16); ++counter4)
				{

                    uint32 q_9 = Q[offset + 9] ^ q9mask[counter4];
					block[12] = t_12 - f_t(Q[offset + 12], Q[offset + 11], q_10, 12) - q_9;
					uint32 m_8 = q_9 - Q[offset + 8];
					block[8] = RR(m_8, RC(8)) - t_8; 					
                    uint32 m_9 = q_10 - q_9;
					block[9] = RR(m_9, RC(9)) - f_t(q_9, Q[offset + 8], Q[offset + 7],9) - t_9; 

/*                     for (int i = offset + 21; i <68; i++) // maybe remove this later
                    {
                        Q[i] = 0;
                    } */

                    Q[offset + 21] = help_21;
                    Q[offset + 22] = help_22;
                    Q[offset + 23] = help_23;
                    Q[offset + 24] = help_24;
/*                     for (int i = offset + 21; i <68; i++) // maybe remove this later
                    {
                        Q[i] = 0;
                    } */

                    
                    //std::cout << bitset<32>(block[0]) << std::endl; // block[0] is initial corrcet (line 89), but gets wrong over the code. My version does not change at all, Steven's does.

                    //uint32 IV[4] ={help_21,help_20,help_23,help_22};
                    
                    //std::cout << Q[offset + 24] << endl;
                    //doing steps for t \in {24,...,33}
                    
                    //Q[24] = step_foward(24,W(block,24));
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

                    //offset + std::cout << " for counter2: " + to_string(counter2) + " for counter3: " + to_string(counter3) + " for counter4: " + to_string(counter4) << endl;

                    t=34;    
					Q[offset + 35] = Q[offset + t-3] + f_t(Q[offset + 34], Q[offset + 33], Q[offset + 32],t) + block[11] + AC(t);

					if (0 != (Q[offset + 35] & (1 << 15))) 
						continue;

					Q[offset + 35] = ((Q[offset + 35] << 16) | (Q[offset + 35] >> 16)) + Q[offset + t]; // RL(16) bzw RR(16)
                    //cout << "progress 1 " + to_string(counter4) << endl;
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
                    Q[offset + 48] = step_foward(offset + 47,W(block,47));
                   /* std::cout << std::endl;
                     std::cout << bitset<32>(Q[offset + 47]) << " | 47" << std::endl;
                    std::cout << bitset<32>(Q[offset + 48]) << " | 48" << std::endl */;
	                if (0 != ((Q[offset + 48] ^ Q[offset + 46]) & 0x80000000))
						continue;
                    // for t = 48 ... 63

                    t = 48; // calculations are always for Q at t+1, this is quite confusing right now. Fix inc.
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue; // place we wrote t+1; place two  before t-1

                    t = 49;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 == ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

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
                    if (0 == ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;
                    
                    t = 60;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t)); // Q[61]
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                    t = 61;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;

                    t = 62;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));
                    if (0 != ((Q[offset + t + 1] ^ Q[offset + t - 1]) >> 31)) continue;     

                    t = 63;
                    Q[offset + t + 1] = step_foward(t + offset,W(block,t));

					uint32 IHV1 = Q[offset + 64] + IHV[1];
					uint32 IHV2 = Q[offset + 63] + IHV[2];
					uint32 IHV3 = Q[offset + 62] + IHV[3];

					bool wang = true;
					if (0x02000000 != ((IHV2^IHV1) & 0x86000000))
                    { 
                        wang = false;
                    }
					if (0 != ((IHV1^IHV3) & 0x82000000)) 
                    {
                        wang = false;
                    }
					if (0 != (IHV1 & 0x06000020))
                    {
                        wang = false;
                    }
					
					bool stevens = true;
					if ( ((IHV1^IHV2)>>31)!=0 || ((IHV1^IHV3)>>31)!= 0 )
                    {
                         stevens = false;
                    }
					if ( (IHV3&(1<<25))!=0 || (IHV2&(1<<25))!=0 || (IHV1&(1<<25))!=0 || ((IHV2^IHV1)&1)!=0)
                    {
                         stevens = false;
                    }
										
					if (!(wang || stevens)) 
                    {
                        continue;
                    }

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
					block2[11] += 1<<15;
					block2[14] += 1<<31;

                    md5_compress_f(block,IV1);      //changes the given IV1
                    md5_compress_f(block2,IV2);     //changes the given IV2


                    if ((IV2[0] == IV1[0] + (1<<31)) && (IV2[1] == IV1[1] + (1<<31) + (1<<25)) && (IV2[2] == IV1[2] + (1<<31) + (1<<25))	&& (IV2[3] == IV1[3] + (1<<31) + (1<<25)))
                    {       
/*                         std::cout << std::endl;
                        std::cout << bitset<32>(ihv[0]) << std::endl;
                        std::cout << bitset<32>(ihv[1]) << std::endl;
                        std::cout << bitset<32>(ihv[2]) << std::endl;
                        std::cout << bitset<32>(ihv[3]) << std::endl; */
                        return 0;
                   }

					if (IV2[0] != IV1[0] + (1<<31))
						std::cout << "!" << std::flush;

                }
            }
        }
    }
}


//##########################################################################################################################################
//##########################################################################################################################################
//########################################## end block 0 start block 1 #####################################################################
//##########################################################################################################################################
//##########################################################################################################################################




void find_block1_11(uint32 block[16], uint32 IHV[4])
{
    uint32 offset = 3; //offset is 3 because the "last" pos for calculation is -3 (+3 = 0) 
    uint32 t = 0;
    fill_n(Q,68,0); // !!!!!!!!!!!!

    Q[0] = IHV[0]; 
    Q[1] = IHV[3];
    Q[2] = IHV[2];
    Q[3] = IHV[1];

	std::vector<uint32> q9q10mask(1<<5);

	for (unsigned k = 0; k < q9q10mask.size(); ++k)
    {
		q9q10mask[k] = ((k<<5) ^ (k<<6) ^ (k<<7) ^ (k<<24) ^ (k<<27)) & 0x880002a0;	
    }
	std::vector<uint32> q9mask(1<<9);

	for (unsigned k = 0; k < q9mask.size(); ++k)
    {
		q9mask[k] = ((k<<1) ^ (k<<3) ^ (k<<8) ^ (k<<12) ^ (k<<15) ^ (k<<18)) & 0x04710c12;
    }
	while (true) 
	{
		uint32 aa = Q[offset] & 0x80000000;

		Q[offset + 2] = (randX() & 0x75bef63e) | 0x0a410041 | aa;
		Q[offset + 3] = (randX() & 0x10345614) | 0x0202a9e1 | (Q[offset + 2] & 0x84000002);
		Q[offset + 4] = (randX() & 0x00145400) | 0xe84ba909 | (Q[offset + 3] & 0x00000014);
		Q[offset + 5] = (randX() & 0x80000000) | 0x75e90b1d | (Q[offset + 4] & 0x00145400);
		Q[offset + 6] = 0x7c23ff5a | (Q[offset + 5] & 0x80000000);
		Q[offset + 7] = (randX() & 0x40000880) | 0x114bf41a;
		Q[offset + 8] = (randX() & 0x00002090) | 0xb352dd01;
		Q[offset + 9] = (randX() & 0x00044000) | 0x7a803124;
		Q[offset + 10] = (randX() & 0x00002000) | 0xf28a92c9 | (Q[offset + 9] & 0x00044000);
		Q[offset + 11] = (randX() & 0x128a8108) | 0xc5710ed7 | (Q[offset + 10] & 0x00002000);
		Q[offset + 12] = (randX() & 0x9edb8d7f) | 0x20003080 | (~Q[offset + 11] & 0x00200000);
		Q[offset + 13] = (randX() & 0x3efb1d77) | 0x4004c008 | (Q[offset + 12] & 0x80000000);
		Q[offset + 14] = (randX() & 0x1fff5d77) | 0x0000a288;
		Q[offset + 15] = (randX() & 0x1efe7ff7) | 0x20008000 | (~Q[offset + 14] & 0x00010000);
		Q[offset + 16] = (randX() & 0x1ffdffff) | 0x20000000 | (~Q[offset + 15] & 0x40020000);
//correct


        //bit conditions doesn't macht the paper        

		block[5] = reverse_md5(block, 5, 0x4787c62a, 12);
		block[6] = reverse_md5(block, 6, 0xa8304613, 17);
		block[7] = reverse_md5(block, 7, 0xfd469501, 22);
		block[11] = reverse_md5(block, 11, 0x895cd7be, 22);
		block[14] = reverse_md5(block, 14, 0xa679438e, 17);
		block[15] = reverse_md5(block, 15, 0x49b40821, 22);

//correct

		const uint32 t_17 = f_t(Q[offset + 16], Q[offset + 15], Q[offset + 14],16) + Q[offset + 13] + 0xf61e2562;
		const uint32 t_18 = Q[offset + 14] + 0xc040b340 + block[6];
		const uint32 t_19 = Q[offset + 15] + 0x265e5a51 + block[11];

		const uint32 t_0 = f_t(Q[offset + 0], Q[offset - 1], Q[offset - 2],0) + Q[offset - 3] + 0xd76aa478;
		const uint32 t_1 = Q[offset - 2] + 0xe8c7b756;		

		const uint32 q1a = 0x02000861 ^ (Q[offset + 0] & 0x80000020);

		unsigned counter = 0;
		while (counter < (1 << 12))
		{
			++counter;

			uint32 q1 = q1a | (randX() & 0x7dfff79e);
			uint32 m1 = Q[offset+2] - q1;
			m1 = RR(m1, 12) - f_t(q1, Q[offset+0], Q[offset-1],1) - t_1;

			const uint32 q16 = Q[offset+16];
			uint32 q17 = t_17 + m1;
			q17 = RL(q17, 5) + q16;
			if (0x40000000 != ((q17^q16) & 0xc0008008)) continue;
			if (0 != (q17 & 0x00020000)) continue;

			uint32 q18 = f_t(q17, q16, Q[offset+15],17) + t_18;
			q18 = RL(q18, 9); q18 += q17;
			if (0x80020000 != ((q18^q17) & 0xa0020000)) continue;

			uint32 q19 = f_t(q18, q17, q16,18) + t_19;
			q19 = RL(q19, 14); q19 += q18;
			if (0x80000000 != (q19 & 0x80020000)) continue;

			uint32 m0 = q1 - Q[offset + 0];
			m0 = RR(m0, 7) - t_0;

			uint32 q20 = f_t(q19, q18, q17,19) + q16 + 0xe9b6c7aa + m0;
			q20 = RL(q20, 20); q20 += q19;
			if (0x00040000 != ((q20^q19) & 0x80040000))	continue;
			
			Q[offset + 1] = q1;
			Q[offset + 17] = q17;
			Q[offset + 18] = q18;
			Q[offset + 19] = q19;
			Q[offset + 20] = q20;

			block[0] = m0;
			block[1] = m1;


			block[5] = reverse_md5(block, 5, 0x4787c62a, 12);

			uint32 q21 = f_t(Q[offset + 20], Q[offset + 19], Q[offset + 18],20) + Q[offset + 17] + AC(20) + block[5];

			q21 = RL(q21, 5); 
            q21 += Q[offset+20];
			if (0 != ((q21 ^ Q[offset + 20]) & 0x80020000)) continue;

			Q[offset + 21] = q21;

			counter = 0;
			break;

		}

		if (counter != 0)continue;

		const uint32 q9b = Q[offset + 9];
		const uint32 q10b = Q[offset + 10];

		block[2] = reverse_md5(block,2, 0x242070db, 17);
		block[3] = reverse_md5(block,3, 0xc1bdceee, 22);
		block[4] = reverse_md5(block,4, 0xf57c0faf, 7);
		block[7] = reverse_md5(block,7, 0xfd469501, 22);

		const uint32 t_10 = Q[offset + 7] + 0xffff5bb1;
		const uint32 t_22 = f_t(Q[offset + 21], Q[offset + 20], Q[offset + 19],21) + Q[offset + 18] + 0x02441453;
		const uint32 t_23 = Q[offset + 19] + 0xd8a1e681 + block[15];
		const uint32 t_24 = Q[offset + 20] + 0xe7d3fbc8 + block[4];

	 	for (unsigned k10 = 0; k10 < (1<<5); ++k10)
		{ 
			uint32 q_10 = q10b | (q9q10mask[k10]&0x08000040);
			uint32 m_10 = RR(Q[offset+11] - q_10,RC(10));
			uint32 q_9 = q9b | (q9q10mask[k10]&0x80000280);

			m_10 -= f_t(q_10, q_9, Q[offset+8],10) + t_10;

			uint32 help_21 = Q[offset + 21];//aa
			uint32 help_22 = t_22 + m_10; //dd
            help_22 = RL(help_22, 9) + help_21;
			if (0 == (help_22 & 0x80000000)) continue;			

			uint32 help_20 = Q[offset + 20];//bb
			uint32 help_23 = t_23 + f_t(help_22, help_21, help_20,22); //cc
			if (0 != (help_23 & 0x20000)) continue;
			help_23 = RL(help_23, 14) + help_22;
			if (0 != (help_23 & 0x80000000)) continue;


			uint32 help_24 = t_24 + f_t(help_23, help_22, help_21,23); 
            help_24 = RL(help_24, RC(23)) + help_23;//bb
			if (0 == (help_24 & 0x80000000)) continue;


			block[10] = m_10;
			Q[offset + 9] = q_9;
			Q[offset + 10] = q_10;
			block[13] = reverse_md5(block,13, 0xfd987193, 12);


            for (unsigned k9 = 0; k9 < (1<<9); ++k9) //tunnel
            {   
               // fill_n(Q,68,0); // !!!!!!!!!!!!

                Q[offset + 21] = help_21;
                Q[offset + 22] = help_22;
                Q[offset + 23] = help_23;
                Q[offset + 24] = help_24;
				Q[offset + 9] = q_9 ^ q9mask[k9]; //tunnel

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

                t=34;    
                Q[offset + 35] = Q[offset + t-3] + f_t(Q[offset + 34], Q[offset + 33], Q[offset + 32],t) + block[11] + AC(t);

                if (0 != (Q[offset + 35] & (1 << 15))) 
                    continue;

                Q[offset + 35] = ((Q[offset + 35] << 16) | (Q[offset + 35] >> 16)) + Q[offset + t]; // RL(16) bzw RR(16)

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
				for (int t = 0; t < 4; ++t)
				{
					IV1[t] =IHV[t];
					IV2[t] =IHV[t] + (1 << 31);
				}

				IV2[1] -= (1 << 25);
				IV2[2] -= (1 << 25);
				IV2[3] -= (1 << 25);

				for (int t = 0; t < 16; ++t)
                {
					block2[t] = block[t];
                }
				block2[4] += 1<<31;
				block2[11] += 1<<15;
				block2[14] += 1<<31;


				md5_compress_f(block,IV1);
				md5_compress_f(block2,IV2);

/*                 std::cout << bitset<32>(IV1[0]) << std::endl;
                std::cout << bitset<32>(IV2[0]) << std::endl; 
  */

				if (IV2[0]==IV1[0] && IV2[1]==IV1[1] && IV2[2]==IV1[2] && IV2[3]==IV1[3])
                {
                    std::cout << "^^" << std::flush;

					return;
                }
				if (IV2[0] != IV1[0])
                {
						std::cout << "!" << std::flush;
                }
			}
		}
	}
}


void find_block1(uint32 block[], const uint32 IV[])
{    

	if ( ((IV[1]^IV[2])&(1<<31))==0 && ((IV[1]^IV[3])&(1<<31))==0 && (IV[3]&(1<<25))==0 && (IV[2]&(1<<25))==0 && (IV[1]&(1<<25))==0 && ((IV[2]^IV[1])&1)==0 )
	{
		uint32 IV2[4] = { IV[0]+(1<<31), IV[1]+(1<<31)+(1<<25), IV[2]+(1<<31)+(1<<25), IV[3]+(1<<31)+(1<<25) };


		if ((IV[1]&(1<<6))!=0 && (IV[1]&1)!=0) {

			std::cout << "S11" << std::flush;

			find_block1_11(block, IV2);

		} else if ((IV[1]&(1<<6))!=0 && (IV[1]&1)==0) {

			std::cout << "S10" << std::flush;

			find_block1_10(block, IV2);

		} else if ((IV[1]&(1<<6))==0 && (IV[1]&1)!=0) {     

			std::cout << "S01" << std::flush;

			find_block1_01(block, IV2);

		} else {

			std::cout << "S00" << std::flush;

			find_block1_00(block, IV2);

		}

		block[4] += 1<<31;

		block[11] += 1<<15;

		block[14] += 1<<31;

	} else {
        
        uint32 IV1[4] = {IV[0],IV[1],IV[2],IV[3]};

		std::cout << "W" << std::flush;
		find_block1_Wang(block, IV1);

	}

	
}


void find_coll(uint32 block_10[16],uint32 block_11[16],uint32 block_20[16],uint32 block_21[16]) // MD5 is the IV or IHV, the names are not correct yet
{   
    uint32 IV[4];
    IV[0] = ihv_g[0];
    IV[1] = ihv_g[1];
    IV[2] = ihv_g[2];
    IV[3] = ihv_g[3];

    std::cout << "try to find collsions. IHV: " << std::flush;
    std::cout << hex << IV[0] << hex << IV[1] << hex << IV[2] << hex << IV[3] << std::endl;
    std::cout << to_hex(IV[0]) << to_hex(IV[1]) << to_hex(IV[2]) << to_hex(IV[3]) << std::endl;
    std::cout << "Block0: " << std::endl;

 	find_block0(block_10, IV);

    std::cout << "Block0 ... " << std::endl;
    std::cout << hex << IV[0] << hex << IV[1] << hex << IV[2] << hex << IV[3] << std::endl;
    std::cout << to_hex(IV[0]) << to_hex(IV[1]) << to_hex(IV[2]) << to_hex(IV[3]) << std::endl;
    std::cout << "compressing" << endl;

	uint32 IHV[4] = { IV[0], IV[1], IV[2], IV[3] };

 	md5_compress_f(block_10,IHV);

    std::cout << hex << IHV[0] << hex << IHV[1] << hex << IHV[2] << hex << IHV[3] << std::endl;
    std::cout << to_hex(IHV[0]) << to_hex(IHV[1]) << to_hex(IHV[2]) << to_hex(IHV[3]) << std::endl;
    std::cout << "Block1: " << std::endl;
    find_block1(block_11, IHV);

    std::cout << "Block1 ... " << std::endl;


	for (int t = 0; t < 16; ++t) 
    {
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


    std::getline(std::cin, test); 
    if( test == "r" ) 
    {
/*         std::cout << "initilize  finding" << std::endl;
        std::cout << process(test) << std::endl;
        find_coll(std::to_string(randX()),std::to_string(randX()));   */
      }
    else if (test == "md5") 
    {   
        std::cout << "Input for md5 sum" << std::endl;
        std::getline(std::cin, test);         
        std::cout << process(test) << std::endl;

    }
    else
    {   
        // std::cout << "second input:" << std::endl;
        // getline(std::cin, test_2); 
        std::cout << "initilize  finding" << std::endl;
        process(test);


        uint32 block_10[16];
        uint32 block_11[16];
        uint32 block_20[16];
        uint32 block_21[16];

        find_coll(block_10,block_11,block_20,block_21);
    }



    return 0;
}
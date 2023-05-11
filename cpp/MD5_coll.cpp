#include <iostream>
#include <string>

#include <bitset>
#include <string.h>

#include <bitset>

#include <cmath>

#include "MD5.cpp"


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
    * 
    * 
    */ 
   
   for (int i = 0; i < 16 ; i++)
   {
    //#todo
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
    

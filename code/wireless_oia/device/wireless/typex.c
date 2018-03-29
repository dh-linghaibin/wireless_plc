
#include    "typex.h"

void DelayUs(u16 Us)
{
 volatile u8 i;
 while(Us--)
 {
    i=10;
	while(i--);
 }

}

void DelayMs(u16 Ms)
{
 volatile u16 i;
 while(Ms--)
 {
    i=11998;
	while(i--);
 }
 
}


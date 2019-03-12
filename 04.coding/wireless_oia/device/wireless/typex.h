


#ifndef TYPEX_H
#define TYPEX_H

# define SEG_FAR   
# define SEG_DATA  
# define SEG_NEAR  
# define SEG_IDATA 
# define SEG_XDATA 
# define SEG_PDATA 
# define SEG_CODE  
# define SEG_BDATA

typedef   signed          char s8;
typedef   signed short     int s16;
typedef   signed           int s32;
typedef unsigned          char u8;
typedef unsigned short     int u16;
typedef unsigned           int u32;

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;
typedef signed char S8;
typedef signed int S16;
typedef signed long S32;

typedef unsigned char BIT;

//#ifndef   FALSE
//#define   FALSE     0
//#endif
//#ifndef   TRUE
//#define   TRUE      !FALSE
//#endif


#ifndef   LOW
#define   LOW     0
#endif
#ifndef   HIGH
#define   HIGH      !LOW
#endif

#ifndef   ZERO
#define   ZERO     0
#endif
#ifndef   ONE
#define   ONE      !ZERO
#endif

#ifndef   NULL
#define   NULL      ((void *) 0)
#endif

typedef union UU16
{
    U16 U16;
    S16 S16;
    U8 U8[2];
    S8 S8[2];
} UU16;

typedef union UU32
{
    U32 U32;
    S32 S32;
    UU16 UU16[2];
    U16 U16[2];
    S16 S16[2];
    U8 U8[4];
    S8 S8[4];
} UU32;

# define SEGMENT_VARIABLE(name, vartype, locsegment) vartype locsegment name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) vartype targsegment * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) vartype targsegment * locsegment name

# define LSB 1
# define MSB 0

void DelayUs(u16 Us);
void DelayMs(u16 Ms);



#define RADIO_DRIVER_EXTENDED_SUPPORT

#define  RADIO_DRIVER_FULL_SUPPORT
#undef  SPI_DRIVER_EXTENDED_SUPPORT


#endif



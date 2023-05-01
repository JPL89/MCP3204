// @File		MCP3204.H		 		
// @Author		JOSIMAR PEREIRA LEITE
// @country		Brazil
// @Date		01/05/23
//
//
// Copyright (C) 2023  JOSIMAR PEREIRA LEITE
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
#ifndef MCP320X_H
#define MCP320X_H

#define MCP3204_CLK PORTCbits.RC0
#define MCP3204_DIN PORTCbits.RC1
#define MCP3204_DOUT PORTCbits.RC2
#define MCP3204_CS PORTCbits.RC3

#define MCP3204_CLK_TRIS TRISCbits.RC0
#define MCP3204_DIN_TRIS TRISCbits.RC1
#define MCP3204_DOUT_TRIS TRISCbits.RC2
#define MCP3204_CS_TRIS TRISCbits.RC3

#define MCP3204_START_MASK 0x020
#define MCP3204_DIFF_MASK 0x040

static unsigned short MCP3204_CHANNELS[] =
{
    0x000,0x200,0x100,0x300,0x080,0x280,0x180,0x380
};

short MCP3204_CONTROL = 0;

void MCP3204_Write2(void)
{
    for(unsigned char bit = 0; bit < 24; bit++)
    {
        MCP3204_CLK = 0;        
        if(MCP3204_CONTROL & (1 << bit)) MCP3204_DIN = 1; else MCP3204_DIN = 0;         
        MCP3204_CLK = 1;
    }    
}

unsigned char MCP3204_Receive(void)
{   
    unsigned char byte = 0x00;
    
    MCP3204_DOUT = 1;
    
    for(unsigned char bit = 0; bit < 8; bit++)
    {
        MCP3204_CLK = 0;        
        byte <<= 1;
        if(MCP3204_DOUT == 1) byte |= 0x01;        
        MCP3204_CLK = 1;
    }
    
    return ( byte );
}

int MCP3204_Read(unsigned char channel)
{             
    MCP3204_CONTROL = MCP3204_START_MASK|MCP3204_DIFF_MASK;     
    MCP3204_CONTROL |= MCP3204_CHANNELS[ channel ];
    
    unsigned char msb = 0;
    unsigned char lsb = 0;  
    
    MCP3204_DOUT = 1;
    
    MCP3204_CS = 0;    
       
    MCP3204_Write2();
    
    msb = MCP3204_Receive();
    lsb = MCP3204_Receive();
    
    MCP3204_CS = 1;    
    
    return ( (msb<<8|lsb) & 0x0FFF );
}

void MCP3204_Init(void)
{
    MCP3204_CLK_TRIS = 0;
    MCP3204_DIN_TRIS = 0;
    MCP3204_DOUT_TRIS = 1;
    MCP3204_CS_TRIS = 0;
    
    MCP3204_CS = 1;
}

#endif // MCP320X

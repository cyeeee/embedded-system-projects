/* Copyright (c) 2012, Texas Instruments Incorporated
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

*  Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

*  Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

*  Neither the name of Texas Instruments Incorporated nor the names of
   its contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#include <ti/grlib/grlib.h>

static const unsigned char pixel_gameOver8BPP_UNCOMP[] =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 
0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xda, 0xd8, 0xd8, 0x07, 0xdb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0xde, 0x4f, 0xe3, 0xe8, 0xe2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0xda, 0xe2, 0xdf, 0xe3, 0xde, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0xe0, 0xe0, 0x4f, 0xdb, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0xdc, 0xdf, 0xdf, 0x2c, 0xdc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x24, 0xe0, 0xdb, 0xd8, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 
0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0xd8, 0x00, 0xd8, 0x00, 0xd8, 0x00, 0xd8, 0x00, 0xd8, 0x00, 0xd8, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0xdb, 0xe4, 0x55, 0xe4, 0xe4, 0xe0, 0x4f, 0xe0, 0xe0, 0xe2, 0xdc, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xdc, 0xe0, 0xde, 0x32, 0xe6, 0xe3, 0xe2, 0xe6, 0xe3, 0xe6, 0xe3, 0x32, 0xe0, 0xda, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0xe2, 0xde, 0xe0, 0xe2, 0xe3, 0xe0, 0xe8, 0xe7, 0xe4, 0x4f, 0xe8, 0xe3, 0xe4, 0xe2, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xde, 0xde, 0xe0, 0xde, 0xdf, 0xdc, 0xe3, 0xe6, 0xe7, 0xe0, 0xe2, 0xe7, 0xe3, 0xe0, 0x32, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xee, 0xee, 0xef, 0xf3, 0xa5, 0xf2, 0xf2, 0xf3, 0xa5, 0xf2, 0xf2, 0xf3, 0xa5, 0xf2, 0xf3, 0xe7, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0xef, 0xec, 0xee, 0xec, 0xf0, 0xf2, 0xf2, 0xf0, 0x82, 0xf0, 0xf2, 0xf0, 0x82, 0xf2, 0xf3, 0xe4, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x24, 0xdf, 0xee, 0xee, 0xec, 0xf2, 0xf0, 0xf3, 0xa5, 0xf2, 0xf0, 0xf3, 0xa5, 0xf2, 0xf0, 0xf3, 0xa5, 0xe6, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 
0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0xde, 0xef, 0xeb, 0xee, 0xeb, 0xec, 0xea, 0xef, 0xf2, 0xf2, 0xef, 0xf3, 0xf0, 0xf2, 0xf0, 0xf3, 0xe4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0xda, 0x24, 0xe3, 0xe4, 0xdf, 0xec, 0xec, 0xec, 0xee, 0xeb, 0xef, 0xf2, 0xf2, 0xa5, 0xf2, 0xf0, 0xf2, 0xa5, 0xf2, 0xf2, 0xe6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xee, 0xee, 0xec, 0xe3, 0xdc, 0x5d, 0xec, 0xee, 0xeb, 0xee, 0xec, 0xec, 0xee, 0xf3, 0xf0, 0xf2, 0xf0, 0x82, 0xf0, 0xf2, 0xe3, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x4f, 0xef, 0xee, 0xe4, 0x24, 0xda, 0x24, 0x32, 0xee, 0xef, 0xee, 0xf0, 0xee, 0xef, 0xee, 0xef, 0xec, 0xef, 0xee, 0xf0, 0xee, 0xe3, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0xe0, 0xf2, 0xe2, 0xe4, 0xf4, 0xf4, 0xef, 0xef, 0xf0, 0xf7, 0xf4, 0xf8, 0xf2, 0xee, 0xec, 0xf2, 0xf7, 0xf7, 0xf6, 0x88, 0xe2, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x7a, 0xe6, 0xe0, 0xef, 0xe6, 0xeb, 0xee, 0xf0, 0xf3, 0xf4, 0xf4, 0xf2, 0xec, 0xee, 0xf0, 0xf6, 0xab, 0xf4, 0xf3, 0xe3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0xe0, 0xee, 0xe8, 0xea, 0xee, 0xee, 0xf2, 0xf0, 0xf0, 0xee, 0xef, 0xee, 0xe8, 0xe3, 0x82, 0xf0, 0xf0, 0xe2, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0xe0, 0xef, 0xe6, 0xec, 0xec, 0xee, 0xee, 0xef, 0xee, 0xe4, 0xde, 0xf0, 0xe6, 0xe2, 0xef, 0xef, 0xee, 0xe3, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 
0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0xdf, 0xeb, 0xe8, 0xf0, 0xf0, 0xf2, 0xef, 0x32, 0xe2, 0xf3, 0xec, 0xf2, 0xe4, 0xde, 0xec, 0xf0, 0xe3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdf, 0xee, 0xef, 0xe7, 0xdb, 0x24, 0xe8, 0xe6, 0xd8, 0x4f, 0xf2, 0xea, 0xd8, 0x4f, 0xef, 0xee, 0xe2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdb, 0x5d, 0xe2, 0x01, 0xdc, 0xef, 0xe0, 0xdf, 0xe7, 0x2c, 0xd8, 0xdc, 0xe6, 0x57, 0xec, 0xe3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0xd8, 0x4f, 0xef, 0xee, 0xeb, 0xde, 0xe0, 0xeb, 0xe7, 0xe4, 0xeb, 0xee, 0xee, 0xe2, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0xd8, 0xde, 0xe0, 0xdf, 0xf2, 0xee, 0xeb, 0xe7, 0xe0, 0xe0, 0xf4, 0xf2, 0xf3, 0xee, 0xef, 0xe0, 0xd8, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x55, 0xec, 0xee, 0xef, 0x7a, 0xea, 0xe0, 0xe0, 0xee, 0xee, 0xee, 0xef, 0x55, 0xdf, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdb, 0x5d, 0xee, 0xee, 0xeb, 0x57, 0xdf, 0xdf, 0xe6, 0xea, 0xe8, 0xe8, 0xe0, 0x07, 0xe7, 0xe3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0xdf, 0xef, 0xef, 0xea, 0xea, 0xe0, 0xd8, 0x24, 0xd8, 0x24, 0xd8, 0x24, 0xdf, 0xe7, 0xeb, 0xe2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 
0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0xdc, 0xec, 0xe8, 0xec, 0xe3, 0xde, 0xe6, 0xe7, 0xe6, 0xe7, 0xe6, 0xe8, 0xe7, 0xe2, 0xde, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x24, 0xda, 0xd8, 0xde, 0xe6, 0xeb, 0xee, 0xef, 0xea, 0xe7, 0xe7, 0xeb, 0xe0, 0xde, 0xe8, 0xe7, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0xdc, 0xe4, 0xe6, 0x5d, 0xec, 0xee, 0xec, 0x5d, 0xec, 0xeb, 0xe0, 0xde, 0xe6, 0xe8, 0xe7, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xea, 0xef, 0xee, 0xef, 0xec, 0xee, 0xee, 0xef, 0xeb, 0xdf, 0xdc, 0xeb, 0xe7, 0xea, 0xe2, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0xd8, 0x00, 0xdc, 0xe4, 0xeb, 0xee, 0xee, 0xec, 0xef, 0xec, 0xef, 0xec, 0xee, 0xe7, 0xe2, 0x24, 0xda, 0xd8, 0xd8, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0xdf, 0xeb, 0xec, 0xee, 0xee, 0xef, 0xec, 0xef, 0xee, 0xef, 0xee, 0xec, 0xe8, 0xe8, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0xe8, 0xef, 0xee, 0xee, 0xec, 0x5d, 0xdc, 0xdc, 0xdc, 0xde, 0xdb, 0xe7, 0xe7, 0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0xda, 0xe4, 0xef, 0xec, 0xe6, 0xd8, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x24, 0xe8, 0xe7, 0xe3, 0xda, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 
0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0xdc, 0xdf, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0xd8, 0xdc, 0xe6, 0xea, 0xe2, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xda, 0xdc, 0xde, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x00
};

static const unsigned int palette_gameOver8BPP_UNCOMP[]=
{
	0x000000, 	0x000033, 	0x000066, 	0x000099, 
	0x0000cc, 	0x0000ff, 	0x003300, 	0x003333, 
	0x003366, 	0x003399, 	0x0033cc, 	0x0033ff, 
	0x006600, 	0x006633, 	0x006666, 	0x006699, 
	0x0066cc, 	0x0066ff, 	0x009900, 	0x009933, 
	0x009966, 	0x009999, 	0x0099cc, 	0x0099ff, 
	0x00cc00, 	0x00cc33, 	0x00cc66, 	0x00cc99, 
	0x00cccc, 	0x00ccff, 	0x00ff00, 	0x00ff33, 
	0x00ff66, 	0x00ff99, 	0x00ffcc, 	0x00ffff, 
	0x330000, 	0x330033, 	0x330066, 	0x330099, 
	0x3300cc, 	0x3300ff, 	0x333300, 	0x333333, 
	0x333366, 	0x333399, 	0x3333cc, 	0x3333ff, 
	0x336600, 	0x336633, 	0x336666, 	0x336699, 
	0x3366cc, 	0x3366ff, 	0x339900, 	0x339933, 
	0x339966, 	0x339999, 	0x3399cc, 	0x3399ff, 
	0x33cc00, 	0x33cc33, 	0x33cc66, 	0x33cc99, 
	0x33cccc, 	0x33ccff, 	0x33ff00, 	0x33ff33, 
	0x33ff66, 	0x33ff99, 	0x33ffcc, 	0x33ffff, 
	0x660000, 	0x660033, 	0x660066, 	0x660099, 
	0x6600cc, 	0x6600ff, 	0x663300, 	0x663333, 
	0x663366, 	0x663399, 	0x6633cc, 	0x6633ff, 
	0x666600, 	0x666633, 	0x666666, 	0x666699, 
	0x6666cc, 	0x6666ff, 	0x669900, 	0x669933, 
	0x669966, 	0x669999, 	0x6699cc, 	0x6699ff, 
	0x66cc00, 	0x66cc33, 	0x66cc66, 	0x66cc99, 
	0x66cccc, 	0x66ccff, 	0x66ff00, 	0x66ff33, 
	0x66ff66, 	0x66ff99, 	0x66ffcc, 	0x66ffff, 
	0x990000, 	0x990033, 	0x990066, 	0x990099, 
	0x9900cc, 	0x9900ff, 	0x993300, 	0x993333, 
	0x993366, 	0x993399, 	0x9933cc, 	0x9933ff, 
	0x996600, 	0x996633, 	0x996666, 	0x996699, 
	0x9966cc, 	0x9966ff, 	0x999900, 	0x999933, 
	0x999966, 	0x999999, 	0x9999cc, 	0x9999ff, 
	0x99cc00, 	0x99cc33, 	0x99cc66, 	0x99cc99, 
	0x99cccc, 	0x99ccff, 	0x99ff00, 	0x99ff33, 
	0x99ff66, 	0x99ff99, 	0x99ffcc, 	0x99ffff, 
	0xcc0000, 	0xcc0033, 	0xcc0066, 	0xcc0099, 
	0xcc00cc, 	0xcc00ff, 	0xcc3300, 	0xcc3333, 
	0xcc3366, 	0xcc3399, 	0xcc33cc, 	0xcc33ff, 
	0xcc6600, 	0xcc6633, 	0xcc6666, 	0xcc6699, 
	0xcc66cc, 	0xcc66ff, 	0xcc9900, 	0xcc9933, 
	0xcc9966, 	0xcc9999, 	0xcc99cc, 	0xcc99ff, 
	0xcccc00, 	0xcccc33, 	0xcccc66, 	0xcccc99, 
	0xcccccc, 	0xccccff, 	0xccff00, 	0xccff33, 
	0xccff66, 	0xccff99, 	0xccffcc, 	0xccffff, 
	0xff0000, 	0xff0033, 	0xff0066, 	0xff0099, 
	0xff00cc, 	0xff00ff, 	0xff3300, 	0xff3333, 
	0xff3366, 	0xff3399, 	0xff33cc, 	0xff33ff, 
	0xff6600, 	0xff6633, 	0xff6666, 	0xff6699, 
	0xff66cc, 	0xff66ff, 	0xff9900, 	0xff9933, 
	0xff9966, 	0xff9999, 	0xff99cc, 	0xff99ff, 
	0xffcc00, 	0xffcc33, 	0xffcc66, 	0xffcc99, 
	0xffcccc, 	0xffccff, 	0xffff00, 	0xffff33, 
	0xffff66, 	0xffff99, 	0xffffcc, 	0xffffff, 
	0x121212, 	0x181818, 	0x1e1e1e, 	0x242424, 
	0x2a2a2a, 	0x303030, 	0x363636, 	0x3c3c3c, 
	0x424242, 	0x484848, 	0x4e4e4e, 	0x545454, 
	0x5a5a5a, 	0x606060, 	0x666666, 	0x6c6c6c, 
	0x727272, 	0x787878, 	0x7e7e7e, 	0x848484, 
	0x8a8a8a, 	0x909090, 	0x969696, 	0x9c9c9c, 
	0xa2a2a2, 	0xa8a8a8, 	0xaeaeae, 	0xb4b4b4, 
	0xbababa, 	0xc0c0c0, 	0xc6c6c6, 	0xcccccc, 
	0xd2d2d2, 	0xd8d8d8, 	0xdedede, 	0xe4e4e4, 
	0xeaeaea, 	0xf0f0f0, 	0xf6f6f6, 	0xfcfcfc
};

const tImage  gameOver8BPP_UNCOMP=
{
	IMAGE_FMT_8BPP_UNCOMP,
	35,
	50,
	256,
	palette_gameOver8BPP_UNCOMP,
	pixel_gameOver8BPP_UNCOMP,
};


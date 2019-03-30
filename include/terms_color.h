/*
 * file name: include/terms_color.h
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Tue 05 Mar 2019 04:33:43 PM CST
 */

#ifndef _TERMS_COLOR_H
#define _TERMS_COLOR_H


#define LIGHT			1
#define NLIGHT			0

#define HLT(x, y)		"\033["#x";"#y"m"

#define RED(x)			HLT(x, 31)
#define GREEN(x)		HLT(x, 32)
#define YELLOW(x)		HLT(x, 33)
#define BLUE(x)			HLT(x, 34)
#define PURPLE(x)		HLT(x, 35)
#define HEV_GREEN(x)		HLT(x, 36)
#define WHITE(x)		HLT(x, 37)
#define DEFAULT(x)		WHITE(x)
#define RESET			"\033[0m"

#endif 

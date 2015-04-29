#pragma once
#include "Macros.h"

struct SIM_EXP Color
{
	Color(unsigned char R = 0, unsigned char G = 0, unsigned char B = 0, unsigned char A = 255);
	unsigned char R, G, B, A;

	void SetColor(unsigned char R = 0, unsigned char G = 0, unsigned char B = 0, unsigned char A = 255);
};
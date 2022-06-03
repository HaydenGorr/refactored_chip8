#pragma once
#include <cstdint>
#include <string>

class bus;

struct t_vect4{	uint8_t r, g, b, a;};

struct theme {
	std::string name;

	float p_col[4];
	float s_col[4];

	uint32_t p32, s32;

	bool operator==(const theme& rhs);

	theme(std::string inName, t_vect4 inP, t_vect4 inS) :name{ inName }
	{
		p32 = inP.r | (inP.g << 8) | (inP.b << 16) | (inP.a << 24);
		s32 = inS.r | (inS.g << 8) | (inS.b << 16) | (inS.a << 24);

		p_col[0] = inP.r/255.0f; p_col[1] = inP.g/255.0f; p_col[2] = inP.b/255.0f; p_col[3] = inP.a/255.0f;
		s_col[0] = inS.r/255.0f; s_col[1] = inS.g/255.0f; s_col[2] = inS.b/255.0f; s_col[3] = inS.a/255.0f;
	}

	void convertPcol_top32() {
		p32 = (int)(p_col[0] * 255) | ((int)(p_col[1] * 255) << 8) | ((int)(p_col[2] * 255) << 16) | ((int)(p_col[3] * 255) << 24);
	}

	void convertscol_tos32() {
		s32 = (int)(s_col[0] * 255) | ((int)(s_col[1] * 255) << 8) | ((int)(s_col[2] * 255) << 16) | ((int)(s_col[3] * 255) << 24);
	}
	theme(std::string inName, t_vect4 inP, t_vect4 inS);

	void invert();

	void convertVectTo32Bit();
};

void loadThemesFromJson(bus&);
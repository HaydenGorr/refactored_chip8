#pragma once
#include <cstdint>
#include <string>

struct t_vect4{	uint8_t r, g, b, a;};

struct theme {
	std::string name;

	t_vect4 p_col;
	t_vect4 s_col;

	uint32_t p32, s32;

	bool operator==(const theme& rhs)
	{
		return (name == rhs.name);
	}

	theme(std::string inName, t_vect4 inP, t_vect4 inS) :name{ inName }, p_col{ inP }, s_col{ inS }
	{
		p32 = inP.r | (inP.g << 8) | (inP.b << 16) | (inP.a << 24);
		s32 = inS.r | (inS.g << 8) | (inS.b << 16) | (inS.a << 24);

	}
};

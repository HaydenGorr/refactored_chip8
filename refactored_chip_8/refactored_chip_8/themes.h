#pragma once
#include <cstdint>
#include <string>

class bus;

struct t_vect4{	uint8_t r, g, b, a;};

struct theme {
	std::string name;

	t_vect4 p_col;
	t_vect4 s_col;

	uint32_t p32, s32;

	bool operator==(const theme& rhs);

	theme(std::string inName, t_vect4 inP, t_vect4 inS);

	void invert();

	void convertVectTo32Bit();
};

void loadThemesFromJson(bus&);
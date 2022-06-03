#include "themes.h"
#include "bus.h"
#include "json/json.h"

#define JSON_DLL

bool theme::operator==(const theme& rhs)
{
	return false;
}

theme::theme(std::string inName, t_vect4 inP, t_vect4 inS) : name{ inName }, p_col{ inP }, s_col{ inS }
{
	convertVectTo32Bit();
}

void theme::invert()
{
	std::swap(p_col, s_col);
	convertVectTo32Bit();
}

void theme::convertVectTo32Bit()
{
	p32 = p_col.r | (p_col.g << 8) | (p_col.b << 16) | (p_col.a << 24);
	s32 = s_col.r | (s_col.g << 8) | (s_col.b << 16) | (s_col.a << 24);
}

void loadThemesFromJson(bus& inbus)
{

	std::ifstream theFile("configs//themes.json", std::ifstream::binary);
	
	Json::Value t;

	std::string strjson;
	std::string strline;
	while (getline(theFile, strline)) {
		strjson += strline;
	}

	theFile.close();

	Json::Reader reader;
	Json::Value root;   

	if (reader.parse(strjson, root)) {
		for (int j = 0; j < root.size(); j++) {
			inbus.themes.push_back(theme(
				root[j]["name"].asString(),
				t_vect4{ (uint8_t)root[j]["primary_RGB"][0].asInt(), (uint8_t)root[j]["primary_RGB"][1].asInt(), (uint8_t)root[j]["primary_RGB"][2].asInt(), (uint8_t)root[j]["primary_RGB"][3].asInt() },
				t_vect4{ (uint8_t)root[j]["secondary_rgb"][0].asInt(), (uint8_t)root[j]["secondary_rgb"][1].asInt(), (uint8_t)root[j]["secondary_rgb"][2].asInt(), (uint8_t)root[j]["secondary_rgb"][3].asInt() }
			));
		}

	}
}
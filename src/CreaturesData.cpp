#include "CreaturesData.h"

#include <fstream>
#include <sstream>

using namespace std;

namespace 
{
	vector<string> split(const string& str, char delimiter)
	{
		vector<string> ret;
		stringstream ss(str);

		string substr;
		while (getline(ss, substr, delimiter))
		{
			if (substr.empty())
				continue;
			ret.push_back(substr);
		}

		return ret;
	}
	
	int string_to_int(const string& str)
	{
		int ret = 0;
		stringstream ss(str);
		ss >> ret;
		return ret;
	}

	float string_to_float(const string& str)
	{
		float ret = 0;
		stringstream ss(str);
		ss >> ret;
		return ret;
	}

	const string NA("N/A");
}

void CreaturesData::Init(const std::string & filename)
{
	ifstream fin(filename);

	datas.clear();
	propertyList.clear();

	string line;
	getline(fin, line);

	propertyList = split(line, '\t');

	if (propertyList.empty() || propertyList[0] != "ID")
	{
		fin.close();
		return;
	}

	while (getline(fin, line))
	{
		vector<string> cols = split(line, '\t');
		if (cols.size() != propertyList.size())
			continue;

		int id = string_to_int(cols[0]);

		kvtable_t table;
		for (size_t i = 0; i < cols.size(); ++i)
		{
			table.insert({ propertyList[i], cols[i] });
		}

		datas.insert({ id, table });
	}
	
	fin.close();
}

CreaturesData* CreaturesData::_instance = nullptr;

const string& CreaturesData::GetStringValue(int creatureId, const std::string & property)
{
	auto iter = datas.find(creatureId);
	if (iter == datas.end())
		return NA;

	kvtable_t& table = iter->second;

	auto iter2 = table.find(property);
	if (iter2 == table.end())
		return NA;

	return table[property];
}

int CreaturesData::GetIntValue(int creatureId, const std::string & property)
{
	string value = GetStringValue(creatureId, property);
	if (value == NA || value.empty())
		return 0;
	return string_to_int(value);
}

float CreaturesData::GetFloatValue(int creatureId, const std::string & property)
{
	string value = GetStringValue(creatureId, property);
	if (value == NA || value.empty())
		return 0.0f;
	return string_to_float(value);
}


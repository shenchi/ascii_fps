#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class CreaturesData
{
public:
	inline static CreaturesData* instance()
	{
		if (nullptr == _instance)
		{
			_instance = new CreaturesData();
		}
		return _instance;
	}
public:

	void				Init(const std::string& filename);

	const std::string&	GetStringValue(int creatureId, const std::string& property);
	int					GetIntValue(int creatureId, const std::string& property);
	float				GetFloatValue(int creatureId, const std::string& property);

private:
	CreaturesData() = default;

private:

	typedef std::unordered_map<std::string, std::string>	kvtable_t;
	typedef std::unordered_map<int, kvtable_t>				datatable_t;

	datatable_t					datas;
	std::vector<std::string>	propertyList;

private:
	static CreaturesData* _instance;
};
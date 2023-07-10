#include "stdafx.h"
#include "ZombieTable.h"
#include "rapidcsv.h"

bool ZombieTable::Load()
{
	std::string filenames;
	filenames = "tables/ZombieTable.csv";

	rapidcsv::Document doc(filenames);
	std::vector<std::string> types = doc.GetColumn<std::string>(0);
	std::vector<std::string> textureIds = doc.GetColumn<std::string>(1);
	std::vector<float> speeds = doc.GetColumn<float>(2);
	std::vector<int> maxHps = doc.GetColumn<int>(3);
	std::vector<int> damages = doc.GetColumn<int>(4);
	std::vector<float> attackRates = doc.GetColumn<float>(5);

	for (int i = 0; i < types.size(); i++)
	{
		ZombieInfo newZom;
		newZom.zombieType = (Zombie::Types)i;
		newZom.textureId = textureIds[i];
		newZom.speed = speeds[i];
		newZom.maxHp = maxHps[i];
		newZom.damage = damages[i];
		newZom.attackRate = attackRates[i];
		table.insert({newZom.zombieType, newZom });
	}
	return true;
}

void ZombieTable::Release()
{
	table.clear();
}

const ZombieInfo& ZombieTable::Get(Zombie::Types id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return ;
	}
	return find->second;
}


#pragma once
#include "../GameEngine/Resources/Textures/Texture.h"
#include "../GameEngine/Utils/Utils.h"

static const glm::vec3 hand_card_size(-0.2, -0.5, 1);
static const glm::vec3 select_card_size(-0.2, -0.5, 1);
static const glm::vec3 table_card_size(select_card_size / 4.5f);

enum class LineTypes
{
	CLOSE_COMBAT = 0,
	ARCHEER,
	BALIST
};

enum class Skills
{
	MoraleBoost,
	ScorchALL,
	Scorch,
	Spy,
	Hero,
	TightBond,
	Medic,
	Agile,
	Muster
};

struct SCard
{
	SCard() 
	{
		static int s_Id = 0;
		m_Id = s_Id++;
	}
	int m_Id = 0;
	std::string name;
	CTexture* texture = nullptr;
	//read xml texture path and after loading
	std::string texture_path;
	glm::vec3 position;
	glm::vec3 scale;
	LineTypes type;
	int strength = 0;
	std::vector<Skills> skills;

	bool FindSkill(Skills skill) const
	{
		return std::find(std::begin(skills), std::end(skills), skill) != std::end(skills);
	}
};
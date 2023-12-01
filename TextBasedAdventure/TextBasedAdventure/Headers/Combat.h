#pragma once

#include <string>
#include <cstdlib>

#include "Graphics.h"

struct Attack
{
	float damage = 2;
	float accuracy = 1.0f;
};

struct Skill
{
	float damageOther = 0;
	float buffAccuracy = 0;
	float heal = 0;
	float manaCost;
};

struct EntityStats
{
	float health = 10;
	float mana = 5, manaRegen = 1;
	float armour = 0;
	float accuracyBuff = 1;
};

std::string CalculateAttack(EntityStats* entity, EntityStats* other, Attack* attack);

std::string PerformSkill(EntityStats* entity, EntityStats* other, Skill* skill);

enum class PlayerSkills
{
	// starting skills for each 3 classes
	Heal,
	Lightning,
	Focus,

	// additional skills they can learn
	Parry, // if the enemy does an attack next turn, block and deal double their damage
};


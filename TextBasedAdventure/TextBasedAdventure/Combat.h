#pragma once


// enum for player abilities
enum class PlayerAttacks
{
	// starting attacks for each 3 classes
	Slash,
	Poke,
	Shoot,

	// additional attacks they can learn
	Stab, // does bleed damage
	Pummel, // makes them less agile
};

enum class PlayerSkills
{
	// starting skills for each 3 classes
	Heal,
	Lightning,
	Focus,

	// additional skills they can learn
	Parry, // if the enemy does an attack next turn, block and deal double their damage
};
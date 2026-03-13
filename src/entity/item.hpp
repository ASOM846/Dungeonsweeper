#pragma once

class Grid;
struct PlayerStats;

struct Item {
  public:
	enum class ItemType {
		None,
		HpUp,
		EvolutionUp,
		Uncover2x2,
	};

	Item(const ItemType &type_ = ItemType::None, const int &price_ = 0,
		 const int val_ = 0, const bool selectable_ = false)
		: type{type_}, price{price_}, val{val_}, selectable{selectable_} {}

	ItemType type;
	int price;
	int val;
	bool selectable;

	void ApplyEffect(Grid &grid, PlayerStats &playerStats);
};

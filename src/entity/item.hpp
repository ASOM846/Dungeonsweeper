#pragma once

struct Item {
	enum class ItemType {
		None,
		HpUp,
		EvolutionUp,
		Uncover2x2,
	};

	Item(const ItemType &type_ = ItemType::None, const int &price_ = 0,
		 const int val_ = 0)
		: type{type_}, price{price_}, val{val_} {}

	ItemType type;
	int price;
	int val;
};

#include "ItemChooser.hpp"
#include <iostream>
#include <memory>
#include <raylib.h>

std::unique_ptr<PassiveItem> ItemChooser::CreateRegen() {
	return std::make_unique<Regen>();
}

std::unique_ptr<PassiveItem> ItemChooser::CreatePointsToEvo() {
	return std::make_unique<PointsToEvo>();
}

std::unique_ptr<PassiveItem> ItemChooser::CreateUncoverRandomRare() {
	return std::make_unique<UncoverRandomRare>();
}

ItemChooser::ItemChooser() {}

ItemChooser::~ItemChooser() {}

void ItemChooser::Init() {
	allFactories.clear();
	allFactories.push_back(&ItemChooser::CreateRegen);
	allFactories.push_back(&ItemChooser::CreatePointsToEvo);
	allFactories.push_back(&ItemChooser::CreateUncoverRandomRare);

	BuildWeightedPool();
	RollChoices();
}

void ItemChooser::Update(PlayerStats &playerStats) {
	if (!playerStats.isChoosePending)
		return;
	if (availableItems.empty())
		RollChoices();

	const int cardW = 170;
	const int cardH = 64;
	const int gap = 32;
	const int baseX = (GetScreenWidth() - (numberOfChooses * cardW +
										   (numberOfChooses - 1) * gap)) /
					  2;
	const int baseY = GetScreenHeight() / 2 - cardH / 2;

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Vector2 mouse = GetMousePosition();
		for (int i = 0; i < numberOfChooses; ++i) {
			int x = baseX + i * (cardW + gap);
			int y = baseY;
			Rectangle rect{(float)x, (float)y, (float)cardW, (float)cardH};
			if (CheckCollisionPointRec(mouse, rect) &&
				i < (int)availableItems.size() && availableItems[i]) {
				// If inventory not full, just add
				if ((int)playerStats.passiveItems.size() <
					playerStats.inventorySize) {
					playerStats.PushBackPassiveItems(
						std::move(availableItems[i]));
					playerStats.isChoosePending = false;
					RollChoices();
				} else {
					replaceCandidate = i;
					selectingReplacement = true;
				}
				return;
			}
		}
		if (selectingReplacement &&
			playerStats.passiveItems.size() >= playerStats.inventorySize) {
			int eqY = baseY + cardH + 40;
			for (size_t i = 0; i < playerStats.passiveItems.size(); ++i) {
				int x = baseX + i * (cardW + gap);
				int y = eqY;
				Rectangle rect{(float)x, (float)y, (float)cardW, (float)cardH};
				if (CheckCollisionPointRec(mouse, rect)) {
					playerStats.passiveItems[i] =
						std::move(availableItems[replaceCandidate]);
					playerStats.isChoosePending = false;
					RollChoices();
					selectingReplacement = false;
					return;
				}
			}
			int rejectX =
				baseX + playerStats.passiveItems.size() * (cardW + gap);
			int rejectY = eqY;
			Rectangle rejectRect{(float)rejectX, (float)rejectY, (float)cardW,
								 (float)cardH};
			if (CheckCollisionPointRec(mouse, rejectRect)) {
				playerStats.isChoosePending = false;
				RollChoices();
				selectingReplacement = false;
				return;
			}
		}
	}
}

void ItemChooser::Render(PlayerStats &playerStats) {
	if (!playerStats.isChoosePending)
		return;
	const int cardW = 170;
	const int cardH = 64;
	const int gap = 32;
	const int baseX = (GetScreenWidth() - (numberOfChooses * cardW +
										   (numberOfChooses - 1) * gap)) /
					  2;
	const int baseY = GetScreenHeight() / 2 - cardH / 2;
	auto itemLabel = [](PassiveItem::PassiveType t) -> const char * {
		switch (t) {
		case PassiveItem::PassiveType::Regen:
			return "+2 HP";
		case PassiveItem::PassiveType::PointsToEvo:
			return "+1 EVO";
		case PassiveItem::PassiveType::UncoverRandom:
			return "uncover random cell";
		default:
			return "";
		}
	};

	auto itemColor = [](PassiveItem::PassiveType t) -> Color {
		switch (t) {
		case PassiveItem::PassiveType::Regen:
			return Color{185, 70, 70, 255};
		case PassiveItem::PassiveType::PointsToEvo:
			return Color{90, 170, 110, 255};
		case PassiveItem::PassiveType::UncoverRandom:
			return GREEN;
		default:
			return Color{100, 100, 100, 255};
		}
	};

	// Draw choices
	for (int i = 0; i < numberOfChooses; ++i) {
		int x = baseX + i * (cardW + gap);
		int y = baseY;
		Color bg = Color{34, 30, 26, 255};
		Color border = Color{120, 96, 72, 255};
		Color accent = Color{80, 80, 80, 255};
		const char *label = "-";
		if (i < (int)availableItems.size() && availableItems[i]) {
			auto *item = availableItems[i].get();
			accent = itemColor(item->GetType());
			label = itemLabel(item->GetType());
		}
		DrawRectangleRounded(
			Rectangle{(float)x, (float)y, (float)cardW, (float)cardH}, 0.18f, 8,
			bg);
		DrawRectangleLinesEx(
			Rectangle{(float)x, (float)y, (float)cardW, (float)cardH}, 2.0f,
			border);
		DrawRectangle(x + 14, y + 12, 40, 40, accent);
		DrawText(TextFormat("%d", i + 1), x + 4, y + 4, 18,
				 Color{180, 160, 135, 255});
		DrawText(label, x + 62, y + 22, 22, Color{220, 200, 170, 255});
	}

	// Draw current equipment below choices
	int eqY = baseY + cardH + 40;
	for (size_t i = 0; i < playerStats.passiveItems.size(); ++i) {
		int x = baseX + i * (cardW + gap);
		int y = eqY;
		Color bg = Color{34, 30, 26, 255};
		Color border = (selectingReplacement && (int)i == replaceCandidate)
						   ? Color{185, 70, 70, 255}
						   : Color{120, 96, 72, 255};
		Color accent = Color{80, 80, 80, 255};
		const char *label = "-";
		if (playerStats.passiveItems[i]) {
			auto *item = playerStats.passiveItems[i].get();
			accent = itemColor(item->GetType());
			label = itemLabel(item->GetType());
		}
		DrawRectangleRounded(
			Rectangle{(float)x, (float)y, (float)cardW, (float)cardH}, 0.18f, 8,
			bg);
		DrawRectangleLinesEx(
			Rectangle{(float)x, (float)y, (float)cardW, (float)cardH}, 2.0f,
			border);
		DrawRectangle(x + 14, y + 12, 40, 40, accent);
		DrawText(TextFormat("%d", (int)i + 1), x + 4, y + 4, 18,
				 Color{180, 160, 135, 255});
		DrawText(label, x + 62, y + 22, 22, Color{220, 200, 170, 255});
	}

	// Draw "Reject" button if inventory is full and waiting for replacement
	if (selectingReplacement &&
		playerStats.passiveItems.size() >= playerStats.inventorySize) {
		int rejectX = baseX + playerStats.passiveItems.size() * (cardW + gap);
		int rejectY = eqY;
		Color bg = Color{55, 55, 55, 180};
		Color border = Color{185, 70, 70, 255};
		DrawRectangleRounded(Rectangle{(float)rejectX, (float)rejectY,
									   (float)cardW, (float)cardH},
							 0.18f, 8, bg);
		DrawRectangleLinesEx(Rectangle{(float)rejectX, (float)rejectY,
									   (float)cardW, (float)cardH},
							 2.0f, border);
		DrawText("Reject", rejectX + 24, rejectY + cardH / 2 - 10, 22,
				 Color{220, 200, 170, 255});
	}
}

int ItemChooser::SpawnRateToWeight(PassiveItem::SpawnRate rate) const {
	switch (rate) {
	case PassiveItem::Common:
		return 60;
	case PassiveItem::Medium:
		return 25;
	case PassiveItem::Rare:
		return 10;
	case PassiveItem::UltraRare:
		return 5;
	default:
		return 1;
	}
}

void ItemChooser::BuildWeightedPool() {
	weightedPool.clear();

	for (auto factory : allFactories) {
		auto probe = factory(); // tworzymy chwilowo, żeby odczytać Rate
		const int weight = SpawnRateToWeight(probe->Rate);

		for (int i = 0; i < weight; ++i) {
			weightedPool.push_back(factory);
		}
	}
}

void ItemChooser::RollChoices() {
	availableItems.clear();

	if (weightedPool.empty())
		return;

	for (int i = 0; i < numberOfChooses; ++i) {
		const int idx = GetRandomValue(0, (int)weightedPool.size() - 1);
		auto factory = weightedPool[idx];
		availableItems.push_back(factory());
	}
}

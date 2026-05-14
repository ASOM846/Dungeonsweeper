#include "ItemChooser.hpp"
#include <algorithm>
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

std::unique_ptr<PassiveItem> ItemChooser::CreateAddKey() {
	return std::make_unique<AddChestKey>();
}

ItemChooser::ItemChooser() {}

ItemChooser::~ItemChooser() {}

void ItemChooser::Init() {
	allFactories.clear();
	allFactories.push_back(&ItemChooser::CreateRegen);
	allFactories.push_back(&ItemChooser::CreatePointsToEvo);
	allFactories.push_back(&ItemChooser::CreateUncoverRandomRare);
	allFactories.push_back(&ItemChooser::CreateAddKey);

	BuildWeightedPool();
	RollChoices();
}

void ItemChooser::Update(PlayerStats &playerStats) {
	if (!playerStats.isChoosePending)
		return;
	if (availableItems.empty())
		RollChoices();

	const int cardW = 230;
	const int cardH = 80;
	const int gap = 40;
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
			int eqY = baseY + cardH + 70;
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

	const int cardW = 230;
	const int cardH = 80;
	const int gap = 40;
	const int baseX = (GetScreenWidth() - (numberOfChooses * cardW +
										   (numberOfChooses - 1) * gap)) /
					  2;
	const int baseY = GetScreenHeight() / 2 - cardH / 2;

	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
				  Color{0, 0, 0, 180});

	DrawText("Select a new item:", baseX, baseY - 35, 22,
			 Color{220, 200, 170, 255});

	for (int i = 0; i < numberOfChooses; ++i) {
		int x = baseX + i * (cardW + gap);
		int y = baseY;

		Color bg = Color{38, 34, 30, 255};
		Color border = Color{140, 115, 85, 255};
		Color accent = Color{80, 80, 80, 255};

		const char *descLabel = "Empty";
		int turns = 0;

		if (i < (int)availableItems.size() && availableItems[i]) {
			auto *item = availableItems[i].get();
			accent = item->color;
			descLabel = item->desc.c_str();
			turns = item->turnsToActivate;
		}

		DrawRectangleRounded(
			Rectangle{(float)x + 4, (float)y + 4, (float)cardW, (float)cardH},
			0.18f, 8, Color{0, 0, 0, 120});
		DrawRectangleRounded(
			Rectangle{(float)x, (float)y, (float)cardW, (float)cardH}, 0.18f, 8,
			bg);
		DrawRectangleLinesEx(
			Rectangle{(float)x, (float)y, (float)cardW, (float)cardH}, 2.0f,
			border);

		DrawRectangleRounded(Rectangle{(float)x + 12, (float)y + 12, 56, 56},
							 0.2f, 4, accent);
		DrawRectangleLinesEx(Rectangle{(float)x + 12, (float)y + 12, 56, 56},
							 1.0f, Color{255, 255, 255, 30});

		DrawText(TextFormat("%d", i + 1), x + 8, y + 6, 12,
				 Color{180, 160, 135, 150});

		DrawText(descLabel, x + 80, y + 22, 20, Color{240, 230, 210, 255});
		if (turns > 0) {
			DrawText(TextFormat("Every %d turns", turns), x + 80, y + 46, 16,
					 Color{160, 145, 125, 255});
		} else {
			DrawText("Passive", x + 80, y + 46, 16, Color{160, 145, 125, 255});
		}
	}

	int eqY = baseY + cardH + 70;
	if (!playerStats.passiveItems.empty() || selectingReplacement) {
		DrawText("Current inventory:", baseX, eqY - 35, 22,
				 Color{220, 200, 170, 255});
	}

	for (size_t i = 0; i < playerStats.passiveItems.size(); ++i) {
		int x = baseX + i * (cardW + gap);
		int y = eqY;

		bool isTargeted = (selectingReplacement && (int)i == replaceCandidate);
		Color bg = isTargeted ? Color{60, 30, 30, 255} : Color{38, 34, 30, 255};
		Color border =
			isTargeted ? Color{220, 80, 80, 255} : Color{100, 80, 60, 255};
		Color accent = Color{80, 80, 80, 255};

		const char *descLabel = "Empty";
		int turns = 0;

		if (playerStats.passiveItems[i]) {
			auto *item = playerStats.passiveItems[i].get();
			accent = item->color;
			descLabel = item->desc.c_str();
			turns = item->turnsToActivate;
		}

		DrawRectangleRounded(
			Rectangle{(float)x + 4, (float)y + 4, (float)cardW, (float)cardH},
			0.18f, 8, Color{0, 0, 0, 120});
		DrawRectangleRounded(
			Rectangle{(float)x, (float)y, (float)cardW, (float)cardH}, 0.18f, 8,
			bg);
		DrawRectangleLinesEx(
			Rectangle{(float)x, (float)y, (float)cardW, (float)cardH},
			isTargeted ? 3.0f : 2.0f, border);

		DrawRectangleRounded(Rectangle{(float)x + 12, (float)y + 12, 56, 56},
							 0.2f, 4, accent);
		DrawRectangleLinesEx(Rectangle{(float)x + 12, (float)y + 12, 56, 56},
							 1.0f, Color{255, 255, 255, 30});

		DrawText(TextFormat("%d", (int)i + 1), x + 8, y + 6, 12,
				 Color{180, 160, 135, 150});

		DrawText(descLabel, x + 80, y + 22, 20, Color{200, 190, 170, 255});
		if (turns > 0) {
			DrawText(TextFormat("Every %d turns", turns), x + 80, y + 46, 16,
					 Color{130, 120, 105, 255});
		}
	}

	if (selectingReplacement &&
		playerStats.passiveItems.size() >= playerStats.inventorySize) {
		int rejectX = baseX + playerStats.passiveItems.size() * (cardW + gap);
		int rejectY = eqY;

		Color bg = Color{45, 35, 35, 255};
		Color border = Color{200, 80, 80, 255};

		DrawRectangleRounded(Rectangle{(float)rejectX + 4, (float)rejectY + 4,
									   (float)cardW, (float)cardH},
							 0.18f, 8, Color{0, 0, 0, 120});
		DrawRectangleRounded(Rectangle{(float)rejectX, (float)rejectY,
									   (float)cardW, (float)cardH},
							 0.18f, 8, bg);
		DrawRectangleLinesEx(Rectangle{(float)rejectX, (float)rejectY,
									   (float)cardW, (float)cardH},
							 2.0f, border);

		int textW = MeasureText("Reject Item", 22);
		DrawText("Reject Item", rejectX + (cardW - textW) / 2,
				 rejectY + (cardH - 22) / 2, 22, Color{240, 180, 180, 255});
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
		auto probe = factory();
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

	std::vector<FactoryFn> usedFactories;

	for (int i = 0; i < numberOfChooses; ++i) {

		FactoryFn factory = nullptr;

		do {
			const int idx = GetRandomValue(0, (int)weightedPool.size() - 1);

			factory = weightedPool[idx];

		} while (std::find(usedFactories.begin(), usedFactories.end(),
						   factory) != usedFactories.end());

		usedFactories.push_back(factory);

		availableItems.push_back(factory());
	}
}

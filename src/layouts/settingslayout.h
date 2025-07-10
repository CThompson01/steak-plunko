#ifndef SETTINGSLAYOUT_H
#define SETTINGSLAYOUT_H
#include "../uielements.h"

typedef struct videotablayout {
	UIElement aspectSelector;
	UIElement resolutionSelector;
	UIElement fullscreenSelector;
	UIElement *layout[3];
} VideoTabLayout;

typedef struct audiotablayout {
	UIElement masterSelector;
	UIElement sfxSelector;
	UIElement musicSelector;
	UIElement *layout[3];
} AudioTabLayout;

typedef struct gametablayout {
	UIElement gamestyleSelector;
	UIElement infiniteMoneySelector;
	UIElement *layout[2];
} GameTabLayout;

typedef struct settingslayout {
	int activeTab;
	VideoTabLayout videoTab;
	AudioTabLayout audioTab;
	GameTabLayout gameLayout;
	UIButton applyButton;
	UIButton closeButton;
} SettingsLayout;

void settingslayout_test();
void initialize_settingslayout(SettingsLayout *settingsLayout);
void resize_settingslayout(SettingsLayout *settingsLayout);

#endif
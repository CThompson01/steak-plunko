#ifndef SETTINGSLAYOUT_H
#define SETTINGSLAYOUT_H
#include "../uielements.h"

typedef struct videotablayout {
	UIElement aspectSelector;
	UIElement resolutionSelector;
	UIElement fullscreenSelector;
	int num_elements;
	UIElement *layout[3];
} VideoTabLayout;

typedef struct audiotablayout {
	UIElement masterSelector;
	UIElement sfxSelector;
	UIElement musicSelector;
	int num_elements;
	UIElement *layout[3];
} AudioTabLayout;

typedef struct gametablayout {
	UIElement gamestyleSelector;
	UIElement infiniteMoneySelector;
	int num_elements;
	UIElement *layout[2];
} GameTabLayout;

typedef struct backdroplayout {
	int x;
	int y;
	int width;
	int height;
} BackdropLayout;

typedef struct settingslayout {
	int activeTab;
	BackdropLayout backdropLayout;
	VideoTabLayout videoTab;
	AudioTabLayout audioTab;
	GameTabLayout gameTab;
	UIButton applyButton;
	UIButton closeButton;
} SettingsLayout;

void settingslayout_test();
void initialize_settingslayout(SettingsLayout *settingsLayout);
void resize_settingslayout(SettingsLayout *settingsLayout);

#endif
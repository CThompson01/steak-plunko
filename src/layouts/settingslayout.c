#include "settingslayout.h"
#include "../settings.h"
#include "../uielements.h"
#include <stdio.h>

#define DEFAULT_WIDTH 480
#define DEFAULT_HEIGHT 854

void settingslayout_test() {
	ResolutionSettings t_resolutionSettings;
	initialize_resolutionsettings(&t_resolutionSettings);
	int t_numLabels = 0;
	char** t_sixteenByNineLabels = get_resolution_labels(t_resolutionSettings, SIXTEEN_BY_NINE, &t_numLabels);
	printf("Test printing labels, this should print 1920x1080: %s\n", t_sixteenByNineLabels[3]);

	printf("Testing creating a settings layout...\n");
	SettingsLayout t_settingsLayout;
	initialize_settingslayout(&t_settingsLayout);

	printf("Printing first option in resolution selector, should be 1280x720: %s\n",
		t_settingsLayout.videoTab.resolutionSelector.element.scrollSelector->options[0]);
	printf("Update options list\n");
	ChangeScrollSelectorOptions(t_settingsLayout.videoTab.resolutionSelector.element.scrollSelector,
		t_sixteenByNineLabels, t_numLabels);
	printf("Printing first option in resolution selector, should be 854x480: %s\n",
		t_settingsLayout.videoTab.resolutionSelector.element.scrollSelector->options[0]);

	printf("Updating options list to use 9:16 aspect ratio\n");
	t_sixteenByNineLabels = get_resolution_labels(t_resolutionSettings, NINE_BY_SIXTEEN, &t_numLabels);
	ChangeScrollSelectorOptions(t_settingsLayout.videoTab.resolutionSelector.element.scrollSelector,
		t_sixteenByNineLabels, t_numLabels);
	printf("Printing first option in resolution selector, should be 480x854: %s\n",
		t_settingsLayout.videoTab.resolutionSelector.element.scrollSelector->options[0]);

	printf("Settings layout test finished!\n");
}

// Initializes the layout for the settings modal assuming a default screen size
void initialize_settingslayout(SettingsLayout *settingsLayout) {
	// Initialize the basic layout stuff
	*settingsLayout = (SettingsLayout) {
		.activeTab = 0,
		.backdropLayout = (BackdropLayout) {0, 0, 0, 0},
		.videoTab = (VideoTabLayout) {
			.aspectSelector = CreateScrollSelectorElement("aspect_selector", "Aspect Ratio",
				(char*[]) {"9:16", "16:9"}, 2,
				-1, -1),
			.resolutionSelector = CreateScrollSelectorElement("resolution_selector", "Resolution",
				(char*[]) {"1280x720", "1518x854", "1920x1080"}, 3,
				-1, -1),
			.fullscreenSelector = CreateScrollSelectorElement("fullscreen_selector", "Fullscreen",
				(char*[]) {"No", "Yes"}, 2,
				-1, -1),
			.num_elements = 3
		},
		.audioTab = (AudioTabLayout) {
			.masterSelector = CreateScrollSelectorElement("mastervolume_selector", "Master Volume",
				(char*[]) {"Off", "On"}, 2,
				-1, -1),
			.sfxSelector = CreateScrollSelectorElement("sfxvolume_selector", "SFX Volume",
				(char*[]) {"Off", "On"}, 2,
				-1, -1),
			.musicSelector = CreateScrollSelectorElement("musicvolume_selector", "Music Volume",
				(char*[]) {"Off", "On"}, 2,
				-1, -1),
			.num_elements = 3
		},
		.gameTab = (GameTabLayout) {
			.gamestyleSelector = CreateScrollSelectorElement("gamestyle_selector", "Game Style",
				(char*[]) {"Physics", "Classic"}, 2,
				-1, -1),
			.infiniteMoneySelector = CreateScrollSelectorElement("infinitemoney_selector", "Infinite Money",
				(char*[]) {"Off", "On"}, 2,
				-1, -1),
			.num_elements = 2
		},
		.applyButton = {0},
		.closeButton = CreateButton("Test Close",
			0, 225,
			200, 50)
	};
	initialize_uibutton(&settingsLayout->applyButton, "Test Apply", 0, 0, 200, 50);

	// Set up tab layouts to have list to all elements
	settingsLayout->videoTab.layout[0] = &settingsLayout->videoTab.aspectSelector;
	settingsLayout->videoTab.layout[1] = &settingsLayout->videoTab.resolutionSelector;
	settingsLayout->videoTab.layout[2] = &settingsLayout->videoTab.fullscreenSelector;

	settingsLayout->audioTab.layout[0] = &settingsLayout->audioTab.masterSelector;
	settingsLayout->audioTab.layout[1] = &settingsLayout->audioTab.sfxSelector;
	settingsLayout->audioTab.layout[2] = &settingsLayout->audioTab.musicSelector;

	settingsLayout->gameTab.layout[0] = &settingsLayout->gameTab.gamestyleSelector;
	settingsLayout->gameTab.layout[1] = &settingsLayout->gameTab.infiniteMoneySelector;

	resize_settingslayout(settingsLayout, DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

void resize_settingslayout(SettingsLayout *settingsLayout, int width, int height) {
	VideoTabLayout *videoTab = &settingsLayout->videoTab;
	AudioTabLayout *audioTab = &settingsLayout->audioTab;
	GameTabLayout *gameTab = &settingsLayout->gameTab;

	// Resize Video Tab
	const int t_padding = 25;
	int t_heightOfTabElementsBlock = AutoLayout_Centered(videoTab->layout, videoTab->num_elements, width, height, t_padding);
	int largestBlock = t_heightOfTabElementsBlock;

	// Resize Audio Tab
	t_heightOfTabElementsBlock = AutoLayout_Centered(audioTab->layout, audioTab->num_elements, width, height, t_padding);
	if (t_heightOfTabElementsBlock > largestBlock) {
		largestBlock = t_heightOfTabElementsBlock;
	}

	// Resize Game Tab
	t_heightOfTabElementsBlock = AutoLayout_Centered(gameTab->layout, gameTab->num_elements, width, height, t_padding);
	if (t_heightOfTabElementsBlock > largestBlock) {
		largestBlock = t_heightOfTabElementsBlock;
	}

	// Resize backdrop
	settingsLayout->backdropLayout.x = 0;
	settingsLayout->backdropLayout.y = 0;
	settingsLayout->backdropLayout.width = width - 50;
	settingsLayout->backdropLayout.height = largestBlock;
}
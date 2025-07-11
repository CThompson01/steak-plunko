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
	// Resize video tab
	VideoTabLayout *videoTab = &settingsLayout->videoTab;

	// Calculate the height of the tab elements to determine how far the screen offsets should be
	const int t_padding = 25;
	const int t_heightOfSelector = 58;
	int t_numElements = videoTab->num_elements;
	int t_heightOfTabElementsBlock = (t_padding * (t_numElements+1)) + (t_heightOfSelector * t_numElements);
	int largestBlock = t_heightOfTabElementsBlock;
	int t_firstElementOffset_y = (height - t_heightOfTabElementsBlock)/2;
	int t_otherElementHeights = 0;
	ScrollSelector *currElement;
	for (int i = 0; i < videoTab->num_elements; i++) {
		currElement = videoTab->layout[i]->element.scrollSelector;
		currElement->x = width/2;
		currElement->y = t_firstElementOffset_y + t_otherElementHeights + t_padding;
		t_otherElementHeights += (t_heightOfSelector + t_padding);
	}

	// Resize audio tab
	AudioTabLayout *audioTab = &settingsLayout->audioTab;
	t_numElements = audioTab->num_elements;
	t_heightOfTabElementsBlock = (t_padding * (t_numElements+1)) + (t_heightOfSelector * t_numElements);
	t_firstElementOffset_y = (height - t_heightOfTabElementsBlock)/2;
	t_otherElementHeights = 0;

	if (t_heightOfTabElementsBlock > largestBlock) {
		largestBlock = t_heightOfTabElementsBlock;
	}

	for (int i = 0; i < audioTab->num_elements; i++) {
		currElement = audioTab->layout[i]->element.scrollSelector;
		currElement->x = width/2;
		currElement->y = t_firstElementOffset_y + t_otherElementHeights + t_padding;
		t_otherElementHeights += (t_heightOfSelector + t_padding);
	}

	// Resize game tab
	GameTabLayout *gameTab = &settingsLayout->gameTab;
	t_numElements = gameTab->num_elements;
	t_heightOfTabElementsBlock = (t_padding * (t_numElements+1)) + (t_heightOfSelector * t_numElements);
	t_firstElementOffset_y = (height - t_heightOfTabElementsBlock)/2;
	t_otherElementHeights = 0;

	if (t_heightOfTabElementsBlock > largestBlock) {
		largestBlock = t_heightOfTabElementsBlock;
	}

	for (int i = 0; i < gameTab->num_elements; i++) {
		currElement = gameTab->layout[i]->element.scrollSelector;
		currElement->x = width/2;
		currElement->y = t_firstElementOffset_y + t_otherElementHeights + t_padding;
		t_otherElementHeights += (t_heightOfSelector + t_padding);
	}

	// Resize backdrop
	settingsLayout->backdropLayout.x = 0;
	settingsLayout->backdropLayout.y = 0;
	settingsLayout->backdropLayout.width = width - 50;
	settingsLayout->backdropLayout.height = largestBlock;
}
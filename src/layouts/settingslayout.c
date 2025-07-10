#include "settingslayout.h"
#include "../settings.h"
#include "../uielements.h"
#include <stdio.h>

#define DEFAULT_WIDTH 480
#define DEFAULT_HEIGHT 854

void settingslayout_test() {
	ResolutionSettings t_resolutionSettings;
	initialize_resolutionsettings(&t_resolutionSettings);
	char** t_sixteenByNineLabels = get_resolution_labels(&t_resolutionSettings);
	printf("Test printing labels, this should print 1920x1080: %s\n", t_sixteenByNineLabels[3]);

	printf("Testing creating a settings layout...\n");
	SettingsLayout t_settingsLayout;
	initialize_settingslayout(&t_settingsLayout);
	printf("Printing first option in resolution selector: %s\n",
		t_settingsLayout.videoTab.resolutionSelector.element.scrollSelector->options[0]);
	printf("Update options list\n");
	t_settingsLayout.videoTab.resolutionSelector.element.scrollSelector->options = get_resolution_labels(&t_resolutionSettings);
	printf("Printing first option in resolution selector: %s\n",
		t_settingsLayout.videoTab.resolutionSelector.element.scrollSelector->options[0]);
	printf("Settings layout test finished!\n");
}

// Initializes the layout for the settings modal assuming a default screen size
void initialize_settingslayout(SettingsLayout *settingsLayout) {
	*settingsLayout = (SettingsLayout) {
		.activeTab = 0,
		.videoTab = (VideoTabLayout) {
			.aspectSelector = CreateScrollSelectorElement("aspect_selector", "Aspect Ratio",
				(char*[]) {"16:9", "3:4"}, 2,
				-1, -1),
			.resolutionSelector = CreateScrollSelectorElement("resolution_selector", "Resolution",
				(char*[]) {"1280x720", "1518x854", "1920x1080"}, 3,
				-1, -1),
			.fullscreenSelector = CreateScrollSelectorElement("fullscreen_selector", "Fullscreen",
				(char*[]) {"no", "yes"}, 2,
				-1, -1)
		},
		.audioTab = (AudioTabLayout) {0},
		.gameLayout = (GameTabLayout) {0},
		.applyButton = {0},
		.closeButton = CreateButton("Test Close",
			0, 225,
			200, 50)
	};

	initialize_uibutton(&settingsLayout->applyButton, "Test Apply", 0, 0, 200, 50);
}

void resize_settingslayout(SettingsLayout *settingsLayout) {

}
#include "settings.h"
#include <string.h>
#include <stdlib.h>

void initialize_appliedgamesettings(AppliedGameSettings *gameSettings) {
	*gameSettings = (AppliedGameSettings) {
		.videoSettings = (VideoSettings) {
			.selectedAspectRatio = (Resolution) {"9:16", 9, 16},
			.selectedResolution = (Resolution) {"480x854", 480, 854},
			.isFullscreen = 0
		},
		.audioSettings = (AudioSettings) {
			.muteMaster = 0,
			.muteSFX = 0,
			.muteMusic = 0
		},
		.gameSettings = (GameplaySettings) {
			.isClassic = 0,
			.infiniteMoney = 0
		}
	};
}

void initialize_resolutionsettings(ResolutionSettings *resolutionSettings) {
	*resolutionSettings = (ResolutionSettings) {
		.nineBySixteen = {(Resolution) {"480x854", 480, 854}},
		.sixteenByNine = {
			(Resolution) {"854x480", 854, 480},
			(Resolution) {"1280x720", 1280, 720},
			(Resolution) {"1518x854", 1518, 854},
			(Resolution) {"1920x1080", 1920, 1080}
		}
	};
}

char** get_resolution_labels(ResolutionSettings *resolutionSettings) {
	char** labels = malloc(4 * sizeof(char*));
	for (int i = 0; i < 4; i++) {
		labels[i] = malloc(25 * sizeof(char));
		strcpy(labels[i], resolutionSettings->sixteenByNine[i].label);
	}
	return labels;
}
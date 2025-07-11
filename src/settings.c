#include "settings.h"
#include <string.h>
#include <stdlib.h>

void initialize_appliedgamesettings(AppliedGameSettings *gameSettings) {
	*gameSettings = (AppliedGameSettings) {
		.videoSettings = (VideoSettings) {
			.selectedAspectRatio = NINE_BY_SIXTEEN,
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

char** get_resolution_labels(ResolutionSettings resolutionSettings, enum AspectRatio aspectRatio, int *num_labels) {
	if (aspectRatio == NINE_BY_SIXTEEN) {
		*num_labels = NINE_BY_SIXTEEN_AMOUNT;
		char** labels = malloc(NINE_BY_SIXTEEN_AMOUNT * sizeof(char*));
		for (int i = 0; i < NINE_BY_SIXTEEN_AMOUNT; i++) {
			labels[i] = malloc(25 * sizeof(char));
			strcpy(labels[i], resolutionSettings.nineBySixteen[i].label);
		}
		return labels;
	} else {
		*num_labels = SIXTEEN_BY_NINE_AMOUNT;
		char** labels = malloc(SIXTEEN_BY_NINE_AMOUNT * sizeof(char*));
		for (int i = 0; i < SIXTEEN_BY_NINE_AMOUNT; i++) {
			labels[i] = malloc(25 * sizeof(char));
			strcpy(labels[i], resolutionSettings.sixteenByNine[i].label);
		}
		return labels;
	}
}

Resolution get_resolution(ResolutionSettings resolutionSettings, enum AspectRatio aspectRatio, char *label) {
	if (aspectRatio == NINE_BY_SIXTEEN) {
		const int num_resolutions = NINE_BY_SIXTEEN_AMOUNT;
		for (int i = 0; i < NINE_BY_SIXTEEN_AMOUNT; i++) {
			if (!strcmp(resolutionSettings.nineBySixteen[i].label, label)) {
				return resolutionSettings.nineBySixteen[i];
			}
		}
	} else {
		const int num_resolutions = SIXTEEN_BY_NINE_AMOUNT;
		for (int i = 0; i < SIXTEEN_BY_NINE_AMOUNT; i++) {
			if (!strcmp(resolutionSettings.sixteenByNine[i].label, label)) {
				return resolutionSettings.sixteenByNine[i];
			}
		}
	}

	return resolutionSettings.sixteenByNine[SIXTEEN_BY_NINE_AMOUNT-1];
}
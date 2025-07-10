#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct resolution {
	char label[50];
	int width;
	int height;
} Resolution;

typedef struct resolutionsettings {
	Resolution nineBySixteen[1];
	Resolution sixteenByNine[4];
} ResolutionSettings;

typedef struct videosettings {
	Resolution selectedAspectRatio;
	Resolution selectedResolution;
	int isFullscreen;
} VideoSettings;

typedef struct audiosettings {
	int muteMaster;
	int muteSFX;
	int muteMusic;
} AudioSettings;

typedef struct gameplaySettings {
	int isClassic;
	int infiniteMoney;
} GameplaySettings;

typedef struct gamesettings {
	VideoSettings videoSettings;
	AudioSettings audioSettings;
	GameplaySettings gameSettings;
} AppliedGameSettings;

void initialize_appliedgamesettings(AppliedGameSettings *gameSettings);
void initialize_resolutionsettings(ResolutionSettings *resolutionSettings);
char** get_resolution_labels(ResolutionSettings *resolutionSettings);

#endif
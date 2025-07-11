#ifndef SETTINGS_H
#define SETTINGS_H
#define NINE_BY_SIXTEEN_AMOUNT 1
#define SIXTEEN_BY_NINE_AMOUNT 4

typedef struct resolution {
	char label[50];
	int width;
	int height;
} Resolution;

enum AspectRatio {
	NINE_BY_SIXTEEN,
	SIXTEEN_BY_NINE
};

typedef struct resolutionsettings {
	Resolution nineBySixteen[NINE_BY_SIXTEEN_AMOUNT];
	Resolution sixteenByNine[SIXTEEN_BY_NINE_AMOUNT];
} ResolutionSettings;

typedef struct videosettings {
	enum AspectRatio selectedAspectRatio;
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
char** get_resolution_labels(ResolutionSettings resolutionSettings, enum AspectRatio aspect_ratio, int *num_labels);
Resolution get_resolution(ResolutionSettings resolutionSettings, enum AspectRatio aspectRatio, char *label);

#endif
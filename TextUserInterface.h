/* Text User Interface with Mufflon, created by Lucas Zimerman Fraulob.


*/
#pragma once
#define bool char

struct Options {
	bool FlyBug;
	bool FlyBugMultipass;
	bool PreparePicture;
	bool UseSprites;
	bool UseGIMP;
	int Threads;
};


void OptionsMenu(struct Options *Options);

void GimpLoadingMenu(int TotalImages, char *ImageName, int Digits);

void GimpCreatingNuf(int TotalImages, Options *options);

void ThreadCompleted();
void ImageCompleted();
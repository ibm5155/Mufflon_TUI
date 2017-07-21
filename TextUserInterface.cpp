#include <MT2D/MT2D.h>
#include <MT2D/MT2D_Display.h>
#include <MT2D/MT2D_Layout.h>
#include <MT2D/MT2D_Keyboard.h>
#include <MT2D/MT2D_System_Calls.h>
#include <MT2D/Cursor/Cursor.h>
#include <string>
#include "TextUserInterface.h"
#include "StringTools.h"


#define RenderFlyBugLine 3
#define RenderFlyBugMultipassLine 4
#define PreparePictureLine 5
#define UseSpritesLine 6
#define UseGIMPLine 7
#define TotalThreadsLine 8

void MT2D_Draw_Horizontal_Load_Bar(int Max, int Current, int SizeX, int SizeY, int PosX, int PosY, bool Display, bool Shadow)
{
	//Min SizeY = 3
	//Min SizeX = 3
	//Max Higher or Equal to 1
	int Fill = PosX + ((SizeX) * Current)/Max;
	int i = PosX+1;
	int jEnd = PosY + SizeY - 1;
	int j;
	MT2D_Create_Window_Layout(PosX, PosY, SizeX, SizeY, Shadow, false, false, 176 , Display);
	while (i < Fill)
	{
		j = PosY + 1;
		while (j <= jEnd)
		{
			(Display == DISPLAY_WINDOW1 ? WINDOW1[j][i] : WINDOW2[j][i]) = 178;
			j++;
		}
		i++;
	}
}


char CharTmp[MAX_HOR];

void DrawOptionsMenu(int Selected, struct Options *options)
{
	MT2D_Cursor_Set_Position(RenderFlyBugLine + Selected, 2);

	MT2D_Create_Window_Layout(0, 0, MAX_HOR - 2, MAX_VER - 2, true, true, false, ' ', DISPLAY_WINDOW1);
	insert_string_on_display(" mufflon_TUI created by: Lucas Zimerman Fraulob", 1, 10, DISPLAY_WINDOW1);


	WINDOW1[RenderFlyBugLine][1] = '['; 
	WINDOW1[RenderFlyBugLine][2] = (options->FlyBug == true ? 'X' : ' ');
	WINDOW1[RenderFlyBugLine][3] = ']';
	insert_string_on_display("FlyBug enabled", RenderFlyBugLine, 5, DISPLAY_WINDOW1);

	WINDOW1[RenderFlyBugMultipassLine][1] = '[';
	WINDOW1[RenderFlyBugMultipassLine][2] = (options->FlyBugMultipass == true ? 'X' : ' ');
	WINDOW1[RenderFlyBugMultipassLine][3] = ']';
	insert_string_on_display("FlyBug Multipass enabled (better image but slower to create)", RenderFlyBugMultipassLine, 5, DISPLAY_WINDOW1);

	WINDOW1[PreparePictureLine][1] = '[';
	WINDOW1[PreparePictureLine][2] = (options->PreparePicture == true ? 'X' : ' ');
	WINDOW1[PreparePictureLine][3] = ']';
	insert_string_on_display("Prepare the picute to fit the Commodore 64 Palette (much slower)", PreparePictureLine, 5, DISPLAY_WINDOW1);

	WINDOW1[UseSpritesLine][1] = '[';
	WINDOW1[UseSpritesLine][2] = (options->UseSprites == true ? 'X' : ' ');
	WINDOW1[UseSpritesLine][3] = ']';
	insert_string_on_display("FlyBug Multipass enabled (better image but slower to create)", UseSpritesLine, 5, DISPLAY_WINDOW1);	
	
	WINDOW1[UseGIMPLine][1] = '[';
	WINDOW1[UseGIMPLine][2] = (options->UseGIMP == true ? 'X' : ' ');
	WINDOW1[UseGIMPLine][3] = ']';
	insert_string_on_display("Use GIMP to prepare the input images (better result, slower)", UseGIMPLine , 5, DISPLAY_WINDOW1);

	WINDOW1[UseSpritesLine][1] = '[';
	WINDOW1[UseSpritesLine][2] = (options->UseSprites == true ? 'X' : ' ');
	WINDOW1[UseSpritesLine][3] = ']';
	sprintf(CharTmp, "Number of Threads: %d (-/+ keys to edit)", options->Threads);
	insert_string_on_display(CharTmp, TotalThreadsLine, 4, DISPLAY_WINDOW1);

	insert_string_on_display("press ENTER to continue", MAX_VER - 4, 5, DISPLAY_WINDOW1);

	MT2D_Draw_Window(DISPLAY_WINDOW1);
}

void OptionsMenu(struct  Options *options)
{
	int Soption = 0;
	bool Exit = false;
	int k =0;
	while (!Exit) {
		DrawOptionsMenu(Soption,options);
		k = MT2D_Keyboard_keytouched();
		switch (k) {
		case arrow_key_pressed:
			k = MT2D_Keyboard_keytouched();
			switch (k) {
			case arrow_up_pressed:
				if (Soption == 0)
				{
					Soption = 5;
				}
				else
				{
					Soption--;
				}
				break;
			case arrow_down_pressed:
				if (Soption == 5)
				{
					Soption = 0;
				}
				else
				{
					Soption++;
				}
			}
			break;
		case ' ':
			switch (Soption) {
			case 0:
				options->FlyBug = !options->FlyBug;
				break;
			case 1:
				options->FlyBugMultipass = !options->FlyBugMultipass;
				options->UseGIMP = false;
				break;
			case 2:
				options->PreparePicture = !options->PreparePicture;
				options->UseGIMP = false;
				break;
			case 3:
				options->UseSprites = !options->UseSprites;
				options->UseGIMP = false;
				break;
			case 4:
				options->UseGIMP = !options->UseGIMP;
				options->FlyBugMultipass = false;
				options->PreparePicture = false;
				break;
			}
			break;
		case '-':
		case '_':
			if (Soption == 5 && options->Threads > 1) {
				options->Threads--;
			}
			break;
		case '+':
		case '=':
			if (Soption == 5) {
				options->Threads++;
			}
			break;
		case enter_pressed:
			Exit = true;
			break;
		}
	}
}

int CompletedImages = 0;
int ThreadsCompleted = 0;

void GimpCreatingNuf(int TotalImages, Options *options)
{	MT2D_Create_Window_Layout(0, 0, MAX_HOR - 2, MAX_VER - 2, true, true, false, ' ', DISPLAY_WINDOW1);
	insert_string_on_display(" mufflon_TUI created by: Lucas Zimerman Fraulob", 1, 10, DISPLAY_WINDOW1);
	insert_string_on_display("Finished Threads", 6, 2, DISPLAY_WINDOW1);
	insert_number_on_display(0, 11, 2, DISPLAY_WINDOW1);
	insert_number_on_display(options->Threads, 11, 67, DISPLAY_WINDOW1);
	insert_string_on_display("Generated Images", 14, 2, DISPLAY_WINDOW1);
	insert_string_on_display("0\%", 19, 2, DISPLAY_WINDOW1);
	insert_string_on_display("100\%", 19, 67, DISPLAY_WINDOW1);

	insert_string_on_display("Generatinc NUF images", 4, 40, DISPLAY_WINDOW1);
	while (CompletedImages < TotalImages)
	{
		MT2D_Draw_Horizontal_Load_Bar(options->Threads, ThreadsCompleted, 70, 3, 2, 7, DISPLAY_WINDOW1, false);
		MT2D_Draw_Horizontal_Load_Bar(TotalImages, CompletedImages, 70, 3, 2, 15, DISPLAY_WINDOW1, false);
		MT2D_Draw_Window(DISPLAY_WINDOW1);
		MT2D_System_Delay(1000);
	}
}

void GimpLoadingMenu(int TotalImages, char *ImageName, int Digits)
{
	FILE *ActualFile;
	int NumberPos = strlen(ImageName) + 7 /*"Newbmp/*/;
	char *FullPATH = (char*) malloc( (NumberPos + 4 /*max numbers*/ + 4 /*.bmp*/) * sizeof(char));
	int N = 0,i=0;
	sprintf(FullPATH, "Newbmp/%sxxxx.bmp", ImageName);
	GetStringFromNumber(&FullPATH[NumberPos], 0, 4);
	FullPATH[NumberPos + Digits] = '.';
	MT2D_Create_Window_Layout(0, 0, MAX_HOR - 2, MAX_VER - 2, true, true, false, ' ', DISPLAY_WINDOW1);
	insert_string_on_display(" mufflon_TUI created by: Lucas Zimerman Fraulob", 1, 10, DISPLAY_WINDOW1);
	insert_string_on_display("Waiting GIMP to finish the task", 4, 2, DISPLAY_WINDOW1);
	MT2D_Draw_Horizontal_Load_Bar(TotalImages, 0, 70, 3, 2, 15, DISPLAY_WINDOW1, false);
	MT2D_Draw_Window(DISPLAY_WINDOW1);
	while(i < TotalImages)
	{
		ActualFile = fopen(FullPATH,"r");
		if (ActualFile)
		{
			fclose(ActualFile);
			i++;
			GetStringFromNumber(&FullPATH[NumberPos], i, 4);
			FullPATH[NumberPos + Digits] = '.';
			MT2D_Create_Window_Layout(0, 0, MAX_HOR - 2, MAX_VER - 2, true, true, false, ' ', DISPLAY_WINDOW1);
			insert_string_on_display(" mufflon_TUI created by: Lucas Zimerman Fraulob", 1, 10, DISPLAY_WINDOW1);
			insert_string_on_display("Waiting GIMP to finish the task", 4, 2, DISPLAY_WINDOW1);

			MT2D_Draw_Horizontal_Load_Bar(TotalImages, i, 70, 3, 2, 15, DISPLAY_WINDOW1, false);
			MT2D_Draw_Window(DISPLAY_WINDOW1);
		}
		else
		{
			MT2D_System_Delay(300);
		}
	}
}

void ThreadCompleted()
{
	ThreadsCompleted++;
}
void ImageCompleted()
{
	CompletedImages++;
}
#include <MT2D/MessageBox/MT2D_MessageBox.h>
#include "mufflon.h"
#include "TextUserInterface.h"
#include "Text.h"
#include "CVAR.h"
#include "StringTools.h"
#include "process.h"
#include <process.h>
#define false 0
#define true 1

struct  Options options;

char *Gimp_EXE = 0;
char *Gimp_PATH = 0;

const char COMMODORE64_PALETTE1[] = "(let* ((C64PAL \\\"C64PALETTE\\\"))  (gimp-palette-new C64PAL) / (gimp-palette-add-entry C64PAL \\\"0\\\" '(000 000 000)) / (gimp-palette-add-entry C64PAL \\\"1\\\" '(255 255 255)) /  (gimp-palette-add-entry C64PAL \\\"2\\\" '(104 055 043)) / (gimp-palette-add-entry C64PAL \\\"3\\\" '(112 164 178)) / (gimp-palette-add-entry C64PAL \\\"4\\\" '(111 061 134)) /  (gimp-palette-add-entry C64PAL \\\"5\\\" '(088 141 067)) /  (gimp-palette-add-entry C64PAL \\\"6\\\" '(053 040 121)) /  (gimp-palette-add-entry C64PAL \\\"7\\\" '(184 199 111)) / (gimp-palette-add-entry C64PAL \\\"8\\\" '(111 079 037)) / (gimp-palette-add-entry C64PAL \\\"9\\\" '(067 057 000)) / (gimp-palette-add-entry C64PAL \\\"10\\\" '(154 103 089)) / (gimp-palette-add-entry C64PAL \\\"11\\\" '(068 068 068)) / (gimp-palette-add-entry C64PAL \\\"12\\\" '(108 108 108)) / (gimp-palette-add-entry C64PAL \\\"13\\\" '(154 210 132)) / (gimp-palette-add-entry C64PAL \\\"14\\\" '(108 094 181))  / (gimp-palette-add-entry C64PAL \\\"15\\\" '(149 149 149)))";
const char FILTER_COMMAND[] = "(let* ( ( Files (cadr (file-glob \\\"bmp\\\\*.*\\\" 1)))  ) / (while (not (null? Files)) / (let* ( (file (car Files))  (image (car (gimp-file-load 1 file file)))  (draw (car (gimp-image-get-active-layer image))) (savename (string-append \\\"New\\\" file))) / (gimp-image-scale image 320 200) / (gimp-convert-indexed image 2 4 0 0 FALSE \\\"C64PALETTE\\\") / (gimp-convert-rgb image) / (file-bmp-save 1 image draw savename savename)  / (gimp-image-delete image) )  / (set! Files (cdr Files))))";
const char COMMODORE64_DEL_PALETTE1[] = "(gimp-palette-delete \\\"C64PALETTE\\\")";
const char EXIT_GIMP[] = "(gimp-quit 0)";


void FixImagesWithGIMP(char *FilePATH)
{
	FILE *GimpExist;
	char *FULL_GIMP_FILE = 0;
	char *SYSCALL;
	char *THIS_FOLDER = GetMainProcessDirectory();
	if (!Gimp_PATH)
	{
		MT2D_MessageBox("You must define the path to reach Gimp on CONFIG.txt");
		exit(-1);
	}
	if (!Gimp_EXE)
	{
		MT2D_MessageBox("You must define the Gimp executable name on CONFIG.txt to run GIMP");
		exit(-1);
	}
	FULL_GIMP_FILE = (char*)malloc((strlen(Gimp_EXE) + strlen(Gimp_PATH)) * sizeof(char));
	SYSCALL = (char*)malloc(  (strlen(THIS_FOLDER) + strlen(COMMODORE64_PALETTE1) + strlen(FILTER_COMMAND) + strlen(COMMODORE64_DEL_PALETTE1) + 40 )*sizeof(char));
	sprintf(FULL_GIMP_FILE, "%s\\%s ", Gimp_PATH, Gimp_EXE);
	GimpExist = fopen(FULL_GIMP_FILE, "r");
	if (GimpExist)
	{
		fclose(GimpExist);
		sprintf(SYSCALL, "--session=\"%s\" -b \"%s\" -b \"%s\" -b \"%s\"\0", THIS_FOLDER, COMMODORE64_PALETTE1, FILTER_COMMAND, COMMODORE64_DEL_PALETTE1);
		StartProcess(FULL_GIMP_FILE, SYSCALL);
		//Gimp will run in paralel so we will just count the files that he created in GimpLoadingMenu...
		GimpLoadingMenu(768,"STARC",4);
	}
}

void InvertString(char *String)
{


}
struct ThreadData {
	int Start;
		int End;
		char *ImgName;
		char *DestPath;
		int Digits;
};

void mufflonThread(void *param)
{
	ThreadData *args = (ThreadData*)param;
	int Start = args->Start;
	int End = args->End;
	char *ImgName = args->ImgName;
	char *DestPath = args->DestPath;
	int Digits = args->Digits;

	FILE *f;
	int i = Start;
	char *OriginPATH;
	char *DestPATH;
	char *numbers = (char*)malloc((Digits + 1) * sizeof(char));
		while (i < End)
		{
			//get the char number
			GetStringFromNumber(numbers, i,Digits);
			OriginPATH = (char*)malloc(80 * sizeof(char));
			DestPATH = (char*)malloc(80 * sizeof(char));			
			if (options.UseGIMP == false)
			{
				sprintf(OriginPATH, "./bmp/%s%s.bmp", ImgName, numbers);
			}
			else
			{
				sprintf(OriginPATH, "./Newbmp/%s%s.bmp", ImgName, numbers);
			}
			GetStringFromNumber(numbers, i, 3);
			sprintf(DestPATH, "./nuf/%s%s.nuf", ImgName, numbers);
			f = fopen(OriginPATH,"r");
			if (f)
			{
				fclose(f);
				mufflon_RUN(&options, OriginPATH, DestPATH);
				free(OriginPATH);
				OriginPATH = (char*)malloc(80 * sizeof(char));
				sprintf(OriginPATH, "./nuf/%s%s_errormap.bmp", ImgName, numbers);
				remove(OriginPATH);
				OriginPATH = (char*)malloc(80 * sizeof(char));
				
				sprintf(OriginPATH, "./nuf/%s%s_result.bmp", ImgName, numbers);
				remove(OriginPATH);
				OriginPATH = (char*)malloc(80 * sizeof(char));

			}
			else
			{

			}
			free(OriginPATH);
			free(DestPATH);
			i++;
			ImageCompleted();
	}
		ThreadCompleted();
}

ThreadData *CreateThreadData(int Start, int End, char *ImgName, char *DestPath, int Digits)
{
	ThreadData *args = (ThreadData*)malloc(sizeof(ThreadData));
	args->DestPath = DestPath;
	args->Digits = Digits;
	args->End = End;
	args->ImgName = ImgName;
	args->Start = Start;
	return args;
}

void main(int argc, char **argv) {
	//Load the default config files
	Texto *CONFIG_FILE = CarregaTexto("CONFIG.txt");
	int TotalImages = 768;
	if (CONFIG_FILE)
	{
		Gimp_EXE = ReadCVAR(CONFIG_FILE, "GIMP_CONSOLE_EXE_NAME");
		Gimp_PATH =  ReadEOFCVAR(CONFIG_FILE, "GIMP_FOLDER_PATH");
		options.FlyBug = ReadBooleanCVAR(CONFIG_FILE,"CONFIG_BOOL_FLYBUG");
		options.FlyBugMultipass = ReadBooleanCVAR(CONFIG_FILE, "CONFIG_BOOL_FLYBUG_MULTIPASS");
		options.PreparePicture = ReadBooleanCVAR(CONFIG_FILE, "CONFIG_BOOL_PREPARE_PICTURES");
		options.UseSprites = ReadBooleanCVAR(CONFIG_FILE, "CONFIG_BOOL_USE_SPRITES");
		options.UseGIMP = ReadBooleanCVAR(CONFIG_FILE, "CONFIG_BOOL_USE_GIMP");
		options.Threads = ReadIntegerCVAR(CONFIG_FILE, "CONFIG_INTEGER_NUM_THREADS");
		if (0 >= options.Threads)
		{
			options.Threads = 1;
		}
	}
	else
	{
		options.UseGIMP = false;
		options.FlyBug = false;
		options.FlyBugMultipass = false;
		options.PreparePicture = false;
		options.UseSprites = false;
		options.Threads = 1;
		CONFIG_FILE = (Texto*)malloc(sizeof(Texto));
		CONFIG_FILE->Linhas = 0;
		CONFIG_FILE->QntLinhas = 0;
		CONFIG_FILE->Fonte = "CONFIG.txt";
		CONFIG_FILE->Salvo = false;
		AdicionaLinha(CONFIG_FILE, "/*===================================");
		AdicionaLinha(CONFIG_FILE, "   Mufflon_TUI CONFIG FILE");
		AdicionaLinha(CONFIG_FILE, " CREATED BY: LUCAS ZIMERMAN FRAULOB");
		AdicionaLinha(CONFIG_FILE, "=====================================*/");
		AdicionaLinha(CONFIG_FILE, " ");
		AdicionaLinha(CONFIG_FILE, "uncomment the vars so the program can find them.");
		AdicionaLinha(CONFIG_FILE, "by uncomment I mean removing the \"//\" before the command.");
		AdicionaLinha(CONFIG_FILE, "true and false options are case sensitive so use \"true\" or \"false\"");
		AdicionaLinha(CONFIG_FILE, "//GIMP_FOLDER_PATH = ");
		AdicionaLinha(CONFIG_FILE, "/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/");
		AdicionaLinha(CONFIG_FILE, "/*   full folder path to reach GIMP */");
		AdicionaLinha(CONFIG_FILE, "/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/");
		AdicionaLinha(CONFIG_FILE, "//GIMP_CONSOLE_EXE_NAME =   //without the .exe");
		AdicionaLinha(CONFIG_FILE, "CONFIG_BOOL_USE_GIMP = false");
		AdicionaLinha(CONFIG_FILE, "CONFIG_BOOL_FLYBUG = false");
		AdicionaLinha(CONFIG_FILE, "CONFIG_BOOL_FLYBUG_MULTIPASS = false");
		AdicionaLinha(CONFIG_FILE, "CONFIG_BOOL_USE_SPRITES = false");
		AdicionaLinha(CONFIG_FILE, "CONFIG_BOOL_PREPARE_PICTURE = false");
		AdicionaLinha(CONFIG_FILE, "/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/");
		AdicionaLinha(CONFIG_FILE, "/*   GIMP should not be enabled with FLYBUG and/or MULTIPASS and/or USE_SPRITES and/or PREPARED set to true*/");
		AdicionaLinha(CONFIG_FILE, "/*   the software will  not allow you to do that on the interface but you can mess with the config here to */");
		AdicionaLinha(CONFIG_FILE, "/*   see what the heck will happen (NOTE: I'll not test this way of doing the things)                      */");
		AdicionaLinha(CONFIG_FILE, "/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/");
		AdicionaLinha(CONFIG_FILE, "CONFIG_INTEGER_NUM_THREADS = 1 //you should never use a value greater than the amount of cpu cores that your machine has...");
		SalvaTexto(CONFIG_FILE);
	}
	OptionsMenu(&options);
	if (options.UseGIMP == true)
	{
		FixImagesWithGIMP("STARC");
	}
	int JobStart = 0;
	int JobPiece = TotalImages / options.Threads;
	int JobEnd = JobPiece;
	int i = 0;
	for (i = 0; i < options.Threads - 1; i++)
	{
		_beginthread(mufflonThread, 0, CreateThreadData(JobStart, JobEnd, "STARC", "", 4));
		JobStart = JobEnd;
		JobEnd += JobPiece;
	}
	JobEnd = TotalImages;
	_beginthread(mufflonThread, 0, CreateThreadData(JobStart, JobEnd, "STARC", "", 4));
	GimpCreatingNuf(TotalImages, &options);
//	mufflonThread(0, 22, "STARC", "",4);
/*	for (i = 0; i < options.Threads; i++)
	{
		t[i].join();
	}
*/
}
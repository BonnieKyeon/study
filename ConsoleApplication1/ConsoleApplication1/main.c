#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARG_NUM 2
#define BUFFER_SIZE 256

char* mFileName = NULL;

void printUsage();
void compare(FILE *file1, FILE *file2);

int isTextFile(char* filePath);

FILE* compressTextFile(FILE *originFile);
FILE* decompressTextFile(FILE *compressedFile);

int main(int argc, char* argv[])
{
	if (argc == ARG_NUM) {
		mFileName = argv[1];

		if (isTextFile(mFileName)) {
			FILE *file;
			int ret = fopen_s(&file, mFileName, "rt");

			if (ret == 0) {
				FILE *compressedFile = compressTextFile(file);
				compare(file, compressedFile);

				FILE *decompressedFile = decompressTextFile(compressedFile);
				compare(decompressedFile, file);

				fclose(file);
			}
			else {
				printf("ret : %d\n", ret);
			}
		}
		else {
			printUsage();
		}
	}
	else {
		printUsage();
	}

	system("pause");
	return 0;
}

FILE* compressTextFile(FILE *originFile) {
	char tempC = '\0';
	char countBuffer[BUFFER_SIZE];
	char c;
	int count = 0;
	FILE *newFile = NULL;
	int ret;
	char* newFileName = "./test_compress.txt";
	ret = fopen_s(&newFile, newFileName, "w");

	printf("compressFile\n");
	if (ret == 0) {
		while ((c = fgetc(originFile)) != EOF) {

			if (tempC == '\0') {
				tempC = c;
				count++;
				printf("tempC = %c\n", tempC);
			}
			else if (tempC == c) {
				count++;
				printf("tempC : %c, count : %d\n", tempC, count);
			}
			else {
				fputc(tempC, newFile);
				if (count > 1) {
					sprintf_s(countBuffer, BUFFER_SIZE, "%d", count);
					fputs(countBuffer, newFile);
				}

				printf("tempC : %c, c : %c, countBuffer : %s\n", tempC, c, countBuffer);
				tempC = c;
				count = 1;
			}
		}

		if (tempC != '\0') {
			fputc(tempC, newFile);
			if (count > 1) {
				sprintf_s(countBuffer, BUFFER_SIZE, "%d", count);
				fputs(countBuffer, newFile);
			}
		}


		fclose(newFile);
	}

	printf("\n");

	return NULL;
}

FILE* decompressTextFile(FILE *compressedFile) {
	return NULL;
}

void printUsage(void) {
	printf("How to use : \n");
}

void compare(FILE *file1, FILE *file2) {

}

int isTextFile(char* filePath) {
	char* extension = strrchr(filePath, '.');
	int ret = 0;

	if (extension != NULL) {
		if (strcmp(extension, ".txt") == 0)
			ret = 1;
	}
	return ret;
}
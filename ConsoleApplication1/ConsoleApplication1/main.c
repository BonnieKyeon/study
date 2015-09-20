#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARG_NUM 2
#define COM_BUFFER_SIZE 256
#define DE_BUFFER_SIZE 10
#define ESCAPE_C '*'

void printUsage();
void compareFileSize(char *file1, char *file2);

int isTextFile(char* filePath);

char* compressFile(char *originalFileName);
char* decompressFile(char *compressedFileName);

int main(int argc, char* argv[])
{
	if (argc == ARG_NUM) {
		char* originalFileName = argv[1];

if (isTextFile(originalFileName)) {
	char* compressedFileName = compressFile(originalFileName);
	if (compressedFileName != NULL) {
		compareFileSize(originalFileName, compressedFileName);

		char* decompressedFileName = decompressFile(compressedFileName);
		if (decompressedFileName != NULL) {
			compareFileSize(compressedFileName, decompressedFileName);
		}
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

char* compressFile(char *originalFileName) {
	char tempC = '\0';
	char countBuffer[COM_BUFFER_SIZE];
	char c;
	int count = 0;
	FILE *originFile;
	FILE *compressedFile;
	char* compressedFileName = "./test_compress.txt";

	int ret;

	ret = fopen_s(&originFile, originalFileName, "rt");
	if (ret == 0) {
		ret = fopen_s(&compressedFile, compressedFileName, "w");
		if (ret == 0) {
			while ((c = fgetc(originFile)) != EOF) {
				if (tempC == '\0') {
					tempC = c;
					count++;
				}
				else if (tempC == c) {
					count++;
				}
				else {
					fputc(tempC, compressedFile);
					if (count > 1) {
						sprintf_s(countBuffer, COM_BUFFER_SIZE, "%d", count);
						fputs(countBuffer, compressedFile);
					}
					tempC = c;
					count = 1;
				}
			}

			if (tempC != '\0') {
				fputc(tempC, compressedFile);
				if (count > 1) {
					sprintf_s(countBuffer, COM_BUFFER_SIZE, "%d", count);
					fputs(countBuffer, compressedFile);
				}
			}

			fclose(compressedFile);
		}
		else {
			compressedFileName = NULL;
		}
		fclose(originFile);
	}
	else {
		compressedFileName = NULL;
	}

	printf("\n");

	return compressedFileName;
}

char* decompressFile(char *compressedFileName) {
	FILE *compressedFile;
	FILE *decompressedFile;
	char* decompressedFileName = "./test_decompress.txt";
	char tempC = '\0';
	char c;
	char countBuffer[DE_BUFFER_SIZE] = { '\0' , };
	int count = 0;
	int length = 0;
	int i;
	int ret;

	ret = fopen_s(&compressedFile, compressedFileName, "rt");
	if (ret == 0) {
		ret = fopen_s(&decompressedFile, decompressedFileName, "w");
		if (ret == 0) {
			while (!feof(compressedFile)) {
				if ((c = fgetc(compressedFile)) != EOF) {
					if (c >= 48 && c <= 57) {
						if (tempC != '\0') {
							*(countBuffer + length) = c;
							length++;
						}
						else {
							tempC = c;
						}
					}
					else {
						if (tempC != '\0') {
							if( length > 0 ) {
								count = atoi(countBuffer);
								length = 0;
								memset(countBuffer, '\0', DE_BUFFER_SIZE);
								for (i = 0; i < count; i++)
									fputc(tempC, decompressedFile);
							}
							else {
								fputc(tempC, decompressedFile);
							}
							tempC = '\0';
						}
						tempC = c;
					}
				}
			}

			if (tempC != '\0') {
				if (length > 0) {
					count = atoi(countBuffer);
					length = 0;
					memset(countBuffer, '\0', DE_BUFFER_SIZE);
					for (i = 0; i < count; i++)
						fputc(tempC, decompressedFile);
				}
				else {
					fputc(tempC, decompressedFile);
				}
			}
			
			fclose(decompressedFile);
		}
		else {
			decompressedFileName = NULL;
		}
		fclose(compressedFile);
	}
	else {
		decompressedFileName = NULL;
	}

	return decompressedFileName;
}

void printUsage(void) {
	printf("How to use : \n");
}

void compareFileSize(char *file1Name, char *file2Name) {
	long file1Size;
	long file2Size;
	FILE *file1;
	FILE *file2;
	int ret;

	if (file1Name != NULL ) {
		ret = fopen_s(&file1, file1Name, "r");
		if (ret == 0) {
			fseek(file1, 0, SEEK_END);
			file1Size = ftell(file1);
			fclose(file1);
		}
	}
	if (file2Name != NULL) {
		fopen_s(&file2, file2Name, "r");
		if (ret == 0) {
			fseek(file2, 0, SEEK_END);
			file2Size = ftell(file2);
			fclose(file2);
		}
	}

	printf("%s size : %ld\n", file1Name, file1Size);
	printf("%s size : %ld\n", file2Name, file2Size);

	if(file2Size > 0)
		printf("%d.%d\n", (file1Size / file2Size), (file1Size % file2Size));
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
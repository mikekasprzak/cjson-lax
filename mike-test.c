/* Very basic test program. Only confirms whether a file is valid or not. */

#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

int main( int argc, char* argv[] ) {
	FILE* inFile;

	/* Barebones get the filename from the command line */
	if ( argc <= 1 ) {
		printf("usage: %s infile.json\n", argv[0]);
		return 1;
	}

	/* Open the File */
	inFile = fopen(argv[1],"r");

	if ( inFile ) {
		size_t fileSize;
		char* fileData;

		/* Get File Size */
		fseek(inFile, 0, SEEK_END);
		fileSize = ftell(inFile);
		fseek(inFile, 0, SEEK_SET);

		if ( fileSize == 0 ) {
			fclose(inFile);
			printf("error: file \"%s\" is empty\n",argv[1]);
			return 1;
		}

		/* Allocate Memory, extra byte to zero terminate */
		fileData = malloc(fileSize+1);

		if ( !fileData ) {
			fclose(inFile);
			printf("error: unable to allocate %zu+1 bytes\n",fileSize);
			return 1;
		}

		/* Load File */
		{
			size_t count;

			count = fread(fileData,1,fileSize,inFile);
			fclose(inFile);

			if ( count != fileSize ) {
				free(fileData);
				printf("error: unable to read %zu bytes (%zu instead)\n",fileSize,count);
				return 1;
			}

			/* Zero Terminate */
			fileData[fileSize] = 0;
		}

		/* Parse JSON */
		cJSON *root = cJSON_Parse(fileData);

		if ( root ) {
			char* outData;

			/* On Success, reprint the data (but as the parses sees it) */
			outData = cJSON_Print(root);

			printf("%s\n",outData);

			free(outData);
			free(fileData);
		}
		else {
			free(fileData);
			printf("error: failed to parse JSON file\n");
			return 1;
		}
	}
	else {
		printf("error: unable to open \"%s\"\n",argv[1]);
		return 1;
	}

	return 0;
}

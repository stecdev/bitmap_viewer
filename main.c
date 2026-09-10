#include <stdint.h>
#include <stdio.h>
#include <raylib.h>

int main(int argc, char* argv[]) {
	printf("BITMAP FILE VIEWER\n");
	printf("stecdev 2026\n");
	printf("========================\n");

	// check for proper usage
	if (argc < 2) {
		printf("Specify which file to run as an argument (useage: ./bmpview /path/to/image.bmp)\n");
		return 1;
	} else if (argc > 2) {
		printf("Too many arguments! (usage: ./bmpview /path/to/image.bmp)\n");
		return 3;
	}

	// open the file
	FILE *img = fopen(argv[1], "rb");
	if (img == NULL) {
		printf("Could not open file!\n");
		return 2;
	}
	fseek(img, 0, SEEK_SET);

	// read the header
	uint8_t header[14];
	fread(header, 14, 1, img);

	// parse header
	// 16 bit file identifier
	// 32 bit size and offset fields
	const uint16_t ident = header[0] + (header[1] << 8); // store the identifier
	const uint32_t size = header[2] + (header[3] << 8) + (header[4] << 16) + (header[5] << 24);
	const uint32_t offset = header[10] + (header[11] << 8) + (header[12] << 16) + (header[13] << 24);

	// print header data
	printf("Header\n");
	printf("ID: 0x%04X (%c%c)\n", ident, header[0], header[1]);
	printf("File size: 0x%08X (%0.6f MiB)\n", size, (float)size/1000000);
	printf("Offset: 0x%08X\n\n", offset);

	if (ident != 0x4D42) { // handle unknown ids
		printf("Unknown ID!\n");
		fclose(img);
		return 4;
	}

	// grab DIB header
	uint8_t dib[40];
	fread(dib, 40, 1, img);

	// parse the dib header
	const uint16_t size_dib = dib[0] + (dib[1]<<8) + (dib[2]<<16) + (dib[3]<<24);
	const int width = dib[4] + (dib[5]<<8) + (dib[6]<<16) + (dib[7]<<24);
	const int height = dib[8] + (dib[9]<<8) + (dib[10]<<16) + (dib[11]<<24);
	const uint16_t bpp = dib[14] + (dib[15]<<8);
	const uint32_t compression = dib[16] + (dib[17]<<8) + (dib[18]<<16) + (dib[19]<<24);
	const uint32_t size_raw = dib[20] + (dib[21]<<8) + (dib[22]<<16) + (dib[23]<<24);

	if (size_dib < 12) {
		printf("Invalid size!\n");
		fclose(img);
		return 7;
	}

	// print dib data
	printf("DIB Header\n");
	printf("Size: %d bytes\n", size_dib);
	printf("Image width: %d px\n", width);
	printf("Image height: %d px\n", height);
	printf("Bits per pixel: %d\n", bpp);
	printf("Compression: %d\n", compression);
	printf("Raw image size: %0.6f MiB\n\n", (float)size_raw/1000000);

	if (compression != 0) { // handle unknown compression types
		printf("Unknown compression type!\n");
		fclose(img);
		return 5;
	} if (bpp != 24) { // handle unknown bpp values
		printf("Unknown bits per pixel value!\n");
		fclose(img);
		return 6;
	}

	// set seek header to start of image data
	printf("Heading to 0x%08X...\n", offset);
	fseek(img, offset, SEEK_SET);
	printf("Beginning of image reached, reading...\n\n");

	// open window at 60 fps
	char title[150];
	sprintf(title,"BMP Image Viewer (%s) | ID: %c%c, Width: %d, Height: %d, BPP: %d", argv[1], header[0], header[1], width, height, bpp);
	InitWindow(width, height, title);
	SetTargetFPS(60);

	// read & dump the image in an array of pixel values, plus draw initial image (remove later)
	Color pixels[width][height];
	BeginDrawing();
	ClearBackground(BLACK);
	for (int y = height - 1; y >= 0; y--) {
		for (int x = 0; x < width; x++) {
			fread(&pixels[x][y].b, 1, 1, img);
			fread(&pixels[x][y].g, 1, 1, img);
			fread(&pixels[x][y].r, 1, 1, img);
			pixels[x][y].a = 255; // default alpha to 100% since 24 bpp does not have alpha information
			DrawPixel(x, y, pixels[x][y]);
		}
	}
	EndDrawing();

	while (!WindowShouldClose()) { /* do nothing */ }

	CloseWindow();

	fclose(img);

	return 0;

}
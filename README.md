# Bitmap Viewer

Basic .bmp file viewer. Currently only supports BM headers and uncompressed 24 BPP.

### Requirements:
- Raylib (on system include path)
### Run Instructions
- Clone repository
- Open terminal, enter the following in order:
  - cd /path/to/bitmap_viewer
  - make
  - ./bmpview /path/to/image.bmp

If the file is supported, a window will open showing the image inside.

stecdev 2026
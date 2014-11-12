/* An ascii bitmap library.
 text_bits holds bitmap information for each character, packed into a byte array.
 This means for each pixel in a 13x7 picture of a character, 1 bit is stored in text_bits saying whether that pixel should be on or off.

 text.c is actually an xbm file (see http://en.wikipedia.org/wiki/X_BitMap)
*/
#define char_width 7
#define char_height 13
// 96 visible characters * 7 bits wide each
#define text_bits_width 672

#define offset(c, i, j) ((i) * text_bits_width + ((c) - 32) * char_width + (j))
#define byteNum(c, i, j) (offset(c, i, j) / 8)
#define bitNum(c, i, j) (offset(c, i, j) % 8)

// returns true if a 13x7 picture of character c has a pixel set at i, j
#define charHasPixelSet(c, i, j) (text_bits[byteNum(c, i, j)] & (1 << bitNum(c - 32, i, j)))

extern const uint8_t text_bits[];

typedef enum {TINY=1, SMALL=2, MEDIUM=3, LARGE=4, HUGE=5} fontsize_t;

void drawChar(u16* videoMemory, int x, int y, char c, u16 color, fontsize_t fontsize);
void drawString(u16* videoMemory, int x, int y, char *string, u16 color, fontsize_t fontsize);

// Draws a single character to the screen.
// charHasPixelSet will tell you if any given character has a pixel set at coordinates i, j.
// You can find fontsize_t in gamelib.h. It is a simple scaling factor, so if a character has a single pixel set and the font
// size is medium, you should fill a 3x3 area on the screen with color. If the font is huge, you should fill a 5x5 area with color.
void drawChar(u16* videoMemory, int x, int y, char c, u16 color, fontsize_t fontsize)
{
    int i;
    int j;
    int k;
    int l;
    for (i = 0; i < char_height; i++) {
        for (j = 0; j < char_width; j++) {
            if (charHasPixelSet(c, i, j)) {
                for (k = i * fontsize; k < (i * fontsize) + fontsize; k++) {
					for (l = j * fontsize; l < (j * fontsize) + fontsize; l++) {
						videoMemory[(l+x) + (k+y) * 256] = color;
					}
                }
            }
        }
        }
}

// Draws a string to the screen, using drawChar.
void drawString(u16* videoMemory, int x, int y, char *string, u16 color, fontsize_t fontsize)
{
    char c = 0;
    for(int i = 0;(c = string[i]); i++) {
        drawChar(videoMemory, x, y, c, color, fontsize);
        x += char_width * fontsize;
    }

}

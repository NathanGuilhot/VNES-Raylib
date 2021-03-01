//Graphics System 
//This is the file to update when changing graphics API

////// ------------ RAYLIB

//----- Struct/type

// typedef struct Texture {
//     unsigned int id;        // OpenGL texture id
//     int width;              // Texture base width
//     int height;             // Texture base height
//     int mipmaps;            // Mipmap levels, 1 by default
//     int format;             // Data format (PixelFormat type)
// } Texture;

// typedef Texture Texture2D;

// Color type, RGBA (32bit)
// typedef struct Color {
//     unsigned char r;
//     unsigned char g;
//     unsigned char b;
//     unsigned char a;
// } Color;

// // Font type, includes texture and charSet array data
// typedef struct Font {
//     int baseSize;           // Base size (default chars height)
//     int charsCount;         // Number of characters
//     int charsPadding;       // Padding around the chars
//     Texture2D texture;      // Characters texture atlas
//     Rectangle *recs;        // Characters rectangles in texture
//     CharInfo *chars;        // Characters info data
// } Font;

Font Text_font; //default font
Font Text_font_bold;
Font Text_font_italic;
Font Text_font_bolditalic;

//----- Load

//VN_LoadTexture()
Texture2D VN_LoadTexture(const char *fileName){
    return LoadTexture(fileName);  
}

Font VN_LoadFont(const char *filename){
    return LoadFontEx(filename, 32,NULL,250);
}

//----- Unload

void VN_UnloadTexture(Texture2D texture){
    UnloadTexture(texture); 
}

void VN_UnloadFont(Font font){
    UnloadFont(font);
}

//----- Draw


//VN_DrawText()
void VN_DrawText(const char *text, int posX, int posY, float fontSize, Color color){
    // DrawTextEx(Text_font, text, (Vector2){posX, posY},fontSize, 1,color);

    Font font = Text_font;
    Vector2 position = (Vector2){posX,posY};
    float spacing = 1;
    float line_spacing = 1.1;

    //Code stolen from raylib DrawTextEX(), I really love your code raysan <3

    int length = TextLength(text);      // Total length in bytes of the text, scanned by codepoints in loop

    int textOffsetY = 0;            // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    
    float scaleFactor = fontSize/font.baseSize;     // Character quad scaling factor

    //Style flags
    bool flag_bold = false;
    bool flag_italic = false;
    bool flag_wave = false;
    bool flag_crossed = false;

    for (int i = 0; i < length;)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetNextCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;

        if (codepoint == '\n')
        {
            textOffsetY += (int)((font.baseSize * line_spacing)*scaleFactor);
            textOffsetX = 0.0f;
        }
        else if (codepoint == '*')
        {
            if (GetNextCodepoint(&text[i+1], &codepointByteCount) == '*') // -> **
            {
                flag_bold = !flag_bold;
                codepointByteCount += 1;
            }
            else
            {
                flag_italic = !flag_italic;
            }

            //Font Weight switching
            if (flag_bold && flag_italic) font = Text_font_bolditalic;
            else if (flag_bold)font = Text_font_bold;
            else if (flag_italic) font = Text_font_italic;
            else font = Text_font;
        }
        else if (codepoint == '~')
        {
            if (GetNextCodepoint(&text[i+1], &codepointByteCount) == '~') // -> ~~
            {
                flag_crossed = !flag_crossed;
                codepointByteCount += 1;
            }
            else
            {
                flag_wave = !flag_wave;
            }
            // flag_wave = !flag_wave;

        }
        else
        {
            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                if (!flag_wave) DrawTextCodepoint(font, codepoint, (Vector2){ position.x + textOffsetX, position.y + textOffsetY }, fontSize, color);
                else DrawTextCodepoint(font, codepoint, (Vector2){ position.x + textOffsetX + sin(time*4-i)*1, position.y + textOffsetY + sin(time*4-i)*2 }, fontSize, color);
            }

            if (font.chars[index].advanceX == 0) textOffsetX += ((float)font.recs[index].width*scaleFactor + spacing);
            else textOffsetX += ((float)font.chars[index].advanceX*scaleFactor + spacing);
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }
}

//VN_DrawRectangle()
void VN_DrawRectangle(int posX, int posY, int width, int height, Color color){
    DrawRectangle(posX, posY, width, height, color); 
}

void VN_DrawRectangleLines(int posX, int posY, int width, int height, Color color){
    DrawRectangleLines(posX, posY, width, height, color); 
}

//VN_DrawTexture()
void VN_DrawTexture(Texture2D texture, int posX, int posY, Color tint){
    DrawTexture(texture, posX, posY, tint);  
}


//----- Misc
void VN_DrawFPS(int x, int y){
    DrawFPS(x,y);
    DrawText(TextFormat("%2i FPS", GetFPS()), x, y, 20, BLACK);
}

//I would like to have VN_TextFormat(), this function seems pretty handy

int VN_MeasureText(const char *text, int fontSize){
    float spacing = 1;
    return MeasureTextEx(Text_font, text, (float)fontSize, spacing).x;
}

//Color def

// #if defined(__cplusplus)
//     #define CLITERAL(type)      type
// #else
//     #define CLITERAL(type)      (type)
// #endif

// #define LIGHTGRAY  CLITERAL(Color){ 200, 200, 200, 255 }   // Light Gray
// #define GRAY       CLITERAL(Color){ 130, 130, 130, 255 }   // Gray
// #define DARKGRAY   CLITERAL(Color){ 80, 80, 80, 255 }      // Dark Gray
// #define YELLOW     CLITERAL(Color){ 253, 249, 0, 255 }     // Yellow
// #define GOLD       CLITERAL(Color){ 255, 203, 0, 255 }     // Gold
// #define ORANGE     CLITERAL(Color){ 255, 161, 0, 255 }     // Orange
// #define PINK       CLITERAL(Color){ 255, 109, 194, 255 }   // Pink
// #define RED        CLITERAL(Color){ 230, 41, 55, 255 }     // Red
// #define MAROON     CLITERAL(Color){ 190, 33, 55, 255 }     // Maroon
// #define GREEN      CLITERAL(Color){ 0, 228, 48, 255 }      // Green
// #define LIME       CLITERAL(Color){ 0, 158, 47, 255 }      // Lime
// #define DARKGREEN  CLITERAL(Color){ 0, 117, 44, 255 }      // Dark Green
// #define SKYBLUE    CLITERAL(Color){ 102, 191, 255, 255 }   // Sky Blue
// #define BLUE       CLITERAL(Color){ 0, 121, 241, 255 }     // Blue
// #define DARKBLUE   CLITERAL(Color){ 0, 82, 172, 255 }      // Dark Blue
// #define PURPLE     CLITERAL(Color){ 200, 122, 255, 255 }   // Purple
// #define VIOLET     CLITERAL(Color){ 135, 60, 190, 255 }    // Violet
// #define DARKPURPLE CLITERAL(Color){ 112, 31, 126, 255 }    // Dark Purple
// #define BEIGE      CLITERAL(Color){ 211, 176, 131, 255 }   // Beige
// #define BROWN      CLITERAL(Color){ 127, 106, 79, 255 }    // Brown
// #define DARKBROWN  CLITERAL(Color){ 76, 63, 47, 255 }      // Dark Brown

// #define WHITE      CLITERAL(Color){ 255, 255, 255, 255 }   // White
// #define BLACK      CLITERAL(Color){ 0, 0, 0, 255 }         // Black
// #define BLANK      CLITERAL(Color){ 0, 0, 0, 0 }           // Blank (Transparent)
// #define MAGENTA    CLITERAL(Color){ 255, 0, 255, 255 }     // Magenta
// #define RAYWHITE   CLITERAL(Color){ 245, 245, 245, 255 }   // My own White (raylib logo)

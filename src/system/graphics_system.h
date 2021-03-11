//Graphics System 
//This is the file to update when changing graphics API

//UI variable (should be int or define ?)
#define screenWidth 800
#define screenHeight 450

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

typedef enum BBCODE_COMMAND
{
    //Style
    BOLD_ON,
    BOLD_OFF,
    ITAL_ON,
    ITAL_OFF,
    UNDERL_ON,
    UNDERL_OFF,
    CROSS_ON,
    CROSS_OFF,

    //Color
    COLOR_ON,
    COLOR_OFF,

    //Animation
    WAVE_ON,
    WAVE_OFF,
} BBCODE_COMMAND;


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

typedef struct BBCODE_RETURN
{
    BBCODE_COMMAND COMMAND;
    Color color;

} BBCODE_RETURN;

//Input "#rrggbb", get Color
Color ColorFromHexa(char* pHexa)
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    r,g,b,a = 255;

    char red_str[3], green_str[3], blue_str[3];

    sscanf(pHexa, "#%2s%2s%2s",red_str, green_str, blue_str);
    r = (int)strtol(red_str,NULL,16);
    g = (int)strtol(green_str,NULL,16);
    b = (int)strtol(blue_str,NULL,16);

    return (Color){r, g, b, a}; //Todo: Maybe return values for keywords ? (blue, red, cyan...)
}



BBCODE_RETURN BBCODE_PARSER(const char *bbcode)
{
    //Basic formating
    if (strcmp(bbcode,"b")==0)
        return (BBCODE_RETURN){BOLD_ON};
    else if (strcmp(bbcode,"/b")==0)
        return (BBCODE_RETURN){BOLD_OFF};
    else if (strcmp(bbcode,"i")==0)
        return (BBCODE_RETURN){ITAL_ON};
    else if (strcmp(bbcode,"/i")==0)
        return (BBCODE_RETURN){ITAL_OFF};
    else if (strcmp(bbcode,"u")==0)
        return (BBCODE_RETURN){UNDERL_ON};
    else if (strcmp(bbcode,"/u")==0)
        return (BBCODE_RETURN){UNDERL_OFF};
    else if (strcmp(bbcode,"s")==0)
        return (BBCODE_RETURN){CROSS_ON};
    else if (strcmp(bbcode,"/s")==0)
        return (BBCODE_RETURN){CROSS_OFF};

    //From here, we probably need to parse the input

    //color
    else
    {
        char main_code[6];
        char first_argument[8];
        sscanf(bbcode,"%[^ =]=%s",&main_code,&first_argument);
        printf("Value : %s\n",main_code);
        if (strcmp(main_code,"color")==0)
        {
            return (BBCODE_RETURN){COLOR_ON, ColorFromHexa(first_argument)};
        }
        else if (strcmp(main_code,"/color")==0)
            return (BBCODE_RETURN){COLOR_OFF};
        
        //Animation
        else if (strcmp(main_code,"wave")==0)
            return (BBCODE_RETURN){WAVE_ON};
        else if (strcmp(main_code,"/wave")==0)
            return (BBCODE_RETURN){WAVE_OFF};
        else
        {
            return (BBCODE_RETURN){BOLD_OFF};
        }
        
    }
    
    

}

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
    bool flag_crossed = false;
    bool flag_underline = false;
    bool flag_wave = false;


    float wave_x_range_default = 1;
    float wave_y_range_default = 2;
    int wave_x_speed_default = 4;
    int wave_y_speed_default = 4;
    float wave_x_offset_default = 0.5;
    float wave_y_offset_default = 0.5;

    float wave_x_range = wave_x_range_default;
    float wave_y_range = wave_y_range_default;
    int wave_x_speed = wave_x_speed_default;
    int wave_y_speed = wave_y_speed_default;
    float wave_x_offset = wave_x_offset_default;
    float wave_y_offset = wave_y_offset_default;

    Color default_color = color;
    // Color current_color = default_color;

    printf("disp_text : %s, l: %d \n",text, length);


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
        else if (codepoint == '~') // Maybe use a different symbol for wave effect (or use a escape character ?)
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
        }
        else if (codepoint == '_' && GetNextCodepoint(&text[i+1], &codepointByteCount) == '_') // -> __
        {
            flag_underline = !flag_underline;
            codepointByteCount += 1;

        }
        // else if (false) 
        else if (codepoint == '[') 
        {
            //BBCODE PARSER
            char bbcode_cmd[128];
            
            strncpy(bbcode_cmd, text + i +1, sizeof(bbcode_cmd));
            
            int i_bis;

            for (i_bis=i; i_bis<length; i_bis++)
            {
                if (GetNextCodepoint(&text[i_bis], &codepointByteCount) == ']')
                {
                    //We closed the BBCODE
                    bbcode_cmd[i_bis - i-1] = '\0';

                    BBCODE_RETURN bbcode_return;
                    bbcode_return = BBCODE_PARSER(bbcode_cmd);

                    switch (bbcode_return.COMMAND)
                    {
                    //Style
                    case BOLD_ON:
                        flag_bold = true;
                        break;
                    case BOLD_OFF:
                        flag_bold = false;
                        break;

                    case ITAL_ON:
                        flag_italic = true;
                        break;
                    case ITAL_OFF:
                        flag_italic = false;
                        break;

                    case UNDERL_ON:
                        flag_underline = true;
                        break;
                    case UNDERL_OFF:
                        flag_underline = false;
                        break;

                    case CROSS_ON:
                        flag_crossed = true;
                        break;
                    case CROSS_OFF:
                        flag_crossed = false;
                        break;

                    //Color
                    case COLOR_ON:
                        color = bbcode_return.color;
                        break;
                    case COLOR_OFF:
                        color = default_color;
                        break;

                    //Animation
                    case WAVE_ON:
                        flag_wave = true;
                        break;
                    case WAVE_OFF:
                        flag_wave = false;
                        break;
                    }

                    if (flag_bold && flag_italic) font = Text_font_bolditalic;
                    else if (flag_bold)font = Text_font_bold;
                    else if (flag_italic) font = Text_font_italic;
                    else font = Text_font;

                    break;
                }
            }
            codepointByteCount += i_bis - i ;
            
        }
        else
        {
            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                float position_x;
                float position_y;
                position_x = position.x + textOffsetX;
                position_y = position.y + textOffsetY;

                if (flag_wave)
                {
                    position_x += sin(time*wave_x_speed-i*wave_x_offset)*wave_x_range;
                    position_y += sin(time*wave_y_speed-i*wave_y_offset)*wave_y_range;
                }

                DrawTextCodepoint(font, codepoint, (Vector2){ position_x, position_y }, fontSize, color);

                //TODO: Draw these lines over spaces when needed
                if (flag_crossed)
                {
                    DrawLine(position_x, position_y+fontSize/2,
                            position_x + ((float)font.chars[index].advanceX*scaleFactor + spacing), position_y+fontSize/2,
                            color);
                }

                if (flag_underline)
                {
                    DrawLine(position_x, position_y+fontSize,
                            position_x + ((float)font.chars[index].advanceX*scaleFactor + spacing), position_y+fontSize,
                            color);
                }

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

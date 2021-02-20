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



//----- Load

//VN_LoadTexture()
Texture2D VN_LoadTexture(const char *fileName){
    return LoadTexture(fileName);  
}

//----- Unload

void VN_UnloadTexture(Texture2D texture){
    UnloadTexture(texture); 
}



//----- Draw


//VN_DrawText()
void VN_DrawText(const char *text, int posX, int posY, int fontSize, Color color){
    DrawText(text, posX, posY,fontSize, color);       // Draw text (using default font)
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

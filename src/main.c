/*******************************************************************************************
*
*   VNES - Raylib port
*
********************************************************************************************/

#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "NSTD_custom_lib.h"
#include "UI_DEFS.h"
#include "input_manager.h"

//----Enums

enum GAME_STATE
{
  MAIN_MENU,
  DIAL,
  END
};
enum GAME_STATE game_st = DIAL;

enum DIAL_T
{
  N /*NARRATOR*/,
  C /*CHOICE*/,
  F /*FIN*/,
  J /*JUMP TO LABEL*/,
  H /*HIDE/SHOW*/,
  LABEL,
  MOV,    /*Move sprite*/
  CJUMP,  /*CONDITIONAL JUMP*/
  CFLAGS, /*CHANGE VALUE OF A FLAG*/
  SWPM /*SWAP EXPRESSION*/,
  BG, /*Change background*/

  // WILL NOT BE USED IN THE END :
  SWPEL /*SWAP LEFT EYE*/,
  SWPER /*SWAP RIGHT EYE*/,
  A /*ANGE*/,
};

//----Struct definition
typedef struct Passage Passage;
struct Passage
{
  enum DIAL_T t; //Type de passage
  char *c;       //Content, en general le texte affiché
};

typedef struct Choice Choice;
struct Choice
{
  char *txt; //Texte du choix
  char *jmp; //Indice du label de destination
};

//-----Variables utiles

unsigned int index = 0; //index dans le label en cours //328 max sans visage+choice
int cursor = 1;
char disp_text[64];

int choice_sel = 0;
int choice_sel_index = 0;
int choice_collection_index = 0;

bool starting;

//---- Flags
typedef struct
{
  char *key;
  int value;
} FLAGS;

#define MAX_EXPRESSION 2 //will be defined in script.h later (?)
#define MAX_BACKGROUND 2

//---- Characters
typedef struct
{
  char *key;
  char *name;
  char *image_name;
  Color color_name;

  Rectangle borders;

  bool visible;
  int x;
  int y;
  int gotox;
  int gotoy; //for interpolation
  int zindex;

  int expression_index;
  Texture2D base_image;
  Texture2D expression[MAX_EXPRESSION];

} CHARA;

typedef struct {
  char *images[MAX_BACKGROUND];
  int bg_index;
  Texture2D texture[MAX_BACKGROUND];
} BACKGROUND;

char *text_to_display;
char *chara_name = "Character Name";
char buffText[20]; //Utile pour le parse du text
char *first_word;
char *second_word;
char *third_word;
char *fourth_word;

// void AddChara(char* pKey, char* pName){
// 	  CharaList[chara_index].key = pKey;
// 	  CharaList[chara_index].name = pName;

// 	chara_index++;
// }
bool init_done = false; //Used to only launch init_dial once

const bool debug_mode = false;


//face var
char sprEl = 0; //gauche
char sprEr = 0; //Drouate
char sprM = 1;  //Bouche

int scrnBrightness = 0;
bool dispAnge = false;

char *index_txt = "indx";
char nb_choice = 1;
char *txt_choix = "choix";

#define FRENCH 1 //bolean, 1 = FR 0 = EN
#include "script_parser.h"
#include "saveload_system.h"

void ParseLabels()
{
  for (int i2 = 0; i2 < LABELS_NUMBERS; i2++)
  {
    ListLabels[i2].name = "null";
    ListLabels[i2].value = 0;
  }

  for (int i = 0; i < sizeof(SCRPT) / sizeof(SCRPT[0]); i++)
  {
    if (SCRPT[i].t == LABEL)
    {
      //L'ajouter à la liste, au premier endroit vide
      for (int i2 = 0; i2 < LABELS_NUMBERS; i2++)
      {
        if (ListLabels[i2].name == "null")
        {
          ListLabels[i2].name = SCRPT[i].c;
          ListLabels[i2].value = i;
          break;
        }
      }
    }
  }
}

void loadCharacterSprites()
{
  for (int i = 0; i < CHARACTER_NUMBER; i++)
  {
    //CharaList[i].image_name

    //Load base sprite
    char filename[32] = "./assets/img/";
    strcat(filename, CharaList[i].image_name);
    strcat(filename, " ");

    char base_filename[32];
    strcpy(base_filename, filename);
    strcat(base_filename, "base.png");

    char expression_filename[32];
    strcpy(expression_filename, filename);
    // strcat(expression_filename, "1.png");

    CharaList[i].base_image = LoadTexture(base_filename);

    CharaList[i].y = screenHeight - CharaList[i].base_image.height;
    CharaList[i].x = (screenWidth - CharaList[i].base_image.width)/2;
    CharaList[i].gotox = CharaList[i].x;
    CharaList[i].gotoy = CharaList[i].y;

    char buff[1];

    //Load expressions
    for (int i2 = 0; i2 < MAX_EXPRESSION; i2++)
    {
      itoa(i2 + 1, buff, 10);
      char file_expression[32];
      strcpy(file_expression, expression_filename);
      strcat(file_expression, buff);
      strcat(file_expression, ".png");
      // printf(file_expression);
      CharaList[i].expression[i2] = LoadTexture(file_expression);
    }

    CharaList[i].expression_index = 0; //default

    if (CharaList[i].color_name.a==0) //If the color is transparent, we asume it has not be defined
    {
      CharaList[i].color_name = DARKGRAY;
    }
  }
}

void loadBackgroundSprites()
{
  for (int i; i<MAX_BACKGROUND; i++){
    char filename[32] = "./assets/img/";
    strcat(filename, Background.images[i]);
    strcat(filename, ".png");

    Background.texture[i] = LoadTexture(filename);
  }
  
  Background.bg_index = 0; //default
}

void DrawBackground()
{
  DrawTexture(Background.texture[Background.bg_index],0,0,WHITE);
}

#define C_MAX_TEXTSPLIT_COUNT 128
#define MAX_TEXT_BUFFER_LENGTH 100

void draw_dial()
{
  DrawBackground();
  

  //Draw characters on the screen
  for (int i = 0; i < CHARACTER_NUMBER; i++)
  {
    if (CharaList[i].visible)
    { //will change later
      DrawTexture(CharaList[i].base_image, CharaList[i].x, CharaList[i].y, WHITE);
      DrawTexture(CharaList[i].expression[CharaList[i].expression_index], CharaList[i].x, CharaList[i].y, WHITE);
    }
  }

  //DRAW Text & Textbox


  if (!inMenuChoice)
  {
    DrawTexture(UI_IMAGE.textbox,(screenWidth-UI_IMAGE.textbox.width)/2,315,WHITE);

    DrawText(chara_name, 163+10, 315+10, 20, UI_TEXTBOX_NAME_COLOR);
    DrawText(disp_text, 163+10, 315+30, 20, UI_TEXTBOX_TEXT_COLOR);
    // DrawTextEx(GetFontDefault(),disp_text,(Vector2){163+20,315+40},(float)20,(float)1,DARKGRAY);
  }

  DrawUI(choice_sel);
}

void updt_dial()
{

  //"Tweening"
  for (int i = 0; i < CHARACTER_NUMBER; i++)
  {
    if (CharaList[i].x != CharaList[i].gotox)
    {
        //This is just broken :(

      // CharaList[i].x += (CharaList[i].gotox - CharaList[i].x) / 10;
      // CharaList[i].x = CharaList[i].gotox;
      //Todo : have some real tweening going on

      //-c * math.cos(t/d * (math.pi/2)) + c + b
      //-CharaList[i].gotox * math.cos(10/3 * (math.pi/2)) + CharaList[i].x + 100

      // CharaList[i].x = -CharaList[i].gotox * cos(10/3 * (3.14f/2)) + CharaList[i].x + 100;
    }
  }

  if (SCRPT[index].t == A || SCRPT[index].t == N)
  {
    if (!inMenuPause)
    {
      if (BTNP("A"))
      {
        if (cursor < strlen(text_to_display))
        {
          cursor = strlen(text_to_display);
        } //display the whole text
        else
        {
          if (index < sizeof(SCRPT) / sizeof(SCRPT[0]) - 1)
          {
            index++;
            cursor = 1;

            memset(disp_text, 0, 64); //Vider le string

            init_dial();
          }
          else
          {
            game_st = END;
            cursor = 0;
          }
        }
      }

      if (cursor < strlen(text_to_display))
      {
        cursor++;
      }
      strncpy(&disp_text, text_to_display, cursor);
    }
  }

  //MENU MANAGER
  if (BTNP("START"))
  {
    // playSomeSound();
    choice_sel = 0;
    if (inMenuPause)
    {
      ListMenuPage[pause_menu_index].visible = false;
      for (int i = pause_menu_index; i < MenuPageNumber; i++)
      {
        ListMenuPage[i].visible = false;
      }

      SAVECONFIG();
    }
    else
    {
      ListMenuPage[pause_menu_index].visible = true;
    }
    inMenuPause = !inMenuPause;

    if (inMenuPause)
    {
      nb_choice = GetVisibleChoiceNumber(pause_menu_index); //Ugly but at least it doesn't crash
      //Check how many visible choice exist
      //Maybe do the same for the choice for consistency sake
    }
    else
    {
      // nb_choice = 0; //reset, if C: it will be handled in the parser
      init_dial();
    }
  }

  if (inMenuChoice || inMenuPause)
  {
    //Handle Input when in menu
    //Up & Down
    if (BTNP("DOWN"))
    {
      if (choice_sel < nb_choice - 1)
      {
        choice_sel++;
        if (!inMenuPause)
        {
          choice_sel_index = ChoiceCollection[c_atoi(SCRPT[index].c)][choice_sel + 1];
        }
      }
    }
    if (BTNP("UP"))
    {
      if (choice_sel > 0)
      {
        choice_sel--;
        if (!inMenuPause)
        {
          choice_sel_index = ChoiceCollection[c_atoi(SCRPT[index].c)][choice_sel + 1];
        }
      }
    }
  }

  if (inMenuChoice && !inMenuPause)
  {
    //Press A in choice
    if (BTNP("A"))
    {
      for (int i = 0; i < LABELS_NUMBERS; i++)
      {
        if (ListLabels[i].name == ListeChoix[choice_sel_index].jmp)
        {
          index = ListLabels[i].value;

          inMenuChoice = false;
          ListMenuPage[choice_menu_index].visible = false;
          for (int i2 = 0; i2 < MAX_ITEMS_MENU_PAGE; i2++)
          {
            ListMenuPage[choice_menu_index].items[i2].visible = false;
          }
        }
      }
      choice_sel = 0;
      init_dial();
    }
  }

  if (inMenuPause)
  {
    // CHOICE_ITEM,
    // SLIDER,
    // CHECKBOX,
    // INPUT,
    // LIST,
    // SCRIPT_RUNNER,
    // MENU_NAV

    static MENU_ITEM item_menu;
    for (int i = pause_menu_index; i < MAX_ITEMS_MENU_PAGE; i++)
    {
      if (ListMenuPage[i].visible)
      {
        item_menu = ListMenuPage[i].items[choice_sel];

        switch (item_menu.type)
        {
        case CHECKBOX:
        {
          if (BTNP("A"))
          {
            *item_menu.variable = !*item_menu.variable;
          }
          break;
        }
        case SLIDER:
        {
          if (BTN("LEFT"))
          {
            if (*item_menu.variable - 1 >= item_menu.values[0])
            {
              *item_menu.variable -= 1;
            }
          }
          else if (BTN("RIGHT"))
          {
            if (*item_menu.variable + 1 <= item_menu.values[1])
            {
              *item_menu.variable += 1;
            }
          }
          break;
        }
        case SCRIPT_RUNNER:
          if (BTNP("A"))
          {
            item_menu.function(item_menu.variable); //It's super hacky, but I love it
          }
          break;
        case MENU_NAV:
          if (BTNP("A"))
          {
            // item_menu.function(item_menu.variable); //It's super hacky, but I love it
            ListMenuPage[item_menu.param].visible = true;
            ListMenuPage[i].visible = false;
            choice_sel = 0;
            nb_choice = GetVisibleChoiceNumber(item_menu.param);
            playSomeSound();
          }
          break;
        case INPUT:
        {
          if (BTN("LEFT"))
          {
            if (*item_menu.variable - 1 >= item_menu.values[0])
            {
              *item_menu.variable -= 1;
            }
          }
          else if (BTN("RIGHT"))
          {
            if (*item_menu.variable + 1 <= item_menu.values[1])
            {
              *item_menu.variable += 1;
            }
          }
          break;
        }
        }
      }
    }
  }
};

void draw_menu()
{
}

void updt_menu()
{
  if (BTNP("A"))
  {
    game_st = DIAL;
    init_dial();
  }
}

void draw_end()
{
  //draw_dial();
  //   vrambuf_put(NTADR_A(14,10),"FIN",3);
  //   vrambuf_put(NTADR_A(5,20),"Merci d'avoir joue! *",21);
}

void updt_end()
{
  if (BTNP("A"))
  {
    game_st = MAIN_MENU;
    index = 0;
  }
}

int main(int argc, char *argv[])
{
  // Initialization
  //--------------------------------------------------------------------------------------

  InitWindow(screenWidth, screenHeight, "VNES_PC");
  InitAudioDevice();

  beep = LoadSound("./assets/audio/sound/beep1.wav");
  ListMenuPage[pause_menu_index].items[4].function=SAVEGAME; //Yes, this is very ugly, but honey I had no choices
  ListMenuPage[pause_menu_index].items[5].function=LOADGAME;

  LOADCONFIG();

  loadCharacterSprites();
  loadBackgroundSprites();
  loadUI_Texture();
  ParseLabels();

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------
  init_dial();

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update

    switch (game_st)
    {
    case MAIN_MENU:
    {
      // if (debug_mode){vrambuf_put(NTADR_A(2,2),"Game",4);}
      updt_menu();

      break;
    }
    case DIAL:
    {
      // if (debug_mode){vrambuf_put(NTADR_A(2,2),"Dialogue",8);vrambuf_put(NTADR_A(2,3),index_txt,3);}
      updt_dial();

      break;
    }
    case END:
    {
      // if (debug_mode){vrambuf_put(NTADR_A(2,2),"END",3);}
      updt_end();

      break;
    }
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);

    switch (game_st)
    {
    case MAIN_MENU:
    {
      // if (debug_mode){vrambuf_put(NTADR_A(2,2),"Game",4);}

      draw_menu();
      break;
    }
    case DIAL:
    {
      // if (debug_mode){vrambuf_put(NTADR_A(2,2),"Dialogue",8);vrambuf_put(NTADR_A(2,3),index_txt,3);}

      draw_dial();

      break;
    }
    case END:
    {
      // if (debug_mode){vrambuf_put(NTADR_A(2,2),"END",3);}

      draw_end();

      break;
    }
    }

    // DrawText("This is a raylib example", 10, 40, 20, DARKGRAY);

    DrawFPS(10, 10);

    if (a_pressed)
    {
      DrawText("A", 200, 10, 20, DARKGRAY);
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

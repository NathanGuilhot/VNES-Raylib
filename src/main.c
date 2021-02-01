/*******************************************************************************************
*
*   VNES - Raylib port
*
********************************************************************************************/

#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

const char btn_next[] = {0x16};

//----Enums

enum GAME_STATE
{
  MENU,
  DIAL,
  CHOICE,
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
  MOV, /*Move sprite*/
  CJUMP, /*CONDITIONAL JUMP*/
  CFLAGS, /*CHANGE VALUE OF A FLAG*/
  SWPM /*SWAP EXPRESSION*/,

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
unsigned char cursor = 1;
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

#define FLAGS_NUMBER 10
int flag_index = 0;

FLAGS FlagList[FLAGS_NUMBER];

void AddFlag(char *pKey, int pValue)
{
  FlagList[flag_index].key = pKey;
  FlagList[flag_index].value = pValue;

  flag_index++;
}

#define MAX_EXPRESSION 2 //will be defined in script.h later

//---- Characters
typedef struct
{
  char *key;
  char *name;
  char *image_name;

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

char *chara_name = "Character Name";
char buffText[10]; //Utile pour le parse du text
char *first_word;

// void AddChara(char* pKey, char* pName){
// 	  CharaList[chara_index].key = pKey;
// 	  CharaList[chara_index].name = pName;

// 	chara_index++;
// }
bool init_done = false; //Used to only launch init_dial once

bool a_pressed = false; //A
bool b_pressed = false; //B
bool u_pressed = false; //UP
bool d_pressed = false; //DOWN
bool l_pressed = false; //LEFT (gauche)
bool r_pressed = false; //RIGHT (drouate)

const bool debug_mode = false;

int i;
int i2;
char oam_id;

//face var
char sprEl = 0; //gauche
char sprEr = 0; //Drouate
char sprM = 1;  //Bouche

int scrnBrightness = 0;
bool dispAnge = false;

char *index_txt = "indx";
char nb_choice = 1;
char *txt_choix = "choix";

#define FR 1 //bolean, 1 = FR 0 = EN

//Visual Novel Content
#if FR
#include "script_fr.h"
#else
#include "script_en.h"
#endif

typedef struct
{
  char *name;
  int value;
} Label;

Label ListLabels[LABELS_NUMBERS];

void ParseLabels()
{
  for (i2 = 0; i2 < LABELS_NUMBERS; i2++)
  {
    ListLabels[i2].name = "null";
    ListLabels[i2].value = 0;
  }

  for (i = 0; i < sizeof(SCRPT) / sizeof(SCRPT[0]); i++)
  {
    if (SCRPT[i].t == LABEL)
    {
      //L'ajouter à la liste, au premier endroit vide
      for (i2 = 0; i2 < LABELS_NUMBERS; i2++)
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

    //todo : get image size
    CharaList[i].y = GetScreenHeight() - 400;
    CharaList[i].x = (GetScreenWidth() >> 1);

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
  }
}

const char expr[] = {
    //liste des expressions
    211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221
    //0'v'   1 :)  	2 :|   	3 :(   	4 :D  	5 D:    6 A_A	7 /	8 \	9 é	10 è
};

int c_atoi(char *str)
{ //custom atoi function so I don't need the stdlib
  int res = 0;
  for (i = 0; str[i] != '\0'; i++)
    res = res * 10 + str[i] - '0';
  return res;
  //Credit : https://www.geeksforgeeks.org/write-your-own-atoi/
}

#define C_MAX_TEXTSPLIT_COUNT 128
#define MAX_TEXT_BUFFER_LENGTH 100

// Split string into multiple strings
const char **c_TextSplit(const char *text, char delimiter, int *count)
{
  // NOTE: Current implementation returns a copy of the provided string with '\0' (string end delimiter)
  // inserted between strings defined by "delimiter" parameter. No memory is dynamically allocated,
  // all used memory is static... it has some limitations:
  //      1. Maximum number of possible split strings is set by MAX_TEXTSPLIT_COUNT
  //      2. Maximum size of text to split is MAX_TEXT_BUFFER_LENGTH

  static const char *result[C_MAX_TEXTSPLIT_COUNT] = {NULL};
  static char buffer[MAX_TEXT_BUFFER_LENGTH] = {0};
  memset(buffer, 0, MAX_TEXT_BUFFER_LENGTH);

  result[0] = buffer;
  int counter = 0;

  if (text != NULL)
  {
    counter = 1;

    // Count how many substrings we have on text and point to every one
    for (int i = 0; i < MAX_TEXT_BUFFER_LENGTH; i++)
    {
      buffer[i] = text[i];
      if (buffer[i] == '\0')
        break;
      else if (buffer[i] == delimiter)
      {
        buffer[i] = '\0'; // Set an end of string at this point
        result[counter] = buffer + i + 1;
        counter++;

        if (counter == C_MAX_TEXTSPLIT_COUNT)
          break;
      }
    }
  }

  *count = counter;
  return result;
}

void draw_ange()
{
  //   vrambuf_clear();
  //   ppu_off();

  //   vram_adr(NTADR_A(0,20));
  //   vram_fill(11, 32*1);

  //   for (i=0;i<sizeof(ANGESPR)/8; i++){ //Draw_Ange
  // vram_adr(NTADR_A(11,5+i));
  // vram_write(ANGESPR[i],8);
  //   }

  //   ppu_on_all();
}

void draw_ange_face()
{
  //   oam_id = oam_spr(111, 67, expr[sprEl], 2, oam_id); //  o
  //   oam_id = oam_spr(129, 67, expr[sprEr], 2, oam_id); //          O

  //   oam_id = oam_spr(121, 73, expr[sprM], 2, oam_id);  //    ___
}

void draw_dial()
{
  DrawText(chara_name, 10, 75, 20, DARKGRAY);
  DrawText(disp_text, 10, 100, 20, DARKGRAY);

  //Dessin du visage
  // if (dispAnge){draw_ange_face();};

  //Draw characters on the screen
  for (int i = 0; i < CHARACTER_NUMBER; i++)
  {
    if (CharaList[i].visible)
    { //will change later
      DrawTexture(CharaList[i].base_image, CharaList[i].x, CharaList[i].y, WHITE);
      DrawTexture(CharaList[i].expression[CharaList[i].expression_index], CharaList[i].x, CharaList[i].y, WHITE);
    }
  }

  DrawText(CharaList[0].name, 300, 10, 10, BLACK);
}

void init_dial()
{
  if (init_done == false)
  {
    //Managing les passages spéciaux
    switch (SCRPT[index].t)
    {
    case C:
    {
      // clrscr();
      game_st = CHOICE;
      // init_draw_choice();
      choice_collection_index = c_atoi(SCRPT[index].c);
      choice_sel_index = ChoiceCollection[choice_collection_index][choice_sel + 1];
      break;
    }
    case J:
    {
      for (i = 0; i < sizeof(ListLabels) / sizeof(ListLabels[0]); i++)
      {
        if (ListLabels[i].name == SCRPT[index].c)
        {
          index = ListLabels[i].value;
          break;
        }
      }
      break;
    }
    case SWPEL:
    {
      sprEl = c_atoi(SCRPT[index].c);
      index++;
      break;
    }
    case SWPER:
    {
      sprEr = c_atoi(SCRPT[index].c);
      index++;
      break;
    }
    case SWPM:
    {

      strncpy(buffText, SCRPT[index].c, 10);
      first_word = strtok(buffText, " ");

      char id_expression[10];

      for (i = 0; i < CHARACTER_NUMBER; i++)
      {
        if (CharaList[i].key != NULL)
        {
          if (strcmp(first_word, CharaList[i].key) == 0)
          {
            strcpy(id_expression, SCRPT[index].c + strlen(first_word) + 1);

            CharaList[i].expression_index = c_atoi(id_expression);
          }
        }
      }

      index++;
      break;
    }
    case H:
    {

      strncpy(buffText, SCRPT[index].c, 10);
      first_word = strtok(buffText, " ");

      for (i = 0; i < CHARACTER_NUMBER; i++)
      {
        if (CharaList[i].key != NULL)
        {
          if (strcmp(first_word, CharaList[i].key) == 0)
          {
            CharaList[i].visible = !CharaList[i].visible;
          }
        }
      }

      index++;
      break;
    }
    case N:
    {
      //Parse le texte pour chercher les noms
      //On veux éviter de faire ça à chaque fois si possible

      strncpy(buffText, SCRPT[index].c, 10);
      first_word = strtok(buffText, " ");

      for (i = 0; i < CHARACTER_NUMBER; i++)
      {
        if (CharaList[i].key != NULL)
        {
          if (strcmp(first_word, CharaList[i].key) == 0)
          {
            chara_name = CharaList[i].name;
            SCRPT[index].c += strlen(first_word) + 1;
            init_done = true; //what?

            break;
          }
          else
          {
            chara_name = "";
          }
        }
      }
      break;
    }
    case F:
    {
      game_st = END;
      index = 0;

      break;
    }
    case LABEL:
    {
      index++;
      break;
    }
    case MOV:
    {

      char mov_to[10];

      strncpy(buffText, SCRPT[index].c, 10);
      first_word = strtok(buffText, " ");

      for (i = 0; i < CHARACTER_NUMBER; i++)
      {
        if (CharaList[i].key != NULL)
        {
          if (strcmp(first_word, CharaList[i].key) == 0)
          {
            strcpy(mov_to, SCRPT[index].c + strlen(first_word) + 1);

            CharaList[i].gotox = c_atoi(mov_to);

            break;
          }
        }
      }

      index++;
      break;
    }
    case CJUMP:
    {
      //CONDITIONAL JUMP
      //C = FLAGKEY SIGN VALUE KEYLABEL1

      //PARSE C

      //GET THE FLAGKEY

      //EVALUATE CONDITION (DETERMINED BY THE SIGN)

      //JUMP TO THE LABEL IF TRUE

      //ELSE : INDEX++ (et là tu mets un CJUMP ou un JUMP)

    }
    case CFLAGS:
    {
      //CHANGE FLAG VALUE
      //C = FLAGKEY (optional +/-) VALUE

      //GET THE FLAGKEY

      //CHECK IF SECOND ARGUMENT IS + OR -
        //IF TRUE : ADD OR SUBSTRACT THE THIRD ARGUMENT
        //ELSE    : DEFINE VALUE OF FLAG TO THIRD ARGUMENT
      
      //INDEX ++
    }

    }
  }
}

void updt_dial()
{

  init_dial();

  for (int i = 0; i < CHARACTER_NUMBER; i++)
  {
    if (CharaList[i].x != CharaList[i].gotox)
    {
      CharaList[i].x += (CharaList[i].gotox - CharaList[i].x) / 10;

      //Todo : have some real tweening going on

      //-c * math.cos(t/d * (math.pi/2)) + c + b
      //-CharaList[i].gotox * math.cos(10/3 * (math.pi/2)) + CharaList[i].x + 100

      // CharaList[i].x = -CharaList[i].gotox * cos(10/3 * (3.14f/2)) + CharaList[i].x + 100;
    }
  }

  if (SCRPT[index].t == A || SCRPT[index].t == N)
  {
    if (IsKeyDown(KEY_SPACE))
    {
      if (!a_pressed)
      {
        a_pressed = true;
        if (cursor < strlen(SCRPT[index].c))
        {
          cursor = strlen(SCRPT[index].c);
        } //Affiche tout le texte; pas top parce que taille varibale pour le texte
        else
        {
          if (index < sizeof(SCRPT) / sizeof(SCRPT[0]) - 1)
          {
            index++;
            cursor = 1;
            init_done = false;

            // disp_text[64];
            memset(disp_text, 0, 64); //Vider le string
          }
          else
          {
            game_st = END;
            cursor = 0;
          }
        }
      }
    }
    else
    {
      a_pressed = false;
    }

    if (cursor < strlen(SCRPT[index].c))
    {
      cursor++;
    }
    strncpy(&disp_text, SCRPT[index].c, cursor);
    // disp_text = SCRPT[index].c;
  }
}

void draw_menu()
{

#if FR
  //   vrambuf_put(NTADR_A(1,1),"Fr",2);
#else
  //   vrambuf_put(NTADR_A(1,1),"En",2);
#endif
}

void updt_menu()
{
  if (IsKeyDown(KEY_SPACE))
  {
    if (!a_pressed)
    {
      game_st = DIAL;
      //   clrscr();
      if (dispAnge)
      {
        draw_ange();
      }
      a_pressed = true;
    }
  }
  else
  {
    a_pressed = false;
  }
}

void draw_choice()
{
  // draw_dial();

  int choice_index = 0; //Pour l'affichage

  nb_choice = ChoiceCollection[c_atoi(SCRPT[index].c)][0];

  for (i = 1; i <= nb_choice; i++)
  {
    choice_index = ChoiceCollection[choice_collection_index][i];
    txt_choix = ListeChoix[choice_index].txt;
    DrawText(txt_choix, 10, 70 + 15 * i, 10, BLACK);
    //     // vrambuf_put(NTADR_A(4,15+i+i),txt_choix, strlen(txt_choix)); //ugly repetition
  }
  DrawText(">", 5, 70 + 15 * (choice_sel + 1), 10, BLACK);
}

void updt_choice()
{
  //Selection
  if (IsKeyDown(KEY_SPACE))
  {
    if (!a_pressed)
    {
      // index = ListeChoix[ChoiceCollection[c_atoi(SCRPT[index].c)][choice_sel+1]].jmp;
      for (i = 0; i < sizeof(ListLabels) / sizeof(ListLabels[0]); i++)
      {
        //char jumpto = ListeChoix[ChoiceCollection[c_atoi(SCRPT[index].c)][choice_sel+1]].jmp;
        if (ListLabels[i].name == ListeChoix[choice_sel_index].jmp)
        {
          index = ListLabels[i].value;
        }
      }
      // index--;
      game_st = DIAL;
      //   clrscr();
      if (dispAnge)
      {
        draw_ange();
      } //(?)
      a_pressed = true;
      choice_sel = 0;
    }
  }
  else
  {
    a_pressed = false;
  }

  //Up & Down
  if (IsKeyPressed(KEY_DOWN))
  {
    if (choice_sel < nb_choice - 1)
    {
      choice_sel++;
      choice_sel_index = ChoiceCollection[c_atoi(SCRPT[index].c)][choice_sel + 1];
    }
  }
  if (IsKeyPressed(KEY_UP))
  {
    if (choice_sel > 0)
    {
      choice_sel--;
      choice_sel_index = ChoiceCollection[c_atoi(SCRPT[index].c)][choice_sel + 1];
    }
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
  if (IsKeyDown(KEY_SPACE))
  {
    if (!a_pressed)
    {
      game_st = MENU;
      //   clrscr();
      a_pressed = true;
      index = 0;
    }
  }
  else
  {
    a_pressed = false;
  }
}

int main()
{
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "VNES_PC");

  loadCharacterSprites();
  ParseLabels();

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update

    switch (game_st)
    {
    case MENU:
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
    case CHOICE:
    {
      // if (debug_mode){vrambuf_put(NTADR_A(2,2),"Choice",6);}
      updt_choice();

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
    case MENU:
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
    case CHOICE:
    {
      // if (debug_mode){vrambuf_put(NTADR_A(2,2),"Choice",6);}

      draw_choice();

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

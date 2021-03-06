//SCRPT_Defs

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
  MUSIC,
  SOUND,

  // WILL NOT BE USED IN THE END :
  SWPEL /*SWAP LEFT EYE*/,
  SWPER /*SWAP RIGHT EYE*/,
  A /*ANGE*/,
};

typedef struct Passage Passage;
struct Passage
{
  enum DIAL_T t; //Passage type
  char *c;       //Content, in general the text to display or a command
};

typedef struct Choice Choice;
struct Choice
{
  char *key;
  char *txt; //Display text of the choice
  char *jmp; //Label to jump to
};

//---- Flags
typedef struct
{
  char *key;
  int value;
} FLAGS;


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

typedef struct BACKGROUND{
  char *images[MAX_BACKGROUND];
  Texture2D texture[MAX_BACKGROUND];
  int bg_index;
  int bg_next;
  bool in_transition;
  float dissolve_duration;
  unsigned char opacity;
} BACKGROUND;

typedef struct Music_list{
  Music music_list[MAX_MUSIC];
  char *music_name[MAX_MUSIC];
  int music_playing;
  bool isplaying;
} Music_list;

typedef struct Sound_list{
  Sound sound_list[MAX_SOUND];
  char *sound_name[MAX_SOUND];
  int sound_playing;
} Sound_list;


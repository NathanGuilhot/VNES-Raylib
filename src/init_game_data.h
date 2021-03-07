//Init_data -> Initatialise and load game data

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
        if (!strcmp(ListLabels[i2].name,"null"))
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

    CharaList[i].base_image = VN_LoadTexture(base_filename);

    CharaList[i].y = screenHeight - CharaList[i].base_image.height;
    CharaList[i].x = (screenWidth - CharaList[i].base_image.width)/2; //Center of the screen
    CharaList[i].gotox = CharaList[i].x;
    CharaList[i].gotoy = CharaList[i].y;

    //Load expressions
    for (int i2 = 0; i2 < MAX_EXPRESSION; i2++)
    {
      char file_expression[32]; 
      strcpy(file_expression, TextFormat("%s%d",expression_filename, i2+1));
      strcat(file_expression, ".png");
      // printf("File : %s\n",file_expression);
      // printf("Index : %d/%d\n",i2,MAX_EXPRESSION);
      CharaList[i].expression[i2] = VN_LoadTexture(file_expression);
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
  for (int i=0; i<MAX_BACKGROUND; i++){
    char filename[32] = "./assets/img/";
    strcat(filename, Background.images[i]);
    strcat(filename, ".png");

    Background.texture[i] = VN_LoadTexture(filename);
  }
  
  Background.bg_index = 0; //default
  Background.bg_next = 1;
  Background.opacity = 250;
  Background.dissolve_duration = 0.5;

}

void loadAudio(){
  //Load Music
  //VN_LoadMusicStream(path)

  
  
  for (int i=0; i<MAX_MUSIC; i++)
  {
    if (MusicList.music_name[i])
    {
      char filename[64] = "./assets/audio/music/";
      // strcat(filename, "./assets/audio/music/");
      strcat(filename, MusicList.music_name[i]);
      strcat(filename, ".mp3");

      MusicList.music_list[i] = VN_LoadMusicStream(filename);
    }
  }

  MusicList.music_playing = 0;
  

  // //Load Sounds
  // //VN_LoadSound(path)
  
  for (int i=0; i<MAX_SOUND; i++)
  {
    if (SoundList.sound_name[i])
    {
      char filename[64] = "./assets/audio/sound/";
      // strcat(filename, "./assets/audio/sound/");
      strcat(filename, SoundList.sound_name[i]);
      strcat(filename,".wav");

      SoundList.sound_list[i] = VN_LoadSound(filename);
    }
  }

  MusicList.music_playing = 0;
}

void InitGame()
{
  InitWindow(screenWidth, screenHeight, "VNES_PC");
  VN_InitAudioDevice();

  // Music music_test;
  // music_test = VN_LoadMusicStream("./assets/audio/music/nichijou1.mp3");
  beep = VN_LoadSound("./assets/audio/sound/beep1.wav");

  Text_font = VN_LoadFont("./assets/font/Ubuntu-Regular.ttf");
  Text_font_bold = VN_LoadFont("./assets/font/Ubuntu-Bold.ttf");
  Text_font_italic = VN_LoadFont("./assets/font/Ubuntu-Italic.ttf");
  Text_font_bolditalic = VN_LoadFont("./assets/font/Ubuntu-BoldItalic.ttf");

  LOADCONFIG();

  loadCharacterSprites();
  loadBackgroundSprites();
  loadUI_Texture();
  loadAudio();

  // PlayMusicStream(MusicList.music_list[0]);

  ParseLabels();

}
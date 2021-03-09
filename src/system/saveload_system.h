//SAVE SYSTEM

/*Why having 2 exactly identical formats?
It's honestly just in case I want to store string at some point,
which are not handled the same way by printf and scanf */

const char* FORMAT_INDEX_IN = "(IND %4d)\n";                 //Index
const char* FORMAT_FLAGS_IN = "(FLG %2d)\n";              //Key, value
const char* FORMAT_CHARA_IN = "(CHR %d,%4d,%4d,%d,%2d)\n"; //visible, x, y, zindex, expression_index
const char* FORMAT_BG_IN = "(BGR %2d)\n"; //index
const char* FORMAT_OPTION_IN = "(OPT %3d,%d,%2d)\n";        //volume, check
const char* FORMAT_MUSIC_IN = "(MUS %2d,%d)\n";        //index, isplaying

const char* FORMAT_INDEX_OUT = "(IND %4d)\n";               //Index
const char* FORMAT_FLAGS_OUT = "(FLG %2d)\n";              //Key, value
const char* FORMAT_CHARA_OUT = "(CHR %d,%4d,%4d,%d,%1d)\n"; //visible, x, y, zindex, expression_index
const char* FORMAT_BG_OUT = "(BGR %2d)\n"; //index
const char* FORMAT_OPTION_OUT = "(OPT %3d,%d,%2d)\n";        //volume, check, CPS
const char* FORMAT_MUSIC_OUT = "(MUS %2d,%d)\n";        //index, isplaying


FILE *SaveFileOpen;
char* save_file_path = "./save/savefile.txt";
char* config_file_path = "./save/config.txt";

bool SAVEGAME(){
    /*Data we need to save:
    - index
    - FlagList
    - CharaList (especially: bool visible, int x, int y, int zindex, int expression_index)
    And... that's it?    
    */

   SaveFileOpen = fopen(save_file_path, "w");
    //    fprintf(SaveFileOpen, "Bonjour !\n");
   fprintf_s(SaveFileOpen, FORMAT_INDEX_OUT,index);
   for (int i = 0; i < FLAGS_NUMBER; i++)
   {
    fprintf_s(SaveFileOpen, FORMAT_FLAGS_OUT, FlagList[i].value);
   }
   for (int i = 0; i < CHARACTER_NUMBER; i++)
   {
    fprintf_s(SaveFileOpen, FORMAT_CHARA_OUT, CharaList[i].visible, CharaList[i].x, CharaList[i].y, CharaList[i].zindex, CharaList[i].expression_index);

    CharaList[i].gotox = CharaList[i].x;
    CharaList[i].gotoy = CharaList[i].y;
   }
   fprintf_s(SaveFileOpen, FORMAT_BG_OUT, Background.bg_index);
   fprintf_s(SaveFileOpen, FORMAT_MUSIC_OUT, MusicList.music_playing, MusicList.isplaying);

   fclose(SaveFileOpen);
   return true;
}



bool LOADGAME(){
    StopMusicStream(MusicList.music_list[MusicList.music_playing]); //Stop the music, we'll restart it later
    if (game_st!=DIAL){ //Used when we are in the menu
        game_st=DIAL;
        ListMenuPage[main_menu_index].visible=false;
    }

    SaveFileOpen = fopen(save_file_path, "r+");
    if (SaveFileOpen!=NULL)
    { //If file exist
        fscanf_s(SaveFileOpen,FORMAT_INDEX_IN, &index);
        for (int i = 0; i < FLAGS_NUMBER; i++)
        {
            fscanf_s(SaveFileOpen, FORMAT_FLAGS_IN, &FlagList[i].value);
        }
        for (int i = 0; i < CHARACTER_NUMBER; i++)
        {
            fscanf_s(SaveFileOpen, FORMAT_CHARA_IN, &CharaList[i].visible, &CharaList[i].x, &CharaList[i].y, &CharaList[i].zindex, &CharaList[i].expression_index);
            CharaList[i].gotox = CharaList[i].x;
            CharaList[i].gotoy = CharaList[i].y;
        }
        fscanf_s(SaveFileOpen, FORMAT_BG_IN, &Background.bg_index);
        fscanf_s(SaveFileOpen,FORMAT_MUSIC_IN, &MusicList.music_playing, &MusicList.isplaying);
        
        if (MusicList.isplaying)
        {
            PlayMusicStream(MusicList.music_list[MusicList.music_playing]);
        }


        init_dial(); //?
        // disp_text=""
        strcpy(disp_text,"");
    }
    else{
        playSomeSound();
        return false;
        
    }

    fclose(SaveFileOpen);    
    return true;
}

bool SAVECONFIG(){
    /*Save OPTION*/
   SaveFileOpen = fopen(config_file_path, "w");

   fprintf_s(SaveFileOpen, FORMAT_OPTION_OUT, OPTION.volume, OPTION.check, OPTION.cps);

   fclose(SaveFileOpen);
}



bool LOADCONFIG(){
    SaveFileOpen = fopen(config_file_path, "r+");
    if (SaveFileOpen!=NULL)
    { //If file exist
        fscanf_s(SaveFileOpen, FORMAT_OPTION_IN, &OPTION.volume, &OPTION.check, &OPTION.cps);
    }
    else{
        playSomeSound();
    }

    fclose(SaveFileOpen);    
}
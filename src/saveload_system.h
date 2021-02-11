//SAVE SYSTEM

const char* FORMAT_INDEX_IN = "(IND %4d)\n";                 //Index
const char* FORMAT_FLAGS_IN = "(FLG %2d)\n";              //Key, value
const char* FORMAT_CHARA_IN = "(CHR %d,%4d,%4d,%d,%2d)\n"; //visible, x, y, zindex, expression_index
const char* FORMAT_OPTION_IN = "(OPT %3d,%d)\n";        //volume, check

const char* FORMAT_INDEX_OUT = "(IND %4d)\n";               //Index
const char* FORMAT_FLAGS_OUT = "(FLG %2d)\n";              //Key, value
const char* FORMAT_CHARA_OUT = "(CHR %d,%4d,%4d,%d,%1d)\n"; //visible, x, y, zindex, expression_index
const char* FORMAT_OPTION_OUT = "(OPT %3d,%d)\n";        //volume, check


FILE *SaveFileOpen;
char* save_file_path = "./save/savefile.txt";

bool SAVEGAME(){
    /*Data we need to save:
    - index
    - FlagList
    - CharaList (especially: bool visible, int x, int y, int zindex, int expression_index)
    - struct OPTION
    And... that's it?    
    */
   char* save_data;

   SaveFileOpen = fopen(save_file_path, "w");
    //    fprintf(SaveFileOpen, "Bonjour !\n");
   fprintf_s(SaveFileOpen, FORMAT_INDEX_OUT,index);
   for (int i = 0; i < sizeof(FlagList)/sizeof(FlagList[0]); i++)
   {
    fprintf_s(SaveFileOpen, FORMAT_FLAGS_OUT, FlagList[i].value);
   }
   for (int i = 0; i < sizeof(CharaList)/sizeof(CharaList[0]); i++)
   {
    fprintf_s(SaveFileOpen, FORMAT_CHARA_OUT, CharaList[i].visible, CharaList[i].x, CharaList[i].y, CharaList[i].zindex, CharaList[i].expression_index);

    CharaList[i].gotox = CharaList[i].x;
    CharaList[i].gotoy = CharaList[i].y;
   }
   fprintf_s(SaveFileOpen, FORMAT_OPTION_OUT, OPTION.volume, OPTION.check);

   fclose(SaveFileOpen);
}



bool LOADGAME(){
    SaveFileOpen = fopen(save_file_path, "r+");
    if (SaveFileOpen!=NULL)
    { //If file exist
        fscanf_s(SaveFileOpen,FORMAT_INDEX_IN, &index);
        for (int i = 0; i < sizeof(FlagList)/sizeof(FlagList[0]); i++)
        {
            fscanf_s(SaveFileOpen, FORMAT_FLAGS_IN, &FlagList[i].value);
        }
        for (int i = 0; i < sizeof(CharaList)/sizeof(CharaList[0]); i++)
        {
            fscanf_s(SaveFileOpen, FORMAT_CHARA_IN, &CharaList[i].visible, &CharaList[i].x, &CharaList[i].y, &CharaList[i].zindex, &CharaList[i].expression_index);
            CharaList[i].gotox = CharaList[i].x;
            CharaList[i].gotoy = CharaList[i].y;
        }
        fscanf_s(SaveFileOpen, FORMAT_OPTION_IN, &OPTION.volume, &OPTION.check);
        
        
        init_dial(); //?
        // disp_text=""
        strcpy(disp_text,"");
    }
    else{
        playSomeSound();
        
    }

    fclose(SaveFileOpen);    
}
//SAVE SYSTEM

bool SAVEGAME(){
    /*Data we need to save:
    - index
    - FlagList
    - CharaList (especially: bool visible, int x, int y, int zindex, int expression_index)
    - struct OPTION
    And... that's it?    
    */


   char* save_data;

   char* save_file = "./save/savefile.txt";

   
   FILE *SaveFileOpen;
   SaveFileOpen = fopen(save_file, "w");
   fprintf(SaveFileOpen, "Bonjour !\n");
   fprintf(SaveFileOpen, "Index : %d !\n",index);
//    fprintf(SaveFileOpen, "Bonjour !\n");
//    fprintf(SaveFileOpen, "Bonjour !\n");
   fclose(SaveFileOpen);

}



bool LOADGAME(){
    
}
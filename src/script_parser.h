// Script parser

//Visual Novel Content
#if FRENCH
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


void init_dial() //Handle parsing and logic
{
  // if (init_done == false) //en fait ça faisait absolument rien du tout mdr
  {
    //Managing les passages spéciaux
    switch (SCRPT[index].t)
    {
    case C:
    {

      ListMenuPage[choice_menu_index].visible = true;
      inMenuChoice = true;
      // choice_sel = 0;
      int choice_index = 0; //Pour l'affichage

      nb_choice = ChoiceCollection[c_atoi(SCRPT[index].c)][0];

      for (i = 1; i <= nb_choice; i++)
      {
        choice_index = ChoiceCollection[choice_collection_index][i];
        
        ListMenuPage[choice_menu_index].items[i - 1].label = ListeChoix[choice_index].txt;
        ListMenuPage[choice_menu_index].items[i - 1].visible = true;
        
      }

      // init_draw_choice();
      choice_collection_index = c_atoi(SCRPT[index].c);
      choice_sel_index = ChoiceCollection[choice_collection_index][choice_sel + 1];
      break;
    }
    case J:
    {
      for (i = 0; i < LABELS_NUMBERS; i++)
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

      strncpy(buffText, SCRPT[index].c, 20);
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

      strncpy(buffText, SCRPT[index].c, 20);
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

      strncpy(buffText, SCRPT[index].c, 20);
      first_word = strtok(buffText, " ");

      for (i = 0; i < CHARACTER_NUMBER; i++)
      {
        if (CharaList[i].key != NULL)
        {
          if (strcmp(first_word, CharaList[i].key) == 0)
          {
            chara_name = CharaList[i].name;
            SCRPT[index].c += strlen(first_word) + 1;
            // init_done = true; //what?

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
    case F: //FIN
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

      strncpy(buffText, SCRPT[index].c, 20);
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
      //C = FLAGKEY SIGN VALUE KEYLABEL

      //PARSE C
      strncpy(buffText, SCRPT[index].c, 20);
      first_word = strtok(buffText, " "); //FLAGKEY
      second_word = strtok(NULL, " ");    //SIGN
      third_word = strtok(NULL, " ");     //VALUE
      fourth_word = strtok(NULL, " ");    //KEYLABEL

      //GET THE FLAGKEY
      for (i = 0; i < FLAGS_NUMBER; i++)
      {
        if (FlagList[i].key != NULL)
        {
          if (strcmp(first_word, FlagList[i].key) == 0)
          {
            static bool flag_condition;

            //EVALUATE CONDITION (DETERMINED BY THE SIGN)
            if (strcmp(second_word, ">") == 0) //+
            {
              flag_condition = (FlagList[i].value > c_atoi(third_word));
            }
            else if (strcmp(second_word, "<") == 0) //+
            {
              flag_condition = (FlagList[i].value < c_atoi(third_word));
            }
            else if (strcmp(second_word, ">=") == 0) //+
            {
              flag_condition = (FlagList[i].value >= c_atoi(third_word));
            }
            else if (strcmp(second_word, "<=") == 0) //+
            {
              flag_condition = (FlagList[i].value <= c_atoi(third_word));
            }
            else if (strcmp(second_word, "==") == 0) //+
            {
              flag_condition = (FlagList[i].value == c_atoi(third_word));
            }
            else if (strcmp(second_word, "!=") == 0) //+
            {
              flag_condition = (FlagList[i].value != c_atoi(third_word));
            }

            //JUMP TO THE LABEL IF TRUE
            if (flag_condition)
            {
              // index = c_atoi(fourth_word);
              //FIND THE KEYLABEL FIRST
              for (i = 0; i < LABELS_NUMBERS; i++)
              {
                if (strcmp(ListLabels[i].name, fourth_word) == 0)
                {
                  index = ListLabels[i].value;
                  break;
                }
              }
            }
            else
            {
              index++;
            }

            break;
          }
        }
      }

      break;
    }
    case CFLAGS:
    {
      //CHANGE FLAG VALUE
      //C = FLAGKEY (optional +/-) VALUE

      strncpy(buffText, SCRPT[index].c, 20);
      first_word = strtok(buffText, " ");
      second_word = strtok(NULL, " ");
      third_word = strtok(NULL, " ");

      //GET THE FLAGKEY
      for (i = 0; i < FLAGS_NUMBER; i++)
      {
        if (FlagList[i].key != NULL)
        {
          if (strcmp(first_word, FlagList[i].key) == 0)
          {
            //CHECK IF SECOND ARGUMENT IS + OR -
            if (strcmp(second_word, "+") == 0) //+
            {
              FlagList[i].value += c_atoi(third_word);
            }
            else if (strcmp(second_word, "-") == 0) //-
            {
              FlagList[i].value -= c_atoi(third_word);
            }
            else if (strcmp(second_word, "*") == 0) //*
            {
              FlagList[i].value *= c_atoi(third_word);
            }
            else if (strcmp(second_word, "/") == 0) // /
            {
              FlagList[i].value /= c_atoi(third_word);
            }
            else
            {
              FlagList[i].value = c_atoi(second_word);
            }

            index++;
            break;
          }
        }
      }

      break;
    }
    }
  }
}
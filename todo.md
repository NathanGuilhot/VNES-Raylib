# VNES - Tracking progress

## Features in progress
*(you can see the Raw if you want to understand what is this mess)*

- [x] Labels
   - [x] Flexible characters addition
   - [x] Datastructure 
  
 - [ ] Trouver un moyen de le rendre flexible dans le code
        - [x] Implementation 
        - [ ] Define characters in script files
          - [x] Load character image and expressions
           - [x] Change expression in SCRPT
           - [ ] Change position in SCRPT
               - [x] Interpolation
                 	- [ ] Variable duration
                  - [ ] Real Tweening
                	- [ ] GotoY support (? Do we really need that ?)
           - [ ] Fix the gotox bug (seems to be defined somewhere else for some reason ?)
           - [x] Hide/Show
    
  - [x] Flags
    - [x] Data Structure
    - [x] Implementation
    - [x] Conditional Jump
    - [x] Chercher comment faire un split en C
      -> char strtok (char* str, const char* delim); 
    
  - [ ] Serious reformatting
    - [ ] Split into multiple files
    - [x] Src folder for the code
    - [ ] delete unused variables and comments
    - [x] Replace n_pressed and inputs check by a function // will be used on other engine; can be easily modified for each platform
      - [x] BTN()
      - [x] BTNP()
    - [ ] Move the SCRIPT include at the top

    

- [x] Background manager
  - [x] Load image
  - [x] Draw the background
  - [x] Change in script
  - [ ] Nicer transition between backgrounds

- [ ] Sound manager
    - [ ] Load Sound and Music
    - [ ] Play Sound from SCRPT
    - [ ] Play Music from SCRPT (loop, fade (???))

      

  - [ ] UI System
    - [x] Datastructure
    - [x] UI Navigation logic (not really robust but that work)
      - [ ] Draw the UI + Logic
        - [x] Slider
        - [x] Button
        - [x] Checkbox
        - [ ] List
        - [x] Script Runner
        - [x] Change page
    - [ ] Move the definition of MenuPage content to the end of the script (so we can pass any function and variable (?)) 
    - [x] Nicer in-game UI (textbox)
      - [ ] Better placement for the textbox
    
  
    
  
  - [ ] Draw free image on screen
    - [ ] Datastructure
    - [ ] Draw images on screen
    - [ ] Define in SCRPT
    - [ ] Load image
    - [ ] Toggle visibility
    - [ ] Position
    - [ ] Tweening
    - [ ] Z-Index (??)
    - [ ] Animation (????)
  
      
  
  - [x] Savefile
    - [x] Save
    - [x] Load
    - [x] Having some sort of persisitent data for settings
    - [ ] Multiple save file : UI ? -> Concatenate the button param/variable into the file path
    - [ ] Do research about save file on DS and PSP
    
      
  
- [x] Executer le parser qu'une seule fois
     - [x] Isoler le code du parser dans un header file (pour pouvoir récupérer les variables globales)
     - [x] Isoler le code qui doit être updaté (typewritter effect, that kind of stuff)


-----
Note taking / brainstroming

Datastructure personnage:
	{
	 key (un char*)
	 nom (char* qui vas être affiché)
	 char* image_name

	 //engine defined:
	 Texture2D base_image;
	 Texture2D expression[MAX_EXPRESSION];
	 int expression_index = 0; //can be user defined I guess, nothing's stoping you
	
	 bool visible = false;
	 int x; int y;
	 int gotox; int gotoy; //for interpolation
	
	 int z_index (?? I will probably need it one day for sure);


​	 
​	}

Idea:
Define the name of a file, and the engine will handle loading it into the array with all needed expression

CharaList[n].img = "ange"
load -> ange base.png, ange happy 1.png, ange happy 2.png, ange angry 2.png...
	or juste "ange base.png" and "ange 1.png" "ange 2.png" "ange n.png" <- Expression, put into an array (less complex I guess, but less user friendly)
																			^ Well, why don't you make a enum ? Or a hashtable ?


NOMBRE_PERSONNAGES;
Chara_index = 0; //Pour la génération

Perso ListePerso[NOMBRE_PERSONNAGES];//Fait genre c'est un array avec une longueur arbitraire
^ Up

//Bad idea, there's no need to add characters at runtime! => Array now defined in script.h
void AddPerso(pKey, pName, &pImg, &listExpression(?)){
	ListePerso[Chara_index].key = pKey
	ListePerso[Chara_index].name = pName
	ListePerso[Chara_index].img = pImg

	Chara_index++;
}
=> Ajoute le perso

-> Dans le SCRPT
//Display
t = H //hide
c = key du personnage //Pour le moment faire en sorte qu'un seul perso à la fois ne soit affiché
	-> Evolution future : c = "key <arg>" -> Position
//Change expression
t = SWM/SWEL/SWER //dans certaines version on pourra simplifier en une/deux commandes
c = "key expression_id" //parse at run time

//Dialogue
t = N (forcément un enum, donc impossible de le def at runtime) < Du coup on peut ne pas avoir d'enum spécifique à un personnage ! (ça évite d'avoir une erreur si la Key est incorrecte)
c = "KEY  DIALOGUE"

-----
DATASTRUCTURE FLAGS
{
	char* key
	bool  value -> A l'avenir, essayer de supporter des entiers
}

NOMBRE_FLAGS;
flag_index = 0;

ListeFlags[NOMBRE_FLAGS]

void AddFlags(char* pKey, bool pValue){

}

//Jump
t = CJUMP
c = FLAGKEY 1 KEYLABEL1 0 KEYLABEL2
c = FLAGKEY (SIGN VALUE KEYLABEL1) (SIGN VALUE KEYLABEL2)

​	-> Si l'un des deux est ommis, juste faire index++
​	-> True et false serait plus intuitif à la redaction; il faut supporter les deux si possible (mieux pour évolution future)

SIGN:
	<, >, ==, <=, >=, !=

//Change flag
t = FLAGS
c = KEY VALUE(TRUE/FALSE)

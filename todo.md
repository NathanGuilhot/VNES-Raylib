# VNES - Tracking progress

##Feature in progress

[###-] Labels
[###--] Flexible characters addition
	[#] Datastructure
	[#] Trouver un moyen de le rendre flexible dans le code
	[#] Implementation 
	[-] Change expression
	[-] Hide/Show
[#---] Flags
	[#] DataStructure
	[-] Implémentation
	[-] Conditional Jump
[#] Chercher comment faire un split en C
	- char strtok (char* str, const char* delim); 


-----

Datastructure personnage:
	{
	 key (un char*)
	 nom (char* qui vas être affiché)
	 img_base (type à definir en fonction des moteur; char* pour neslib) <pointeur!
	 *//Liste expressions ?*
	}

NOMBRE_PERSONNAGES;
Chara_index = 0; //Pour la génération

Perso ListePerso[NOMBRE_PERSONNAGES];//Fait genre c'est un array avec une longueur arbitraire


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
	-> Si l'un des deux est ommis, juste faire index++
	-> True et false serait plus intuitif à la redaction; il faut supporter les deux si possible (mieux pour évolution future)

//Change flag
t = FLAGS
c = KEY VALUE(TRUE/FALSE)

#include<stdio.h> 
#include<stdlib.h> 
#include<String.h>
#include<conio.h>

#define Malloc(type) (type*)malloc(sizeof(type))
#define CMAX 50
#define IdMAX 10
#define NbrMAX 10
#define NbrIDs 100

char Tableau_IDs[NbrIDs][IdMAX];

int indice_Tab_IDs=0;


void ecrire_indice(){
	FILE *f=fopen("data/indice.txt","w");
	fprintf(f, " %d ", indice_Tab_IDs);
    fclose(f);
}
void recuperer_indice(){
	FILE *f=fopen("data/indice.txt","r");
	fscanf(f, " %d ", &indice_Tab_IDs);
    fclose(f);
}



typedef struct _naissance{
	int j,m,a;
	char lieu[CMAX];
} naissance;
typedef struct _personne {
	char nom[CMAX]; //
	char prenom[CMAX];
	char situation[CMAX]; //
	char nom_prenom[CMAX];//
	char sexe; //
	naissance naiss;  
	char id[IdMAX];   
	int  nbr_enf;	//
	char enfs_id[NbrMAX][IdMAX]; //
	char id_pere[IdMAX];
	char id_mere[IdMAX];
	char id_conjoint[IdMAX];
}personne;

typedef struct _noeud{
	personne *infos;
	struct _noeud *gauche;
	struct _noeud *droite;
	int hauteur;
}noeud;

noeud *racineID=NULL,*racineNOM=NULL;

int max(int a, int b) { 
    return (a > b)? a : b; 
} 

noeud* _arbre_creer_noeud(personne *infos){
	noeud *nv=Malloc(noeud);
	if(nv==NULL){
		printf("probleme dans la creation de noeud");				
		return NULL ;}
	nv->infos=infos;
	nv->gauche=NULL;
	nv->droite=NULL;
	nv->hauteur = 1;
	return nv ;
}
int _arbre_hauteur(noeud *N) { 
    if (N == NULL) 
        return 0; 
    return N->hauteur; 
} 

noeud *_arbre_RotationD( noeud *y) { 
     noeud *x = y->gauche; 
     noeud *T2 = x->droite; 
    //  rotation 
    x->droite = y; 
    y->gauche = T2; 
  
    // mise a jour des hauteurs
    y->hauteur = max(_arbre_hauteur(y->gauche), _arbre_hauteur(y->droite))+1; 
    x->hauteur = max(_arbre_hauteur(x->gauche), _arbre_hauteur(x->droite))+1; 
  
    // Return nouveau racine
    return x; 
} 
noeud *_arbre_RotationG( noeud *x) { 
    noeud *y = x->droite; 
    noeud *T2 = y->gauche;  
    // rotation
	y->gauche = x; 
    x->droite = T2; 
  
    //  mise a jour des hauteurs 
    x->hauteur = max(_arbre_hauteur(x->gauche), _arbre_hauteur(x->droite))+1; 
    y->hauteur = max(_arbre_hauteur(y->gauche), _arbre_hauteur(y->droite))+1; 
  
    // Return nouveau racine 
    return y; 
} 

int _arbre_equilibre( noeud *N) { 
    if (N == NULL) 
        return 0; 
    return _arbre_hauteur(N->gauche) - _arbre_hauteur(N->droite); 
} 

noeud* _arbre_rechercher_noeud_parID(char id[],noeud *racine){
	
	if(racine==NULL) return NULL ;
	if(strcmp(id,racine->infos->id)==0) return racine ;
	else
		if(strcmp(id,racine->infos->id)<0)
			return _arbre_rechercher_noeud_parID(id,racine->gauche);
		else
			return _arbre_rechercher_noeud_parID(id,racine->droite);
	
}

void _arbre_inOrdre_parID(noeud *racine) 
{ 
    if(racine != NULL) 
    { 
        
        _arbre_inOrdre_parID(racine->gauche); 
        printf("%s ", racine->infos->id); 
        _arbre_inOrdre_parID(racine->droite); 
    } 
} 

noeud* _arbre_inserer_noeud_parID(personne *infos,noeud *racine){
	
	if(racine==NULL) 
		return(_arbre_creer_noeud(infos));
	if(strcmp(infos->id,racine->infos->id)<0)
			racine->gauche=_arbre_inserer_noeud_parID(infos,racine->gauche);
	else if(strcmp(infos->id,racine->infos->id)>0)
			racine->droite=_arbre_inserer_noeud_parID(infos,racine->droite);
	else {	printf("La personne deja existe");
			return racine ;
			}
			
	racine->hauteur=1 + max(_arbre_hauteur(racine->gauche),_arbre_hauteur(racine->droite));
	int balance=_arbre_equilibre(racine);
	
	if (balance > 1 && strcmp(infos->id,racine->gauche->infos->id)<0) 
        return _arbre_RotationD(racine); 
    //  cas droite droite  
    if (balance < -1 && strcmp(infos->id,racine->droite->infos->id)>0) 
        return _arbre_RotationG(racine); 
    // cas gauche droite 
    if (balance > 1 && strcmp(infos->id,racine->gauche->infos->id)>0) 
    { 
        racine->gauche =  _arbre_RotationG(racine->gauche); 
        return _arbre_RotationD(racine); 
    } 
    // cas droite gauche  
    if (balance < -1 && strcmp(infos->id,racine->droite->infos->id)<0) 
    { 
        racine->droite = _arbre_RotationD(racine->droite); 
        return _arbre_RotationG(racine); 
    } 
	return racine ;
}

noeud* _arbre_inserer_noeud_parNOM(personne *infos,noeud *racine){
	
	if(racine==NULL)
		return(_arbre_creer_noeud(infos));
	if(strcmp(infos->nom_prenom,racine->infos->nom_prenom)<0)
			racine->gauche=_arbre_inserer_noeud_parNOM(infos,racine->gauche);
	else if(strcmp(infos->nom_prenom,racine->infos->nom_prenom)>0)
			racine->droite=_arbre_inserer_noeud_parNOM(infos,racine->droite);
	else {	printf("la personne deja existe");
			return racine ;
			}
			
	racine->hauteur=1 + max(_arbre_hauteur(racine->gauche),_arbre_hauteur(racine->droite));
	int balance=_arbre_equilibre(racine);
	
	if (balance > 1 && strcmp(infos->nom_prenom,racine->gauche->infos->nom_prenom)<0) 
        return _arbre_RotationD(racine); 
    //  cas droite droite  
    if (balance < -1 && strcmp(infos->nom_prenom,racine->droite->infos->nom_prenom)>0) 
        return _arbre_RotationG(racine);
    // cas gauche droite 
    if (balance > 1 && strcmp(infos->nom_prenom,racine->gauche->infos->nom_prenom)>0) 
    { 
        racine->gauche =  _arbre_RotationG(racine->gauche); 
        return _arbre_RotationD(racine); 
    } 
    // cas droite gauche  
    if (balance < -1 && strcmp(infos->nom_prenom,racine->droite->infos->nom_prenom)<0) 
    { 
        racine->droite = _arbre_RotationD(racine->droite); 
        return _arbre_RotationG(racine); 
    } 
	return racine ;
}

//*-*-*-*-*-*-*-*-*-*-*-*-*-* (MENU -personne-) *-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
noeud* _arbre_rechercher_noeud_parNOM(char nom_prenom[],noeud *racine){
	
	if(racine==NULL) return NULL ;
	if(strcmp(nom_prenom,racine->infos->nom_prenom)==0) return racine ;
	else
		if(strcmp(nom_prenom,racine->infos->nom_prenom)<0)
			return _arbre_rechercher_noeud_parNOM(nom_prenom,racine->gauche);
		else
			return _arbre_rechercher_noeud_parNOM(nom_prenom,racine->droite);
	}

void _arbre_inOrdre_parNOM(noeud *racine) 
{ 
    if(racine != NULL) 
    { 
        _arbre_inOrdre_parNOM(racine->gauche); 
        printf("%s ", racine->infos->nom_prenom); 
        _arbre_inOrdre_parNOM(racine->droite); 
    } 
} 

int _personne_ajouter(noeud **racine_arbreId, noeud **racine_arbreNom){
	personne *nv=Malloc(personne);
	strcpy(nv->id,Tableau_IDs[indice_Tab_IDs]);
	printf("\n\n  \t\t ---> ENREGISTREMENT D'UNE NOUVELLE PERSONNE <--\n\n");
	printf("\tnom : ");
	scanf("%s",nv->nom);
	printf("\tprenom : ");
	scanf("%s",nv->prenom);
	strcpy(nv->nom_prenom,nv->nom);
	strcat(nv->nom_prenom,nv->prenom);
	
	//  !!! a faire !!! etudier le cas de l'existance de la personne qu'on veut ajouter (return 0)
	
	printf("\tsexe :\n\t\tF si feminin, M si masculin ;\n\t -> Votre choix : ");
	getchar(); //vider le buffer
	scanf("%c",&nv->sexe);
					printf("\tDate et lieu de naissance :\n ");
					printf("\t\tLe Jour           : ");
         			scanf("%d", &(nv->naiss.j));
		 			printf("\t\tLe Mois           : ");
		 			scanf("%d", &(nv->naiss.m));
         			printf("\t\tL'Annee           : ");
         			scanf("%d", &(nv->naiss.a));
        			printf("\t\tLieu de naissance : ");
        			scanf("%s",nv->naiss.lieu);
	// traitement de l'existance ou non des fils et l'affectation des ID dans le cas d'existance
	
	printf("\tEtat :\n\t\t0 si celibataire, 1 si marie ;\n\t -> Votre choix : ");
	int etat;
	scanf("%d",&etat);
	//getchar(); //vider le buffer
	if(etat==0)
	{
			
		strcpy(nv->situation, "celib");
		nv->nbr_enf=0;}
		else 
		{
			strcpy(nv->situation,"marie");
			printf("\tNombre d'enfants ? ");
			scanf("%d",&nv->nbr_enf);
		}
	if(nv->nbr_enf!=0)
	{
		char prenom_fils[CMAX],nom_prenom_fils[CMAX];
		strcpy(nom_prenom_fils,nv->nom);
		printf("\tEntrer le prenom du :\n");
		int cpt=0;
		for(int i=0;i<nv->nbr_enf;i++){
			printf("\t	- Fils %d : ",i+1);
			scanf("%s",prenom_fils);
			strcat(nom_prenom_fils,prenom_fils);
			noeud *noeudfils=_arbre_rechercher_noeud_parNOM(nom_prenom_fils,*racine_arbreNom);
			if(noeudfils==NULL)
			{
				strcpy(nv->enfs_id[i],"\0");
				cpt++;
				printf("\t\t- INEXISTANT Vous devez l'enregistrer !!\n");
			}
			else {strcpy(nv->enfs_id[i],noeudfils->infos->id);
				strcpy(noeudfils->infos->id_pere,nv->id);}
			
		}
				nv->nbr_enf-=cpt;
			
	}
	//Traitement pour le/la conjointe
	if(etat==1){ 
	char nom_conj[CMAX], prenom_conj[CMAX], nom_prenom_conj[CMAX]; //nouveau à 12:46 le 17/01/2019
	char conj[CMAX]="de conjoint(e)";
	if(nv->sexe=='M')
		strcpy(conj,"de sa femme ");
	if(nv->sexe=='F')
		strcpy(conj,"de son marie ");
		
	printf("\tEntrer le nom %s",conj);
		scanf("%s",nom_conj);

	printf("\tle prenom %s",conj);
	scanf("%s",prenom_conj);
	strcpy(nom_prenom_conj,nom_conj);
	strcat(nom_prenom_conj,prenom_conj);
	noeud *noeudconj=_arbre_rechercher_noeud_parNOM(nom_prenom_conj,*racine_arbreNom);
	if(noeudconj==NULL)
	{
		strcpy(nv->id_conjoint,"\0");
		printf("\t\tConjoint(e) INEXISTANTE. Tu dois l'enregistrer !!\n");
	}
	else
	{ 
		strcpy(nv->id_conjoint,noeudconj->infos->id);
		strcpy(noeudconj->infos->id_conjoint,nv->id); 
	}
		
	}
	
	
	// traitement de l'existance dans la base ou non du pere et l'affectation des ID
	char prenom_pere[CMAX], nom_prenom_pere[CMAX]; //nouveau à 12:00 le 17/01/2019
	strcpy(nom_prenom_pere,nv->nom);
	printf("\tEntrer le prenom du pere : ");
	scanf("%s",prenom_pere);
	strcat(nom_prenom_pere,prenom_pere);
	noeud *noeudpere=_arbre_rechercher_noeud_parNOM(nom_prenom_pere,*racine_arbreNom);
	if(noeudpere==NULL)
	{
		strcpy(nv->id_pere,"\0");
		printf("\t\tPere INEXISTANT. Tu dois l'enregistrer !!\n");
	}
	else // le pere existe
	{
		strcpy(nv->id_pere,noeudpere->infos->id); // association du pere a son fils (la nouvelle personne) (ID)
	
	
	
		strcpy(noeudpere->infos->enfs_id[noeudpere->infos->nbr_enf],nv->id); // ajout de l'ID de l'enfant (la nouvelle personne) dans pere (enfs_id) 
		noeudpere->infos->nbr_enf++;   // incrementer le nombre des fils par un
	}
	// traitement de l'existance dans la base ou non du mere et l'affectation des ID
		char nom_mere[CMAX], prenom_mere[CMAX], nom_prenom_mere[CMAX]; //nouveau à 12:46 le 17/01/2019
	
	printf("\tEntrer le nom du mere :");
		scanf("%s",nom_mere);

	printf("\tLe prenom du mere : ");
	scanf("%s",prenom_mere);
	strcpy(nom_prenom_mere,nom_mere);
	strcat(nom_prenom_mere,prenom_mere);
	noeud *noeudmere=_arbre_rechercher_noeud_parNOM(nom_prenom_mere,*racine_arbreNom);
	if(noeudmere==NULL)
	{
		strcpy(nv->id_mere,"\0");
		printf("\t\tMere INEXISTANTE. Tu dois l'enregistrer !!\n");
	}
	else
	{ 
		strcpy(nv->id_mere,noeudmere->infos->id);
		strcpy(noeudmere->infos->enfs_id[noeudmere->infos->nbr_enf],nv->id); // ajout de l'ID de l'enfant (la nouvelle personne) dans mere (enfs_id) 
		noeudmere->infos->nbr_enf++;   // incrementer le nombre des fils par un
	}
	
	*racine_arbreNom=_arbre_inserer_noeud_parNOM(nv,*racine_arbreNom);
	*racine_arbreId=_arbre_inserer_noeud_parID(nv,*racine_arbreId);
	printf("\t\t ****** L'ajout de la personne est valide ****** \n");
	
	indice_Tab_IDs++;  //passant à l'ID suivant
	return 1 ;        //insertion valide
}

// Affichage inordre selon le nom de la liste des personnes ******************************************************************************************

void _arbre_inOrdre_parNOM(noeud *racineRecursive,noeud *racineArbre) { 
    if(racineRecursive != NULL) 
    {
        _arbre_inOrdre_parNOM(racineRecursive->gauche,racineArbre); //racineArbre = Arbre des Noms
        // recherche du nom du pere a partir de son ID
       char nom_prenomPere[CMAX]="NON ENREGESTRE",nom_prenomMere[CMAX]="NON ENREGESTRE",nom_prenomConj[CMAX]="NON ENREGESTRE";
        noeud *noeudpere=_arbre_rechercher_noeud_parID(racineRecursive->infos->id_pere,racineArbre);
        if(noeudpere!=NULL){
        	strcpy(nom_prenomPere,noeudpere->infos->nom);
        	strcat(nom_prenomPere," ");
       		strcat(nom_prenomPere,noeudpere->infos->prenom);
		}
       		
	    // recherche du nom du pere a partir de son ID
      	 noeud *noeudmere=_arbre_rechercher_noeud_parID(racineRecursive->infos->id_mere,racineArbre);
	   if(noeudmere!=NULL){
	   	strcpy(nom_prenomMere,noeudmere->infos->nom);
       	strcat(nom_prenomMere," ");
       	strcat(nom_prenomMere,noeudmere->infos->prenom);
	   }
       	
		noeud *noeudconj=_arbre_rechercher_noeud_parID(racineRecursive->infos->id_conjoint,racineArbre);
		if(noeudconj!=NULL)
		{
	   	strcpy(nom_prenomConj,noeudconj->infos->nom);
       	strcat(nom_prenomConj," ");
       	strcat(nom_prenomConj,noeudconj->infos->prenom);
	   }
       	
       	
		printf("\t%s\t%s\t%c\t%s\t\t%s\t\t%s\t\t%s\t\t%d\t\n",racineRecursive->infos->nom,racineRecursive->infos->prenom,racineRecursive->infos->sexe,nom_prenomPere,nom_prenomMere,racineRecursive->infos->situation,nom_prenomConj,racineRecursive->infos->nbr_enf);
        _arbre_inOrdre_parNOM(racineRecursive->droite,racineArbre); 
    } 
} 

void _personne_affichage_parNOM(noeud *racineNOM,noeud *racineID){
	
        printf("\n===================================================================================================================================================\n");
        printf("\tNom\tPrenom\tSexe\tPere\t\t\tMere\t\t\tSituation\tConjoint(e)\t\tNombre d'enfants enregestres\t\n");
        printf("===================================================================================================================================================\n");
		_arbre_inOrdre_parNOM(racineNOM,racineID);
}

void generer_tableauIDs(){
	FILE *f=fopen("data/table_IDs.txt","w");
	char chaine[IdMAX],resultat[IdMAX];
	int nbr=100+indice_Tab_IDs;
	for(int i=0;i<100;i++){
	itoa(nbr,chaine,10);
	strcpy(resultat,"AB");
	strcat(resultat,chaine);
	fputs(resultat,f);
	nbr++;
	}
	fclose(f);
}

void chargerIDs(){
	FILE *F=fopen("data/table_IDs.txt","r");
	for(int i=0;i<=100;i++){
	fgets(Tableau_IDs[i],6,F);
	}
	fclose(F);
}
void imprimer1(noeud *racine,personne *infos){
	FILE *F=fopen("data/imprime1.html","w");
	char nom_Pere[CMAX]="NON ENREGESTRE",prenom_Pere[CMAX]="NON ENREGESTRE";
        noeud *noeudpere=_arbre_rechercher_noeud_parID(infos->id_pere,racine);
        if(noeudpere!=NULL){
        	strcpy(nom_Pere,noeudpere->infos->nom);
        	strcpy(prenom_Pere,noeudpere->infos->prenom);
		}
       		
       	char nom_Mere[CMAX]="NON ENREGESTRE",prenom_Mere[CMAX]="NON ENREGESTRE";
        noeud *noeudMere=_arbre_rechercher_noeud_parID(infos->id_mere,racine);
        if(noeudMere!=NULL){
        	strcpy(nom_Mere,noeudMere->infos->nom);
        	strcpy(prenom_Mere,noeudMere->infos->prenom);
		}
	fputs("<!DOCTYPE html><html><head><style>table { width:80%; margin:auto;} table, th, td { border: 2px solid black; border-collapse:collapse;} th, td { padding: 5px; text-align: left;}</style></head><body>",F);
	fprintf(F,"<table>  <caption><h3>EXTRAIT DE L'ACTE DE NAISSANCE</h3></caption>  <tr><td colspan='3'><b>DATE ET LIEU DE NAISSANCE</b> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  %d-%d-%d  %s</td></tr>  <tr><td colspan='3'><b>NOM </b> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; %s</td></tr>  <tr><td colspan='3'><b>PRENOM</b>   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%s</td></tr>  <tr><td><b>SEXE</b> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%c </td><th>PERE</th><th>MERE</th></tr><tr><td><b>NOM</b></td><td>%s</td><td>%s</td></tr><tr><td><b>PRENOM</b></td><td>%s</td><td>%s</td></tr><tr><td colspan='3'><b></br></br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SIGNATURE</br></br></br></br></b></td></tr></table></body></html>",infos->naiss.j,infos->naiss.m,infos->naiss.a,infos->naiss.lieu,infos->nom,infos->prenom,infos->sexe,nom_Pere,nom_Mere,prenom_Pere,prenom_Mere);
	fclose(F);
}
void imprimer2(noeud *racine,personne *infos){
	FILE *F=fopen("data/imprime2.html","w");
	fputs("<!DOCTYPE html><html><head><style>table { width:80%; margin:auto;} table, th, td { border: 2px solid black; border-collapse:collapse;} th, td { padding: 5px; text-align: left;}</style></head><body>",F);
	fprintf(F,"<table><caption><h3>COPIE COMPLETE DE L'ACTE DE NAISSANCE</h3></caption><tr><td colspan='3'><b>DATE ET LIEU DE NAISSANCE</b>   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%d-%d-%d  %s</td></tr><tr><td colspan='3'><b>NOM</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   %s</td></tr><tr><td colspan='3'><b>PRENOM </b>  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%s</td></tr><tr><td colspan='3'><b>NOMBRE DE FILS</b> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%d</td></tr>",infos->naiss.j,infos->naiss.m,infos->naiss.a,infos->naiss.lieu,infos->nom,infos->prenom,infos->nbr_enf);
	if(infos->nbr_enf!=0)
	fputs("<tr><th>PRENOM</th><th>NOM</th><th>SEXE</th></tr>",F);
	for(int i=0;i<infos->nbr_enf;i++){
		
		char nom[CMAX]="NON ENREGESTRE",prenom[CMAX]="NON ENREGESTRE",sexe='-';
        noeud *noeudfils=_arbre_rechercher_noeud_parID(infos->enfs_id[i],racine);
        if(noeudfils!=NULL){
        	strcpy(nom,noeudfils->infos->nom);
        	strcpy(prenom,noeudfils->infos->prenom);
        	sexe=noeudfils->infos->sexe;
		}
		
		
		
	fprintf(F,"<tr><td>%s</td><td>%s</td><td>%c</td></tr>",nom,prenom,sexe);
	
	}
		fprintf(F,"<tr><td colspan='3'></br></br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SIGNATURE</br></br></br></br></td></tr></table></body></html>");
	fclose(F);
}
//-*-*-*-*-*-*-*-*-*-*-*-*-*-  FONCTION SUPPRIMER  -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*


/* Étant donné un arbre de recherche binaire non vide, retourner le
noeud avec la valeur de clé minimale trouvée dans cet arbre.
Notez que l’arbre entier n’a pas besoin d’être
cherché. */
noeud * _arbre_min_Value_Noeud(noeud* nd) 
{ 
	noeud* courant = nd;

	/* faire une boucle pour trouver la feuille la plus à gauche */
	while (courant->gauche != NULL) 
		courant = courant->gauche; 

	return courant; 
} 


// Fonction récursive pour supprimer un nœud avec les infos d'une personne 
// Retourne ,en suite, la racine de
// l'arbre modifié. 
noeud* _arbre_supprimer_noeud(noeud *racine, personne *infos) 
{ 
	// ÉTAPE 1: effectuez une suppression BST standard 

	if (racine == NULL) 
		return racine; 

	// Si id de la personne à supprimer est plus petite que la
	// id de la personne , elle se trouve dans le sous-arbre de gauche 
	if (strcmp(infos->id,racine->infos->id)<0) 
		racine->gauche = _arbre_supprimer_noeud(racine->gauche, infos); 

	// Si id de la personne à supprimer est supérieure à la
	// id de la personne de la racine, alors elle se trouve dans le sous-arbre de droite 
	else if(strcmp(infos->id,racine->infos->id)>0) 
		racine->droite = _arbre_supprimer_noeud(racine->droite, infos); 

	// id de la personne est identique à id de la personne, alors ceci est
	// le nœud à supprimer 
	else
	{ 
		// noeud avec un seul fils ou pas d'fils 
		if( (racine->gauche == NULL) || (racine->droite == NULL) ) 
		{ 
			noeud *temp = (racine->gauche) ? racine->gauche : racine->droite; 

			// cas de : pas de fils 
			if (temp == NULL) 
			{ 
				temp = racine; 
				racine = NULL; 
			} 
			else // cas de : un seul fils 
			*racine = *temp; // ecraser le contenu du racine par le contenu du 
							// fils non vide et liberer le fils (deja copié dans la racine)
			free(temp); 
		} 
		else
		{ 
			// noeud avec deux fils: Obtenez le inorder 
			// successeur (remplacer le noeud a supprimer
			//par le plus petit noeud dans le sous-arbre de droite)
			//(ou bien le plus grand dans le sous-arbre gauche)
			
			noeud* temp = _arbre_min_Value_Noeud(racine->droite); 

			// Copiez les infos du noeud successeur 
			// inordre sur ce noeud (precedent).
			racine->infos = temp->infos; 

			// Supprimer le successeur inorder
			racine->droite = _arbre_supprimer_noeud(racine->droite, temp->infos); 
		} 
	} 

	// Si l'arbre n'a qu'un seul noeud, alors retourner NULL. 
	if (racine == NULL) 
	return racine; 

	// ÉTAPE 2: MISE À JOUR DE LA HAUTEUR DU NŒUD ACTUEL 
	racine->hauteur = 1 + max(_arbre_hauteur(racine->gauche),_arbre_hauteur(racine->droite)); 

	// ÉTAPE 3: OBTENIR LE FACTEUR D’ÉQUILIBRE DE CE NŒUD (à
	// vérifie si ce noeud est devenu déséquilibré) 
	int balance = _arbre_equilibre(racine); 

	// Si ce nœud devient déséquilibré, il y a 4 cas 

	// cas de : gauche gauche 
	if (balance > 1 && _arbre_equilibre(racine->gauche) >= 0) 
		return _arbre_RotationD(racine); 

	// cas de  : Gauche droite 
	if (balance > 1 && _arbre_equilibre(racine->gauche) < 0) 
	{ 
		racine->gauche = _arbre_RotationG(racine->gauche); 
		return _arbre_RotationD(racine); 
	} 

	// cas de : droit droit
	if (balance < -1 && _arbre_equilibre(racine->droite) <= 0) 
		return _arbre_RotationG(racine); 

	// cas de : Droite Gauche 
	if (balance < -1 && _arbre_equilibre(racine->droite) > 0) 
	{ 
		racine->droite = _arbre_RotationD(racine->droite); 
		return _arbre_RotationG(racine); 
	} 

	return racine; 
}


// Fonction récursive pour supprimer un nœud avec les infos d'une personne 
// Retourne ,en suite, la racine de
// l'arbre modifié. 
noeud* _arbre_supprimer_noeud_NOM(noeud *racine, personne *infos) 
{ 
	// ÉTAPE 1: effectuez une suppression  standard 

	if (racine == NULL) 
		return racine; 

	// Si nom_prenom de la personne à supprimer est plus petite que la
	// nom_prenom de la personne , elle se trouve dans le sous-arbre de gauche 
	if (strcmp(infos->nom_prenom,racine->infos->nom_prenom)<0) 
		racine->gauche = _arbre_supprimer_noeud_NOM(racine->gauche, infos); 

	// Si nom_prenom de la personne à supprimer est supérieure à la
	// nom_prenom de la personne de la racine, alors elle se trouve dans le sous-arbre de droite 
	else if(strcmp(infos->nom_prenom,racine->infos->nom_prenom)>0) 
		racine->droite = _arbre_supprimer_noeud_NOM(racine->droite, infos); 

	// nom_prenom de la personne est identique à nom de la personne, alors ceci est
	// le nœud à supprimer 
	else
	{ 
		// noeud avec un seul fils ou pas d'fils 
		if( (racine->gauche == NULL) || (racine->droite == NULL) ) 
		{ 
			noeud *temp = (racine->gauche) ? racine->gauche : racine->droite; 

			// cas de : pas de fils 
			if (temp == NULL) 
			{ 
				temp = racine; 
				racine = NULL; 
			} 
			else // cas de : un seul fils 
			*racine = *temp; // ecraser le contenu du racine par le contenu du 
							// fils non vide et liberer le fils (deja copié dans la racine)
			free(temp); 
		} 
		else
		{ 
			// noeud avec deux fils: Obtenez le inorder 
			// successeur (remplacer le noeud a supprimer
			//par le plus petit noeud dans le sous-arbre de droite)
			//(ou bien le plus grand dans le sous-arbre gauche)
			
			noeud* temp = _arbre_min_Value_Noeud(racine->droite); 

			// Copiez les infos du noeud successeur 
			// inordre sur ce noeud (precedent).
			racine->infos = temp->infos; 

			// Supprimer le successeur inorder
			racine->droite = _arbre_supprimer_noeud_NOM(racine->droite, temp->infos); 
		} 
	} 

	// Si l'arbre n'a qu'un seul noeud, alors retourner NULL. 
	if (racine == NULL) 
	return racine; 

	// ÉTAPE 2: MISE À JOUR DE LA HAUTEUR DU NŒUD ACTUEL 
	racine->hauteur = 1 + max(_arbre_hauteur(racine->gauche),_arbre_hauteur(racine->droite)); 

	// ÉTAPE 3: OBTENIR LE FACTEUR D’ÉQUILIBRE DE CE NŒUD (à
	// vérifie si ce noeud est devenu déséquilibré) 
	int balance = _arbre_equilibre(racine); 

	// Si ce nœud devient déséquilibré, il y a 4 cas 

	// cas de : gauche gauche 
	if (balance > 1 && _arbre_equilibre(racine->gauche) >= 0) 
		return _arbre_RotationD(racine); 

	// cas de  : Gauche droite 
	if (balance > 1 && _arbre_equilibre(racine->gauche) < 0) 
	{ 
		racine->gauche = _arbre_RotationG(racine->gauche); 
		return _arbre_RotationD(racine); 
	} 

	// cas de : droit droit
	if (balance < -1 && _arbre_equilibre(racine->droite) <= 0) 
		return _arbre_RotationG(racine); 

	// cas de : Droite Gauche 
	if (balance < -1 && _arbre_equilibre(racine->droite) > 0) 
	{ 
		racine->droite = _arbre_RotationD(racine->droite); 
		return _arbre_RotationG(racine); 
	} 

	return racine; 
}



// *-*-*--*-*-*-*-*-*-*-*-*-( Modifier les informations )-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
int menu_modification(){
    int choix_modif;
    printf("\n===================================================\n");
    printf("            \n Que voulez vous modifier ?  \n");
    printf("\n===================================================\n");
    printf("1.Le Nom                          ; \n");
    printf("2.Le Prenom                       ;\n");
    printf("3.La Situation                    ; \n");
    printf("4.Le Lieu et la Date de Naissance ; \n");
    printf("5.Le Nombre d'Enfants             ;\n");
    printf("6.Quitter                         ; \n");
    printf("\n--> Entrer votre CHOIX : ");

    scanf("%d",&choix_modif);           
    return choix_modif;
}

noeud* _arbre_modifier_noeud(noeud **racine_arbreNom) { //nouveaule 26/01/2019
	int choix_modif;
	char nom_personne_modifier[CMAX], prenom_personne_modifier[CMAX], nom_prenom_personne_modifier[CMAX];
	 system("cls");
	printf("\n\n--> LA MODIFICATION DES INFORMATIONS <--\n\n");
	printf("Veiller entrer le nom de la personne que vous modifier ses informations : ");
	scanf("%s", nom_personne_modifier);
	strcpy(nom_prenom_personne_modifier,nom_personne_modifier);
	printf("Veiller entrer son prenom : ");
	scanf("%s", prenom_personne_modifier);
	strcat(nom_prenom_personne_modifier,prenom_personne_modifier);
	noeud *noeud_personne_modifier=_arbre_rechercher_noeud_parNOM(nom_prenom_personne_modifier,*racine_arbreNom);
	if(noeud_personne_modifier==NULL)
	{
		printf("La personne voulant modifier est INEXISTANTE. Tu dois l'enregistrer !!\n");
		system("PAUSE");
	}
	else
		
	    do{
      choix_modif = menu_modification();
      switch(choix_modif){
      	
         case 1 : char nv_nom_modifier[CMAX];
		 			printf("Entrer le nouveau Nom : ");
         		  scanf("%s", nv_nom_modifier);
		 		  strcpy(noeud_personne_modifier->infos->nom,nv_nom_modifier);
		 		  printf("Nom modifier ! merci.");
                  break;
                  
         case 2 : char nv_prenom_modifier[CMAX];
		 		  printf("Entrer le nouveau Prenom : ");
				  scanf("%s",nv_prenom_modifier);
				  strcpy(noeud_personne_modifier->infos->prenom,nv_prenom_modifier);
				  printf("Prenom modifier ! merci.");
                  break;
                  
         case 3 : noeud_personne_modifier->infos->situation;
                  break;
                          
         
         case 4 : 	int jr,mois,ans;
         			char nv_lieu[CMAX];
		 			printf("Le Jour : ");
         			scanf("%d", &jr);
		 			noeud_personne_modifier->infos->naiss.j=jr;
		 			printf("Le Mois : ");
		 			scanf("%d", &mois);
         			noeud_personne_modifier->infos->naiss.m=mois;
         			printf("L'Annee : ");
         			scanf("%d", &ans);
        			noeud_personne_modifier->infos->naiss.a=ans;
        			printf("Lieu de naissance : ");
        			scanf("%s", nv_lieu);
        			strcpy(noeud_personne_modifier->infos->naiss.lieu,nv_lieu);
         			break;
         			
        case 5 : 	int nv_nbr_enf;
					printf("Entrer le nouveau nombre d'enfants : ");
        			scanf("%d",&nv_nbr_enf);
					noeud_personne_modifier->infos->nbr_enf=nv_nbr_enf;
        			break;

        case 6 : break;
         default : printf("entrer un choix qui existe dans la liste");
                                   
         }
    } while(choix_modif!=6);
    return *racine_arbreNom;
}	
// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*	

// *-*-*--*-*-*-*-*-*-*-*-*-( Ajouter des nouveaux enfants )-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

int _personne_ajouter_enfant(noeud **racine_arbreId, noeud **racine_arbreNom){
	char nom_pereEnfant[CMAX], prenom_pereEnfant[CMAX], nom_prenom_pereEnfant[CMAX];
	personne *nv_enfant_ajouter=Malloc(personne);
	// Traitement du pere du nv enfant *********************
	printf("\tNom du pere : ");
	scanf("%s",nom_pereEnfant);
	strcpy(nom_prenom_pereEnfant,nom_pereEnfant);
	printf("\tPrenom : ");
	scanf("%s",prenom_pereEnfant);
	strcat(nom_prenom_pereEnfant,prenom_pereEnfant);
	noeud *noeudpere_enfant=_arbre_rechercher_noeud_parNOM(nom_prenom_pereEnfant,*racine_arbreNom);
	if(noeudpere_enfant==NULL)
	{
		strcpy(nv_enfant_ajouter->id_pere,"\0");
		printf("Ce nom INEXISTANT. Tu dois enregistrer !!\n ---> On continue pour le nouveau enfant :  (Taper Entrer)\n");
		system("PAUSE");
	}
	
	strcpy(nv_enfant_ajouter->id,Tableau_IDs[indice_Tab_IDs]);
	strcpy(nv_enfant_ajouter->nom,nom_pereEnfant);
	printf("\tLe prenom de l'enfant : ");
	scanf("%s",nv_enfant_ajouter->prenom);
	strcpy(nv_enfant_ajouter->nom_prenom,nom_pereEnfant);
	strcat(nv_enfant_ajouter->nom_prenom,nv_enfant_ajouter->prenom);
	printf("\tsexe :\n\t\tF si feminin, M si masculin ;\n\t -> Votre choix : ");
	getchar(); //vider le buffer
	scanf("%c",&nv_enfant_ajouter->sexe);
	strcpy(nv_enfant_ajouter->situation, "celib");
	nv_enfant_ajouter->nbr_enf=0;
	if(noeudpere_enfant!=NULL)
	{
		strcpy(nv_enfant_ajouter->id_pere,noeudpere_enfant->infos->id); // association du pere a son fils (la nouvelle personne) (ID)
		strcpy(noeudpere_enfant->infos->enfs_id[noeudpere_enfant->infos->nbr_enf],nv_enfant_ajouter->id); // ajout de l'ID de l'enfant (la nouvelle personne) dans pere (enfs_id) 
		noeudpere_enfant->infos->nbr_enf++;   // incrementer le nombre des fils par un
	}
	// Traitement du mere du nv enfant *********************
	char nom_mereEnfant[CMAX], prenom_mereEnfant[CMAX], nom_prenom_mereEnfant[CMAX];
	printf("\tEntrer le nom du mere :");
	scanf("%s",nom_mereEnfant);

	printf("\tle prenom du mere : ");
	scanf("%s",prenom_mereEnfant);
	strcpy(nom_prenom_mereEnfant,nom_mereEnfant);
	strcat(nom_prenom_mereEnfant,prenom_mereEnfant);
	noeud *noeudmere_enfant=_arbre_rechercher_noeud_parNOM(nom_prenom_mereEnfant,*racine_arbreNom);
	if(noeudmere_enfant==NULL)
	{
		strcpy(nv_enfant_ajouter->id_mere,"\0");
		printf("La Mere est INEXISTANTE. Tu dois l'enregistrer !!\n ---> On continue pour le nouveau enfant :  (Taper Entrer)\n");
		system("PAUSE");
	}
	else
	{ 
		strcpy(nv_enfant_ajouter->id_mere,noeudmere_enfant->infos->id);
		strcpy(noeudmere_enfant->infos->enfs_id[noeudmere_enfant->infos->nbr_enf],nv_enfant_ajouter->id); // ajout de l'ID de l'enfant (la nouvelle personne) dans mere (enfs_id) 
		noeudmere_enfant->infos->nbr_enf++;   // incrementer le nombre des fils par un
	}
	printf("\tDate et lieu de naissance :\n ");
	printf("\t\tLe Jour           : ");
    scanf("%d", &(nv_enfant_ajouter->naiss.j));
	printf("\t\tLe Mois           : ");
	scanf("%d", &(nv_enfant_ajouter->naiss.m));
    printf("\t\tL'Annee           : ");
  	scanf("%d", &(nv_enfant_ajouter->naiss.a));
    printf("\t\tLieu de naissance : ");
  	scanf("%s",nv_enfant_ajouter->naiss.lieu);
        			
    // le pere existe
	
		strcpy(nv_enfant_ajouter->id_pere,noeudpere_enfant->infos->id); // association du pere a son fils (la nouvelle personne) (ID)
		strcpy(noeudpere_enfant->infos->enfs_id[noeudpere_enfant->infos->nbr_enf],nv_enfant_ajouter->id); // ajout de l'ID de l'enfant (la nouvelle personne) dans pere (enfs_id) 
		noeudpere_enfant->infos->nbr_enf++;   // incrementer le nombre des fils par un
		
	// traitement de l'existance dans la base ou non du mere et l'affectation des ID

	strcpy(nv_enfant_ajouter->id_mere,noeudmere_enfant->infos->id);
	strcpy(noeudmere_enfant->infos->enfs_id[noeudmere_enfant->infos->nbr_enf],nv_enfant_ajouter->id); // ajout de l'ID de l'enfant (la nouvelle personne) dans mere (enfs_id) 
		noeudmere_enfant->infos->nbr_enf++;   // incrementer le nombre des fils par un
	*racine_arbreNom=_arbre_inserer_noeud_parNOM(nv_enfant_ajouter,*racine_arbreNom);
	*racine_arbreId=_arbre_inserer_noeud_parID(nv_enfant_ajouter,*racine_arbreId);
	
	indice_Tab_IDs++;  //passant à l'ID suivant
	return 1 ;        //insertion valide
	
}
	// *-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*  AGE  -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*	

int nombre_jour( naissance d ) {
    int  m = (d.m + 9) % 12;   
    int  a = d.a - (m >= 10); 
    return 1461*a/4 - a/100 + a/400 + (m*306 + 5)/10 + d.j;
}

 int Age( naissance d1 ) {
 	naissance d2;
 	d2.j=1;
	d2.m=30;
	d2.a=2019;
 	int diff=nombre_jour(d2) - nombre_jour(d1);
    return diff/365;
}

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*


///////////////////////////////////////////////
void fichier_ecrire_arbre_secondaire(noeud *racine,FILE *F) 
{ 
    if(racine != NULL) 
    { 
        fwrite(racine->infos,sizeof(personne),1,F);
        fichier_ecrire_arbre_secondaire(racine->gauche,F); 
        fichier_ecrire_arbre_secondaire(racine->droite,F); 
    } 
} 
void fichier_ecrire_arbre(noeud *racine){
	FILE *F=fopen("data/Arbre.bin","wb");
	fichier_ecrire_arbre_secondaire(racine,F);
	fclose(F);
}
////////////////////////////////////////////////////
void fichier_generer_arbre(){
	FILE *F=fopen("data/Arbre.bin","rb");
	 while(1){
		personne *infos=Malloc(personne);
			if(fread(infos,sizeof(personne),1,F)) {
    			racineID=_arbre_inserer_noeud_parID(infos,racineID);
    			racineNOM=_arbre_inserer_noeud_parNOM(infos,racineNOM);
			}else break;
	}
	fclose(F);
	
}

void menu_personne()
{
  int choix_gestion_personne;

  do{
	system("cls");
    printf("\n\n\n\t\t\t\t\t\t*********************** -- GESTION DES PERSONNES -- ***************************");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*         1 : Ajouter une nouvelle personne                                ;  *");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*         2 : Supprimer une personne                                       ;  *");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*         3 : Modifier les enregistrements d'une personne. (Mise a jour)   ;  *");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*         4 : Consulter la liste des personnes                             ;  *");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*         5 : Chercher une personne par son Nom et Prenom ou par ID        ;  *");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*         6 : Retourner le nom de Mere ou Pere d'une personne              ;  *");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*         7 : Retourner l'age d'une personne                               ;  *");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*         8 : Trier la liste des personnes (selon le nom et l'identifiant) ;  *");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*         9 : Imprimer donnees (ETAT CIVIL)                                ;  *");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*        10 : Enregistrer les modifications                                ;  *");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*                                                              ( 0 : Sortir ) *");
    printf("\n\t\t\t\t\t\t*                                                                             *");
    printf("\n\t\t\t\t\t\t*******************************************************************************");
    printf("\n\n\n--> Votre CHOIX : ");
    scanf("%d",&choix_gestion_personne);

	switch(choix_gestion_personne) {
		case 1 : 	system("cls");
					_personne_ajouter(&racineID, &racineNOM);
						system("PAUSE");
				break;
		case 2 : //Supprimer une personne.
			{		char nom_suppr[CMAX], prenom_suppr[CMAX], nom_prenom_suppr[CMAX];
					system("cls");
					printf("\n\n\t\t---> SUPRESSION D'UNE PERSOONE <---\n\n");
					printf("Entrer le nom de la personne a supprimer : ");
					scanf("%s",nom_suppr);
					strcpy(nom_prenom_suppr,nom_suppr);
					printf("Entrer son prenom : ");
					scanf("%s",prenom_suppr);
					strcat(nom_prenom_suppr,prenom_suppr);
					noeud *noeud_supprNOM = _arbre_rechercher_noeud_parNOM(nom_prenom_suppr,racineNOM);
					if(noeud_supprNOM==NULL)
					{
						printf("La personne voulant supprimer est INEXISTANTE !!");
						
					}
					else
					{
						noeud *noeud_supprID = _arbre_rechercher_noeud_parID(noeud_supprNOM->infos->id,racineID);
						racineNOM=_arbre_supprimer_noeud_NOM(racineNOM, noeud_supprNOM->infos);
						racineID=_arbre_supprimer_noeud(racineID, noeud_supprID->infos);
						printf("\n\n - La personne est bien supprimer !!\n\n");
						
					}
					system("PAUSE");
			}
				break;
		case 3 : //Modifier les enregistrements d’une personne. (Mise à jour).
				racineNOM=_arbre_modifier_noeud(&racineNOM);
				break;
		case 4 : // Consulter la liste des personnes..
				system("cls");
				_personne_affichage_parNOM(racineNOM,racineID);
				system("PAUSE");
				
				break;
		case 5 : //Chercher une personne par son Nom et Prenom ou par ID.
				system("cls");
				int choix_recherche;
				char nom_personne_recherche[CMAX], prenom_personne_recherche[CMAX], nom_prenom_personne_recherche[CMAX], id_recherche[IdMAX];
				printf("Voulez-Vous effectuer la recherche par :\n--> 1. Nom et prenom ;\n--> 2. ID.\n\n--->Votre CHOIX : ");
				scanf("%d",&choix_recherche);
				if(choix_recherche==1) {
				printf("Veiller entrer le nom de la personne que vous chercher ses informations : ");
				scanf("%s", nom_personne_recherche);
				strcpy(nom_prenom_personne_recherche,nom_personne_recherche);
				printf("Veiller entrer son prenom : ");
				scanf("%s", prenom_personne_recherche);
				strcat(nom_prenom_personne_recherche,prenom_personne_recherche);
				noeud *noeud_personne_rechercher=_arbre_rechercher_noeud_parNOM(nom_prenom_personne_recherche,racineNOM);
				if(noeud_personne_rechercher==NULL)
				{
					printf("La personne voulant rechercher est INEXISTANTE. Tu dois l'enregistrer !!\n");
				}
				
				else
				{
					system("cls");
					printf("\n--> INFORMATION SUR LA PERSONNE RECHERCHEE <--\n\n");
					printf("Nom       : %s\t\t;\nPrenom    : %s\t\t;\nSexe      : %c\t\t\t;\nSituation : %s\t\t;\nDate et Lieu de naissance : %d/%d/%d  -  %s .\n\n\n\n\n",noeud_personne_rechercher->infos->nom,noeud_personne_rechercher->infos->prenom,noeud_personne_rechercher->infos->sexe,noeud_personne_rechercher->infos->situation,noeud_personne_rechercher->infos->naiss.j,noeud_personne_rechercher->infos->naiss.m,noeud_personne_rechercher->infos->naiss.a,noeud_personne_rechercher->infos->naiss.lieu);
				}
				system("PAUSE");
				}
				if(choix_recherche==2){
					printf("\n\nVeiller entrer l'Identifiant (ID) de la personne que vous chercher ses informations : ");
				scanf("%s", id_recherche);
				noeud *noeud_id_recherche = _arbre_rechercher_noeud_parID(id_recherche,racineID);
				if(noeud_id_recherche==NULL){
					printf("Cette personne est INEXISTANTE !!");
				}
				else {
						printf("\n--> INFORMATION SUR LA PERSONNE RECHERCHEE <--\n\n");
						printf("Nom       : %s\t\t;\nPrenom    : %s\t\t;\nSexe      : %c\t\t\t;\nSituation : %s\t\t;\nDate et Lieu de naissance : %d/%d/%d  -  %s .\n\n\n\n\n",noeud_id_recherche->infos->nom,noeud_id_recherche->infos->prenom,noeud_id_recherche->infos->sexe,noeud_id_recherche->infos->situation,noeud_id_recherche->infos->naiss.j,noeud_id_recherche->infos->naiss.m,noeud_id_recherche->infos->naiss.a,noeud_id_recherche->infos->naiss.lieu);
						system("PAUSE");
				}
				
				}
				break;
		case 6 : //Retourner le nom de mère ou père d’une personne.
			{
				char nom_pers[CMAX], prenom_pers[CMAX], nom_prenom_pers[CMAX];
					system("cls");
					printf("\n\n\t\t---> RECHERCHE DES PARENTS D'UNE PERSONNE <---\n\n");
					printf("Entrer le nom de la personne : ");
					scanf("%s",nom_pers);
					strcpy(nom_prenom_pers,nom_pers);
					printf("Entrer son prenom : ");
					scanf("%s",prenom_pers);
					strcat(nom_prenom_pers,prenom_pers);
					noeud *noeud_chercherPARENT = _arbre_rechercher_noeud_parNOM(nom_prenom_pers,racineNOM);
					if(noeud_chercherPARENT==NULL)
					{
						printf("La personne voulant chercher ses -- PARENTS -- est INEXISTANTE !!");
					}
					else
					system("cls");
					noeud *noeud_chercherMERE = _arbre_rechercher_noeud_parID(noeud_chercherPARENT->infos->id_mere,racineID);
					noeud *noeud_chercherPERE = _arbre_rechercher_noeud_parID(noeud_chercherPARENT->infos->id_pere,racineID);
					printf("\n\n\t\t---> LES PARENTS DE CETTE PERSONNE <---\n\n");
					printf(" - Pere : %s %s\n",noeud_chercherPERE->infos->nom, noeud_chercherPERE->infos->prenom);
					printf(" - Mere : %s %s\n\n",noeud_chercherMERE->infos->nom, noeud_chercherMERE->infos->prenom);
					system("PAUSE");
			}
				break;
		case 7 : // Retourner l’age d’une personne.
			{
					system("cls");
					char nom_prenom[CMAX],nom[CMAX],prenom[CMAX];
					printf("Veiller entrer le nom de la personne  : ");
					scanf("%s", nom);
					strcpy(nom_prenom,nom);
					printf("Veiller entrer son prenom : ");
					scanf("%s", prenom);
					strcat(nom_prenom,prenom);
					noeud *A=NULL;
					A=_arbre_rechercher_noeud_parNOM(nom_prenom,racineNOM);
					if(A){
					int age=Age(A->infos->naiss);
					printf("Age : %d\n ",age);
					}else
					printf("la personne n existe pas \n ");
					system("PAUSE");
			}
				break;
		case 8 : //Trier la liste des personnes (selon le nom et l’identifiant)
				break;
		case 9 : //Imprimer données (ETAT CIVIL)
			{
					system("cls");
					char nom_prenom[CMAX],nom[CMAX],prenom[CMAX];
					printf("Veiller entrer le nom de la personne  : ");
					scanf("%s", nom);
					strcpy(nom_prenom,nom);
					printf("Veiller entrer son prenom : ");
					scanf("%s", prenom);
					strcat(nom_prenom,prenom);
					noeud *A=NULL;
					A=_arbre_rechercher_noeud_parNOM(nom_prenom,racineNOM);
					if(A){
					imprimer1(racineID,A->infos);
						printf("\n Imprime est genere, chercher le fichier imprime1.html\n ");
					}else
					printf("la personne n existe pas \n ");
					system("PAUSE");
				}
					
					break;
		case 10 : // pour enregistrer les modifications
						fichier_ecrire_arbre(racineID);
 						ecrire_indice();
 						printf("\nles modifications sont effectuees avec succes \n ");
						system("PAUSE");
				break;
		case 0 : break; //sortir de ce menu et revenir au menu_1.
	}
} while(choix_gestion_personne!=0);
}

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

//*-*-*-*-*-*-*-*-*-*-*-*-*-* (MENU -enfant-) *-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void menu_enfants()
{
	int choix_gestion_personne;
  	do
  	{
  		system("cls");
      	printf("\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t******************** -- GESTION DES ENFANTS -- *********************");
        printf("\n\t\t\t\t\t\t*                                                                   *");
        printf("\n\t\t\t\t\t\t*        -->  voulez-vous :                                         *");
        printf("\n\t\t\t\t\t\t*                                                                   *");
        printf("\n\t\t\t\t\t\t*                                                                   *");
        printf("\n\t\t\t\t\t\t*   1 : Afficher les informations sur les enfants d'une personne ;  *");
        printf("\n\t\t\t\t\t\t*                                                                   *");
        printf("\n\t\t\t\t\t\t*   2 : Ajouter les enfants d'une personne                       ;  *");
        printf("\n\t\t\t\t\t\t*                                                                   *");
        printf("\n\t\t\t\t\t\t*   3 : Imprimer acte                                            ;  *");
        printf("\n\t\t\t\t\t\t*                                                                   *");
		printf("\n\t\t\t\t\t\t*   4 : Enregistrer les modifications                            ;  *");
        printf("\n\t\t\t\t\t\t*                                                                   *");
		printf("\n\t\t\t\t\t\t*   0 : sortir                                                   .  *");
        printf("\n\t\t\t\t\t\t*                                                                   *");
        printf("\n\t\t\t\t\t\t********************************************************************");
        printf("\nVotre CHOIX :");
        scanf("%d",&choix_gestion_personne);
        switch(choix_gestion_personne)
        {
        	case 0:
        	{
        		// enregistrer les informations et sortir;
        		break;
        	}
        	case 1:
        	{
                // Afficher les informations sur les enfants d'une personne;
                if(1){
				char nom_pers[CMAX], prenom_pers[CMAX], nom_prenom_pers[CMAX];
					system("cls");
					printf("\n\n\t\t---> RECHERCHE DES INFORMATIONS SUR LES ENFANT D'UNE PERSONNE <---\n\n");
					printf("Entrer le nom de la personne : ");
					scanf("%s",nom_pers);
					strcpy(nom_prenom_pers,nom_pers);
					printf("Entrer son prenom : ");
					scanf("%s",prenom_pers);
					strcat(nom_prenom_pers,prenom_pers);
					noeud *noeud_chercherPERSONNE = _arbre_rechercher_noeud_parNOM(nom_prenom_pers,racineNOM);
					if(noeud_chercherPERSONNE==NULL)
					{
						printf("La personne voulant chercher ses -- ENFANTS -- est INEXISTANTE !!");
					}
					else
					system("cls");
					if(noeud_chercherPERSONNE->infos->nbr_enf==0)
						printf("Cette personne n'a pas d'enfants");
					else {
					printf("\n\n\t\t---> LES FILS DE CETTE PERSONNE <---\n\n");
					
					for(int i=0; i<noeud_chercherPERSONNE->infos->nbr_enf; i++)
					{
					noeud *noeud_chercherENFANT = _arbre_rechercher_noeud_parID(noeud_chercherPERSONNE->infos->enfs_id[i],racineID);
					printf("\n\n Informations du FILS %d :\n",i+1);
					printf("--------------------------------------------------------------------\n\n",i+1);
					printf("Nom       : %s\t\t;\nPrenom    : %s\t\t;\nSexe      : %c\t\t\t;\nSituation : %s\t\t;\nDate et Lieu de naissance : %d/%d/%d  -  %s .\n\n\n\n\n",noeud_chercherENFANT->infos->nom,noeud_chercherENFANT->infos->prenom,noeud_chercherENFANT->infos->sexe,noeud_chercherENFANT->infos->situation,noeud_chercherENFANT->infos->naiss.j,noeud_chercherENFANT->infos->naiss.m,noeud_chercherENFANT->infos->naiss.a,noeud_chercherENFANT->infos->naiss.lieu);
					}
					system("PAUSE");
					}
					}
        		break;
        	}
        	case 2:
        	{
        		// Ajouter les enfants d’une personne;
        		system("cls");
        		printf("\n\n\t\t**********  ENREGISTREMENT DES ENFANTS *********\n\n");
        		_personne_ajouter_enfant(&racineID, &racineNOM);
        		printf("\n**********  Les informations de cette ENFANT sont ENREGISTREES *********\n\n");
        		system("PAUSE");
        		break;
        	}
        	case 3:	//Imprimer données (ETAT CIVIL)
        		{
					system("cls");
					char nom_prenom[CMAX],nom[CMAX],prenom[CMAX];
					printf("Veiller entrer le nom de la personne  : ");
					scanf("%s", nom);
					strcpy(nom_prenom,nom);
					printf("Veiller entrer son prenom : ");
					scanf("%s", prenom);
					strcat(nom_prenom,prenom);
					noeud *A=NULL;
					A=_arbre_rechercher_noeud_parNOM(nom_prenom,racineNOM);
					if(A){
					imprimer2(racineID,A->infos);
					printf("\n Imprime est genere, chercher le fichier imprime2.html\n ");
					}else
					printf("la personne n existe pas \n ");
					system("PAUSE");
				
        		break;
        	}	
        	case 4:	// pour enregistrer les modifications
        	{
						fichier_ecrire_arbre(racineID);
 						ecrire_indice();
        			printf("\nles modifications sont effectuees avec succes \n ");
					system("PAUSE");
				break;
        	}	
   }
  	} while (choix_gestion_personne!=0);
}
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

//*-*-*-*-*-*-*-*-*-*-*-*-*-* (MENU -1-) *-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void login(){
	char userid[]="admin",password[]="admin",p[15],u[15],c;
   int a,b;
   while(1)
   {
   	
   		system("cls");
        printf("\n\n\n\n\n\n\n\t\t\n\n\t\t\t\t\t\t*******************************************************************");
        printf("\n\t\t\t\t\t\t*                                                                 *");
        printf("\n\t\t\t\t\t\t*                                                                 *");
        printf("\n\t\t\t\t\t\t*     BIENVENU SUR APPLICATION DE GESTION NUMERISEE DES ACTES     *");
        printf("\n\t\t\t\t\t\t*                          D'ETAT CIVIL                           *");
        printf("\n\t\t\t\t\t\t*                                                                 *");
        printf("\n\t\t\t\t\t\t*                                                                 *");
        printf("\n\t\t\t\t\t\t*******************************************************************\n\n\n\n");
   	  printf("\n\t\t\t\t\t\t                     Enter ID et mot de passe \n");
      printf("\n\t\t\t\t\t\t                     Identifiant: ");
      scanf("%s",u);
      printf("\t\t\t\t\t\t                     Mot de passe: ");
	  for(int i=0;i<5;i++){
			c = getch();
			printf("*");
			p[i]=c;
		  	}
      a=strcmp(u,userid);
      b=strncmp(p,password,5);
      a=strcmp(u,userid);
      b=strcmp(p,password);
      if(a==0&&b==0)
      {
         printf("\n\n\t\t\t\t\t\t                    vous etes connecte avec succes.");
         getch();
         break;
      }
      else
      {
         printf("\n\n\t\t\t\t\t\t                    Mot de passe ou/et Identifiant incorrect.");
      }
      getch();
    
   }
	
	
	
	
}


void menu_1()
{
    int choix_1;
    do
    {
    	
    	system("cls");
        printf("\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t*********************** -- MENU PRINCIPAL -- ************************");
        printf("\n\t\t\t\t\t\t*                                                                   *");
		printf("\n\t\t\t\t\t\t*                                                                   *");       
        printf("\n\t\t\t\t\t\t*   -----> Quelle gestion voulez vous effectuer :                   *");
        printf("\n\t\t\t\t\t\t*                                                                   *");
        printf("\n\t\t\t\t\t\t*                                                                   *");
        printf("\n\t\t\t\t\t\t*                                                                   *");
        printf("\n\t\t\t\t\t\t*             1 : Gestion des Personnes                 ;           *");
        printf("\n\t\t\t\t\t\t*                                                                   *");
        printf("\n\t\t\t\t\t\t*             2 : Gestion des Enfants d'une Personne    ;           *");
        printf("\n\t\t\t\t\t\t*                                                                   *");
        printf("\n\t\t\t\t\t\t*             0 : sortir                                .           *");
        printf("\n\t\t\t\t\t\t*                                                                   *");
        printf("\n\t\t\t\t\t\t*********************************************************************\n");
		printf("\n\n --> Donner votre CHOIX : ");
        scanf("%d",&choix_1);
        switch(choix_1)
        {
            case 0 :
            {
            	
            	        system("cls");
        printf("\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t*******************************************************************");
        printf("\n\t\t\t\t\t\t*                                                                 *");
        printf("\n\t\t\t\t\t\t*                                                                 *");
        printf("\n\t\t\t\t\t\t*                            AU REVOIR                            *");
        printf("\n\t\t\t\t\t\t*                                                                 *");
        printf("\n\t\t\t\t\t\t*                                                                 *");
        printf("\n\t\t\t\t\t\t*******************************************************************\n\n\n\n");
 		
 		system("PAUSE");            	
                break;
            }
            case 1 :
            {
                menu_personne(); //menu_personne();
                break;
            }
            case 2 :
            {
                menu_enfants(); //menu_enfants();
                break;
            }
        }
    } while (choix_1!=0);
}


int main()

{
		login();
 		recuperer_indice();
 		generer_tableauIDs();
		chargerIDs();
		fichier_generer_arbre();
		menu_1();
 		
	return 0;
}

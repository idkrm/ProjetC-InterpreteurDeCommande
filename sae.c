#include <stdio.h>
#include <string.h>
#pragma warning (disable : 4996)

//pour les inscriptions
enum{LONG_MAX = 31};
typedef struct {
	char nom[LONG_MAX];
	char roles[LONG_MAX];
    int id;
}inscription;

enum { MAX_INSCR = 51 };
inscription inscrit[MAX_INSCR]; //tableau qui permettra de recenser tous les inscrits
int nb_inscrit = 0; //permettra d'afficher l'identifiant entre parenth�ses


//pour les missions
typedef struct {
    int id_miss; 
    char nom[LONG_MAX];
    float remu;
}mission;

enum { MISSION_MAX = 501 };
mission m[MISSION_MAX]; //tableau pour regrouper toutes les missions
int nb_miss = 0; //permettra de pr�ciser la mission publi�e est la combien i�me (entre parenth�se)


//consultation des missions non trait�es
//imbrication des structures mission et inscription dans la structure consultation
typedef struct {
    mission id_miss;
    mission nom[LONG_MAX];
    mission remu;
    inscription nom;
    int s_trait;
}consultation;


void inscr(char* input) {
    char nom[LONG_MAX];
    char roles[LONG_MAX];

    //associe le deuxi�me mot de la cha�ne de caract�re � la variable "roles" et le troisi�me mot � la variable "nom"
    sscanf(input, "%*s %s %s", roles, nom);

    //si le r�le ne correspond pas � l'un des trois codes, msg d'erreur
    if (strcmp(roles, "OP") != 0 && strcmp(roles, "AG") != 0 && strcmp(roles, "IN") != 0) {
        printf("Role incorrect\n");
        return;
    }


    //v�rifie si le nom existe d�j� ou pas
    int nomconnu = 0;
    for (int i = 0; i < nb_inscrit; i++) {
        if (strcmp(nom, inscrit[i].nom) == 0) {
            nomconnu = 1;
            break;
        }
    }
    //affiche le message d'erreur si le nom est connu
    if (nomconnu) {
        printf("Nom incorrect\n");
    }

    else {
        //copie du nom, du role et de l'identifiant dans la tableau "inscrit"
        strcpy(inscrit[nb_inscrit].nom, nom);
        strcpy(inscrit[nb_inscrit].roles, roles);
        inscrit[nb_inscrit].id = nb_inscrit + 1;

        printf("Inscription realisee (%d)\n", nb_inscrit + 1);
        nb_inscrit++;
    }
}






void miss(char* input) {
    int id = 0;
    char nom[LONG_MAX] = "";
    float remu = 0;

    //associe le deuxi�me mot � la variable "id", le troisi�me � "nom" et le quatri�me � "remu"
    sscanf(input, "%*s %d %s %f", &id, nom, &remu);

    //v�rifie si l'identifiant existe
    int id_inscription_valide = 0;
    for (int i = 0; i < nb_inscrit; i++) {
        if (nb_miss + 1 == inscrit[i].id) {
            id_inscription_valide = 1;
            break;
        }
    }
    //si n'existe pas, msg d'erreur
    if (id_inscription_valide == 0) {
        printf("Identifiant incorrect\n");
        return;
    }
    
    //si la r�mun�ration est inf�rieur ou �gale � 0, msg d'erreur
    if (remu <= 0) {
        printf("Remuneration incorrecte\n");
        return;
    }


    else{
        //garde les donn�es dans le tableau "m"
        m[nb_miss].id_miss = id;
        strcpy(m[nb_miss].nom, nom);
        m[nb_miss].remu = remu;

        printf("Mission publiee (%d)\n", nb_miss + 1);
        ++nb_miss;
    }
}


void consult(char* input) {

}



int main() {
    enum{INPUT_MAX = 100};
    char input[INPUT_MAX];
    char action[INPUT_MAX];

    while(strcmp(action,"exit") !=0){

        //entrez une cha�ne de caract�re avec fgets et la placer dans la variable "input"
        fgets(input, sizeof(input), stdin);


        //v�rifiez si "input" est �gal � 0 ou pas (donc si la variable est nulle ou pas)
        size_t input_length = strlen(input);
        // si non nulle, effacez ce qu'elle contient
        if (input_length > 0 && input[input_length - 1] == '\n') {
            input[input_length - 1] = '\0';
        }

        //prendre le premier mot de "input" et la mettre dans la variable "action"
        sscanf(input, "%s", action);


        //v�rifiez � quoi correspond "action", ex�cuter la fonction correspondante 
        if (strcmp(action, "inscription") == 0) {
            inscr(input);
            continue;
        }

        else if (strcmp(action, "mission") == 0)
            miss(input);

        else if (strcmp(action, "consultation"))
            consult(input);
    }

    //si "action" = "exit", quitter le programme
    if (strcmp(action, "exit") == 0)
        exit();
}

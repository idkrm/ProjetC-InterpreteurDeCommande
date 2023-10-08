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
int nb_inscrit = 0; //permettra d'afficher l'identifiant entre parenthèses


//pour les missions
typedef struct {
    int id_miss; 
    char nom[LONG_MAX];
    float remu;
}mission;

enum { MISSION_MAX = 501 };
mission m[MISSION_MAX]; //tableau pour regrouper toutes les missions
int nb_miss = 0; //permettra de préciser la mission publiée est la combien ième (entre parenthèse)


//consultation des missions non traitées
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

    //associe le deuxième mot de la chaîne de caractère à la variable "roles" et le troisième mot à la variable "nom"
    sscanf(input, "%*s %s %s", roles, nom);

    //si le rôle ne correspond pas à l'un des trois codes, msg d'erreur
    if (strcmp(roles, "OP") != 0 && strcmp(roles, "AG") != 0 && strcmp(roles, "IN") != 0) {
        printf("Role incorrect\n");
        return;
    }


    //vérifie si le nom existe déjà ou pas
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

    //associe le deuxième mot à la variable "id", le troisième à "nom" et le quatrième à "remu"
    sscanf(input, "%*s %d %s %f", &id, nom, &remu);

    //vérifie si l'identifiant existe
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
    
    //si la rémunération est inférieur ou égale à 0, msg d'erreur
    if (remu <= 0) {
        printf("Remuneration incorrecte\n");
        return;
    }


    else{
        //garde les données dans le tableau "m"
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

        //entrez une chaîne de caractère avec fgets et la placer dans la variable "input"
        fgets(input, sizeof(input), stdin);


        //vérifiez si "input" est égal à 0 ou pas (donc si la variable est nulle ou pas)
        size_t input_length = strlen(input);
        // si non nulle, effacez ce qu'elle contient
        if (input_length > 0 && input[input_length - 1] == '\n') {
            input[input_length - 1] = '\0';
        }

        //prendre le premier mot de "input" et la mettre dans la variable "action"
        sscanf(input, "%s", action);


        //vérifiez à quoi correspond "action", exécuter la fonction correspondante 
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

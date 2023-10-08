#include <stdio.h>
#include <string.h>
#pragma warning (disable : 4996)

//pour les inscriptions
enum{LONG_MAX = 31};
typedef struct inscription{
	char nom[LONG_MAX];
	char roles[LONG_MAX];
    int id;
}inscription;

enum { MAX_INSCR = 51 };
inscription inscrit[MAX_INSCR]; //tableau qui permettra de recenser tous les inscrits
int nb_inscrit = 0; //permettra d'afficher l'identifiant entre parenthèses


//pour les missions
typedef struct mission{
    int id_miss; 
    char nom[LONG_MAX];
    float remu;
}mission;

enum { MISSION_MAX = 501 };
mission m[MISSION_MAX]; //tableau pour regrouper toutes les missions
int nb_miss = 0; //permettra de préciser la mission publiée est la combien ième (entre parenthèse)


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
    int id_existe = 0;
    for (int i = 0; i < nb_inscrit; i++) {
        if (nb_miss +1 == inscrit[i].id) {
            id_existe = 1;
            break;
        }
    }
    //si n'existe pas, msg d'erreur
    if (id_existe == 0) {
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
    int nb_s_trait = 0;

    if (nb_miss > 0) {
        for (int i = 0; i < nb_miss; ++i) 
            printf("%d %s %s %.2f (%d)\n", m[i].id_miss, m[i].nom, inscrit[i].nom, m[i].remu, nb_s_trait);
        }

    else
        printf("Aucune mission disponible\n");
    }



void detail(char* input) {
    int detail_id = 1, nb_s_trait = 0;
    int id_inscription_valide = 0;

    sscanf(input, "%*s %d", &detail_id);
    int id_existe = 0;
    for (int i = 0; i < nb_inscrit; i++) {
        if (detail_id == inscrit[i].id) {
            id_existe = 1;
            break;
        }
    }

    if (id_existe == 0) {
        printf("Identifiant incorrect\n");
        return;
    }

    else {
        for (int j = 0; j <= detail_id; ++j) {
            if (j == detail_id) {
                printf("%d %s %s %.2f (%d)\n", m[j - 1].id_miss, m[j - 1].nom, inscrit[j - 1].nom, m[j - 1].remu, nb_s_trait);
                printf("detail mission ici\n");
            }
            else
                continue;
        }
    }
}



void acceptation(char* input) {
    int id_entreprise, id_mission;
    sscanf(input, "%*s %d %d", &id_entreprise, &id_mission);

    id_entreprise = inscrit[id_entreprise - 1].id;
    id_mission = m[id_mission - 1].id_miss;

    if (id_entreprise == "OP")
        printf("Entreprise incorrecte");

    else
        printf("Mission enregistree\n");
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
        if (strcmp(action, "inscription") == 0)
            inscr(input);

        else if (strcmp(action, "mission") == 0)
            miss(input);

        else if (strcmp(action, "consultation") == 0)
            consult(input);

        else if (strcmp(action, "detail") == 0)
            detail(input);

        else if (strcmp(action, "acceptation") == 0)
            acceptation(input);
    }

    //si "action" == "exit", quitter le programme
    if (strcmp(action, "exit") == 0)
        exit();
}

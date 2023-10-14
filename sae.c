#include <stdio.h>
#include <string.h>
#pragma warning(disable : 4996)

enum { LONG_MAX = 30 };
enum { MAX_INSCR = 50 };
enum { MISSION_MAX = 500 };
enum { SS_TRAIT_MAX = 5 };

//definition d'une structure pour les inscriptions pour une meilleure organisation des données
typedef struct {
    char nom[LONG_MAX];
    char roles[LONG_MAX];
    int id;
} inscription;

//structure pour les missions
typedef struct {
    int id_miss;
    char nom_miss[LONG_MAX];
    float remu;
    int auteur;
    int nb_ss_trait;
} mission;

//structure pour les missions attribuées (qui reprend les mêmes variables que la structure mission)
typedef struct {
    int id_miss_atrb;
    char nom_miss_atrb[LONG_MAX];
    float remu_atrb;
    int auteur_atrb;
    int nb_ss_trait_atrb;
} mission_atrb;


int exit(char* input) {
    return 0;
}


void inscr(char* input, inscription* inscrit, int* nb_inscrit) {
    if (*nb_inscrit >= MAX_INSCR) return;

    char nom[LONG_MAX];
    char roles[LONG_MAX];

    //on utilise sscanf pour lire la chaîne de caractère "input" obtenu avec la fonction fgets
    (void)sscanf(input, "%*s %s %s\n", roles, nom); //(void) devant la fonction afin d'éviter l'avertissement "return value ignored : sscanf"

    if (strcmp(roles, "OP") != 0 && strcmp(roles, "AG") != 0 && strcmp(roles, "IN") != 0) {
        printf("Role incorrect\n");
        return;
    }

    int nomconnu = 0;
    for (int i = 0; i < *nb_inscrit; i++) {
        if (strcmp(nom, inscrit[i].nom) == 0) {
            nomconnu = 1;
            break;
        }
    }

    if (nomconnu) {
        printf("Nom incorrect\n");
    }
    else {
        strcpy(inscrit[*nb_inscrit].nom, nom);
        strcpy(inscrit[*nb_inscrit].roles, roles);
        inscrit[*nb_inscrit].id = *nb_inscrit + 1;

        printf("Inscription realisee (%d)\n", *nb_inscrit + 1);
        (*nb_inscrit)++;
    }
}



void miss(char* input, mission* m, int* nb_miss, inscription* inscrit, int* nb_inscrit) {
    if (*nb_miss >= MISSION_MAX) return;

    int id = 0;
    char nom[LONG_MAX] = "";
    float remu = 0;

    (void)sscanf(input, "%*s %d %s %f\n", &id, nom, &remu);

    int id_existe = 0;
    for (int i = 0; i < *nb_inscrit; i++) {
        if (id == inscrit[i].id) {
            id_existe = 1;
            break;
        }
    }

    if (id_existe == 0) {
        printf("Identifiant incorrect\n");
        return;
    }

    if (remu < 0) {
        printf("Remuneration incorrecte\n");
        return;
    }

    else {
        printf("Mission publiee (%d)\n", *nb_miss + 1);

        m[*nb_miss].id_miss = *nb_miss + 1;
        strcpy(m[*nb_miss].nom_miss, nom);
        m[*nb_miss].remu = remu;
        m[*nb_miss].auteur = id;
        m[*nb_miss].nb_ss_trait = 0;

        (*nb_miss)++;
    }
}


void consult(mission* m, int* nb_miss, inscription* inscrit, int* nb_inscrit, mission_atrb* m_atrb, int* nb_miss_atrb) {

    if (*nb_miss > 0) {
        for (int i = 0; i < *nb_miss; ++i) {
            for (int j = 0; j < *nb_inscrit; ++j) {
                if (m[i].auteur == inscrit[j].id) {
                    printf("%d %s %s %.2f (%d)\n", m[i].id_miss, m[i].nom_miss, inscrit[j].nom, m[i].remu, m[i].nb_ss_trait);
                }
            }
        }
    }
    else {
        printf("Aucune mission disponible\n");
    }
}


void detail(char* input, mission* m, int* nb_miss, inscription* inscrit, int* nb_inscrit, mission_atrb* m_atrb, int* nb_miss_atrb) {
    int detail_id = 1;
    int id_inscription_valide = 0;

    (void)sscanf(input, "%*s %d", &detail_id);
    int id_existe = 0;
    for (int i = 0; i < *nb_inscrit; i++) {
        if (detail_id == m[i].id_miss) {
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
                printf("%d %s %s %.2f (%d)\n", m[j - 1].id_miss, m[j - 1].nom_miss, inscrit[j - 1].nom, m[j - 1].remu, m[j-1].nb_ss_trait);
                printf("detail mission ici\n");
            }
            else
                continue;
        }
    }
}


void acceptation(char* input, inscription* inscrit, int* nb_inscrit, mission* m, int* nb_miss, mission_atrb* m_atrb, int* nb_miss_atrb) {
    int id_entreprise, id_mission;
    (void)sscanf(input, "%*s %d %d", &id_entreprise, &id_mission);

    id_entreprise = inscrit[id_entreprise - 1].id;

    if (id_entreprise == 0) {
        printf("Entreprise incorrecte\n");
    }
    else {
        int mission_non_atrb = - 1; //définition et initialisation de la variable qui permettra de vérifier si une mission 
                                    //est présente dans le tableau mission (donc si la mission existe et si elle n'est pas attribuée)
                                    //initialisé à -1 car sinon la mission 1 ne pourra pas être acceptée
        for (int i = 0; i < *nb_miss; i++) {
            if (m[i].id_miss == id_mission) {
                mission_non_atrb = i;
                break;
            }
        }

        if (mission_non_atrb != - 1) {
            printf("Acceptation enregistree\n");

            strcpy(m_atrb[*nb_miss_atrb].nom_miss_atrb, m[mission_non_atrb].nom_miss);
            m_atrb[*nb_miss_atrb].remu_atrb = m[mission_non_atrb].remu;
            m_atrb[*nb_miss_atrb].auteur_atrb = m[mission_non_atrb].auteur;
            m_atrb[*nb_miss_atrb].nb_ss_trait_atrb = m[mission_non_atrb].nb_ss_trait;

            (*nb_miss_atrb)++;

            m[mission_non_atrb] = m[*nb_miss - 1];
        }
        else
            printf("Mission incorrecte\n");
    }
}


void soustraitance(char* input, inscription* inscrit, int* nb_inscrit, mission* m, int* nb_miss, mission_atrb* m_atrb, int* nb_miss_atrb) {
    int id_entreprise = 0, id_mission;
    float remu;

    (void)sscanf(input, "%*s %d %d %f\n", &id_entreprise, &id_mission, &remu);

    id_entreprise = inscrit[id_entreprise - 1].id;
    if (strcmp(inscrit[id_entreprise - 1].roles, "AG") != 0) {
        printf("Entreprise incorrecte\n");
    }

    int mission_non_atrb = -1;
    for (int i = 0; i < *nb_miss; i++) {
        if (m[i].id_miss == id_mission) {
            mission_non_atrb = i;
            break;
        }
    }

    if ((mission_non_atrb == -1) || (m[mission_non_atrb].nb_ss_trait >= SS_TRAIT_MAX)) 
        printf("Mission incorrecte\n");

    else if (remu < 0)
        printf("Remuneration incorrecte\n");

    else {
        //création de la nouvelle mission
        mission new_mission;

        //copie des informations de la nouvelle mission
        new_mission.id_miss = *nb_miss + 1;
        strcpy(new_mission.nom_miss, m[mission_non_atrb].nom_miss);
        new_mission.remu = remu;
        new_mission.auteur = id_entreprise;
        new_mission.nb_ss_trait = m[mission_non_atrb].nb_ss_trait + 1; 

        //ajouter la nouvelle mission au tableau des missions m
        m[*nb_miss] = new_mission;

        printf("Sous-traitance enregistree (%d)\n", *nb_miss + 1);
        ++(*nb_miss);

        //copie de la mission d'origine dans le tableau des missions attribuées (m_atrb)
        strcpy(m_atrb[*nb_miss_atrb].nom_miss_atrb, m[mission_non_atrb].nom_miss);
        m_atrb[*nb_miss_atrb].remu_atrb = m[mission_non_atrb].remu;
        m_atrb[*nb_miss_atrb].auteur_atrb = m[mission_non_atrb].auteur;
        m_atrb[*nb_miss_atrb].nb_ss_trait_atrb = m[mission_non_atrb].nb_ss_trait;

        (*nb_miss_atrb)++; //incrémentation du nombre de missions dans le tableau des missions attribuées (m_atrb)

        m[mission_non_atrb] = m[*nb_miss - 1];
    }
}


enum code { ZERO = 0, UN = 1, DEUX = 2, TROIS = 3 };
void rapport(char* input, inscription* inscrit, int* nb_inscrit, mission* m, int* nb_miss, mission_atrb* m_atrb, int* nb_miss_atrb) {
    int id_mission_atrb, code, id_miss_existe = -1;

    (void)sscanf(input, "%*s %d, %d", &id_mission_atrb, &code);

    for (int i = 0; i < *nb_miss_atrb; ++i)
        if (id_miss_existe == m_atrb[i].id_miss_atrb) {
            id_miss_existe = i;
            break;
        }

    if (id_miss_existe == -1)
        printf("Mission incorrecte");

    else if (code > ZERO || code < TROIS)
        printf("Code de retour incorrect");

    else {
        printf("Rapport enregistree");

    }
}



int main() {
    //attribution de la valeur 100 à INPUT_MAX
    //définition de la variable "input" (qui va servir à récupérer ce qu'entre l'utilisateur) à la taille maximum de 100 caractères
    //et de la variable "action" (qui permettra de savoir quelle fonction executer) aussi à la taille maximale de 100 caractères
    enum { INPUT_MAX = 100 };
    char input[INPUT_MAX];
    char action[INPUT_MAX] = "";


    //tableau des personnes inscrites afin de les garder en mémoire 
    inscription inscrit[MAX_INSCR];
    int nb_inscrit = 0; //nombre de personnes inscritent/présentent dans le tableau


    //tableau des différentes missions non attribuées
    mission m[MISSION_MAX];
    int nb_miss = 0; //nombre de missions non attribuées dans le tableau


    //tableau des missions attribuées
    mission_atrb m_atrb[MISSION_MAX];
    int nb_miss_atrb = 0; //nombre de missions attribuées


    //faire une boucle tant que le premier mot saisit par l'utilisateur n'est pas "exit"
    while (strcmp(action, "exit") != 0) {
        fgets(input, sizeof(input), stdin); //prendre la chaîne de caractère saisie par l'utilisateur

        (void)sscanf(input, "%s", action); //prendre le premier mot de la chaîne de caractère saisie par l'utilisateur pour la mettre dans la variable "action"

        //si action = à un des mots ci-dessous, appelle la fonction correspondante
        if (strcmp(action, "inscription") == 0)
            inscr(input, inscrit, &nb_inscrit);

        else if (strcmp(action, "mission") == 0)
            miss(input, m, &nb_miss, inscrit, &nb_inscrit);

        else if (strcmp(action, "consultation") == 0)
            consult(m, &nb_miss, inscrit, &nb_inscrit, m_atrb, &nb_miss_atrb);

        else if (strcmp(action, "detail") == 0)
            detail(input, m, &nb_miss, inscrit, &nb_inscrit, m_atrb, &nb_miss_atrb);

        else if (strcmp(action, "acceptation") == 0)
            acceptation(input, inscrit, &nb_inscrit, m, &nb_miss, m_atrb, &nb_miss_atrb);

        else if (strcmp(action, "sous-traitance") == 0)
            soustraitance(input, inscrit, &nb_inscrit, m, &nb_miss, m_atrb, &nb_miss_atrb);

        else if (strcmp(action, "rapport") == 0)
            rapport(input, inscrit, &nb_inscrit, m, &nb_miss, m_atrb, &nb_miss_atrb);
    }

    //si action = "exit", arrête le programme
    if (strcmp(action, "exit") == 0)
        exit(input);
}

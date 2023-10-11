#include <stdio.h>
#include <string.h>
#pragma warning(disable : 4996)

enum { LONG_MAX = 30 };
enum { MAX_INSCR = 50 };
enum { MISSION_MAX = 500 };

typedef struct inscription {
    char nom[LONG_MAX];
    char roles[LONG_MAX];
    int id;
} inscription;

typedef struct mission {
    int id_miss;
    char nom_miss[LONG_MAX];
    float remu;
    int auteur;
    int nb_ss_trait;
} mission;

typedef struct {
    int id_miss_atrb;
    char nom_miss_atrb[LONG_MAX];
    float remu_atrb;
    int auteur_atrb;
    int nb_ss_trait_atrb;
} mission_atrb;



void inscr(char* input, inscription* inscrit, int* nb_inscrit) {
    if (*nb_inscrit >= MAX_INSCR) return;

    char nom[LONG_MAX];
    char roles[LONG_MAX];

    sscanf(input, "%*s %s %s", roles, nom);

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

    sscanf(input, "%*s %d %s %f", &id, nom, &remu);

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

    if (remu <= 0) {
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

    sscanf(input, "%*s %d", &detail_id);
    int id_existe = 0;
    for (int i = 0; i < *nb_inscrit; i++) {
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
    sscanf(input, "%*s %d %d", &id_entreprise, &id_mission);

    id_entreprise = inscrit[id_entreprise - 1].id;

    if (id_entreprise == 0) {
        printf("Entreprise incorrecte\n");
    }
    else {
        printf("Acceptation enregistree\n");

        int numero_mission = 0;
        for (int i = 0; i < *nb_miss; i++) {
            if (m[i].id_miss == id_mission) {
                numero_mission = i;
                break;
            }
        }

        if (numero_mission != 0) {
            strcpy(m_atrb[*nb_miss_atrb].nom_miss_atrb, m[numero_mission].nom_miss);
            m_atrb[*nb_miss_atrb].remu_atrb = m[numero_mission].remu;
            m_atrb[*nb_miss_atrb].auteur_atrb = m[numero_mission].auteur;
            m_atrb[*nb_miss_atrb].nb_ss_trait_atrb = m[numero_mission].nb_ss_trait;

            (*nb_miss_atrb)++;

            m[numero_mission] = m[*nb_miss - 1];
            (*nb_miss)--;
        }
    }
}



int main() {
    enum { INPUT_MAX = 100 };
    char input[INPUT_MAX];
    char action[INPUT_MAX];

    inscription inscrit[MAX_INSCR];
    int nb_inscrit = 0;

    mission m[MISSION_MAX];
    int nb_miss = 0;

    mission_atrb m_atrb[MISSION_MAX];
    int nb_miss_atrb = 0;

    while (strcmp(action, "exit") != 0) {
        fgets(input, sizeof(input), stdin);
        size_t input_length = strlen(input);

        if (input_length > 0 && input[input_length - 1] == '\n') {
            input[input_length - 1] = '\0';
        }

        sscanf(input, "%s", action);

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
    }

    if (strcmp(action, "exit") == 0)
        return 0;
}

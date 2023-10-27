#include <stdio.h>
#include <string.h>
#pragma warning(disable : 4996)

enum {
    LONG_MAX = 30, //longueur maximale pour les noms d'entreprises et des missions
    MAX_INSCR = 50, //nombre maximum d'inscriptions
    MISSION_MAX = 500, //nombre maximum de missions
    SS_TRAIT_MAX = 5, //nombre maximum qu'une mission peut recevoir de sous-traitements
    NB_MAX_CODES = 3 //nombre maximum de code rapport qu'une mission peut recevoir
};

enum codes { ZERO = 0, UN = 1, DEUX = 2, TROIS = 3 }; //codes des rapports

//definition d'une structure pour les inscriptions
typedef struct {
    char nom[LONG_MAX];
    char roles[LONG_MAX];
    int id;

} inscription;

//structure pour les missions non attribuées
typedef struct {
    int id_miss;
    char nom_miss[LONG_MAX];
    float remu;
    int auteur;
    int nb_ss_trait;
    char code_rapport[NB_MAX_CODES];
} mission;

//structure pour les missions attribuées (qui reprend les mêmes variables que la structure mission)
typedef struct {
    int id_miss_atrb;
    char nom_miss_atrb[LONG_MAX];
    float remu_atrb;
    int auteur_atrb;
    int nb_ss_trait_atrb;
    char code_rapport[NB_MAX_CODES];
} mission_atrb;

//structure pour les missions terminées
typedef struct {
    int id_miss_trmn;
    char nom_miss_trmn[LONG_MAX];
    float remu_trmn;
    int auteur_trmn;
    int nb_ss_trait_trmn;
}mission_terminee;

//fonction pour arrêter le programme
int exit(char* input) {
    return 0;
}

//fonction pour inscrire une entreprise
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

    int nom_existe = 0;
    for (int i = 0; i < *nb_inscrit; i++) {
        if (strcmp(nom, inscrit[i].nom) == 0) {
            nom_existe = 1;
            break;
        }
    }

    if (nom_existe) {
        printf("Nom incorrect\n");
    }
    else {
        strcpy(inscrit[*nb_inscrit].nom, nom);
        strcpy(inscrit[*nb_inscrit].roles, roles);
        inscrit[*nb_inscrit].id = *nb_inscrit + 1;

        printf("Inscription realisee (%d)\n", *nb_inscrit + 1);
        ++(*nb_inscrit);
    }
}


//fonction pour publier des missions
void miss(char* input, mission* m, int* nb_miss, inscription* inscrit, int* nb_inscrit, int* nb_miss_total) {
    if (*nb_miss_total >= MISSION_MAX) return;

    int id_op = 0, nb_ss_trait = 0;
    char nom[LONG_MAX] = "";
    float remu = 0;

    (void)sscanf(input, "%*s %d %s %f\n", &id_op, nom, &remu);

    int id_existe = 0;
    for (int i = 0; i < *nb_inscrit; ++i) {
        if (id_op == inscrit[i].id) {
            id_existe = i + 1;
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
        printf("Mission publiee (%d)\n", *nb_miss_total + 1);

        m[*nb_miss].id_miss = *nb_miss_total + 1;
        strcpy(m[*nb_miss].nom_miss, nom);
        m[*nb_miss].remu = remu;
        m[*nb_miss].auteur = id_op;
        m[*nb_miss].nb_ss_trait = nb_ss_trait;

        ++(*nb_miss_total);
        ++(*nb_miss);
    }
}

//fonction pour consulter toutes les missions non attribuées
void consult(mission* m, int* nb_miss, inscription* inscrit, int* nb_inscrit) {

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

//fonction pour consulter les détails d'une mission non attribuée
void detail(char* input, mission* m, int* nb_miss, inscription* inscrit, int* nb_inscrit, mission_atrb* m_atrb, int* nb_miss_atrb) {
    int detail_id_miss = 0, id_miss = 0;
    int id_inscription_valide = 0;
    enum codes code;

    (void)sscanf(input, "%*s %d", &detail_id_miss);
    int id_miss_existe = 0;
    for (int i = 0; i < *nb_miss; i++) {
        if (detail_id_miss == m[i].id_miss) {
            id_miss_existe = 1;
            break;
        }
    }

    if (id_miss_existe == 0) {
        printf("Identifiant incorrect\n");
        return;
    }
    else {
        for (int i = 0; i < *nb_miss; ++i) {
            for (int j = 0; j < *nb_inscrit; ++j) {
                if ((m[i].id_miss == detail_id_miss) && (m[i].auteur == inscrit[j].id)) {
                    printf("%d %s %s %.2f (%d)\n", m[i].id_miss, m[i].nom_miss, inscrit[j].nom, m[i].remu, m[i].nb_ss_trait);

                }
            }
        }
    }

    for (int k = 0; k <= TROIS; ++k) {
        if (m[id_miss].code_rapport[k] == 1)
            printf("Local non disponible\n");
        else if (m[id_miss].code_rapport[k] == 2)
            printf("Pas de signal dans le boitier general\n");
        else if (m[id_miss].code_rapport[k] == 3)
            printf("Recepteur defectueux\n");
    
    }
}


//fonction pour qu'une entreprise puisse accepter une mission
void acceptation(char* input, inscription* inscrit, int* nb_inscrit, mission* m, int* nb_miss, mission_atrb* m_atrb, int* nb_miss_atrb) {
    int id_entreprise, id_mission_non_atrb;
    (void)sscanf(input, "%*s %d %d", &id_entreprise, &id_mission_non_atrb);

    id_entreprise = inscrit[id_entreprise - 1].id;

    if (strcmp(inscrit[id_entreprise].roles, "OP") == 0) {
        printf("Entreprise incorrecte\n");
    }
    else {
        int mission_non_atrb = - 1; //définition et initialisation de la variable qui permettra de vérifier si une mission 
                                    //est présente dans le tableau mission (donc si la mission existe et si elle n'est pas attribuée)
                                    //initialisé à -1 car sinon la mission 1 ne pourra pas être acceptée
        for (int i = 0; i < *nb_miss; i++) {
            if (m[i].id_miss == id_mission_non_atrb) {
                mission_non_atrb = i;
                break;
            }
        }

        if (mission_non_atrb != - 1) {
            printf("Acceptation enregistree\n");

            m_atrb[*nb_miss_atrb].id_miss_atrb = m[mission_non_atrb].id_miss;
            strcpy(m_atrb[*nb_miss_atrb].nom_miss_atrb, m[mission_non_atrb].nom_miss);
            m_atrb[*nb_miss_atrb].remu_atrb = m[mission_non_atrb].remu;
            m_atrb[*nb_miss_atrb].auteur_atrb = m[mission_non_atrb].auteur;
            m_atrb[*nb_miss_atrb].nb_ss_trait_atrb = m[mission_non_atrb].nb_ss_trait;

            ++(*nb_miss_atrb);
            
            m[mission_non_atrb] = m[*nb_miss - 1];
            (*nb_miss)--;
        }
        else
            printf("Mission incorrecte\n");
    }
}

//fonction de sous-traitance des missions
void soustraitance(char* input, inscription* inscrit, int* nb_inscrit, mission* m, int* nb_miss, mission_atrb* m_atrb, int* nb_miss_atrb, int* nb_miss_total) {
    int id_entreprise = 0, id_mission;
    float remu;

    (void)sscanf(input, "%*s %d %d %f\n", &id_entreprise, &id_mission, &remu);

    id_entreprise = inscrit[id_entreprise - 1].id;
    if (strcmp(inscrit[id_entreprise - 1].roles, "AG") != 0) {
        printf("Entreprise incorrecte\n");
        return;
    }

    int mission_non_atrb = -1;
    for (int i = 0; i < *nb_miss; i++) {
        if (m[i].id_miss == id_mission) {
            mission_non_atrb = i;
            break;
        }
    }

    if ((mission_non_atrb == -1) || (m[mission_non_atrb].nb_ss_trait >= SS_TRAIT_MAX)) {
        printf("Mission incorrecte\n");
        return;
    }

    else if (remu < 0) {
        printf("Remuneration incorrecte\n");
        return;
    }

    else {
        //création de la nouvelle mission
        mission nouvelle_mission;
        int pos_code = 0;

        //copie des informations de la nouvelle mission
        nouvelle_mission.id_miss = *nb_miss_total + 1;
        strcpy(nouvelle_mission.nom_miss, m[mission_non_atrb].nom_miss);
        nouvelle_mission.remu = remu;
        nouvelle_mission.auteur = id_entreprise;
        nouvelle_mission.nb_ss_trait = m[mission_non_atrb].nb_ss_trait + 1; 
        nouvelle_mission.code_rapport[pos_code] = m[mission_non_atrb].code_rapport;

        //ajouter la nouvelle mission au tableau des missions m
        m[*nb_miss] = nouvelle_mission;

        printf("Sous-traitance enregistree (%d)\n", *nb_miss_total + 1);
        ++(*nb_miss_total);
        ++(*nb_miss);

        //copie de la mission d'origine dans le tableau des missions attribuées (m_atrb)
        m_atrb[*nb_miss_atrb].id_miss_atrb = m[mission_non_atrb].id_miss;
        strcpy(m_atrb[*nb_miss_atrb].nom_miss_atrb, m[mission_non_atrb].nom_miss);
        m_atrb[*nb_miss_atrb].remu_atrb = m[mission_non_atrb].remu;
        m_atrb[*nb_miss_atrb].auteur_atrb = m[mission_non_atrb].auteur;
        m_atrb[*nb_miss_atrb].nb_ss_trait_atrb = m[mission_non_atrb].nb_ss_trait;
        m_atrb[*nb_miss_atrb].code_rapport[pos_code] = m[mission_non_atrb].code_rapport;

        ++(*nb_miss_atrb); //incrémentation du nombre de missions dans le tableau des missions attribuées (m_atrb)

        m[mission_non_atrb] = m[*nb_miss - 1];
        --(*nb_miss);
    }
}


//fonction pour faire le rapport d'une mission
void rapport(char* input, inscription* inscrit, int* nb_inscrit, mission* m, int* nb_miss, mission_atrb* m_atrb, int* nb_miss_atrb, mission_terminee* m_trmn, int* nb_miss_trmn, int* nb_miss_total) {
    int id_mission_atrb, nb_ss_trait = 0;
    enum codes code;
    float majoration2 = 5.5 / 100, majoration3 = 4.0 / 100;

    (void)sscanf(input, "%*s %d %d", &id_mission_atrb, &code);

    //vérifie si la mission existe
    int id_miss_existe = -1;
    for (int i = 0; i < *nb_miss_atrb; ++i)
        if (id_mission_atrb == m_atrb[i].id_miss_atrb) {
            id_miss_existe = i;
            break;
        }
    
    //si elle n'existe pas, msg d'erreur
    if (id_miss_existe == -1) {
        printf("Mission incorrecte\n");
        return;
    }

    //vérifie si la mission est bien la dernière de la chaîne de sous-traitement
    for (int j = 0; j < *nb_miss_atrb; ++j) 
        if (strcmp(m_atrb[id_miss_existe].nom_miss_atrb, m_atrb[j].nom_miss_atrb) == 0) 
            if (m_atrb[id_miss_existe].nb_ss_trait_atrb < m_atrb[j].nb_ss_trait_atrb) {
                printf("Mission incorrecte\n");
                return;
            }
            

    //si le code est inférieur strict ou supérieur strict à 0 et 3, msg d'erreur
    if (code < ZERO || code > TROIS) {
        printf("Code de retour incorrect\n");
        return;
    }

    else {
        //copie de la mission et ses sous-traitement dans le tableau des missions terminées (m_trmn)
        for (int k = 0; k < *nb_miss_atrb; ++k) {
            if (m_atrb[k].nom_miss_atrb == m_atrb[id_miss_existe].nom_miss_atrb) {
                m_trmn[*nb_miss_trmn].id_miss_trmn = m_atrb[k].id_miss_atrb;
                strcpy(m_trmn[*nb_miss_trmn].nom_miss_trmn, m_atrb[k].nom_miss_atrb);
                m_trmn[*nb_miss_trmn].auteur_trmn = m_atrb[k].auteur_atrb;
                m_trmn[*nb_miss_trmn].remu_trmn = m_atrb[k].remu_atrb;
                m_trmn[*nb_miss_trmn].nb_ss_trait_trmn = m_atrb[k].nb_ss_trait_atrb;

                ++(*nb_miss_trmn);
            }
            else
                continue;
        }

        if (code == 0)
            printf("Rapport enregistre\n");


        //création d'une nouvelle mission
        else{
            mission nouvelle_mission;
            int pos_code = 0;

            nouvelle_mission.id_miss = *nb_miss_total + 1;
            strcpy(nouvelle_mission.nom_miss, m_atrb[id_miss_existe].nom_miss_atrb); 
            nouvelle_mission.auteur = m_atrb[id_miss_existe].auteur_atrb; 
            nouvelle_mission.nb_ss_trait = nb_ss_trait;

            if (code == 1) {
                nouvelle_mission.remu = m_atrb[id_miss_existe].remu_atrb;
                nouvelle_mission.code_rapport[pos_code] = code;
                printf("Rapport enregistre (%d)\n", *nb_miss_total + 1);
                ++pos_code;
            }

            else if (code == 2) {
                //calcule la nouvelle rémunération de la nouvelle mission
                nouvelle_mission.remu = (m_atrb[id_miss_existe].remu_atrb) + (majoration2 * (m_atrb[id_miss_existe].remu_atrb));
                nouvelle_mission.code_rapport[pos_code] = code;
                printf("Rapport enregistre (%d)\n", *nb_miss_total + 1);
                ++pos_code;
            }

            else if (code == 3) {
                nouvelle_mission.remu = (m_atrb[id_miss_existe].remu_atrb) + (majoration3 * (m_atrb[id_miss_existe].remu_atrb));
                nouvelle_mission.code_rapport[pos_code] = code;
                printf("Rapport enregistre (%d)\n", *nb_miss_total + 1);
                ++pos_code;
            }

            //ajoute la nouvelle mission au tableau des missions non attribuées (m)
            m[*nb_miss] = nouvelle_mission;

            //incrémentation du nombre total de missions et du nombre de missions dans le tableau des missions non attribuées (m)
            ++(*nb_miss_total);
            ++(*nb_miss);
        }
    }
}


//fonction pour avoir un récapitulatif des entreprises
void recap(char* input, inscription* inscrit, int* nb_inscrit, mission* m, int* nb_miss, mission_atrb* m_atrb, int* nb_miss_atrb, mission_terminee* m_trmn, int* nb_miss_trmn, int* nb_miss_total) {
    int id_entreprise;
    (void)sscanf(input, "%*s %d", &id_entreprise);

    //vérifie si l'entreprise existe, si oui id_entreprise_existe prend la valeur i sinon 0
    int id_entreprise_existe = -1;
    for (int i = 0; i < *nb_inscrit; ++i)
        if (id_entreprise == inscrit[i].id) {
            id_entreprise_existe = i;
            break;
        }

    //si id_entreprise_existe == -1 (donc n'existe pas), msg d'erreur
    if (id_entreprise_existe == -1) {
        printf("Entreprise incorrecte\n");
        return;
    }

    printf("* non attribuees\n");
    for (int j = 0; j < *nb_miss; ++j) 
        if (m[j].auteur == id_entreprise) 
            printf("%d %s %s %.2f (%d)\n", m[j].id_miss, m[j].nom_miss, inscrit[m[j].auteur - 1].nom, m[j].remu, m[j].nb_ss_trait);
        
    printf("* attribuees\n");
    for (int k = 0; k < *nb_miss; ++k) 
        if(m_atrb[k].auteur_atrb == id_entreprise)
            printf("%d %s %s %.2f (%d)\n", m_atrb[k].id_miss_atrb, m_atrb[k].nom_miss_atrb, inscrit[m_atrb[k].auteur_atrb - 1].nom, m_atrb[k].remu_atrb, m_atrb[k].nb_ss_trait_atrb);   

    printf("* terminees\n");
    for (int l = 0; l < *nb_miss; ++l)
        if (m_trmn[l].auteur_trmn == id_entreprise)
            printf("%d %s %s %.2f (%d)\n", m_trmn[l].id_miss_trmn, m_trmn[l].nom_miss_trmn, inscrit[m_trmn[l].auteur_trmn - 1].nom, m_trmn[l].remu_trmn, m_trmn[l].nb_ss_trait_trmn);

    /*printf("* a realiser\n");
    for (int m = 0; m < *nb_miss; ++m) 
        if (m_atrb[m].auteur_atrb != id_entreprise)
            printf("%d %s %s %.2f (%d)\n", m_atrb[m].id_miss_atrb, m_atrb[m].nom_miss_atrb, inscrit[m_atrb[m].auteur_atrb - 1].nom, m_atrb[m].remu_atrb, m_atrb[m].nb_ss_trait_atrb);

    printf("* realisees\n");
    for (int n = 0; n < *nb_miss; ++n)
        if (m_trmn[n].auteur_trmn != id_entreprise)
            printf("%d %s %s %.2f (%d)\n", m_trmn[n].id_miss_trmn, m_trmn[n].nom_miss_trmn, inscrit[m_trmn[n].auteur_trmn - 1].nom, m_trmn[n].remu_trmn, m_trmn[n].nb_ss_trait_trmn);*/
}


int main() {
    //attribution de la valeur 100 à INPUT_MAX
    //définition de la variable "input" (qui va servir à récupérer ce qu'entre l'utilisateur) à la taille maximum de 100 caractères
    //et de la variable "action" (qui permettra de savoir quelle fonction executer) aussi à la taille maximale de 100 caractères
    enum { INPUT_MAX = 100 };
    char input[INPUT_MAX];
    char action[INPUT_MAX] = "";
    int nb_miss_total = 0; //nombre de missions au total qui sera aussi l'id des missions 

    //tableau des personnes inscrites afin de les garder en mémoire 
    inscription inscrit[MAX_INSCR];
    int nb_inscrit = 0; //nombre de personnes inscritent/présentent dans le tableau


    //tableau des différentes missions non attribuées
    mission m[MISSION_MAX];
    int nb_miss = 0; //nombre de missions non attribuées dans le tableau


    //tableau des missions attribuées
    mission_atrb m_atrb[MISSION_MAX];
    int nb_miss_atrb = 0; //nombre de missions attribuées

    mission_terminee m_trmn[MISSION_MAX];
    int nb_miss_trmn = 0;


    //faire une boucle tant que le premier mot saisit par l'utilisateur n'est pas "exit"
    while (strcmp(action, "exit") != 0) {
        fgets(input, sizeof(input), stdin); //prendre la chaîne de caractère saisie par l'utilisateur

        (void)sscanf(input, "%s", action); //prendre le premier mot de la chaîne de caractère saisie par l'utilisateur pour la mettre dans la variable "action"

        //si action = à un des mots ci-dessous, appelle la fonction correspondante
        if (strcmp(action, "inscription") == 0)
            inscr(input, inscrit, &nb_inscrit);

        else if (strcmp(action, "mission") == 0)
            miss(input, m, &nb_miss, inscrit, &nb_inscrit, &nb_miss_total);

        else if (strcmp(action, "consultation") == 0)
            consult(m, &nb_miss, inscrit, &nb_inscrit);

        else if (strcmp(action, "detail") == 0)
            detail(input, m, &nb_miss, inscrit, &nb_inscrit, m_atrb, &nb_miss_atrb, &nb_miss_total);

        else if (strcmp(action, "acceptation") == 0)
            acceptation(input, inscrit, &nb_inscrit, m, &nb_miss, m_atrb, &nb_miss_atrb);

        else if (strcmp(action, "sous-traitance") == 0)
            soustraitance(input, inscrit, &nb_inscrit, m, &nb_miss, m_atrb, &nb_miss_atrb, &nb_miss_total);

        else if (strcmp(action, "rapport") == 0)
            rapport(input, inscrit, &nb_inscrit, m, &nb_miss, m_atrb, &nb_miss_atrb, m_trmn, &nb_miss_trmn, &nb_miss_total);

        else if (strcmp(action, "recapitulatif") == 0)
            recap(input, inscrit, &nb_inscrit, m, &nb_miss, m_atrb, &nb_miss_atrb, m_trmn, &nb_miss_trmn, &nb_miss_total);
    }

    //si action = "exit", arrête le programme
    if (strcmp(action, "exit") == 0)
        exit(input);
}

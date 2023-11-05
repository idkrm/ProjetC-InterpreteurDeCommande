#include <stdio.h>
#include <string.h>
#pragma warning(disable : 4996)

enum {
    LONG_MAX = 31, //longueur maximale pour les noms d'entreprises et des missions
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

//structure pour les missions non attribuees
typedef struct {
    int id_miss, auteur, nb_ss_trait;
    char nom_miss[LONG_MAX];
    double remu;
    char code_rapport[NB_MAX_CODES];
    int echec[MAX_INSCR];
} mission;

//structure pour les missions attribuees (qui reprend les memes variables que la structure mission)
typedef struct {
    mission miss_atrb;
    int proprio; //proprietaire de la mission, cad l'id de l'entreprise qui a accepte la mission
} mission_atrb;

//structure pour les missions terminees
typedef struct {
    mission miss_trmn;
    int proprio;
}mission_terminee;


//declaration de fonctions annexes (fonctions non demandees dans le sujet, leur definition se trouve a la toute fin du code, apres le main)
//fonctions qui verifie si une mission, une entreprise ou un nom existe
int nom_connu(const char* nom, const inscription* inscrit, int nb_inscrit); //empeche une entreprise de s'inscrire deux fois
int mission_existe(int id_miss_existe, mission* m, int* nb_miss); //verifie si une mission existe
int entreprise_existe(int id, const inscription* inscrit, int* nb_inscrit); //verifie si une entreprise existe
int mission_atrb_existe(int id, mission_atrb* m_atrb, int* nb_miss_atrb); //verifie si une mission attribuee existe

//fonctions qui permettent de copier les donnees d'un tableau dans un autre tableau
void copie_m_to_atrb(int id, mission_atrb* m_atrb, int* nb_miss_atrb, mission* m, int* nb_miss); //tableau des missions non attribuees vers les missions attribuees
void copie_atrb_to_trmn(int id, mission_atrb* m_atrb, int* nb_miss_atrb, mission_terminee* m_trmn, int* nb_miss_trmn); //depuis le tableau des mission attribuees au tableau des missions terminees

//fonctions en lien avec les codes rapports et les echecs
void afficher_codes_rapport(int id_miss, mission* m); //permet d'afficher les msg d'erreur des codes rapports
void affectation_code_rapport_echec(mission* nouvelle_mission, enum codes code, int proprio); //affecte les nouveaux codes rapports et id des entreprises qui réalisent des echecs dans les tableaux correspondants
void remplir(int* tableau, int taille, int valeur_initiale); //remplis les tableaux des codes rapports et des echecs à la valeur "vide" : -1 (qui signifie qu'il n'y a ni code rapport ni echec)

//fonctions qui permettent d'afficher les differents recapitulatifs
void recap_non_attribuees(int id_entreprise, mission* m, int* nb_miss, inscription* inscrit);
void recap_attribuees(int id_entreprise, mission_atrb* m_atrb, int* nb_miss_atrb, inscription* inscrit);
void recap_terminees(int id_entreprise, mission_terminee* m_trmn, int* nb_miss_trmn, inscription* inscrit);
void recap_a_realiser(int id_entreprise, mission_atrb* m_atrb, int* nb_miss_atrb, inscription* inscrit);
void recap_realisees(int id_entreprise, mission_terminee* m_trmn, int* nb_miss_trmn, inscription* inscrit);



//fonction pour arreter le programme
int exit(char* input) {
    return 0;
}

//fonction pour inscrire une entreprise
void inscr(char* input, inscription* inscrit, int* nb_inscrit) {
    if (*nb_inscrit >= MAX_INSCR) return;

    char nom[LONG_MAX], roles[LONG_MAX];

    //on utilise sscanf pour lire la chaine de caractere "input" obtenu avec la fonction fgets
    (void)sscanf(input, "%*s %s %s\n", roles, nom); //(void) devant la fonction afin d'eviter l'avertissement "return value ignored"

    //si le role est different de OP AG ou IN, msg d'erreur
    if (strcmp(roles, "OP") != 0 && strcmp(roles, "AG") != 0 && strcmp(roles, "IN") != 0) {
        printf("Role incorrect\n");
        return;
    }

    //si le nom existe deja, msg d'erreur
    if (nom_connu(nom, inscrit, *nb_inscrit)) {
        printf("Nom incorrect\n");
    }
    else { //sinon copie des informations renseignees dans le tableau des entreprises inscrites
        strcpy(inscrit[*nb_inscrit].nom, nom);
        strcpy(inscrit[*nb_inscrit].roles, roles);
        inscrit[*nb_inscrit].id = *nb_inscrit + 1;

        printf("Inscription realisee (%d)\n", *nb_inscrit + 1);
        ++(*nb_inscrit); //incrementation du nombre d'inscrit
    }
}


//fonction pour publier des missions
void miss(char* input, mission* m, int* nb_miss, inscription* inscrit, int* nb_inscrit, int* nb_miss_total) {
    if (*nb_miss_total >= MISSION_MAX) return; //si le nombre maximum de missions est atteint, le programme s'arrête

    int id_op, nb_ss_trait = 0, longueur = 0;
    char nom[LONG_MAX] = "";
    float remu;

    (void)sscanf(input, "%*s %d %s %f\n", &id_op, nom, &remu);

    int id_existe = entreprise_existe(id_op, inscrit, nb_inscrit); //verifie si l'entreprise existe, si non renvoie -1

    if ((id_existe == -1) || (strcmp(inscrit[id_existe].roles, "OP") != 0)) { //si elle n'existe pas ou qu'elle n'a pas le role OP, msg d'erreur
        printf("Identifiant incorrect\n");
        return;
    }

    if (remu <= 0) { //si la remuneration est inferieure ou egale a zero, msg d'erreur
        printf("Remuneration incorrecte\n");
        return;
    }

    else {
        printf("Mission publiee (%d)\n", *nb_miss_total + 1); //sinon msg de publication

        //copie des informations de la mission dans le tableau des missions
        m[*nb_miss].id_miss = *nb_miss_total + 1;
        strcpy(m[*nb_miss].nom_miss, nom);
        m[*nb_miss].remu = remu;
        m[*nb_miss].auteur = id_op;
        m[*nb_miss].nb_ss_trait = nb_ss_trait;

        //remplis le tableau des codes rapports et des echecs par -1 (ce qui signifie qu'il n'y a actuellement pas de rapports ni d'echecs)
        int vide = -1;
        remplir(&m[*nb_miss].code_rapport, NB_MAX_CODES, vide);
        remplir(&m[*nb_miss].echec, MAX_INSCR, vide);


        ++(*nb_miss_total); //incrementation du nombre total de mission
        ++(*nb_miss); //incrementation du nombre des missions dans le tableau mission m
    }
}


//fonction pour consulter toutes les missions non attribuees
void consult(mission* m, int* nb_miss, inscription* inscrit, int* nb_inscrit) {

    //premiere boucle qui parcourt nb_miss, deuxieme qui parcourt nb_inscrit et lorsque m[i].auteur == inscrit[j].id, cad lorsque l'auteur de la mission (son id) dans le tableau m
    //est egal a l'id qui se trouve dans le tableau inscrit, afficher la mission (cette double boucle permet d'afficher le nom de l'auteur de la mission (puisque le nom de l'auteur 
    //ne se trouve pas dans le tableau des missions))
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

//fonction pour consulter les details d'une mission non attribuee
void detail(char* input, mission* m, int* nb_miss, inscription* inscrit, int* nb_inscrit) {
    int detail_id_miss = 0, id_miss = 0;

    (void)sscanf(input, "%*s %d", &detail_id_miss);

    int id_existe = mission_existe(detail_id_miss, m, nb_miss); //fonction qui verifie si une mission existe, si elle n'existe pas elle renvoie la valeur -1
    if (id_existe == -1) {
        printf("Identifiant incorrect\n");
        return;
    }

    //double boucle qui a le meme fonctionnement que dans la fonction ci dessus
    for (int i = 0; i < *nb_miss; ++i) {
        for (int j = 0; j < *nb_inscrit; ++j)
            if ((m[i].id_miss == detail_id_miss) && (m[i].auteur == inscrit[j].id)) {
                printf("%d %s %s %.2f (%d)\n", m[i].id_miss, m[i].nom_miss, inscrit[j].nom, m[i].remu, m[i].nb_ss_trait);
            }
    }

    afficher_codes_rapport(id_miss, m); //fonction qui affiche les messages des codes rapports (s'il y en a)
}


//fonction pour qu'une entreprise puisse accepter une mission
void acceptation(char* input, inscription* inscrit, int* nb_inscrit, mission* m, int* nb_miss, mission_atrb* m_atrb, int* nb_miss_atrb) {
    int id_entreprise, id_mission_non_atrb;
    (void)sscanf(input, "%*s %d %d", &id_entreprise, &id_mission_non_atrb);

    int id_existe = entreprise_existe(id_entreprise, inscrit, nb_inscrit); //verifie si une entreprise existe, renvoie -1 si n'existe pas

    if (id_existe == -1 || strcmp(inscrit[id_existe].roles, "OP") == 0) { //si -1 ou si c'est un OP, msg d'erreur
        printf("Entreprise incorrecte\n");
        return;
    }

    else { //sinon verifie si la mission existe
        int mission_non_atrb = -1;
        for (int i = 0; i < *nb_miss; i++) {
            if (m[i].id_miss == id_mission_non_atrb) {
                mission_non_atrb = i; //si elle existe la variable prend la valeur i
                break;
            }
        }

        //si l'entreprise est responsable d'un echec de la mission, msg d'erreur
        for (int j = 0; j < MAX_INSCR; ++j)
            if (m[mission_non_atrb].echec[j] == id_entreprise) {
                printf("Entreprise incorrecte\n");
                return;
            }

        //sinon l'acceptation est enregistree
        if (mission_non_atrb != -1) {
            printf("Acceptation enregistree\n");

            //remplis les codes rapports et echecs par la valeur "vide"
            int vide = -1;
            remplir(&m_atrb[*nb_miss_atrb].miss_atrb.code_rapport, NB_MAX_CODES, vide);
            remplir(&m_atrb[*nb_miss_atrb].miss_atrb.echec, MAX_INSCR, vide);

            copie_m_to_atrb(mission_non_atrb, m_atrb, nb_miss_atrb, m, nb_miss); //fonction qui permet de copier les donnees du tableau mission au tableau des missions attribuees
            m_atrb[*nb_miss_atrb].proprio = id_entreprise; //assigne cette mission a l'entreprise correspondante

            ++(*nb_miss_atrb);

            m[mission_non_atrb] = m[*nb_miss - 1]; //suppression de la mission du tableau des missions non attribuees (puisqu'elle est maintenant attribuee)
            (*nb_miss)--; //decrementation du nombre de mission dans la tableau des missions non attribuees
            return;
        }
        else {
            printf("Mission incorrecte\n");
            return;
        }
    }
}

//fonction de sous-traitance des missions
void soustraitance(char* input, inscription* inscrit, int* nb_inscrit, mission* m, int* nb_miss, mission_atrb* m_atrb, int* nb_miss_atrb, int* nb_miss_total) {
    int id_entreprise, id_mission;
    float remu;

    (void)sscanf(input, "%*s %d %d %f\n", &id_entreprise, &id_mission, &remu);

    //verifie si l'entreprise existe
    int id_existe = entreprise_existe(id_entreprise, inscrit, nb_inscrit);

    //si elle n'existe pas ou si elle n'a pas le role AG, msg d'erreur
    if (id_existe == -1 || strcmp(inscrit[id_existe].roles, "AG") != 0) {
        printf("Entreprise incorrecte\n");
        return;
    }

    //verifie si une mission existe
    int mission_non_atrb = mission_existe(id_mission, m, nb_miss);
    if ((mission_non_atrb == -1) || (m[mission_non_atrb].nb_ss_trait >= SS_TRAIT_MAX)) {
        printf("Mission incorrecte\n");
        return;
    }

    else if (remu <= 0) { //si la remuneration est inferieure ou egale a 0, msg d'erreur
        printf("Remuneration incorrecte\n");
        return;
    }

    else {
        //creation de la nouvelle mission
        mission nouvelle_mission;

        //copie des informations de la nouvelle mission
        nouvelle_mission.id_miss = *nb_miss_total + 1;
        strcpy(nouvelle_mission.nom_miss, m[mission_non_atrb].nom_miss);
        nouvelle_mission.remu = remu;
        nouvelle_mission.auteur = id_entreprise;
        nouvelle_mission.nb_ss_trait = m[mission_non_atrb].nb_ss_trait + 1;

        //remplissage des tableaux codes rapports et echecs par la valeur "vide" (puisque c'est une nouvelle mission, il est normal que cela soit vide)
        int vide = -1;
        remplir(&nouvelle_mission.code_rapport, NB_MAX_CODES, vide);
        remplir(&nouvelle_mission.echec, MAX_INSCR, vide);

        //ajout de la nouvelle mission au tableau des missions m
        m[*nb_miss] = nouvelle_mission;

        printf("Sous-traitance enregistree (%d)\n", *nb_miss_total + 1);
        ++(*nb_miss_total);
        ++(*nb_miss);

        //copie de la mission d'origine dans le tableau des missions attribuees (m_atrb)
        copie_m_to_atrb(mission_non_atrb, m_atrb, nb_miss_atrb, m, nb_miss);

        ++(*nb_miss_atrb); //incrementation du nombre de missions dans le tableau des missions attribuees (m_atrb)

        m[mission_non_atrb] = m[*nb_miss - 1]; //suppression de la mission du tableau des missions non attribuees
        --(*nb_miss);
    }
}


//fonction pour realiser le rapport d'une mission
void rapport(char* input, mission* m, int* nb_miss, mission_atrb* m_atrb, int* nb_miss_atrb, mission_terminee* m_trmn, int* nb_miss_trmn, int* nb_miss_total) {
    int id_mission_atrb, nb_ss_trait = 0;
    enum codes code;
    double majoration2 = 5.5 / 100, majoration3 = 4.0 / 100;

    (void)sscanf(input, "%*s %d %d", &id_mission_atrb, &code);

    //verifie si la mission attribuee existe
    int id_miss_existe = mission_atrb_existe(id_mission_atrb, m_atrb, nb_miss_atrb);

    //si elle n'existe pas, msg d'erreur
    if (id_miss_existe == -1) {
        printf("Mission incorrecte\n");
        return;
    }

    //verifie si la mission est bien la derniere de la chaine de sous-traitement
    for (int j = 0; j < *nb_miss_atrb; ++j)
        if (strcmp(m_atrb[id_miss_existe].miss_atrb.nom_miss, m_atrb[j].miss_atrb.nom_miss) == 0)
            if (m_atrb[id_miss_existe].miss_atrb.nb_ss_trait < m_atrb[j].miss_atrb.nb_ss_trait) {
                printf("Mission incorrecte\n");
                return;
            }


    //si le code est inferieur strict ou superieur strict a 0 et 3, msg d'erreur
    if (code < ZERO || code > TROIS) {
        printf("Code de retour incorrect\n");
        return;
    }

    else {
        //copie de la mission et ses sous-traitement dans le tableau des missions terminees (m_trmn)
        copie_atrb_to_trmn(id_miss_existe, m_atrb, nb_miss_atrb, m_trmn, nb_miss_trmn);

        ++(*nb_miss_trmn);
        m_atrb[id_miss_existe] = m_atrb[*nb_miss_atrb - 1]; //suppression de la mission du tableau des missions attribuees
        --(*nb_miss_atrb);
    }

    //code rapport
    if (code == ZERO)
        printf("Rapport enregistre\n");


    //creation d'une nouvelle mission
    else {
        mission nouvelle_mission;
        int vide = -1;

        //remplis le tableau des codes rapports par -1 (la valeur qui represente le vide)
        remplir(&nouvelle_mission.code_rapport, NB_MAX_CODES, vide);
        remplir(&nouvelle_mission.echec, MAX_INSCR, vide);

        //renseigne les informations de la nouvelle mission
        nouvelle_mission.id_miss = *nb_miss_total + 1;
        strcpy(nouvelle_mission.nom_miss, m_atrb[id_miss_existe].miss_atrb.nom_miss);
        nouvelle_mission.auteur = m_atrb[id_miss_existe].miss_atrb.auteur;
        nouvelle_mission.nb_ss_trait = nb_ss_trait;

        //copie des anciens code rapport (s'il y en a)
        for (int i = 0; i < NB_MAX_CODES; ++i)
            if ((nouvelle_mission.code_rapport[i] == vide) && ((m_atrb[id_miss_existe].miss_atrb.code_rapport[i] == UN) || (m_atrb[id_miss_existe].miss_atrb.code_rapport[i] == DEUX) || (m_atrb[id_miss_existe].miss_atrb.code_rapport[i] == TROIS)))
                nouvelle_mission.code_rapport[i] = m_atrb[id_miss_existe].miss_atrb.code_rapport[i];

        //copie des id des entreprises qui ont realise des echecs (s'il y en a)
        for (int j = 0; j < MAX_INSCR; ++j)
            if (nouvelle_mission.echec[j] == vide)
                nouvelle_mission.echec[j] = m_atrb[id_miss_existe].miss_atrb.echec[j];


        //calcul des nouvelles remunerations et affecte les nouveaux codes rapports et id des entreprises qui realisent des echecs dans les tableaux correspondants
        if (code == UN) {
            nouvelle_mission.remu = m_atrb[id_miss_existe].miss_atrb.remu;
            affectation_code_rapport_echec(&nouvelle_mission, code, m_atrb[id_miss_existe].proprio);
            printf("Rapport enregistre (%d)\n", *nb_miss_total + 1);
        }

        if (code == DEUX) {
            nouvelle_mission.remu = (m_atrb[id_miss_existe].miss_atrb.remu) + (majoration2 * (m_atrb[id_miss_existe].miss_atrb.remu));
            affectation_code_rapport_echec(&nouvelle_mission, code, m_atrb[id_miss_existe].proprio);
            printf("Rapport enregistre (%d)\n", *nb_miss_total + 1);
        }

        if (code == TROIS) {
            nouvelle_mission.remu = (m_atrb[id_miss_existe].miss_atrb.remu) + (majoration3 * (m_atrb[id_miss_existe].miss_atrb.remu));
            affectation_code_rapport_echec(&nouvelle_mission, code, m_atrb[id_miss_existe].proprio);
            printf("Rapport enregistre (%d)\n", *nb_miss_total + 1);
        }

        //ajoute la nouvelle mission au tableau des missions non attribuees (m)
        m[*nb_miss] = nouvelle_mission;

        //incrementation du nombre total de missions et du nombre de missions dans le tableau des missions non attribuees (m)
        ++(*nb_miss_total);
        ++(*nb_miss);
    }
}


//fonction pour avoir un recapitulatif des entreprises
void recap(char* input, inscription* inscrit, int* nb_inscrit, mission* m, int* nb_miss, mission_atrb* m_atrb, int* nb_miss_atrb, mission_terminee* m_trmn, int* nb_miss_trmn, int* nb_miss_total) {
    int id_entreprise;
    (void)sscanf(input, "%*s %d", &id_entreprise);

    int id_entreprise_existe = entreprise_existe(id_entreprise, inscrit, nb_inscrit);

    //si id_entreprise_existe == -1 (donc n'existe pas), msg d'erreur
    if (id_entreprise_existe == -1) {
        printf("Entreprise incorrecte\n");
        return;
    }

    //affiche les differents recapitulatifs
    recap_non_attribuees(id_entreprise, m, nb_miss, inscrit);
    recap_attribuees(id_entreprise, m_atrb, nb_miss_atrb, inscrit);
    recap_a_realiser(id_entreprise, m_atrb, nb_miss_atrb, inscrit);
    recap_terminees(id_entreprise, m_trmn, nb_miss_trmn, inscrit);
    recap_realisees(id_entreprise, m_trmn, nb_miss_trmn, inscrit);
}

int main() {
    //attribution de la valeur 100 a INPUT_MAX
    //definition de la variable "input" (qui va servir a recuperer ce qu'entre l'utilisateur) a la taille maximum de 100 caract?res
    //et de la variable "action" (qui permettra de savoir quelle fonction executer) aussi a la taille maximale de 100 caract?res
    enum { INPUT_MAX = 100 };
    char input[INPUT_MAX];
    char action[INPUT_MAX] = "";
    int nb_miss_total = 0, nb_inscrit = 0, nb_miss = 0, nb_miss_atrb = 0, nb_miss_trmn = 0; //nombre d'elements dans les tableaux

    inscription inscrit[MAX_INSCR]; //tableau des entreprises isncrites
    mission m[MISSION_MAX]; //tableau des missions non attribuees
    mission_atrb m_atrb[MISSION_MAX]; //tableau des missions attribuees
    mission_terminee m_trmn[MISSION_MAX]; //tableau des missions terminees

    //faire une boucle tant que le premier mot saisit par l'utilisateur n'est pas "exit"
    while (strcmp(action, "exit") != 0) {
        fgets(input, sizeof(input), stdin); //prendre la chaine de caract?re saisie par l'utilisateur

        (void)sscanf(input, "%s", action); //prendre le premier mot de la chaine de caract?re saisie par l'utilisateur pour la mettre dans la variable "action"

        //si action = a un des mots ci-dessous, appelle la fonction correspondante
        if (strcmp(action, "inscription") == 0)
            inscr(input, inscrit, &nb_inscrit);

        else if (strcmp(action, "mission") == 0)
            miss(input, m, &nb_miss, inscrit, &nb_inscrit, &nb_miss_total);

        else if (strcmp(action, "consultation") == 0)
            consult(m, &nb_miss, inscrit, &nb_inscrit);

        else if (strcmp(action, "detail") == 0)
            detail(input, m, &nb_miss, inscrit, &nb_inscrit);

        else if (strcmp(action, "acceptation") == 0)
            acceptation(input, inscrit, &nb_inscrit, m, &nb_miss, m_atrb, &nb_miss_atrb);

        else if (strcmp(action, "sous-traitance") == 0)
            soustraitance(input, inscrit, &nb_inscrit, m, &nb_miss, m_atrb, &nb_miss_atrb, &nb_miss_total);

        else if (strcmp(action, "rapport") == 0)
            rapport(input, m, &nb_miss, m_atrb, &nb_miss_atrb, m_trmn, &nb_miss_trmn, &nb_miss_total);

        else if (strcmp(action, "recapitulatif") == 0)
            recap(input, inscrit, &nb_inscrit, m, &nb_miss, m_atrb, &nb_miss_atrb, m_trmn, &nb_miss_trmn, &nb_miss_total);
    }

    //si action = "exit", arrete le programme
    if (strcmp(action, "exit") == 0)
        exit(input);
}


//definition des fonctions annexes
//fonction qui permet de verifier si un nom d'entreprise est deja connu ou non (pour la fonction inscr)
int nom_connu(const char* nom, const inscription* inscrit, int nb_inscrit) {
    for (int i = 0; i < nb_inscrit; i++)
        if (strcmp(nom, inscrit[i].nom) == 0)
            return 1;  //le nom est deja connu

    return 0;
}

//fonction qui permet de verifier si l'id d'une mission existe (utilise dans la fonction detail), si oui renvoie i si non -1
int mission_existe(int id, mission* m, int* nb_miss) {
    for (int i = 0; i < *nb_miss; i++) {
        if (id == m[i].id_miss)
            return i;
    }
    return -1;
}

//verifie si une entreprise existe, si oui renvoie i sinon -1
int entreprise_existe(int id, const inscription* inscrit, int* nb_inscrit) {
    for (int i = 0; i < *nb_inscrit; ++i) {
        if (id == inscrit[i].id)
            return i;
    }
    return -1;
}

//verifie si une mission attribuee existe ou non, si oui renvoie i si non -1
int mission_atrb_existe(int id, mission_atrb* m_atrb, int* nb_miss_atrb) {
    for (int i = 0; i < *nb_miss_atrb; ++i)
        if (id == m_atrb[i].miss_atrb.id_miss) {
            return i;
        }
    return -1;
}

//copie des missions non attribuees dans le tableau des missions attribuees
void copie_m_to_atrb(int id, mission_atrb* m_atrb, int* nb_miss_atrb, mission* m, int* nb_miss) {
    int vide = -1;

    m_atrb[*nb_miss_atrb].miss_atrb.id_miss = m[id].id_miss;
    strcpy(m_atrb[*nb_miss_atrb].miss_atrb.nom_miss, m[id].nom_miss);
    m_atrb[*nb_miss_atrb].miss_atrb.remu = m[id].remu;
    m_atrb[*nb_miss_atrb].miss_atrb.auteur = m[id].auteur;
    m_atrb[*nb_miss_atrb].miss_atrb.nb_ss_trait = m[id].nb_ss_trait;

    for (int i = 0; i < NB_MAX_CODES; ++i)
        if (m_atrb[*nb_miss_atrb].miss_atrb.code_rapport[i] == vide)
            m_atrb[*nb_miss_atrb].miss_atrb.code_rapport[i] = m[id].code_rapport[i];

    for (int j = 0; j < MAX_INSCR; ++j)
        if ((m_atrb[*nb_miss_atrb].miss_atrb.echec[j] == vide) && (m[id].echec != vide))
            m_atrb[*nb_miss_atrb].miss_atrb.echec[j] = m[id].echec[j];
}

//copie des missions attribuees dans le tableau des missions terminees
void copie_atrb_to_trmn(int id, mission_atrb* m_atrb, int* nb_miss_atrb, mission_terminee* m_trmn, int* nb_miss_trmn) {
    for (int i = 0; i < *nb_miss_atrb; ++i) {
        if (m_atrb[i].miss_atrb.nom_miss == m_atrb[id].miss_atrb.nom_miss) {
            m_trmn[*nb_miss_trmn].miss_trmn.id_miss = m_atrb[i].miss_atrb.id_miss;
            strcpy(m_trmn[*nb_miss_trmn].miss_trmn.nom_miss, m_atrb[i].miss_atrb.nom_miss);
            m_trmn[*nb_miss_trmn].miss_trmn.auteur = m_atrb[i].miss_atrb.auteur;
            m_trmn[*nb_miss_trmn].miss_trmn.remu = m_atrb[i].miss_atrb.remu;
            m_trmn[*nb_miss_trmn].miss_trmn.nb_ss_trait = m_atrb[i].miss_atrb.nb_ss_trait;
            m_trmn[*nb_miss_trmn].proprio = m_atrb[i].proprio;
        }
    }
}

//permet d'afficher les msg des codes rapport pour la fonction detail
void afficher_codes_rapport(int id_miss, mission* m) {
    for (int k = DEUX; k >= ZERO; --k) {
        if (m[id_miss].code_rapport[k] == UN)
            printf("Local non accessible\n");

        else if (m[id_miss].code_rapport[k] == DEUX)
            printf("Pas de signal dans le boitier general\n");

        else if (m[id_miss].code_rapport[k] == TROIS)
            printf("Recepteur defectueux\n");

    }
}

//affecte les nouveaux codes rapports et ID des entreprises qui realisent des echecs dans les tableaux correspondant
void affectation_code_rapport_echec(mission* nouvelle_mission, enum codes code, int proprio) {
    for (int i = 0; i < NB_MAX_CODES; ++i) {
        if (nouvelle_mission->code_rapport[i] == -1) {
            nouvelle_mission->code_rapport[i] = code;
            break;
        }
    }
    for (int j = 0; j < MAX_INSCR; ++j) {
        if (nouvelle_mission->echec[j] == -1) {
            nouvelle_mission->echec[j] = proprio;
            break;
        }
    }
}

//sert a remplir les tableaux codes rapports et echecs de la valeur "vide" : -1
void remplir(int* tableau, int taille, int valeur_initiale) {
    for (int i = 0; i < taille; ++i) {
        tableau[i] = valeur_initiale;
    }
}

//fonctions pour les recapitulatifs :
void recap_non_attribuees(int id_entreprise, mission* m, int* nb_miss, inscription* inscrit) {
    int non_attribuees = 0;
    for (int j = 0; j < *nb_miss; ++j)
        if (m[j].auteur == id_entreprise) {
            non_attribuees = 1;
            break;
        }

    if (non_attribuees)
        printf("* non attribuees\n");
    for (int j = 0; j < *nb_miss; ++j)
        if (m[j].auteur == id_entreprise)
            printf("%d %s %s %.2f (%d)\n", m[j].id_miss, m[j].nom_miss, inscrit[m[j].auteur - 1].nom, m[j].remu, m[j].nb_ss_trait);
}

void recap_attribuees(int id_entreprise, mission_atrb* m_atrb, int* nb_miss_atrb, inscription* inscrit) {
    int attribuees = 0;
    for (int k = 0; k < *nb_miss_atrb; ++k)
        if (m_atrb[k].miss_atrb.auteur == id_entreprise && m_atrb[k].proprio != id_entreprise) {
            attribuees = 1;
            break;
        }

    if (attribuees)
        printf("* attribuees\n");
    for (int k = 0; k < *nb_miss_atrb; ++k)
        if (m_atrb[k].miss_atrb.auteur == id_entreprise && m_atrb[k].proprio != id_entreprise)
            printf("%d %s %s %.2f (%d)\n", m_atrb[k].miss_atrb.id_miss, m_atrb[k].miss_atrb.nom_miss, inscrit[m_atrb[k].miss_atrb.auteur - 1].nom, m_atrb[k].miss_atrb.remu, m_atrb[k].miss_atrb.nb_ss_trait);
}

void recap_terminees(int id_entreprise, mission_terminee* m_trmn, int* nb_miss_trmn, inscription* inscrit) {
    int terminees = 0;
    for (int l = 0; l < *nb_miss_trmn; ++l)
        if (m_trmn[l].miss_trmn.auteur == id_entreprise) {
            terminees = 1;
            break;
        }

    if (terminees)
        printf("* terminees\n");
    for (int l = 0; l < *nb_miss_trmn; ++l)
        if (m_trmn[l].miss_trmn.auteur == id_entreprise)
            printf("%d %s %s %.2f (%d)\n", m_trmn[l].miss_trmn.id_miss, m_trmn[l].miss_trmn.nom_miss, inscrit[m_trmn[l].miss_trmn.auteur - 1].nom, m_trmn[l].miss_trmn.remu, m_trmn[l].miss_trmn.nb_ss_trait);
}

void recap_a_realiser(int id_entreprise, mission_atrb* m_atrb, int* nb_miss_atrb, inscription* inscrit) {
    int a_realiser = 0;
    for (int o = 0; o < *nb_miss_atrb; ++o)
        if (m_atrb[o].proprio == id_entreprise) {
            a_realiser = 1;
            break;
        }

    if (a_realiser)
        printf("* a realiser\n");
    for (int o = 0; o < *nb_miss_atrb; ++o)
        if (m_atrb[o].proprio == id_entreprise)
            printf("%d %s %s %.2f (%d)\n", m_atrb[o].miss_atrb.id_miss, m_atrb[o].miss_atrb.nom_miss, inscrit[m_atrb[o].miss_atrb.auteur - 1].nom, m_atrb[o].miss_atrb.remu, m_atrb[o].miss_atrb.nb_ss_trait);
}

void recap_realisees(int id_entreprise, mission_terminee* m_trmn, int* nb_miss_trmn, inscription* inscrit) {
    int realisees = 0;
    for (int n = 0; n < *nb_miss_trmn; ++n)
        if (m_trmn[n].proprio == id_entreprise) {
            realisees = 1;
            break;
        }

    if (realisees)
        printf("* realisees\n");
    for (int n = 0; n < *nb_miss_trmn; ++n)
        if (m_trmn[n].proprio == id_entreprise)
            printf("%d %s %s %.2f (%d)\n", m_trmn[n].miss_trmn.id_miss, m_trmn[n].miss_trmn.nom_miss, inscrit[m_trmn[n].miss_trmn.auteur - 1].nom, m_trmn[n].miss_trmn.remu, m_trmn[n].miss_trmn.nb_ss_trait);

}
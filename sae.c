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
    int idMission;
    char nomMission[LONG_MAX];
    double remu;
    int idAuteur;
    int nbSousTraitement;
    char codeRapport[NB_MAX_CODES];
    int echec[MAX_INSCR]; //tableau pour stocker les entreprises qui sont responsables des echecs des missions
} mission;

//structure pour les missions attribuees (qui reprend les memes variables que la structure mission)
typedef struct {
    mission missionAttribuee;
    int proprio; //proprietaire de la mission, cad l'id de l'entreprise qui a accepte la mission
} missionAttribuee;

//structure pour les missions terminees
typedef struct {
    mission missionTerminee;
    int proprio;
}missionTerminee;


//declaration de fonctions annexes (fonctions non demandees dans le sujet, leur definition se trouve a la toute fin du code, apres le main)

//fonctions qui verifie si une mission, une entreprise ou un nom existe
int nomConnu(const char* nom, const inscription* inscrit, int nbInscrit); //empeche une entreprise de s'inscrire deux fois
int missionExiste(int idmissionExiste, mission* m, int* nbMission); //verifie si une mission existe
int entrepriseExiste(int id, const inscription* inscrit, int* nbInscrit); //verifie si une entreprise existe
int missionAttribueeExiste(int id, missionAttribuee* missionAttribuee, int* nbMissionionAttribuee); //verifie si une mission attribuee existe
//fonctions qui permettent de copier les donnees d'un tableau dans un autre tableau
void copie_m_to_atrb(int id, missionAttribuee* missionAttribuee, int* nbMissionionAttribuee, mission* m, int* nbMission); //tableau des missions non attribuees vers les missions attribuees
void copie_atrb_to_trmn(int id, missionAttribuee* missionAttribuee, int* nbMissionionAttribuee, missionTerminee* missionTerminee, int* nbMissionionTerminee); //depuis le tableau des mission attribuees au tableau des missions terminees
//fonctions en lien avec les codes rapports et les echecs
void afficher_codes_rapport(int idMission, mission* m); //permet d'afficher les msg d'erreur des codes rapports
void affectation_codeRapport_echec(mission* nouvelle_mission, enum codes code, int proprio); //affecte les nouveaux codes rapports et id des entreprises qui réalisent des echecs dans les tableaux correspondants
void remplir(int* tableau, int taille, int valeur_initiale); //remplis les tableaux des codes rapports et des echecs à la valeur "vide" : -1 (qui signifie qu'il n'y a ni code rapport ni echec)
//fonctions qui permettent d'afficher les differents recapitulatifs
void recap_non_attribuees(int id_entreprise, mission* m, int* nbMission, inscription* inscrit);
void recap_attribuees(int id_entreprise, missionAttribuee* missionAttribuee, int* nbMissionionAttribuee, inscription* inscrit);
void recap_terminees(int id_entreprise, missionTerminee* missionTerminee, int* nbMissionionTerminee, inscription* inscrit);
void recap_a_realiser(int id_entreprise, missionAttribuee* missionAttribuee, int* nbMissionionAttribuee, inscription* inscrit);
void recap_realisees(int id_entreprise, missionTerminee* missionTerminee, int* nbMissionionTerminee, inscription* inscrit);


//fonction pour arreter le programme
int exit(char* input) {
    return 0;
}

//fonction pour inscrire une entreprise
void inscr(char* input, inscription* inscrit, int* nbInscrit) {
    char nom[LONG_MAX];
    char roles[LONG_MAX];
    //on utilise sscanf pour lire la chaine de caractere "input" obtenu avec la fonction fgets
    (void)sscanf(input, "%*s %s %s\n", roles, nom); //(void) devant la fonction afin d'eviter l'avertissement "return value ignored"

    if (strcmp(roles, "OP") != 0 && strcmp(roles, "AG") != 0 && strcmp(roles, "IN") != 0) { //si le role est different de OP AG ou IN, msg d'erreur
        printf("Role incorrect\n");
        return;
    }

    if (nomConnu(nom, inscrit, *nbInscrit))//si le nom existe deja, msg d'erreur
        printf("Nom incorrect\n");
    else { //sinon copie des informations renseignees dans le tableau des entreprises inscrites
        strcpy(inscrit[*nbInscrit].nom, nom);
        strcpy(inscrit[*nbInscrit].roles, roles);
        inscrit[*nbInscrit].id = *nbInscrit + 1;

        printf("Inscription realisee (%d)\n", *nbInscrit + 1);
        ++(*nbInscrit); //incrementation du nombre d'inscrit
    }
}

//fonction pour publier des missions
void miss(char* input, mission* m, int* nbMission, inscription* inscrit, int* nbInscrit, int* nbMissionTotal) {
    int idOp, nbSousTraitement = 0, longueur = 0;
    char nom[LONG_MAX] = "";
    float remu;
    (void)sscanf(input, "%*s %d %s %f\n", &idOp, nom, &remu);

    int idExiste = entrepriseExiste(idOp, inscrit, nbInscrit); //verifie si l'entreprise existe, si non renvoie -1
    if ((idExiste == -1) || (strcmp(inscrit[idExiste].roles, "OP") != 0)) { //si elle n'existe pas ou qu'elle n'a pas le role OP, msg d'erreur
        printf("Identifiant incorrect\n");
        return;
    }

    if (remu <= 0) { //si la remuneration est inferieure ou egale a zero, msg d'erreur
        printf("Remuneration incorrecte\n");
        return;
    }
    else {
        printf("Mission publiee (%d)\n", *nbMissionTotal + 1); //sinon msg de publication

        //copie des informations de la mission dans le tableau des missions
        m[*nbMission].idMission = *nbMissionTotal + 1;
        strcpy(m[*nbMission].nomMission, nom);
        m[*nbMission].remu = remu;
        m[*nbMission].idAuteur = idOp;
        m[*nbMission].nbSousTraitement = nbSousTraitement;
        //remplis le tableau des codes rapports et des echecs par -1 (ce qui signifie qu'il n'y a actuellement pas de rapports ni d'echecs)
        int vide = -1;
        remplir(&m[*nbMission].codeRapport, NB_MAX_CODES, vide);
        remplir(&m[*nbMission].echec, MAX_INSCR, vide);

        ++(*nbMissionTotal); //incrementation du nombre total de mission
        ++(*nbMission); //incrementation du nombre des missions dans le tableau mission m
    }
}


//fonction pour consulter toutes les missions non attribuees
void consult(mission* m, int* nbMission, inscription* inscrit, int* nbInscrit) {
    //premiere boucle qui parcourt nbMission, deuxieme qui parcourt nbInscrit et lorsque m[i].idAuteur == inscrit[j].id, cad lorsque l'idAuteur de la mission (son id) dans le tableau m
    //est egal a l'id qui se trouve dans le tableau inscrit, afficher la mission (cette double boucle permet d'afficher le nom de l'idAuteur de la mission (puisque le nom de l'idAuteur 
    //ne se trouve pas dans le tableau des missions))
    if (*nbMission > 0) {
        for (int i = 0; i < *nbMission; ++i) {
            for (int j = 0; j < *nbInscrit; ++j) {
                if (m[i].idAuteur == inscrit[j].id) {
                    printf("%d %s %s %.2f (%d)\n", m[i].idMission, m[i].nomMission, inscrit[j].nom, m[i].remu, m[i].nbSousTraitement);
                }
            }
        }
    }
    else
        printf("Aucune mission disponible\n");
}

//fonction pour consulter les details d'une mission non attribuee
void detail(char* input, mission* m, int* nbMission, inscription* inscrit, int* nbInscrit) {
    int detailIdMission = 0, idMission = 0;
    (void)sscanf(input, "%*s %d", &detailIdMission);

    int id_existe = missionExiste(detailIdMission, m, nbMission); //fonction qui verifie si une mission existe, si elle n'existe pas elle renvoie la valeur -1
    if (id_existe == -1) {
        printf("Identifiant incorrect\n");
        return;
    }

    for (int i = 0; i < *nbMission; ++i) { //double boucle qui a le meme fonctionnement que dans la fonction ci dessus
        for (int j = 0; j < *nbInscrit; ++j)
            if ((m[i].idMission == detailIdMission) && (m[i].idAuteur == inscrit[j].id)) {
                printf("%d %s %s %.2f (%d)\n", m[i].idMission, m[i].nomMission, inscrit[j].nom, m[i].remu, m[i].nbSousTraitement);
            }
    }
    afficher_codes_rapport(idMission, m); //fonction qui affiche les messages des codes rapports (s'il y en a)
}


//fonction pour qu'une entreprise puisse accepter une mission
void acceptation(char* input, inscription* inscrit, int* nbInscrit, mission* m, int* nbMission, missionAttribuee* missionAttribuee, int* nbMissionionAttribuee) {
    int idEntreprise, idMissionNonAttribuee;
    (void)sscanf(input, "%*s %d %d", &idEntreprise, &idMissionNonAttribuee);

    int idExiste = entrepriseExiste(idEntreprise, inscrit, nbInscrit); //verifie si une entreprise existe, renvoie -1 si n'existe pas

    if (idExiste == -1 || strcmp(inscrit[idExiste].roles, "OP") == 0) { //si -1 ou si c'est un OP, msg d'erreur
        printf("Entreprise incorrecte\n");
        return;
    }
    else { //sinon verifie si la mission existe
        int missionNonAttribuee = -1;
        for (int i = 0; i < *nbMission; i++)
            if (m[i].idMission == idMissionNonAttribuee) {
                missionNonAttribuee = i; //si elle existe la variable prend la valeur i
                break;
            }

        for (int j = 0; j < MAX_INSCR; ++j) //si l'entreprise est responsable d'un echec de la mission, msg d'erreur
            if (m[missionNonAttribuee].echec[j] == idEntreprise) {
                printf("Entreprise incorrecte\n");
                return;
            }

        if (missionNonAttribuee != -1) { //sinon l'acceptation est enregistree
            printf("Acceptation enregistree\n");

            //remplis les codes rapports et echecs par la valeur "vide"
            int vide = -1;
            remplir(&missionAttribuee[*nbMissionionAttribuee].missionAttribuee.codeRapport, NB_MAX_CODES, vide);
            remplir(&missionAttribuee[*nbMissionionAttribuee].missionAttribuee.echec, MAX_INSCR, vide);

            copie_m_to_atrb(missionNonAttribuee, missionAttribuee, nbMissionionAttribuee, m, nbMission); //fonction qui permet de copier les donnees du tableau mission au tableau des missions attribuees
            missionAttribuee[*nbMissionionAttribuee].proprio = idEntreprise; //assigne cette mission a l'entreprise correspondante

            ++(*nbMissionionAttribuee);
            m[missionNonAttribuee] = m[*nbMission - 1]; //suppression de la mission du tableau des missions non attribuees (puisqu'elle est maintenant attribuee)
            (*nbMission)--; //decrementation du nombre de mission dans la tableau des missions non attribuees
            return;
        }
        else {
            printf("Mission incorrecte\n");
            return;
        }
    }
}

//fonction de sous-traitance des missions
void soustraitance(char* input, inscription* inscrit, int* nbInscrit, mission* m, int* nbMission, missionAttribuee* missionAttribuee, int* nbMissionionAttribuee, int* nbMissionTotal) {
    int idEntreprise, idMissionion;
    float remu;
    (void)sscanf(input, "%*s %d %d %f\n", &idEntreprise, &idMissionion, &remu);

    int idExiste = entrepriseExiste(idEntreprise, inscrit, nbInscrit); //verifie si l'entreprise existe

    //si elle n'existe pas ou si elle n'a pas le role AG, msg d'erreur
    if (idExiste == -1 || strcmp(inscrit[idExiste].roles, "AG") != 0) {
        printf("Entreprise incorrecte\n");
        return;
    }

    //verifie si une mission existe
    int missionNonAttribuee = missionExiste(idMissionion, m, nbMission);
    if ((missionNonAttribuee == -1) || (m[missionNonAttribuee].nbSousTraitement >= SS_TRAIT_MAX)) {
        printf("Mission incorrecte\n");
        return;
    }
    else if (remu <= 0) { //si la remuneration est inferieure ou egale a 0, msg d'erreur
        printf("Remuneration incorrecte\n");
        return;
    }
    else {
        //creation de la nouvelle mission
        mission nouvelleMission;

        //copie des informations de la nouvelle mission
        nouvelleMission.idMission = *nbMissionTotal + 1;
        strcpy(nouvelleMission.nomMission, m[missionNonAttribuee].nomMission);
        nouvelleMission.remu = remu;
        nouvelleMission.idAuteur = idEntreprise;
        nouvelleMission.nbSousTraitement = m[missionNonAttribuee].nbSousTraitement + 1;

        //remplissage des tableaux codes rapports et echecs par la valeur "vide" (puisque c'est une nouvelle mission, il est normal que cela soit vide)
        int vide = -1;
        remplir(&nouvelleMission.codeRapport, NB_MAX_CODES, vide);
        remplir(&nouvelleMission.echec, MAX_INSCR, vide);

        m[*nbMission] = nouvelleMission; //ajout de la nouvelle mission au tableau des missions m

        printf("Sous-traitance enregistree (%d)\n", *nbMissionTotal + 1);
        ++(*nbMissionTotal);
        ++(*nbMission);
        ++(*nbMissionionAttribuee); //incrementation du nombre de missions dans le tableau des missions attribuees 

        //copie de la mission d'origine dans le tableau des missions attribuees
        copie_m_to_atrb(missionNonAttribuee, missionAttribuee, nbMissionionAttribuee, m, nbMission);
        m[missionNonAttribuee] = m[*nbMission - 1]; //suppression de la mission du tableau des missions non attribuees
        --(*nbMission);
    }
}


//fonction pour realiser le rapport d'une mission
void rapport(char* input, mission* m, int* nbMission, missionAttribuee* missionAttribuee, int* nbMissionionAttribuee, missionTerminee* missionTerminee, int* nbMissionionTerminee, int* nbMissionTotal) {
    int idMissionion_atrb, nbSousTraitement = 0;
    enum codes code;
    double majoration2 = 5.5 / 100, majoration3 = 4.0 / 100;
    (void)sscanf(input, "%*s %d %d", &idMissionion_atrb, &code);

    int idMissionExiste = missionAttribueeExiste(idMissionion_atrb, missionAttribuee, nbMissionionAttribuee); //verifie si la mission attribuee existe
    
    if (idMissionExiste == -1) { //si elle n'existe pas, msg d'erreur
        printf("Mission incorrecte\n");
        return;
    }

    for (int j = 0; j < *nbMissionionAttribuee; ++j) //verifie si la mission est bien la derniere de la chaine de sous-traitement
        if (missionAttribuee[idMissionExiste].missionAttribuee.nbSousTraitement < missionAttribuee[j].missionAttribuee.nbSousTraitement) {
            printf("Mission incorrecte\n");
            return;
        }

    if (code < ZERO || code > TROIS) { //si le code est inferieur strict ou superieur strict a 0 et 3, msg d'erreur
        printf("Code de retour incorrect\n");
        return;
    }
    else {
        //copie de la mission dans le tableau des missions terminees 
        copie_atrb_to_trmn(idMissionExiste, missionAttribuee, nbMissionionAttribuee, missionTerminee, nbMissionionTerminee);

        ++(*nbMissionionTerminee);
        missionAttribuee[idMissionExiste] = missionAttribuee[*nbMissionionAttribuee - 1]; //suppression de la mission du tableau des missions attribuees
        --(*nbMissionionAttribuee);
    }

    //code rapport
    if (code == ZERO)
        printf("Rapport enregistre\n");
    else { //creation d'une nouvelle mission si le code est autre que 0
        mission nouvelleMission;
        int vide = -1;

        //remplis le tableau des codes rapports par -1 (la valeur qui represente le vide)
        remplir(&nouvelleMission.codeRapport, NB_MAX_CODES, vide);
        remplir(&nouvelleMission.echec, MAX_INSCR, vide);

        //renseigne les informations de la nouvelle mission
        nouvelleMission.idMission = *nbMissionTotal + 1;
        strcpy(nouvelleMission.nomMission, missionAttribuee[idMissionExiste].missionAttribuee.nomMission);
        nouvelleMission.idAuteur = missionAttribuee[idMissionExiste].missionAttribuee.idAuteur;
        nouvelleMission.nbSousTraitement = nbSousTraitement;

        for (int i = 0; i < NB_MAX_CODES; ++i) //copie des anciens code rapport (s'il y en a)
            if ((nouvelleMission.codeRapport[i] == vide) && ((missionAttribuee[idMissionExiste].missionAttribuee.codeRapport[i] == UN) || (missionAttribuee[idMissionExiste].missionAttribuee.codeRapport[i] == DEUX) || (missionAttribuee[idMissionExiste].missionAttribuee.codeRapport[i] == TROIS)))
                nouvelleMission.codeRapport[i] = missionAttribuee[idMissionExiste].missionAttribuee.codeRapport[i];
        for (int j = 0; j < MAX_INSCR; ++j) //copie des id des entreprises qui ont realise des echecs (s'il y en a)
            if (nouvelleMission.echec[j] == vide)
                nouvelleMission.echec[j] = missionAttribuee[idMissionExiste].missionAttribuee.echec[j];

        //calcul des nouvelles remunerations et affecte les nouveaux codes rapports et id des entreprises qui realisent des echecs dans les tableaux correspondants
        if (code == UN) {
            nouvelleMission.remu = missionAttribuee[idMissionExiste].missionAttribuee.remu;
            affectation_codeRapport_echec(&nouvelleMission, code, missionAttribuee[idMissionExiste].proprio);
            printf("Rapport enregistre (%d)\n", *nbMissionTotal + 1);
        }
        if (code == DEUX) {
            nouvelleMission.remu = (missionAttribuee[idMissionExiste].missionAttribuee.remu) + (majoration2 * (missionAttribuee[idMissionExiste].missionAttribuee.remu));
            affectation_codeRapport_echec(&nouvelleMission, code, missionAttribuee[idMissionExiste].proprio);
            printf("Rapport enregistre (%d)\n", *nbMissionTotal + 1);
        }
        if (code == TROIS) {
            nouvelleMission.remu = (missionAttribuee[idMissionExiste].missionAttribuee.remu) + (majoration3 * (missionAttribuee[idMissionExiste].missionAttribuee.remu));
            affectation_codeRapport_echec(&nouvelleMission, code, missionAttribuee[idMissionExiste].proprio);
            printf("Rapport enregistre (%d)\n", *nbMissionTotal + 1);
        }

        //ajoute la nouvelle mission au tableau des missions non attribuees (m)
        m[*nbMission] = nouvelleMission;
        ++(*nbMissionTotal);
        ++(*nbMission);
    }
}


//fonction pour avoir un recapitulatif des entreprises
void recap(char* input, inscription* inscrit, int* nbInscrit, mission* m, int* nbMission, missionAttribuee* missionAttribuee, int* nbMissionionAttribuee, missionTerminee* missionTerminee, int* nbMissionionTerminee, int* nbMissionTotal) {
    int idEntreprise;
    (void)sscanf(input, "%*s %d", &idEntreprise);

    int idEntrepriseExiste = entrepriseExiste(idEntreprise, inscrit, nbInscrit);
    
    if (idEntrepriseExiste == -1) { //si idEntrepriseExiste == -1 (donc n'existe pas), msg d'erreur
        printf("Entreprise incorrecte\n");
        return;
    }

    //affiche les differents recapitulatifs
    recap_non_attribuees(idEntreprise, m, nbMission, inscrit);
    recap_attribuees(idEntreprise, missionAttribuee, nbMissionionAttribuee, inscrit);
    recap_a_realiser(idEntreprise, missionAttribuee, nbMissionionAttribuee, inscrit);
    recap_terminees(idEntreprise, missionTerminee, nbMissionionTerminee, inscrit);
    recap_realisees(idEntreprise, missionTerminee, nbMissionionTerminee, inscrit);
}

int main() {
    //attribution de la valeur 100 a INPUT_MAX
    //definition de la variable "input" (qui va servir a recuperer ce qu'entre l'utilisateur) a la taille maximum de 100 caract?res
    //et de la variable "action" (qui permettra de savoir quelle fonction executer) aussi a la taille maximale de 100 caract?res
    enum { INPUT_MAX = 100 };
    char input[INPUT_MAX];
    char action[INPUT_MAX] = "";
    int nbMissionTotal = 0, nbInscrit = 0, nbMission = 0, nbMissionionAttribuee = 0, nbMissionionTerminee = 0; //nombre de missions au total qui sera aussi l'id des missions 
    inscription inscrit[MAX_INSCR];
    mission m[MISSION_MAX];
    missionAttribuee missionAttribuee[MISSION_MAX];
    missionTerminee missionTerminee[MISSION_MAX];

    //faire une boucle tant que le premier mot saisit par l'utilisateur n'est pas "exit"
    while (strcmp(action, "exit") != 0) {
        fgets(input, sizeof(input), stdin); //prendre la chaine de caract?re saisie par l'utilisateur
        (void)sscanf(input, "%s", action); //prendre le premier mot de la chaine de caract?re saisie par l'utilisateur pour la mettre dans la variable "action"

        //si action = a un des mots ci-dessous, appelle la fonction correspondante
        if (strcmp(action, "inscription") == 0)
            inscr(input, inscrit, &nbInscrit);
        else if (strcmp(action, "mission") == 0)
            miss(input, m, &nbMission, inscrit, &nbInscrit, &nbMissionTotal);
        else if (strcmp(action, "consultation") == 0)
            consult(m, &nbMission, inscrit, &nbInscrit);
        else if (strcmp(action, "detail") == 0)
            detail(input, m, &nbMission, inscrit, &nbInscrit);
        else if (strcmp(action, "acceptation") == 0)
            acceptation(input, inscrit, &nbInscrit, m, &nbMission, missionAttribuee, &nbMissionionAttribuee);
        else if (strcmp(action, "sous-traitance") == 0)
            soustraitance(input, inscrit, &nbInscrit, m, &nbMission, missionAttribuee, &nbMissionionAttribuee, &nbMissionTotal);
        else if (strcmp(action, "rapport") == 0)
            rapport(input, m, &nbMission, missionAttribuee, &nbMissionionAttribuee, missionTerminee, &nbMissionionTerminee, &nbMissionTotal);
        else if (strcmp(action, "recapitulatif") == 0)
            recap(input, inscrit, &nbInscrit, m, &nbMission, missionAttribuee, &nbMissionionAttribuee, missionTerminee, &nbMissionionTerminee, &nbMissionTotal);
    }
    //si action = "exit", arrete le programme
    if (strcmp(action, "exit") == 0)
        exit(input);
}


//definition des fonctions annexes
//fonction qui permet de verifier si un nom d'entreprise est deja connu ou non (pour la fonction inscr)
int nomConnu(const char* nom, const inscription* inscrit, int nbInscrit) {
    for (int i = 0; i < nbInscrit; i++)
        if (strcmp(nom, inscrit[i].nom) == 0)
            return 1;  //le nom est deja connu

    return 0;
}

//fonction qui permet de verifier si l'id d'une mission existe (utilise dans la fonction detail), si oui renvoie i si non -1
int missionExiste(int id, mission* m, int* nbMission) {
    for (int i = 0; i < *nbMission; i++) {
        if (id == m[i].idMission)
            return i;
    }
    return -1;
}

//verifie si une entreprise existe, si oui renvoie i sinon -1
int entrepriseExiste(int id, const inscription* inscrit, int* nbInscrit) {
    for (int i = 0; i < *nbInscrit; ++i) {
        if (id == inscrit[i].id)
            return i;
    }
    return -1;
}

//verifie si une mission attribuee existe ou non, si oui renvoie i si non -1
int missionAttribueeExiste(int id, missionAttribuee* missionAttribuee, int* nbMissionionAttribuee) {
    for (int i = 0; i < *nbMissionionAttribuee; ++i)
        if (id == missionAttribuee[i].missionAttribuee.idMission) {
            return i;
        }
    return -1;
}

//copie des missions non attribuees dans le tableau des missions attribuees
void copie_m_to_atrb(int id, missionAttribuee* missionAttribuee, int* nbMissionionAttribuee, mission* m, int* nbMission) {
    int vide = -1;

    missionAttribuee[*nbMissionionAttribuee].missionAttribuee.idMission = m[id].idMission;
    strcpy(missionAttribuee[*nbMissionionAttribuee].missionAttribuee.nomMission, m[id].nomMission);
    missionAttribuee[*nbMissionionAttribuee].missionAttribuee.remu = m[id].remu;
    missionAttribuee[*nbMissionionAttribuee].missionAttribuee.idAuteur = m[id].idAuteur;
    missionAttribuee[*nbMissionionAttribuee].missionAttribuee.nbSousTraitement = m[id].nbSousTraitement;

    for (int i = 0; i < NB_MAX_CODES; ++i)
        if (missionAttribuee[*nbMissionionAttribuee].missionAttribuee.codeRapport[i] == vide)
            missionAttribuee[*nbMissionionAttribuee].missionAttribuee.codeRapport[i] = m[id].codeRapport[i];
    for (int j = 0; j < MAX_INSCR; ++j)
        if ((missionAttribuee[*nbMissionionAttribuee].missionAttribuee.echec[j] == vide) && (m[id].echec != vide))
            missionAttribuee[*nbMissionionAttribuee].missionAttribuee.echec[j] = m[id].echec[j];
}

//copie des missions attribuees dans le tableau des missions terminees
void copie_atrb_to_trmn(int id, missionAttribuee* missionAttribuee, int* nbMissionionAttribuee, missionTerminee* missionTerminee, int* nbMissionionTerminee) {
    for (int i = 0; i < *nbMissionionAttribuee; ++i)
        if (missionAttribuee[i].missionAttribuee.nomMission == missionAttribuee[id].missionAttribuee.nomMission) {
            missionTerminee[*nbMissionionTerminee].missionTerminee.idMission = missionAttribuee[i].missionAttribuee.idMission;
            strcpy(missionTerminee[*nbMissionionTerminee].missionTerminee.nomMission, missionAttribuee[i].missionAttribuee.nomMission);
            missionTerminee[*nbMissionionTerminee].missionTerminee.idAuteur = missionAttribuee[i].missionAttribuee.idAuteur;
            missionTerminee[*nbMissionionTerminee].missionTerminee.remu = missionAttribuee[i].missionAttribuee.remu;
            missionTerminee[*nbMissionionTerminee].missionTerminee.nbSousTraitement = missionAttribuee[i].missionAttribuee.nbSousTraitement;
            missionTerminee[*nbMissionionTerminee].proprio = missionAttribuee[i].proprio;
        }
}

//permet d'afficher les msg des codes rapport pour la fonction detail
void afficher_codes_rapport(int idMission, mission* m) {
    for (int k = DEUX; k >= ZERO; --k) {
        if (m[idMission].codeRapport[k] == UN)
            printf("Local non accessible\n");
        else if (m[idMission].codeRapport[k] == DEUX)
            printf("Pas de signal dans le boitier general\n");
        else if (m[idMission].codeRapport[k] == TROIS)
            printf("Recepteur defectueux\n");
    }
}

//affecte les nouveaux codes rapports et ID des entreprises qui realisent des echecs dans les tableaux correspondant
void affectation_codeRapport_echec(mission* nouvelle_mission, enum codes code, int proprio) {
    for (int i = 0; i < NB_MAX_CODES; ++i)
        if (nouvelle_mission->codeRapport[i] == -1) {
            nouvelle_mission->codeRapport[i] = code;
            break;
        }
    for (int j = 0; j < MAX_INSCR; ++j)
        if (nouvelle_mission->echec[j] == -1) {
            nouvelle_mission->echec[j] = proprio;
            break;
        }
}

//sert a remplir les tableaux codes rapports et echecs de la valeur "vide" : -1
void remplir(int* tableau, int taille, int valeur_initiale) {
    for (int i = 0; i < taille; ++i)
        tableau[i] = valeur_initiale;
}

//fonctions pour les recapitulatifs :
void recap_non_attribuees(int id_entreprise, mission* m, int* nbMission, inscription* inscrit) {
    int non_attribuees = 0;
    for (int j = 0; j < *nbMission; ++j)
        if (m[j].idAuteur == id_entreprise) {
            non_attribuees = 1;
            break;
        }

    if (non_attribuees)
        printf("* non attribuees\n");
    for (int j = 0; j < *nbMission; ++j)
        if (m[j].idAuteur == id_entreprise)
            printf("%d %s %s %.2f (%d)\n", m[j].idMission, m[j].nomMission, inscrit[m[j].idAuteur - 1].nom, m[j].remu, m[j].nbSousTraitement);
}

void recap_attribuees(int id_entreprise, missionAttribuee* missionAttribuee, int* nbMissionionAttribuee, inscription* inscrit) {
    int attribuees = 0;
    for (int k = 0; k < *nbMissionionAttribuee; ++k)
        if (missionAttribuee[k].missionAttribuee.idAuteur == id_entreprise && missionAttribuee[k].proprio != id_entreprise) {
            attribuees = 1;
            break;
        }

    if (attribuees)
        printf("* attribuees\n");
    for (int k = 0; k < *nbMissionionAttribuee; ++k)
        if (missionAttribuee[k].missionAttribuee.idAuteur == id_entreprise && missionAttribuee[k].proprio != id_entreprise)
            printf("%d %s %s %.2f (%d)\n", missionAttribuee[k].missionAttribuee.idMission, missionAttribuee[k].missionAttribuee.nomMission, inscrit[missionAttribuee[k].missionAttribuee.idAuteur - 1].nom, missionAttribuee[k].missionAttribuee.remu, missionAttribuee[k].missionAttribuee.nbSousTraitement);
}

void recap_terminees(int id_entreprise, missionTerminee* missionTerminee, int* nbMissionionTerminee, inscription* inscrit) {
    int terminees = 0;
    for (int l = 0; l < *nbMissionionTerminee; ++l)
        if (missionTerminee[l].missionTerminee.idAuteur == id_entreprise) {
            terminees = 1;
            break;
        }

    if (terminees)
        printf("* terminees\n");
    for (int l = 0; l < *nbMissionionTerminee; ++l)
        if (missionTerminee[l].missionTerminee.idAuteur == id_entreprise)
            printf("%d %s %s %.2f (%d)\n", missionTerminee[l].missionTerminee.idMission, missionTerminee[l].missionTerminee.nomMission, inscrit[missionTerminee[l].missionTerminee.idAuteur - 1].nom, missionTerminee[l].missionTerminee.remu, missionTerminee[l].missionTerminee.nbSousTraitement);
}

void recap_a_realiser(int id_entreprise, missionAttribuee* missionAttribuee, int* nbMissionionAttribuee, inscription* inscrit) {
    int a_realiser = 0;
    for (int o = 0; o < *nbMissionionAttribuee; ++o)
        if (missionAttribuee[o].proprio == id_entreprise) {
            a_realiser = 1;
            break;
        }

    if (a_realiser)
        printf("* a realiser\n");
    for (int o = 0; o < *nbMissionionAttribuee; ++o)
        if (missionAttribuee[o].proprio == id_entreprise)
            printf("%d %s %s %.2f (%d)\n", missionAttribuee[o].missionAttribuee.idMission, missionAttribuee[o].missionAttribuee.nomMission, inscrit[missionAttribuee[o].missionAttribuee.idAuteur - 1].nom, missionAttribuee[o].missionAttribuee.remu, missionAttribuee[o].missionAttribuee.nbSousTraitement);
}

void recap_realisees(int id_entreprise, missionTerminee* missionTerminee, int* nbMissionionTerminee, inscription* inscrit) {
    int realisees = 0;
    for (int n = 0; n < *nbMissionionTerminee; ++n)
        if (missionTerminee[n].proprio == id_entreprise) {
            realisees = 1;
            break;
        }

    if (realisees)
        printf("* realisees\n");
    for (int n = 0; n < *nbMissionionTerminee; ++n)
        if (missionTerminee[n].proprio == id_entreprise)
            printf("%d %s %s %.2f (%d)\n", missionTerminee[n].missionTerminee.idMission, missionTerminee[n].missionTerminee.nomMission, inscrit[missionTerminee[n].missionTerminee.idAuteur - 1].nom, missionTerminee[n].missionTerminee.remu, missionTerminee[n].missionTerminee.nbSousTraitement);

}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * Projet : Carnet numérique d'adresses
 * Description : Ce programme permet de gérer un carnet d'adresses numérique en ajoutant, modifiant, supprimant et affichant des contacts.
 * Chaque contact contient un nom, un prénom, un téléphone, un email et un pays associé.
 * Le programme vérifie également la validité des numéros de téléphone et des emails.
 */

// Structure représentant un contact
typedef struct {
    char nom[50];
    char prenom[50];
    char telephone[50];
    char mail[50];
    char pays[50];  // Le pays du contact
} Contact;

// Structure représentant un pays avec son indicatif et les contraintes sur le numéro de téléphone
typedef struct {
    char pays[50];
    char indicatif[10];
    int nb_chiffres; // Nombre de chiffres spécifiques pour ce pays
} PaysIndicatif;

// Liste des pays et leurs indicatifs
PaysIndicatif indicatifs[] = {
    {"Tunisie", "+216", 8},
    {"France", "+33", 10},
    {"Allemagne", "+49", 10},
    {"Etats-Unis", "+1", 10},
    {"Royaume-Uni", "+44", 10},
    {"Espagne", "+34", 9},
    {"Italie", "+39", 10},
    {"Canada", "+1", 10},
    {"Maroc", "+212", 9},
    {"Egypte", "+20", 10},
    {"Belgique", "+32", 9},
    {"Suisse", "+41", 9},
    {"Pays-Bas", "+31", 9},
    {"Australie", "+61", 9},
    {"Inde", "+91", 10},
    {"Japon", "+81", 10},
    {"Bresil", "+55", 11},
    {"Argentine", "+54", 10}
};

#define NB_PAYS (sizeof(indicatifs) / sizeof(PaysIndicatif))

// Enlève le caractère de retour à la ligne '\n' d'une chaîne de caractères
void enleverRetourLigne(char* str) {
    str[strcspn(str, "\n")] = '\0';
}

// Alloue dynamiquement une liste de contacts
Contact* allouerListe(int taille) {
    if (taille <= 0) {
        printf("Erreur : taille invalide pour la liste ! \n");
        return NULL;
    }

    Contact* liste = (Contact*)malloc(taille * sizeof(Contact));
    if (liste == NULL) {
        printf("Erreur: Allocation impossible! \n");
        exit(1);
    }

    return liste;
}

// Libère la mémoire allouée pour la liste de contacts
void libererListe(Contact* liste) {
    if (liste != NULL) {
        free(liste);
        printf("Memoire liberee avec succes !\n");
    } else {
        printf("Aucune memoire à liberer (la liste est NULL) !\n");
    }
}

// Vérifie si un numéro de téléphone est valide pour un pays donné
int is_valid_phone(char phone[], int nb_chiffres) {
    int length = strlen(phone);
    int start = (phone[0] == '+') ? 1 : 0; // Commencer après le '+'

    // Vérifier si le numéro a le bon nombre de chiffres
    if (length != nb_chiffres + start) {
        return 0;
    }

    for (int i = start; i < length; i++) {
        if (!isdigit(phone[i])) {
            return 0;
        }
    }

    return 1;
}

// Vérifie si une adresse email est valide
int estEmailValide(char email[]) {
    int at_index = -1;
    int dot_index = -1;
    int length = strlen(email);

    for (int i = 0; i < length; i++) {
        if (email[i] == '@') {
            if (at_index == -1) {
                at_index = i;
            } else {
                return 0;
            }
        } else if (email[i] == '.') {
            dot_index = i;
        }
    }

    return at_index != -1 && dot_index != -1 && at_index != 0 && at_index != length - 1 && dot_index > at_index + 1 && dot_index != length - 1;
}

// Permet de choisir un pays pour un contact et d'ajouter l'indicatif au numéro de téléphone
void choisirPays(Contact* contact) {
    int pays_valide = 0;
    while (!pays_valide) {
        printf("Choisir un pays (par exemple, France, Tunisie, etc.) :  ");
        fgets(contact->pays, 50, stdin);
        enleverRetourLigne(contact->pays);

        // Vérifier si le pays est valide et appliquer l'indicatif correspondant
        for (int i = 0; i < NB_PAYS; i++) {
            if (strcasecmp(contact->pays, indicatifs[i].pays) == 0) {
                printf("Indicatif ajoute : %s\n", indicatifs[i].indicatif);
                strcat(contact->telephone, indicatifs[i].indicatif);
                pays_valide = 1;
                return;
            }
        }

        printf("Pays non trouve ou incorrect. Essayez encore.\n");
    }
}
/* Ajoute un nouveau contact dans la liste*/
void ajoutercontact(Contact* liste, int index) {
    printf("Entrer le nom du contact %d: ", index + 1);
    fgets(liste[index].nom, 50, stdin);
    enleverRetourLigne(liste[index].nom);

    printf("Entrer le prenom du contact %d: ", index + 1);
    fgets(liste[index].prenom, 50, stdin);
    enleverRetourLigne(liste[index].prenom);

    choisirPays(&liste[index]);

    int numero_valide = 0;
    while (!numero_valide) {
        printf("Entrer le telephone du contact %d (sans indicatif) ", index + 1);
        fgets(liste[index].telephone, 50, stdin);
        enleverRetourLigne(liste[index].telephone);

        // Vérification du numéro
        for (int i = 0; i < NB_PAYS; i++) {
            if (strcasecmp(liste[index].pays, indicatifs[i].pays) == 0) {
                if (is_valid_phone(liste[index].telephone, indicatifs[i].nb_chiffres)) {
                    numero_valide = 1;
                    break;
                } else {
                    printf("Erreur : Le numero de telephone ne correspond pas au format du pays choisi. Essayez encore.\n");
                    break;
                }
            }
        }
    }

    int valideEmail = 0;
    while (!valideEmail) {
        printf("Entrer l'email du contact %d: ", index + 1);
        fgets(liste[index].mail, 50, stdin);
        enleverRetourLigne(liste[index].mail);
        valideEmail = estEmailValide(liste[index].mail);
        if (!valideEmail) {
            printf("Email invalide, veuillez entrer un email valide (ex: example@domain.com).\n");
        }
    }
}

// Affiche tous les contacts dans la liste
void affichercontacts(Contact* liste, int taille) {
    if (taille==0) {
        printf("Aucun contact a afficher !\n");
        return;
    }

    for (int i = 0; i < taille; i++) {
        printf("\n*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*\n");
        printf("Contact %d:\n", i + 1);
        printf("Nom: %s\n", liste[i].nom);
        printf("Prenom: %s\n", liste[i].prenom);
        printf("Telephone: %s\n", liste[i].telephone);
        printf("Email: %s\n", liste[i].mail);
        printf("Pays: %s\n", liste[i].pays);
    }
}

//supprime un contact
void supprimercontact(Contact* liste, int* taille, int index) {
    if (taille==0) {
        printf("Aucun contact a supprimer !\n");
        return;
    }

    if (index < 0 || index >= *taille) {
        printf("Indice invalide ! \n");
        return;
    }

    for (int i = index; i < *taille - 1; i++) {
        liste[i] = liste[i + 1];
    }

    (*taille)--;

    liste = (Contact*)realloc(liste, (*taille) * sizeof(Contact));
    if (liste == NULL && *taille > 0) {
        printf("Erreur de reallocation de memoire.\n");
        exit(1);
    }

    printf("Contact supprime avec succes !\n");
}

//permet de modifier un contact
void modifierContact(Contact* liste, int taille, int index) {
    if (taille==0) {
        printf("Aucun contact a modifier !\n");
        return;
    }

    if (index < 0 || index >= taille) {
        printf("Index invalide !\n");
        return;
    }

    printf("Entrer le nouveau nom du contact %d: ", index + 1);
    fgets(liste[index].nom, 50, stdin);
    enleverRetourLigne(liste[index].nom);

    printf("Entrer le nouveau prenom du contact %d: ", index + 1);
    fgets(liste[index].prenom, 50, stdin);
    enleverRetourLigne(liste[index].prenom);

    printf("Entrer le nouveau telephone du contact %d: ", index + 1);
    fgets(liste[index].telephone, 50, stdin);
    enleverRetourLigne(liste[index].telephone);

    printf("Entrer le nouveau email du contact %d: ", index + 1);
    fgets(liste[index].mail, 50, stdin);
    enleverRetourLigne(liste[index].mail);

    choisirPays(&liste[index]);

    // Validation du numéro de téléphone
    for (int i = 0; i < NB_PAYS; i++) {
        if (strcasecmp(liste[index].pays, indicatifs[i].pays) == 0) {
            if (!is_valid_phone(liste[index].telephone, indicatifs[i].nb_chiffres)) {
                printf("Erreur : Le numéro de telephone ne correspond pas au format du pays choisi.\n");
                return;
            }
        }
    }

    printf("Contact modifie avec succes !\n");
}

//programme principal

int main() {
    int taille = 0;
    int choix, index;
    Contact* contacts = allouerListe(10);

    printf("            ********* MON CARNET D'ADRESSES********* \n\n\n");
    // Affichage du nombre de contacts au début
    printf("Nombre initial de contacts : %d\n", taille);

    while (1) {

        printf("\n1. Ajouter un contact\n2. Modifier un contact\n3. Supprimer un contact\n4. Afficher les contacts\n5. Quitter\n");
        printf("Votre choix: ");
        scanf("%d", &choix);
        getchar();  // pour consommer le '\n' restant

        switch (choix) {
            case 1:
                ajoutercontact(contacts, taille);
                taille++;
                printf("\n");
                break;
            case 2:
                if (taille>0){
                        printf("Entrer l'indice du contact a modifier: ");
                        scanf("%d", &index);
                        getchar();
                        modifierContact(contacts, taille, index - 1);  // l'indice est ajusté pour commencer à 0
                        printf("\n");
                        break;}
                else {
                        printf("aucun contact a modifier ! \n");
                        printf("\n");
                        break;
                }
            case 3:
                if (taille>0) {
                        printf("Entrer l'indice du contact à supprimer: ");
                        scanf("%d", &index);
                        getchar();
                        supprimercontact(contacts, &taille, index - 1);
                        printf("\n");
                        break;}
                else {
                        printf("Aucun contact a supprimer !\n");
                        printf("\n");
                        break;
                }
            case 4:
                affichercontacts(contacts, taille);
                printf("\n");
                break;
            case 5:
                libererListe(contacts);
                printf("\n");
                return 0;
            default:
                printf("Choix invalide. Essayez encore.\n");
        }

        // Affichage du nombre de contacts après chaque action
        printf("Nombre actuel de contacts : %d\n", taille);
    }

    return 0;
}
//fin

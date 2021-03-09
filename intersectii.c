#include "intersectii.h"


// functie ajutatoare
void afiseaza_lista_intervale(ListaIntervale2D *lista) {
    int i;
    for (i = 0; i < lista->dimensiune; i++) {
        Interval2D *interv = lista->intervale[i];
        printf("{punct stanga x: %d, punct stanga y: %d, punct dreapta x: %d, punct dreapta y: %d}\n",
                    interv->punct_stanga->x, interv->punct_stanga->y,
                    interv->punct_dreapta->x, interv->punct_dreapta->y);
    }
}


ListaIntervale2D* citeste_intrare(char *nume_fisier_intrare) {
    // TODO citeste intervalele din fisierul dat ca parametru
    // si le salveaza in lista de intervale

    FILE *file = fopen(nume_fisier_intrare, "r");
    if(!file)
        return NULL;

    //citesc primul numar reprezentat de numarul total de segmente
    int nr_segments;
    fscanf(file, "%d", &nr_segments);

    //alocare
    ListaIntervale2D *lista = malloc(sizeof(ListaIntervale2D));
    if(!lista)
        return NULL;
    lista->dimensiune = nr_segments;

    lista->intervale = malloc(sizeof(Interval2D *) * nr_segments);
    if(!lista->intervale)
        return NULL;

    for(int i = 0; i < nr_segments; i++) {
        lista->intervale[i] = malloc(sizeof(Interval2D));
        if(!lista->intervale[i])
            return NULL;

        lista->intervale[i]->punct_dreapta = malloc(sizeof(Punct2D));
        lista->intervale[i]->punct_stanga = malloc(sizeof(Punct2D));
        if(!lista->intervale[i]->punct_dreapta ||
        !lista->intervale[i]->punct_stanga)
            return NULL;
    }

    //citire in lista
    for(int i = 0; i < nr_segments; i++) {

        int x1, y1, x2, y2;
        fscanf(file, "%d %d %d %d", &x1, &y1, &x2, &y2);

        lista->intervale[i]->punct_stanga->x = x1;
        lista->intervale[i]->punct_stanga->y = y1;
        lista->intervale[i]->punct_dreapta->x = x2;
        lista->intervale[i]->punct_dreapta->y = y2;
    }
    fclose(file);
    return lista;
}

// ! Functie pentru actualizarea in arbore
// ! O veti da ca parametru cand initializati arborele
void actualizare_cu_delta(Nod *nod, int v2) {
    nod->info += v2;
}

// ! Functie pentru combinarea raspunsurilor in arbore
// ! O veti da ca parametru cand initializati arborele
int suma_raspunsurilor(int r1, int r2) {
    return r1 + r2;
}

ListaIntervale2D_aux* conversie(ListaIntervale2D *lista) {

    ListaIntervale2D_aux *lista_aux = malloc(sizeof(ListaIntervale2D_aux));
    if(!lista_aux)
        return NULL;
    
    lista_aux->dimensiune = lista->dimensiune;
    lista_aux->intervale = malloc(sizeof(Interval2D_aux *) * lista_aux->dimensiune);
    for(int i = 0; i < lista_aux->dimensiune; i++) {

        lista_aux->intervale[i] = malloc(sizeof(Interval2D_aux));
        if(!lista_aux->intervale[i])
            return NULL;

        lista_aux->intervale[i]->punct_stanga = malloc(sizeof(Punct2D_aux));
        lista_aux->intervale[i]->punct_dreapta = malloc(sizeof(Punct2D_aux));
        if(!lista_aux->intervale[i]->punct_stanga ||
        !lista_aux->intervale[i]->punct_dreapta)
            return NULL;

        lista_aux->intervale[i]->punct_stanga->x = lista->intervale[i]->punct_stanga->x;
        lista_aux->intervale[i]->punct_dreapta->x = lista->intervale[i]->punct_dreapta->x;
        lista_aux->intervale[i]->punct_stanga->y = lista->intervale[i]->punct_stanga->y;
        lista_aux->intervale[i]->punct_dreapta->y = lista->intervale[i]->punct_dreapta->y;
    }

    //fiecarui punct ii asociez intervalul sau
    for(int i = 0; i < lista_aux->dimensiune; i++)
        lista_aux->intervale[i]->punct_dreapta->idx = lista_aux->intervale[i]->punct_stanga->idx = i;

    return lista_aux;
}

int calculeaza_numar_intersectii(ListaIntervale2D *lista) {
    // TODO calculati numarul de intersectii folosind arbori de intervale conform enuntului

    //aflu y maxim din lista
    int y_max = -1;
    for(int i = 0; i < lista->dimensiune; i ++) {

        if(lista->intervale[i]->punct_dreapta->y > y_max)
            y_max = lista->intervale[i]->punct_dreapta->y;
    }

    //initializez arborele de intervale
    ArboreDeIntervale *arbore = construieste_arbore(0, y_max, 0, actualizare_cu_delta, suma_raspunsurilor);

    //nu mai folosesc lista si copiez tot din ea in lista_aux
    ListaIntervale2D_aux *lista_aux = conversie(lista);

    //adaug toate punctele intr-un vector de puncte
    Punct2D_aux **puncte = malloc(sizeof(Punct2D_aux *) * 2 * lista_aux->dimensiune);
    if(!puncte)
        return 0;

    for(int i = 0; i < 2 * lista_aux->dimensiune; i++) {

        puncte[i] = malloc(sizeof(Punct2D_aux));
        if(!puncte[i])
            return 0;
    }

    int j = 0;
    //adaug tot din lista_aux in puncte
    for(int i = 0; i < 2 * lista_aux->dimensiune; i += 2) {

        puncte[i] = lista_aux->intervale[j]->punct_stanga;
        puncte[i+1] = lista_aux->intervale[j]->punct_dreapta;
        j++;
    }

    //sortez punctele in functie de x
    Punct2D_aux *aux;
    for(int i = 0; i < 2 * lista_aux->dimensiune; i++) {
        for(int j = 0; j < (2 * lista_aux->dimensiune) - i - 1; j++) {

            if(puncte[j]->x > puncte[j+1]->x) {

                aux = puncte[j];
                puncte[j] = puncte[j+1];
                puncte[j+1] = aux;
            } 
        }
    }

    //vector de frecvente pt intervale
    int *frecvente = calloc(lista_aux->dimensiune, sizeof(int));
    if(!frecvente)
        return 0;

    Interval *interval = malloc(sizeof(Interval));
    if(!interval)
        return 0;

    int intersectii = 0;
    for(int i = 0; i < 2 * lista_aux->dimensiune; i++) {

        interval->capat_dreapta = interval->capat_stanga = puncte[i]->y;

        //conditie de oprire: daca a ajuns la ultimul element, doar sterge
        if(i == 2 * lista_aux->dimensiune - 1) {

            actualizare_interval_in_arbore(arbore, interval, -1);
            frecvente[puncte[i]->idx] = 0;
        }
        else {
            //daca e segment orizontal
            if(puncte[i]->idx != puncte[i+1]->idx) {

                //verific in vectorul de frecvente
                //daca nu exista, il adaug
                if(frecvente[puncte[i]->idx] == 0) {

                    actualizare_interval_in_arbore(arbore, interval, 1);
                    frecvente[puncte[i]->idx] = 1;
                }
                //daca exista, il sterg
                else if(frecvente[puncte[i]->idx] == 1) {

                    actualizare_interval_in_arbore(arbore, interval, -1);
                    frecvente[puncte[i]->idx] = 0;
                }
            }
            //daca e segment vertical
            else if(puncte[i]->idx == puncte[i+1]->idx) { 

                interval->capat_stanga = puncte[i]->y;
                interval->capat_dreapta = puncte[i+1]->y;
                i++;
                intersectii += interogare_interval_in_arbore(arbore, interval);
            }
        }
    }
    free(interval);
    free(frecvente);
    for(int i = 0; i < lista->dimensiune * 2; i++)
        free(puncte[i]);
    free(puncte);
    return intersectii;
}

int calculeaza_numar_intersectii_trivial(ListaIntervale2D *lista) {
    // TODO: solutia triviala, verific fiecare segment cu fiecare segment daca se intersecteaza 

    int intersectii = 0;

    for(int i = 0; i < lista->dimensiune; i++) {
        
        Interval2D *intI = lista->intervale[i];

        for(int j = i+1; j < lista->dimensiune; j++) {

            Interval2D *intJ = lista->intervale[j];

            //verific pe intervale daca fiecare sunt incluse in celalalt sau invers
            if(
            (intJ->punct_stanga->x >= intI->punct_stanga->x) &&
            (intJ->punct_stanga->x <= intI->punct_dreapta->x) && 
            (intI->punct_stanga->y >= intJ->punct_stanga->y) &&
            (intI->punct_stanga->y <= intJ->punct_dreapta->y))
                intersectii++;

            else if(
            (intI->punct_stanga->x >= intJ->punct_stanga->x) &&
            (intI->punct_stanga->x <= intJ->punct_dreapta->x) && 
            (intJ->punct_stanga->y >= intI->punct_stanga->y) &&
            (intJ->punct_stanga->y <= intI->punct_dreapta->y))
                intersectii++;
        }
    }
    return intersectii;
}
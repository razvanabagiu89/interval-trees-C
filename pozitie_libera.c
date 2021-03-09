#include "pozitie_libera.h"

void actualizare(Nod *nod, int v) {
    nod->info += v;
}

int suma_raspunsurilor(int r1, int r2) {
    return r1 + r2;
}

DateIntrare* citeste_fisier_in(char *nume_fisier_intrare) {
    // TODO citeste datele din fisier si populeaza pointerul la structura

    DateIntrare *date = calloc(1, sizeof(DateIntrare));

    FILE *file = fopen(nume_fisier_intrare, "r");
    int E_MAX, N;
    fscanf(file, "%d %d", &E_MAX, &N);
    date->numarul_maxim_din_intrari = E_MAX;
    date->numar_intrari = N;

    int *S = calloc(N, sizeof(int));
    for(int i = 0; i < N; i++)
        fscanf(file, "%d", &S[i]);

    date->intrari = S;
    fclose(file);
    return date;
}


int* calculeaza_vector_raspuns(DateIntrare *date_intrare) {
    // TODO creeaza vectorul raspuns folosind arbori de intervale

    int max = date_intrare->numarul_maxim_din_intrari;
    int *intrari = date_intrare->intrari;
    int *T = calloc(max + 1, sizeof(int));

    ArboreDeIntervale *arbore = construieste_arbore(0, max, 0, actualizare, suma_raspunsurilor);

    Interval *interval = calloc(1, sizeof(Interval));
    Interval *interval_max = calloc(1, sizeof(Interval));
    interval_max->capat_stanga = 0;
    interval_max->capat_dreapta = max;

    int i = 0;
    while(1) {

        int stop = 0;
        interval->capat_stanga = interval->capat_dreapta = intrari[i];

        //conditie de oprire
        stop = interogare_interval_in_arbore(arbore, interval_max);
        if(stop == max + 1)
            break;

        //daca nu exista
        if(T[intrari[i]] == 0) {
            T[intrari[i]] = intrari[i];
            actualizare_interval_in_arbore(arbore, interval, 1);
        }

        //daca exista deja pe pozitie element
        else {
            
            for(int j = intrari[i]; j >= 0; j--) {

                if(T[j] == 0) {
                    T[j] = intrari[i];
                    interval->capat_dreapta = interval->capat_stanga = j;
                    actualizare_interval_in_arbore(arbore, interval, 1);
                    break;
                }
            }
        }
    i++;   
    }
    return T;
}
int* calculeaza_vector_raspuns_trivial(DateIntrare *date_intrare) {
    // TODO creeaza vectorul raspuns folosind 2 for-uri

    int nr = date_intrare->numar_intrari;
    int max = date_intrare->numarul_maxim_din_intrari;
    int *intrari = date_intrare->intrari;
    int *T = calloc(max + 1, sizeof(int));

    for(int i = 0; i < nr; i++) {

        if(T[intrari[i]] == 0)
            T[intrari[i]] = intrari[i];

        else {

            for(int j = intrari[i]; j >= 0; j--) {

                if(T[j] == 0) {
                    T[j] = intrari[i];
                    break;
                }
            }
        }
    }
    return T;
}
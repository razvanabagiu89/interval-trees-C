#include "arbori_intervale.h"

Nod* creare_nod(int left, int right) {

    Nod* nod = calloc(1, sizeof(Nod));
    if(!nod)
        return NULL;

    nod->interval = malloc(sizeof(Interval));
    nod->interval->capat_dreapta = right;
    nod->interval->capat_stanga = left;

    if(left < right) {

        int mid = (left + right)/2;
        nod->copil_stanga = creare_nod(left, mid);
        nod->copil_dreapta = creare_nod(mid+1, right);

    }
    return nod;
}

ArboreDeIntervale *construieste_arbore(int capat_stanga, int capat_dreapta,
        int valoare_predifinita_raspuns_copil,
        void (*f_actualizare)(Nod *nod, int v_actualizare),
        int (*f_combinare_raspunsuri_copii)(int raspuns_stanga, int raspuns_dreapta)) {
    // TODO implementati functia de constructie a arborelui prezentata in enunt
    // TODO initializati campurile unui ArboreDeIntervale*

    ArboreDeIntervale* arb = malloc(sizeof(ArboreDeIntervale));
    if(!arb)
        return NULL;

    arb->valoare_predifinita_raspuns_copil = valoare_predifinita_raspuns_copil;
    arb->f_actualizare = f_actualizare;
    arb->f_combinare_raspunsuri_copii = f_combinare_raspunsuri_copii;
    arb->dimensiune = 0;

    arb->radacina = creare_nod(capat_stanga, capat_dreapta);
    return arb;
}


void actualizare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod,
                                        Interval *interval, int v_actualizare) {
    // TODO implementati functia de actualizare pentru un nod
    // Hint: pentru a actualiza un nod cand este cazul folositi arbore->f_actualizare(nod, v_actualizare);

   if((interval->capat_stanga <= nod->interval->capat_stanga) &&
    (interval->capat_dreapta >= nod->interval->capat_dreapta)) 
        arbore->f_actualizare(nod, v_actualizare);

    
    else {

        int mid = (nod->interval->capat_stanga + nod->interval->capat_dreapta)/2;
        if(interval->capat_stanga <= mid)
            actualizare_interval_pentru_nod(arbore, nod->copil_stanga,
            interval, v_actualizare);

        if(mid < interval->capat_dreapta)
            actualizare_interval_pentru_nod(arbore, nod->copil_dreapta,
            interval, v_actualizare);

        arbore->f_actualizare(nod, v_actualizare);
   }
}


// Functia este deja implementata, se cheama functia de mai sus cu radacina arborelui
void actualizare_interval_in_arbore(ArboreDeIntervale *arbore, 
                                        Interval *interval, int v_actualizare) {
    actualizare_interval_pentru_nod(arbore, arbore->radacina, interval, v_actualizare);
}


int interogare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod, Interval *interval) {
    // TODO implementati functia de interogare pentru un nod prezentata in enunt
    // Hint: cand vreti sa calculati combinarea raspunsurilor folositi
    // arbore->f_combinare_raspunsuri_copii(raspuns_copil_stanga, raspuns_copil_drepta)

    if((interval->capat_stanga <= nod->interval->capat_stanga) &&
    (interval->capat_dreapta >= nod->interval->capat_dreapta))
        return nod->info;

    else {

        int mid = (nod->interval->capat_dreapta + nod->interval->capat_stanga)/2;
        int left_val = 0, right_val = 0;

        if(interval->capat_stanga <= mid) {
            left_val = interogare_interval_pentru_nod(arbore, nod->copil_stanga,
            interval);
        }

        if(mid < interval->capat_dreapta) {
            right_val = interogare_interval_pentru_nod(arbore, nod->copil_dreapta,
            interval);
        }
        return arbore->f_combinare_raspunsuri_copii(left_val,right_val);
    }
}

// Functia este deja implementata, se cheama functia de mai sus cu radacina arborelui
int interogare_interval_in_arbore(ArboreDeIntervale *arbore, Interval *interval) {
    return interogare_interval_pentru_nod(arbore, arbore->radacina, interval);
}


// ----- DOAR pentru bonus si DOAR daca considerati ca e necesara ----- //
void seteaza_info_in_nod_la_valoare_capat_dreapta(Nod* nod) {
    // TODO cred ca e destul de clar ce vrea sa faca functia asta
}


void seteaza_info_in_arbore_la_valoare_capat_dreapta(ArboreDeIntervale* arbore) {
    seteaza_info_in_nod_la_valoare_capat_dreapta(arbore->radacina);
}

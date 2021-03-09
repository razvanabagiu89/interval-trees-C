# Interval trees in C

## How to run tests:
```bash
make runTestArbori
make runTestIntersectii
make runTestPozitie
```


## Partea 1: Implementare C Arbori de Intervale (50 puncte)


1) construieste_arbore()

Pentru aceasta functie mi-am implementat o functie recursiva separata pentru
creare unui singur nod folosindu-ma de pseudocodul din enunt. Pentru
constructia arborelui am alocat memoria si setat campurile necesare, iar apoi
am apelat functia de creare nod pentru radacina arborelui.

2) actualizare_interval_pentru_nod()

Am urmat pseudocodul din enunt folosindu-ma si de pointerii la functii.

3) interogare_interval_pentru_nod()

Am urmat pseudocodul din enunt folosindu-ma si de pointerii la functii.


## Partea 2: Intersecții de intervale (40 puncte)

1) calculeaza_numar_intersectii_trivial()

Pentru aceasta functie am parcurs toata lista de intervale, si am pus
conditia de intersectii astfel: am verificat fiecare segment cu toate
segmentele urmatoare, iar daca x-ul celorlaltor segmente este inclus
in intervalul x-urilor din segmentul verificat, analog si pentru
y, atunci avem o intersectie. Altfel, daca segmentul verificat are
x-ul inclus in intervalul de x al celorlaltor segmente, analog si
pentru y avem o intersectie. Pe scurt, am verificat intervalele
tuturor segmentelor pentru a determina daca au o intersectie.

2) calculeaza_numar_intersectii()

Datorita unui post pe forum, am inteles ca nu se pot modifica campuri
din structurile deja existente, dar se pot crea altele. Am avut nevoie
doar de un camp suplimentar in structura Punct2D, asa ca am reconstruit-o
cu campul 'idx' ce stocheaza din ce interval a facut parte inainte de sortare,
numind-o Punct2D_aux, si alte 2 structuri identice cu cele din schelet, dar
modificate pentru a se adapta la Punct2D_aux: Interval2D_aux,
ListaIntervale2D_aux.

Am aflat y maxim din lista pentru a construi arborele, iar apoi am 
implementat o functie auxiliara ce face conversia de la lista data ca
parametru, la o lista auxiliara(lista_aux) ce se foloseste de noile
structuri. In vectorul de puncte 'puncte' am stocat toate punctele,
indiferent daca e punct stanga, sau dreapta, apoi le-am sortat in
functie de x folosind bubble sort. Am luat un vector de frecvente de
lungime numar de intervale, pentru a determina daca unui segment i-am
intalnit capatul stang(pe pozitia sa din frecvente se va pune 1), iar daca
i-am intalnit capatul drept(pe pozitia sa din frecvente se va pune 0). Cand
se pune 1, se actualizeaza arborele cu +1, iar cand se pune 0, se actualizeaza
arborele cu -1. Toate acestea se fac in cazul in care segmentul este orizontal,
adica din 'puncte', fiind sortat, segmentele orizontale vor avea urmatorul
punct dintr-un interval diferit. Altfel, daca urmatorul punct e din acelasi
interval inseamna ca este un segment vertical (deoarece au coordonata x egala,
'puncte' fiind sortat dupa x). La intalnirea unui segment vertical se face
interogare pe arbore, si se sare peste urmatorul punct care este de fapt
tot capatul acestui segment vertical. Cand s-a ajuns la ultimul punct,
inseamna ca acesta este un capat de segment orizontal si se face doar
stergere. Toate interogarile se aduna in variabila intersectii care se
returneaza.


## Partea 3: Poziție liberă (20 de puncte) - Bonus

In fisierul pozitie_libera.c am adaugat 2 functii pe care o sa le
folosesc in implementarea cu arbori: actualizare() si
suma_raspunsurilor().

1) calculeaza_vector_raspuns_trivial()

Am generat vectorul T cu zero pe toate elementele si m-am folosit de
cele 2 for-uri pentru a insera pe pozitie, sau in spatele ei pana cand voi
gasi o pozitie libera.

2) calculeaza_vector_raspuns()

Am construit arborele cu intervalul 0 -> elementul maxim si cele 2 functii
adaugate in fisier, enumerate mai sus. Am alocat 2 intervale si anume 'interval'
ce se va actualiza pentru fiecare punct si 'interval_max' care este predifinit
acelasi interval cu cel din arbore->radacina. La fiecare parcurgere a punctului
din lista initiala, verific daca este sau nu in vectorul T, apoi actualizez
arborele cu +1 pe intervalul unde se insereaza punctul respectiv. Conditia
de oprire a loop-ului se face prin interogare() pe interval_max.


## Project structure
```bash
├── arbori_intervale.c
├── arbori_intervale.h
├── arboriTest
├── arboriTest.c
├── intersectii.c
├── intersectii.h
├── intersectiiTest
├── intersectiiTest.c
├── makefile
├── pozitie_libera.c
├── pozitie_libera.h
├── pozitieTest
├── pozitieTest.c
├── README.md
├── Tema 2 SD 2020 - Arbori de intervale.pdf
└── tests
    ├── intersectie1.in
    ├── intersectie2.in
    ├── pozitie1.in
    ├── pozitie2.in
    └── pozitie3.in

```

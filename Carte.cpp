#include <iostream>


//pct 1 - cele 2 struct (nu mai avem struct lista..)
struct Carte {
	int idCarte;
	char* titlu;
	int nrAutori;
	char** numeAutori;
	float pret;
};

//spre deosebire de lista dubla, in loc de prev si next aici avem doar next
struct Nod {
	Nod* next;
	Carte info;
};


//pct 2- fct creare - folosita in main la inserare
//     - fct copiere - ajutatoare pentru fct de inserare si extragere
//     - fct inserare la inceput conform LIFO


Carte creareCarte(int idCarte, const char* titlu, int nrAutori, const char** numeAutori, float pret) {
	Carte c;
	c.idCarte = idCarte;
	c.titlu = (char*)malloc(sizeof(char)*(strlen(titlu) + 1));
	strcpy(c.titlu, titlu);
	c.nrAutori = nrAutori;
	c.numeAutori = (char**)malloc(sizeof(char*)*nrAutori); //aici avem sizeof(char*)!!! - alocare spatiu pt adresele elementelor, in functie de nr elementelor
	for (int i = 0; i < nrAutori; i++) { //peste tot in for avem [i]!!!
		c.numeAutori[i]= (char*)malloc(sizeof(char)*(strlen(numeAutori[i]) + 1)); //!!! clasic ca la un char* doar in plus e c.numeAutori[i] si numeAutori[i] ! i pt ca avem m multe elemente!!!
		strcpy(c.numeAutori[i], numeAutori[i]);
	}
	c.pret = pret;
	return c;
}

Carte copiereCarte(Carte c) {
	Carte copie;
	copie.idCarte = c.idCarte;
	copie.titlu = (char*)malloc(sizeof(char)*(strlen(c.titlu) + 1));
	strcpy(copie.titlu, c.titlu);
	copie.nrAutori = c.nrAutori;
	copie.numeAutori = (char**)malloc(sizeof(char*)*c.nrAutori);
	for (int i = 0; i < c.nrAutori; i++) {
		copie.numeAutori[i] = (char*)malloc(sizeof(char)*(strlen(c.numeAutori[i]) + 1));
		strcpy(copie.numeAutori[i], c.numeAutori[i]);
	}
	copie.pret = c.pret;
	return copie;
}

//inserare la inceput conform principiului LIFO
Nod* inserareInceput(Nod* cap, Carte c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod)); //alocam memorie pentru un nod nou
	nou->info = copiereCarte(c); //folosim functia ajutatoare copiereCarte!! copiem info utila in noul nod (info primita din main, adica o Carte)
	nou->next = cap; //noul nod pointeaza inainte catre fostul cap
	return nou; //de ce la inserare la inceput se returneaza nou? in rest peste tot la inserare se returneaza lista (cap sau ld)
}

//inserare la sfarsit
Nod* inserareSfarsit(Nod* cap, Carte c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereCarte(c);
	nou->next = NULL; //noul nod va pointa catre NULL pt ca va fi ultimul din lista;
	if (cap) {
		Nod*p = cap; //se creaza un nou pointer catre capul listei pt a parcurge lista fara a o altera
		while (p->next) { //cat timp exista p->next, parcurgem toata lista pana ajungem la ultimul nod care pointeaza catre NULL. Ne oprim la el si atunci se iese din while (astfel gasim ultimul nod)
			             //while(p->next) e practic while (p->next != NULL)
			p = p->next; //se trece la urmatorul nod (daca acesta exista). Daca nu exista (adica daca p->next=NULL?) se iese din while, iar p ramane sa pointeze catre ultimul nod.
		}
		p->next = nou; //fostul ultim nod(cel pe care l-am gasit in while) va pointa catre noul nod
	}
	else { //daca nu exista noduri in lista
		cap = nou; //noul nod este singurul nod si deci primul din lista
	}
	return cap;
}



//pct 3 - afisare
void afisareCarte(Carte c) {
	printf("Cartea cu id-ul %d, titlul %s, scrisa de %d autori avand numele ", c.idCarte, c.titlu, c.nrAutori);
	for (int i = 0; i < c.nrAutori; i++) {
		printf(" %s, ", c.numeAutori[i]);
	}
	printf("are pretul %.2f lei\n", c.pret);
}

void afisareLista(Nod* cap) {
	Nod* p = cap; //se creaza un nou pointer catre capul listei (primul nod) pentru a parcurge lista fara a o altera
	while (p) {
		afisareCarte(p->info); //se afiseaza info utila din nod (p->info este practic o carte? in fiec nod este o carte care reprez info utila)
		p = p->next;
	}
}

//pct 4 
//fct de extragere de la inceput - conform principiului LIFO
Carte extragereCarte(Nod* &cap) { //adresa la &cap in loc sa fie la &lista in main la apelarea functiei
	Carte c = copiereCarte(cap->info); //se copiaza in c prima Carte din lista cu ajutorul functiei copiereCarte
	free(cap->info.titlu);
	for (int i = 0; i < cap->info.nrAutori; i++) {
		free(cap->info.numeAutori[i]);
	}
	free(cap->info.numeAutori);
	Nod* p = cap;
	if (p) {
		cap = p->next;
		free(p); //stergem efectiv fostul prim nod (nodul extras)
	}
	else {
		printf("Nu exista noduri in lista!"); //nu avem ce sa extragem
	}
	return c; 
}

//pct 5
Carte* filtrare(Nod* cap, const char* autorCautat) { //spre deoseb de extragere aici avem Carte* in loc de Carte tipul returnat
	Nod* p = cap; 
	int contor = 0;
	while (p) {
		for (int i = 0; i < p->info.nrAutori; i++) {
			if (strcmp(p->info.numeAutori[i], autorCautat) == 0) {
				contor++;
			}
		}
		p = p->next;
	}
	Carte* vector = (Carte*)malloc(sizeof(Carte)*contor); //alocam memorie fix pentru cartile care au autorul cautat
	p = cap; //revenim la inceputul listei
	int i = 0;
	int j = 0;
	while (p && i < contor) {
		for (int j = 0; j < p->info.nrAutori; j++) {
			if (strcmp(p->info.numeAutori[j], autorCautat) == 0) {
				vector[i] = copiereCarte(p->info);
				p = p->next;
				i++;

			}
			else {
				p = p->next;
			}
		}
	}
	return vector;
}




//functii extra facute de mine (pe langa subiect):

//inserare pe pozitie 
Nod* inserarePePozitie(Nod* cap, Carte c, int poz) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereCarte(c);
	Nod* p = cap;
	int contor = 1; //initializam un contor cu 1, care semnifica pozitia curenta, pt a ajunge la pozitia dorita poz
	while (p && contor < poz) {
		p = p->next;
		contor++;
	}
	if (p) {
		nou->next = p->next;
		p->next = nou;
	}
	else {
		cap = inserareSfarsit(cap, c);
	}
	return cap;
}

//inserare sortata - cu cheie de cautare id-ul
//ca la  Magazin 1044 s05. de vazut si la LDI - Farmacie! (inserarea facuta de mine) sau Intrerupator.
Nod* inserareSortata(Nod* cap, Carte c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereCarte(c);
	if (cap) { //la inserare pe pozitie nu avem acest prim if - merge si fara
		Nod* p = cap;
		while (p->next && p->next->info.idCarte < c.idCarte) { //aici e p->next in loc de p cum era la LDI Farmacie. De ce? 
			                    //- daca nu avem p->next, insereaza id-ul m mic dupa cel m mare gasit, in loc sa insereze inaite
			                   //trebuie sa ne oprim inainte de nodul care are id-ul m mare decat id-ul noului nod si sa introducem dupa el
			p = p->next;      //pentru ca aici nu avem prev si atunci nu putem sa trecem la nodul care are id-ul mai mare si sa introducem inaintea lui (in stanga lui)
		}
		if (p->next) { //de ce la inserare pe pozitie este if(p) iar aici e if(p->next)? - merge si cu p
			nou->next = p->next;//nu mai avem primul rand din cele trei de la LDI
			p->next = nou; //iar astea doua sunt cu next in loc de prev (aici nu exista prev)
		}
		else { //nu putem sa folosim inserarea la sfarsit si un singur else in loc de astea doua? (cum e la Farmacie) - de incercat (ca mai sus la inserare pe pozitie)
			/*p->next = nou;
			nou->next = NULL;*/
			cap = inserareSfarsit(cap, c);
		}
	}
	//else { //si atunci sa nu mai avem al doilea else (cum e si la Farmacie cel comentat, de care nu mai avem nevoie)
	//	nou->next = NULL;
	//	return nou;
	//}
	return cap;
}


//inserare sortata ca la Florarie:
//Nod* inserareSortata2(Nod* cap, Carte c) {
//	Nod* nou = (Nod*)malloc(sizeof(Nod));
//	nou->info = copiereCarte(c);
//	nou->next = NULL;
//	if (cap) {
//		if (cap->info.idCarte > c.idCarte) {
//			nou->next = cap;
//			cap = nou;
//		}
//		else {
//			Nod* p = cap;
//			while (p->next && p->next->info.idCarte < c.idCarte) {
//				p = p->next;
//			}
//			nou->next = p->next;
//			p->next = nou;
//		}
//	}
//	else {
//		cap = nou;
//	}
//	return cap;
//}


//inserare inainte de nod - Portocala(2018-recuperare sem3)  - complicata, e cu fct ajutatoare si vector de float (asa e structura)
//Nod* inserareInainteDeNod(Nod* cap, Carte c, int id) {
//	if (cap) {
//		if()
//	}
//}
//sau inserare dupa Nod - Oras(2019-1049, seminar 3) - mai bine ca la Oras?? e cu char dar e fara fct ajutatoare 


//extragere in fct de un criteriu - de la Florarie 1048 s05 - e cu popstack si pushstack nu prea inteleg. de gasit alta

//extragere care returneaza lista simpla ?? n-am gasit

//filtrare care returneaza lista simpla - Hotel 1050 s03 (e ca la Restaurant doar ca acolo avem lista circulara)
Nod* filtrareCarti(Nod* cap, float pretMax) {
	Nod* listaFiltrata = NULL;
	Nod* p = cap;
	while (p) {
		if (p->info.pret < pretMax) {
			listaFiltrata = inserareSfarsit(listaFiltrata, p->info);
		}
		p = p->next;
	}
	return listaFiltrata;
}



//stergere 
void stergereLista(Nod* cap) { //spre deoseb de lista dubla, aici avem Nod* (cu steluta)
	while (cap) {
		free(cap->info.titlu);
		for (int i = 0; i < cap->info.nrAutori; i++) {
			free(cap->info.numeAutori[i]);
		}
		free(cap->info.numeAutori);
		Nod* p = cap; //nu se poate fara auxiliar pt ca aici nu avem prev deci trebuie salvat nodul curent in p cand avansam (e in loc de ld.first->prev de la lista dubla)
		cap = cap->next; //aici nu trebuia sa fie cap=p->next? (cum e la extragere) - nu conteaza cum avansam important e sa stergem p (free(p))
		free(p);
	}
}



void main() {

	//pct 1
	Nod* lista = NULL; //cu * !!!
	                   //la lista dubla cand initializam lista folosim acel struct in plus (ListaDubla) pe care aici nu il avem

	//pct 2
	const char* v1[1] = { "William" }; //const!!!!
	const char* v2[2] = { "Zamfi", "Iancu" };
	const char* v3[3] = { "Ivan", "Popa", "Pocatilu" };
	const char* v4[1] = { "Creanga" };
	const char* v5[1] = { "Autor1" };
	const char* v6[1] = { "Autor2" };
	const char* v7[1] = { "Autor3" };
	lista = inserareInceput(lista, creareCarte(1, "Imparatul", 1, v1, 1000.f));
	lista = inserareInceput(lista, creareCarte(2, "Programare", 2, v2, 500.2f));
	lista = inserareSfarsit(lista, creareCarte(3, "Structuri", 3, v3, 650.5f));
	lista = inserarePePozitie(lista, creareCarte(4, "Basme", 1, v4, 250.3f), 3);
	/*lista = inserareSortata2(lista, creareCarte(6, "Carte1", 3, v5, 700));*/ //eroare de executie cand apelam inserarea sortata!
	/*lista = inserareSortata(lista, creareCarte(5, "Carte2", 2, v6, 700)); //eroare de exec la creareCarte??
	lista = inserareSortata(lista, creareCarte(7, "Carte3", 3, v7, 700));*/

	//pct 3
	afisareLista(lista);
	
	
	//pct 5
	//testare filtrare 
	Carte* v = filtrare(lista, "Zamfi");
	Nod* p = lista; //aici lista e acelasi lucru cu cap (pur si simplu alta denumire decat in fct de filtrare unde e Nod* cap)
	                //aici in main am initializat-o Nod* lista pt ca ar suna ciudat cap de ex cand inseram in lista
	int contor = 0;
	while (p) {
		for (int i = 0; i < p->info.nrAutori; i++) {
			if (strcmp(p->info.numeAutori[i], "Zamfi") == 0) {
				contor++;
			}
		}
		p = p->next;
	}
	for (int i = 0; i < contor; i++) {
		printf("\nComenzi cu autorul cautat: "); 
		afisareCarte(v[i]);
	}

	//testare filtrare care returneaza lista:
	printf("\nLista filtrata:\n");
	Nod* listaF = NULL; //se initializeaza o noua lista
	listaF = filtrareCarti(lista, 600); //primeste lista de mai sus, care se filtreaza cu pret maxim 600
	afisareLista(listaF);


	//pct 4 - daca punem extragerea inainte de filtrare nu mai merge filtrarea! de ce? 
	printf("\nCartea extrasa:\n");
	afisareCarte(extragereCarte(lista)); //lista simpla e deja pointer de cand o declaram in main 
										 //deci nu mai e nevoie de & aici(spre deosebire de lista dubla)
										 //in schimb e nevoie de & la parametrul functiei de extragere!



	printf("\nLista dupa extragere:\n");
	afisareLista(lista);



	//testare stergere
	stergereLista(lista);
	lista = NULL; 

}
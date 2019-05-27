#include <iostream>



struct Carte {
	int idCarte;
	char* titlu;
	int nrAutori;
	char** numeAutori;
	float pret;
};


struct Nod {
	Nod* next;
	Carte info;
};


Carte creareCarte(int idCarte, const char* titlu, int nrAutori, const char** numeAutori, float pret) {
	Carte c;
	c.idCarte = idCarte;
	c.titlu = (char*)malloc(sizeof(char)*(strlen(titlu) + 1));
	strcpy(c.titlu, titlu);
	c.nrAutori = nrAutori;
	c.numeAutori = (char**)malloc(sizeof(char*)*nrAutori);
	for (int i = 0; i < nrAutori; i++) {
		c.numeAutori[i] = (char*)malloc(sizeof(char)*(strlen(numeAutori[i]) + 1));
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
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereCarte(c);
	nou->next = cap;
	return nou;
}

//inserare la sfarsit
Nod* inserareSfarsit(Nod* cap, Carte c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereCarte(c);
	nou->next = NULL;
	if (cap) {
		Nod*p = cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		cap = nou;
	}
	return cap;
}


//inserare sortata - cu cheie de cautare id-ul
Nod* inserareSortata(Nod* cap, Carte c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereCarte(c);
	if (cap) {
		Nod* p = cap;
		while (p->next && p->next->info.idCarte < c.idCarte) {
			p = p->next;
		}
		if (p->next) {
			nou->next = p->next;
			p->next = nou;
		}
		else {
			cap = inserareSfarsit(cap, c);
		}
	}
	return cap;
}


void afisareCarte(Carte c) {
	printf("Cartea cu id-ul %d, titlul %s, scrisa de %d autori avand numele ", c.idCarte, c.titlu, c.nrAutori);
	for (int i = 0; i < c.nrAutori; i++) {
		printf(" %s, ", c.numeAutori[i]);
	}
	printf("are pretul %.2f lei\n", c.pret);
}

void afisareLista(Nod* cap) {
	Nod* p = cap;
	while (p) {
		afisareCarte(p->info);
		p = p->next;
	}
}

//fct de extragere de la inceput - conform principiului LIFO
Carte extragereCarte(Nod* &cap) {
	Carte c = copiereCarte(cap->info);
	free(cap->info.titlu);
	for (int i = 0; i < cap->info.nrAutori; i++) {
		free(cap->info.numeAutori[i]);
	}
	free(cap->info.numeAutori);
	Nod* p = cap;
	if (p) {
		cap = p->next;
		free(p);
	}
	else {
		printf("Nu exista noduri in lista!");
	}
	return c;
}


Carte* filtrare(Nod* cap, const char* autorCautat) {
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
	Carte* vector = (Carte*)malloc(sizeof(Carte)*contor);
	p = cap;
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


void stergereLista(Nod* cap) {
	while (cap) {
		free(cap->info.titlu);
		for (int i = 0; i < cap->info.nrAutori; i++) {
			free(cap->info.numeAutori[i]);
		}
		free(cap->info.numeAutori);
		Nod* p = cap;
		cap = cap->next;
		free(p);
	}
}



void main() {

	Nod* lista = NULL;

	const char* v1[1] = { "Autor1" };
	const char* v2[2] = { "Autor2", "Autor3" };
	const char* v3[3] = { "Ion Ivan", "Marius Popa", "Paul Pocatilu" };
	const char* v5[1] = { "Autor4" };
	const char* v6[1] = { "Autor5" };
	const char* v7[1] = { "Autor6" };
	lista = inserareInceput(lista, creareCarte(1, "Curs", 1, v1, 1000.f));
	lista = inserareInceput(lista, creareCarte(2, "Programare orientata obiect", 2, v2, 500.2f));
	lista = inserareInceput(lista, creareCarte(3, "Structuri de date", 3, v3, 650.5f));
	lista = inserareSortata(lista, creareCarte(6, "Carte1", 3, v5, 700));
	lista = inserareSortata(lista, creareCarte(5, "Carte2", 2, v6, 700));
	lista = inserareSortata(lista, creareCarte(7, "Carte3", 3, v7, 700));

	afisareLista(lista);

	printf("Cartea extrasa:\n");
	afisareCarte(extragereCarte(lista));

	Carte* v = filtrare(lista, "Zamfi");
	Nod* p = lista;

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
		printf("Comenzi cu autorul cautat: ");
		afisareCarte(v[i]);
	}

	stergereLista(lista);
	lista = NULL;

}
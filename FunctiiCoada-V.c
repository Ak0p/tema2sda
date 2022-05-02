/*-- functiiCoada-V.c -- elementele cozii sunt memorate intr-un vector --*/
#include "TCoada-V.h"

/* creeaza coada vida cu elemente de dimensiune d;
   este necesar si un al doilea parametru = numar maxim elemente in coada */
void* InitQ(size_t d,...)
{
	AQV a = (AQV)malloc(sizeof(TCoadaV));  	/* adresa descriptor alocat */
  	va_list ap;
  	if (!a)
		return NULL;
	/* preia param.2 = nr.max.elemente */
	va_start(ap,d);
	a->maxe = va_arg(ap,size_t);
	va_end(ap);

 	a->dime = d;

  	/* aloca spatiu pentru vector si test esec */
  	a->v = (char*)calloc(a->maxe+1,d);
  	if(!a->v)
	{
		free(a);
		return NULL;
	}

  	/* actualizeaza adrese din descriptor */
  	a->sv = a->v + (a->maxe+1) * d;
  	a->ic = a->sc = a->v;

  	return (void*)a;
}

/* adauga element la sfarsitul cozii */
int IntrQ(void *a, void *ae)
{
	if(PLINA(a))
		return 0;
  	memcpy(SC(a), ae, DIME(a));  /* copiaza elementul la sfarsitul cozii */
  	Avans(a,SC(a));              /* sfarsitul cozii avanseaza */
  	return 1;
}

/* extrage primul element din coada la adresa ae */
int ExtrQ (void *a, void *ae)
{
	if(VIDA(a))
		return 0;
	memcpy(ae, IC(a), DIME(a));  /* copiaza elementul de la inceputul cozii */
  	Avans(a,IC(a));              /* inceputul cozii avanseaza */
  	return 1;
}

/* copiaza primul element din coada la adresa ae */
int PrimQ(void *a, void *ae)
{
	if(VIDA(a))
		return 0;
  	memcpy(ae, IC(a), DIME(a));  /* copiaza elementul de la inceputul cozii */
	return 1;
}

/* test coada vida */
int VidaQ(void *a)
{
	return VIDA(a);
}

/* dimensiunea elementelor din coada */
size_t DimEQ(void *a)
{
	return DIME(a);
}

/* transforma coada in coada vida */
void ResetQ(void *a)
{
	SC(a) = IC(a) = V(a); /* inceput si sfarsit coada = inceput vector */
}

/* elibereaza intregul spatiu ocupat de coada */
void DistrQ(void **aa)
{
	free(V(*aa));  	/* elibereaza spatiul ocupat de vector 			*/
  	free(*aa);		/* elibereaza spatiul alocat pentru descriptor 	*/
  	*aa = NULL;
}

/*- afisare elemente din coada -*/
void AfisareQ(void *a, TF1 afi_el)
{
  	char *p;
	printf("Coada: ");
	if(VidaQ(a))
	{
		printf("vida\n");
		return;
	}
  	for(p = IC(a); p != SC(a); Avans(a,p))  /* pentru fiecare element, pana la ultimul */
		afi_el(p);
	printf("\n");
}

/* Operatii de deplasare din coada sursa in coada destinatie.
   Rezultat < 0 in caz de eroare (de exemplu dimensiunile elementelor difera),
   0 daca sursa vida, 1 = succes */

/* muta in destinatie primul element din sursa */
int MutaQ (void *ad, void *as)
{
	if(DIMDIF(as,ad))
		return -1;  	/* dimensiuni diferite */
  	if(VIDA(as))
		return 0;       /* sursa vida */
  	if(PLINA(ad))
		return -2;      /* nu exista spatiu pentru mutarea in destinatie */

  	memcpy(SC(ad), IC(as), DIME(as));    /* muta element */
  	Avans(ad,SC(ad)); 					 /* actualizeaza SC pentru coada destinatie */
	Avans (as,IC(as)); 					 /* actualizeaza IC pentru coada sursa */
  	return 1;
}

/* concatenare (muta toate elem.din sursa) */
int ConcatQ(void *ad, void *as)
{
	if(DIMDIF(as,ad))
		return -1;  		/* dimensiuni diferite */
  	if(VIDA(as))
		return 0;        	/* sursa vida */
  	if(Liber(ad) < Ocupat(as))
		return -2; 			/* nu exista spatiu pt mutarea sursei */
  	for(; IC(as) != SC(as); Avans(ad,SC(ad)), Avans(as,IC(as)))
    	memcpy(SC(ad), IC(as), DIME(as));   /* muta element */
  	return 1;
}

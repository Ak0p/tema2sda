/*-- functiiStiva-V.c -- elementele stivei sunt memorate intr-un vector --*/
#include "TStiva-V.h"

/* creeaza stiva vida cu elemente de dimensiune d;
	   este necesar si un al doilea parametru = numar maxim elemente in coada */
void *InitS(size_t d,...)
{
	ASt a = (ASt)malloc(sizeof (TStiva));			/* adresa descriptor alocat */
  	va_list ap;
  	if (!a)
		return NULL;

  	/* preia param.2 = nr.max.elemente */
  	va_start(ap,d);
  	a->maxe = va_arg(ap,size_t);
  	va_end(ap);

  	a->dime = d;

  	/* aloca spatiu pentru vector si test esec */
  	a->b = (char*)calloc(a->maxe, d);
  	if (!a->b)
	{
		free(a);
		return NULL;
	}

  	/* actualizeaza adrese din descriptor */
  	a->vf = a->b;
  	a->sv = a->b + d * a->maxe;

  	return (void *)a;
}

/* pune element in varful stivei */
int Push(void *a, void *ae)
{
	if (PLINA(a))
		return 0;
  	memcpy (VF(a), ae, DIME(a));  /* copiaza elementul in varful stivei */
  	VF(a) += DIME(a);             /* deplaseaza varful stivei */
  	return 1;
}

/* extrage elementul din varful stivei la adresa ae */
int Pop(void *a, void *ae)
{
	if(VIDA(a))
		return 0;
  	VF(a) -= DIME(a);             /* deplaseaza varful stivei */
  	memcpy(ae, VF(a), DIME(a));  /* copiaza elementul scos din stiva */
  	return 1;
}

/* copiaza elementul din varful stivei la adresa ae */
int Top(void *a, void *ae)
{
	if(VIDA(a))
		return 0;
  	memcpy (ae, VF(a)-DIME(a), DIME(a));   /* copiaza elementul din varful stivei */
  	return 1;
}

/* test stiva vida */
int VidaS(void *a)
{
	return VF(a) == BS(a);
}

/* dimensiunea elementelor din stiva */
size_t DimES(void *a)
{
	return DIME(a);
}

/* transforma stiva in stiva vida */
void ResetS(void *a)
{
	VF(a) = BS(a);
}

/* elibereaza intregul spatiu ocupat de stiva */
void DistrS(void **aa)
{
	free(BS(*aa)); 		/* elibereaza spatiul ocupat de vector */
	free(*aa); 			/* elibereaza spatiul alocat pentru descriptor */
  	*aa = NULL;
}

/* inverseaza continutul stivei */
int InvS(void *a)
{
	char *temp = VF(a), *p, *q;
  	if(temp >= SV(a))
		temp = (char *)malloc(DIME(a)); /* zona tampon alocata dinamic */

  	if(!temp)
		return 0;          /* nu exista spatiu -> operatie imposibila */
  	/* inverseaza perechi de elemente; p, q pornesc de la baza, respectiv varful stivei */
  	for(p = BS(a), q = VF(a)-DIME(a); p < q; p += DIME(a), q -= DIME(a))
  	{
		memcpy(temp, p, DIME(a));
    	memcpy(p, q, DIME(a));
    	memcpy(q, temp, DIME(a));
  	}

  	if(temp != VF(a))
		free(temp); 					/* daca tampon alocat dinamic, atunci elibereaza */
  	return 1;
}

/*- afisare elemente din stiva; -> baza la varf -*/
void AfisareS(void *a, TF1 afi_el)
{
  	char *p;
	printf("Stiva: ");
	if(VidaS(a))
	{
		printf("vida\n");
		return;
	}
  	for(p = BS(a); p < VF(a); p += DIME(a))   /* incepand de la baza */
		afi_el(p);
	printf("\n");
}

/* Operatii de deplasare din coada sursa in coada destinatie. Rezultat < 0 in caz de
   eroare (de exemplu dimensiunile elementelor difera), 0 daca sursa vida, 1 = succes */

/* muta elementul din varful stivei sursa */
int MutaS(void *ad, void *as)
{
	/* teste dim.diferite, sursa vida, lipsa spatiu in stiva destinatie */
  	if(DIMDIF(as,ad))
		return -1;    		/* dimensiuni diferite */
	if(VIDA(as))
		return 0;          	/* sursa vida */
  	if(PLINA(ad))
		return -2; 			/* nu exista suficient spatiu in destinatie */
  	/* mutare element */
  	VF(as) -= DIME(as); 				/* actualizeaza varful stivei sursa */
  	memcpy(VF(ad), VF(as), DIME(as));	/* muta elementul din varful stivei sursa */
  	VF(ad) += DIME(ad);					/* actualizeaza varful stivei destinatie */
  	return 1;
}

/* muta stiva sursa peste destinatie */
int Suprapune(void *ad, void *as)
{
	size_t n = VF(as) - BS(as);       /* numar octeti mutati */
  	/* teste dim.diferite, sursa vida, lipsa spatiu in stiva destinatie */
  	if(DIMDIF(as,ad))
		return -1;      		/* dimensiuni diferite */
  	if(VIDA(as))
		return 0;				/* sursa vida */
  	if(VF(ad) + n > SV(ad))
		return -2; 				/* nu exista suficient spatiu in destinatie */
  	/* transfer elemente */
  	memcpy(VF(ad), BS(as), n); 	/* copiaza elementele stivei sursa */
  	VF(ad) += n; 				/* actualizeaza varful stivei destinatie */
	VF(as) = BS(as);     		/* stiva sursa = stiva vida */
  	return 1;
}

/* suprapune inversul sursei destinatie */
int Rastoarna(void *ad, void *as)
{
	size_t n = VF(as) - BS(as);
  	if(DIMDIF(as,ad))
		return -1;      	/* dimensiuni diferite */
  	if(VIDA(as))
		return 0;           /* sursa vida */
  	if(VF(ad) + n > SV(ad))
		return -2;			/* nu exista spatiu pentru mutarea in destinatie */
  	/* cat timp mai exista elemente in sursa -> muta din varf sursa in varf destinatie */
 	 while(VF(as) > BS(as))
  	{
		VF(as) -= DIME(as);  					/* actualizeaza varful stivei sursa */
    	memcpy(VF(ad), VF(as), DIME(as)); 		/* muta elementul curent */
    	VF(ad) += DIME(as);						/* actualizeaza varful stivei destinatie */
  	}
  	return 1;
}

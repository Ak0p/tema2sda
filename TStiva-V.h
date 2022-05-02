/*-- TStiva-V.h -- elementele stivei sunt memorate intr-un vector --*/
#include "TStiva.h"

#ifndef _STIVA_VECTOR_
#define _STIVA_VECTOR_

typedef struct
{
	size_t 	dime; 		/* dimensiunea unui element al stivei 			*/
	size_t 	maxe; 		/* numarul maxim de elemente in stiva 			*/
	char 	*b;			/* adresa de inceput a vectorului / baza stivei */
	char 	*sv; 		/* adresa de sfarsit a vectorului				*/
	char 	*vf; 		/* varful stivei 								*/
} TStiva, *ASt;

/*- macrodefinitii - acces campuri */
#define DIME(a) (((ASt)(a))->dime)
#define BS(a) 	(((ASt)(a))->b)
#define SV(a) 	(((ASt)(a))->sv)
#define VF(a) 	(((ASt)(a))->vf)

/*- teste -*/
/* verifica daca doua stive au elemente de dimensiuni diferite */
#define DIMDIF(s,d)	(DIME(s) != DIME(d))
/* verifica daca o stiva este vida */
#define VIDA(a)  	(VF(a) == BS(a))
/* verifica daca stiva este plina */
#define PLINA(a) 	(VF(a) == SV(a))

#endif

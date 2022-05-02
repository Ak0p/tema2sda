/*-- TCoada-V.h -- elementele cozii sunt memorate intr-un vector --*/
#include "TCoada.h"

#ifndef _COADA_VECTOR_
#define _COADA_VECTOR_

typedef struct
{
	size_t 	dime; 		/* dimensiunea unui element din coada  		*/
	size_t 	maxe; 		/* numarul maxim de elemente din coada 		*/
	char 	*v; 		/* adresa de inceput a vectorului 			*/
	char 	*sv; 		/* adresa de sfarsit a vectorului  			*/
	char 	*ic; 		/* adresa primului element din coada 		*/
	char 	*sc; 		/* adresa de sfarsit a cozii 				*/
} TCoadaV, *AQV;

/*- macrodefinitii - acces campuri -*/
#define V(a) 	(((AQV)(a))->v)
#define SV(a) 	(((AQV)(a))->sv)
#define IC(a) 	(((AQV)(a))->ic)
#define SC(a) 	(((AQV)(a))->sc)
#define DIME(a) (((AQV)(a))->dime)

/*- teste si prelucrari -*/
/* verifica daca doua cozi au elemente de dimensiuni diferite 			*/
#define DIMDIF(s,d) 	(DIME(s) != DIME(d))
/* verifica daca o coada este vida 										*/
#define VIDA(a) 		(IC(a) == SC(a))
/* verifica daca o coada este plina 									*/
#define PLINA(a) 		((SC(a)+DIME(a)) == (IC(a) == V(a) ? SV(a) : IC(a)))
/* avans in coada (p avanseaza in coada a) 								*/
#define Avans(a,p) 		((p) = ((p) += DIME(a)) < SV(a) ? (p) : V(a))
/* determina numarul de octeti ocupati de coada							*/
#define Ocupat(a) 		( (SC(a) - IC(a)) + ( (IC(a)<=SC(a)) ? 0 : (SV(a)-V(a)) ) )
/* determina numarul de octeti liberi din vectorul alocat 				*/
#define Liber(a) 		( (IC(a)-SC(a)-DIME(a)) + ( (IC(a)<=SC(a)) ? (SV(a)-V(a)) : 0) )

#endif

/* MONOPOLY 1/5/26 */

#include "acciones.h"

#define EOS '\0'
#define EOI '\n'

#define prt printf
#define prts(A) printf("%s",A)
#define prtv(A) printf("%i",(A))
#define tab printf("   ")
#define nln puts("")


u1 input(u1 len,char* str);
/* funcion que hace una entrada de cadena de caracteres */

void turno_humano(u1 jugador);
/* Efectua turno de humano */

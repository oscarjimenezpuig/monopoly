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

void pulsa_intro();
/* funcion que demanda la pulsacion de la tecla intro */

void presentacion_humano(u1 jugador);
/* presentacion del humano */

void turno_humano(u1 jugador);
/* efectua turno de humano */

u2 subasta_humano(u1 jugador,u1 casilla);
/* se demanda el dinero que ofrecera un humano por una casilla */

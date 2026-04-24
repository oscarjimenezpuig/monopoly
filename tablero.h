/* MONOPOLY 24/4/26 */

#include <stdio.h>

#define TABSIZ 40 /* numero de casillas del tablero */
#define BARSIZ    /* numero de barrios */
#define JUGMINSIZ 2 /* minimo numero de jugadores */
#define JUGMAXSIZ 8 /* maximo numero de jugadores */

#define NOMSIZ 20

#define NADA 0
#define SALIDA 1
#define CALLE 2
#define NEGOCIO 3
#define FERROCARRIL 4
#define CARCEL 5
#define IRCARCEL 6
#define COMUNIDAD 7
#define SUERTE 8

typedef unsigned char u1;
typedef unsigned short u2;
typedef signed char s1;
typedef signed short s2;

typedef struct {
    u1 tipo;
    char nombre[NOMSIZ+1];
    s1 barrio;
    s1 poseedor; /* numero de jugador que posee la casilla */
    u1 jugadores; /* identidad de los jugadores que estan en ella */
} Casilla;

typedef struct {
    u1 id;
    char nombre[NOMSIZ+1];
    u2
    u1 jugador;
} Barrio;

typedef struct {
    u1 id;
    char nombre[NOMSIZ+1];
    s2 dinero;
    u1 humano;
    s1 casilla;
} Jugador;

typedef Casilla Tablero[TABSIZ];
typedef Barrio Barrios[BARSIZ];
typedef Jugadores Jugador[JUGSIZ];

extern Tablero tablero;
extern Barrio barrios;
extern Jugadores jugadores;



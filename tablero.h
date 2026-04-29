/* MONOPOLY 24/4/26 */

#include <stdio.h>

#include"random.h"

#define EOS '\0'
#define TAB printf("\t");
#define NLN printf("\n");

#define TABSIZ 40 /* numero de casillas del tablero */
#define BARSIZ 8   /* numero de barrios */
#define JUGMINSIZ 2 /* minimo numero de jugadores */
#define JUGMAXSIZ 8 /* maximo numero de jugadores */
#define NUMCASHOT 4 /* numero de casas necesarias para construir hotel */

#define NOMSIZ 40

#define NADA 0
#define SALIDA 1
#define CALLE 2
#define NEGOCIO 3
#define TRENES 4
#define CARCEL 5
#define IR_CARCEL 6
#define COMUNIDAD 7
#define SUERTE 8
#define IMPUESTO 9
#define PARKING 10

#define MATPOS ((TABSIZ%8==0)?(TABSIZ/8):(1+(TABSIZ/8)))


typedef struct {
    u1 barrio; /* numero del barrio al que pertence */
    u2 precio_casa; /* precio de una casa */
    u2 alquiler[NUMCASHOT+2]; /* alquiler dependiendo del numero de casas */
    u1 casas;
    u1 hotel;
} Calle;

typedef struct {
    u1 alquiler_base; /* precio base */
    u2 alquiler;
} Negocio;

typedef struct {
    u2 alquiler_base; /* alquiler basico por un transporte (directamente proporcional) */
    u1 estaciones; /* estaciones que tiene el propietario de esta estacion */
    u2 alquiler; /* alquiler total */
} Tren;

typedef struct {
    u2 precio;
    s1 poseedor;
    union {
        Calle calle;
        Negocio negocio;
        Tren tren;
    };
} Comprable;

typedef struct {
    u2 premio;
} Salida;

typedef struct {
    u2 impuesto;
} Impuesto;

typedef struct {
    u2 sancion;
} Carcel;

typedef struct {
    u1 tipo;
    u1 numero; /* numero de casilla a partir de 0 */
    char nombre[NOMSIZ+1];
    union {
        Comprable comprable;
        Salida salida;
        Impuesto impuesto;
        Carcel carcel;
    };
} Casilla;

typedef struct {
    u1 id; /* identidad del barrio, a partir de 0 */
    char nombre[NOMSIZ+1]; /* nombre del barrio */
    u1 calles; /* numero de calles que tiene el barrio */
} Barrio;

typedef struct {
    u1 id;
    char nombre[NOMSIZ+1];
    s2 dinero;
    s1 casilla;
    struct {
        u1 condenado : 1;
        u1 carta : 1;
        u1 humano : 1;
    };
    u1 posesion[MATPOS];
} Jugador;

typedef Casilla Tablero[TABSIZ];
typedef Barrio Barrios[BARSIZ];
typedef Jugador Jugadores[JUGMAXSIZ];

extern Tablero tablero;
extern Barrios barrios;
extern Jugadores jugadores;

extern u1 numero_jugadores;

void tabinit();
/* define el tablero */

void tabprt();
/* imprime todo el tablero */

void juginit();
/* define todos los jugadores y el numero de jugadores de la partida */



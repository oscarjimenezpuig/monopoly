/* MONOPOLY 30/4/26 */

#include "cartas.h"

/* CARTAS COMUNIDAD */

#define CCSIZ 16 /* tamaño de las cartas de comunidad */

#define fcc(A) void fcc_##A(Jugador* j)
#define ncr(S) printf("%s.\n",S);
#define paga(A) printf("Paga: %i.\n",(A));j->dinero-=(A);
#define cobra(A) printf("Cobra: %i.\n",(A));j->dinero+=(A);

fcc(0){
    j->casilla=0;
    ncr("Ves a la casilla de salida y cobra el premio");
}

fcc(1){
    ncr("Error de la banca a tu favor");
    cobra(200);
}

fcc(2){
    ncr("Pagos al Hospital");
    paga(50);
}

fcc(3){
    ncr("Herencia");
    cobra(100);
}

fcc(4){
    int destino=rnd(0,TABSIZ-1);
    printf("Vas a la casilla %i.\n",destino);
    j->casilla=destino;
}

fcc(5) {
    ncr("Condenado a la carcel");
    j->condenado=1;
}

fcc(6) {
    ncr("Carta de libertad de la carcel");
    j->carta=1;
}

fcc(7) {
    const u1 regalo=10;
    printf("Cumpleaños, recibe %i de regalo de cada jugador.\n",regalo);
    for(int k=0;k<numero_jugadores;k++) {
        Jugador* jj=jugadores+k;
        if(jj!=j && jj->dinero>=0) {
            jj->dinero-=regalo;
            j->dinero+=regalo;
        }

    }
}

fcc(8) {
    ncr("Cobro de dividendos");
    cobra(50);
}

fcc(9) {
    ncr("Impuesto sobre la renta");
    paga(100);
}

fcc(10) {
    ncr("Ingreso Hospital por enfermedad grave");
    paga(100);
}

fcc(11) {
    ncr("Cobro de intereses");
    cobra(25);
}

fcc(12) {
    ncr("Venta de acciones");
    cobra(50);
}

fcc(13) {
    ncr("Tasas escolares");
    paga(50);
}

fcc(14) {
    ncr("Participacion en concurso de belleza");
    cobra(10);
}

fcc(15) {
    ncr("Devolucion de impuestos");
    cobra(20);
}

typedef void (*Comunidad)(Jugador*);

#define gfcc(A) fcc_##A
static Comunidad comunidad[CCSIZ]={gfcc(0), gfcc(1),gfcc(2),gfcc(3),gfcc(4),gfcc(5),gfcc(6),gfcc(7),gfcc(8),gfcc(9),gfcc(10),gfcc(11),gfcc(12),gfcc(13),gfcc(14),gfcc(15)};

void carta_comunidad(u1 nj) {
    Jugador* j=jugadores+nj;
    int ncarta=rnd(0,CCSIZ-1);
    comunidad[ncarta](j);
}

#undef fcc
#undef gfcc

/* CARTAS SUERTE */

#define fcs(A) void fcs_##A(Jugador* j) 
#define ir(A) j->casilla=(A)

static void busca_casilla_tipo(Jugador* j,u1 t) {
    /* esta funcion busca una casilla del tipo dado lo mas cercana, hacia adelante, desde donde esta
     *  el jugador */
    u1 nc=j->casilla;
    Casilla* pc=tablero+nc+1;
    s1 find=-1;
    while(find==-1) {
        if(pc->tipo==t) find=pc-tablero;
        pc++;
        if(pc==tablero+TABSIZ) pc=tablero;
    }
    j->casilla=find;
}

fcs(0) {
    fcc_0(j);
}

fcs(1) {
    ncr("Avanzaras a la calle mas cercana");
    busca_casilla_tipo(j,CALLE);
}

fcs(2) {
    ncr("Avanzas a la estacion mas cercana");
    busca_casilla_tipo(j,TRENES);
}

fcs(3) {
    ncr("Vas a Espanya");
    ir(19);
}

fcs(4) {
    ncr("Vas a Paseo de Gracia");
    ir(28);
}

fcs(5) {
    ncr("Avanzas al servicio publico mas cercano");
    busca_casilla_tipo(j,NEGOCIO);
}

fcs(6) {
    ncr("Retrocedes tres posiciones");
    ir(j->casilla-3);
}

fcs(7) {
    fcc_5(j);
}

fcs(8) {
    fcc_6(j);
}

fcs(9) {
    ncr("Banco te paga un dividendo");
    cobra(50);
}

fcs(10) {
    ncr("Reparaciones, pagaras 25 por casa y 100 por hotel");
    u2 penal=0;
    for(int n=0;n<TABSIZ;n++) {
        Casilla c=tablero[n];
        if(c.tipo==CALLE && c.comprable.poseedor==j->id) {
            penal+=c.comprable.calle.casas*25;
            penal+=c.comprable.calle.hotel*100;
        }
    }
    j->dinero-=penal;
}

fcs(11) {
    ncr("Multa por exceso de velocidad");
    paga(15);
}

fcs(12) {
    ncr("Has sido elegido Presidente. Pagaras 50 a cada jugador");
    Jugador* pj=jugadores;
    while(pj!=jugadores+numero_jugadores) {
        if(pj!=j && pj->dinero>=0) {
            j->dinero-=50;
            pj->dinero+=50;
        }
        pj++;
    }
}

fcs(13) {
    ncr("Vencimiento del prestamo");
    cobra(150);
}

fcs(14) {
    ncr("Ganas concurso de Iniciativa Profesional");
    cobra(100);
}

fcs(15) {
    ncr("Vas a Sagrada Familia");
    ir(39);
}

typedef void (*Suerte)(Jugador*);

#define CSSIZ 16 /* numero de cartas suerte */

#define gfcs(A) fcs_##A

static Suerte suerte[CSSIZ]={gfcs(0),gfcs(1),gfcs(2),gfcs(3),gfcs(4),gfcs(5),gfcs(6),gfcs(7),gfcs(8),gfcs(9),gfcs(10),gfcs(11),gfcs(12),gfcs(13),gfcs(14),gfcs(15)};

void carta_suerte(u1 nj) {
    Jugador* j=jugadores+nj;
    int ncarta=rnd(0,CSSIZ-1);
    suerte[ncarta](j);
}

#undef fcs
#undef gfcs





    














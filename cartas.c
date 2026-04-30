/* MONOPOLY 30/4/26 */

#include "cartas.h"

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
static Comunidad comunidad[CCSIZ]={fcc(1),fcc(2),fcc(3),fcc(4),fcc(5),fcc(6),fcc(7),fcc(8),fcc(9),fcc(10),fcc(11),fcc(12),fcc(13),fcc(14),fcc(15)};

void carta_comunidad(u1 nj) {
    Jugador* j=jugadores+nj;
    int ncarta=rnd(0,CCSIZ-1);
    comunidad[ncarta](j);
}






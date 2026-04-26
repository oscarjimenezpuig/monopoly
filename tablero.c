/* MONOPOLY 24/4/26 */

#include "tablero.h"

Tablero tablero;
Barrios barrios;
Jugadores jugadores;

static char* copy(char* d,char* o) {
    /* copia dos cadenas */
    char* po=o;
    char* pd=d;
    while(*po!=EOS) *pd++=*po++;
    *pd=EOS;
    return d;
}

static void barnew(u1 id,char* nombre) {
    Barrio* b=barrios+id;
    copy(b->nombre,nombre);
    b->id=id;
}

static void barinit() {
    /* creacion de todos los barrios */
    barnew(0,"Gotic");
    barnew(1,"El Born");
    barnew(2,"Gracia");
    barnew(3,"Sants");
    barnew(4,"Poble Sec");
    barnew(5,"Eixample");
    barnew(6,"Les Corts");
    barnew(7,"Sant Marti");
}

#define dal(A,B,C,D,E,F) (u2[]){A,B,C,D,E,F}

static Casilla* casnew(u1 numero,u1 tipo,char* nombre) {
    /* creacion de una casilla */
    Casilla* c=tablero+numero;
    c->numero=numero;
    c->tipo=tipo;
    copy(c->nombre,nombre);
    return c;
}

static Comprable* compnew(Casilla* c,u2 precio) {
    /* creacion de un comprable */
    Comprable* cm=&(c->comprable);
    cm->precio=precio;
    cm->poseedor=0;
    return cm;
}

static void callnew(u1 numero,char* nombre,u1 barrio,u2 precio,u2 precio_casa,u2* alquiler) {
    Casilla* c=casnew(numero,CALLE,nombre);
    Comprable* cm=compnew(c,precio);
    cm->calle.barrio=barrio;
    cm->calle.precio_casa=precio_casa;
    cm->calle.casas=0;
    cm->calle.hotel=0;
    for(u1 k=0;k<NUMCASHOT;k++) {
        cm->calle.alquiler[k]=alquiler[k];
    }
} 

static void callprt(Casilla c) {
    Barrio b=barrios[c.comprable.calle.barrio];
    printf("Num: %i Carrer: %s Barri: %s",c.numero,c.nombre,b.nombre);NLN;
    if(c.comprable.poseedor) {
        Jugador j=jugadores[c.comprable.poseedor];
        TAB;printf("Propietario: %s",j.nombre);NLN;
        u1 casas=(c.comprable.calle.casas);
        u1 hotel=(c.comprable.calle.hotel);
        TAB;printf("Casas; %hhi Hoteles; %hhi",casas,hotel);NLN;
        TAB;printf("Alquiler: %hhi",c.comprable.calle.alquiler[casas+hotel]);NLN;
    } else {
        TAB;printf("Precio terreno: %hhi",c.comprable.precio);NLN;
        TAB;printf("Precio casa: %hhi",c.comprable.calle.precio_casa);NLN;
    }
}   

static void callinit() {
    /* se definen todas las calles */
    callnew(1,"Avinyo",0,60,50,dal(2,10,30,90,160,250));
    callnew(3,"Tallers",0,60,50,dal(4,20,60,180,320,450));
    callnew(6,"Princesa",1,100,50,dal(6,30,90,270,400,550));
    callnew(8,"Passeig del Born",1,100,50,dal(6,30,90,270,400,550));
    callnew(9,"Montcada",1,120,50,dal(8,40,100,300,450,600));
    callnew(11,"Verdi",2,140,100,dal(10,50,150,450,625,750));
    callnew(13,"Gran de Gracia",2,140,100,dal(10,50,150,450,625,750));
    callnew(14,"Travessera de Gracia",2,160,100,dal(12,60,180,500,700,900));
    callnew(16,"Sants",3,180,100,dal(14,70,200,550,750,950));
    callnew(18,"Creu Coberta",3,180,100,dal(14,70,200,550,750,950));
    callnew(19,"Espanya",3,200,100,dal(16,80,220,600,800,1000));
    callnew(21,"Paral.lel",4,220,150,dal(18,90,250,700,875,1050));
    callnew(23,"Avinguda Paral.lel",4,220,150,dal(18,90,250,700,875,1050));
    callnew(24,"Blai",4,240,150,dal(20,100,300,750,925,1100));
    callnew(26,"Rambles",5,260,150,dal(22,110,330,800,975,1150));
    callnew(28,"Passeig de Gracia",5,280,150,dal(24,120,360,850,1015,1200));
    callnew(29,"Catalunya",5,300,150,dal(26,130,390,900,1100,1275));
    callnew(31,"Diagonal",6,300,200,dal(26,130,390,900,1100,1275));
    callnew(32,"Numancia",6,300,200,dal(26,130,390,900,1100,1275));
    callnew(34,"Entensa",6,320,200,dal(28,150,450,1000,1200,1400));
    callnew(37,"Meridiana",7,350,200,dal(35,175,500,1100,1300,1500));
    callnew(39,"Sagrada Familia",7,400,200,dal(50,200,600,1400,1700,2000));
}

static void negnew(u1 numero,char* nombre,u2 precio,u2 alquiler_base) {
    Casilla* c=casnew(numero,NEGOCIO,nombre);
    Comprable* cm=compnew(c,precio);
    cm->negocio.alquiler_base=alquiler_base;
    cm->negocio.alquiler=alquiler_base*dado(1);
}

static void negprt(Casilla c) {
    printf("Num: %i Nom: %s");NLN;
    if(c.comprable.poseedor) {
        Jugador j=jugadores[c.comprable.poseedor];
        TAB;printf("Propietario: %s",j.nombre);NLN;
        TAB;printf("Alquiler: %i",c.comprable.negocio.alquiler);NLN;
    } else {
        TAB; printf("Precio: %i",c.comprable.precio);
    }
}

static void trsnew(u1 numero,char* nombre,u2 precio,u2 alquiler) {
    Casilla* c=casnew(numero,TRANSPORTE,nombre);
    Comprable* cm=compnew(c,precio);
    cm->transporte.alquiler=alquiler;
}




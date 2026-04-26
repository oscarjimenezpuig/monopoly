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
    printf("%02i. Calle %s (%s)",c.numero,c.nombre,b.nombre);NLN;
    if(c.comprable.poseedor) {
        Jugador j=jugadores[c.comprable.poseedor];
        TAB;printf("Propietario: %s",j.nombre);NLN;
        u1 casas=(c.comprable.calle.casas);
        u1 hotel=(c.comprable.calle.hotel);
        TAB;printf("Casas: %i Hoteles: %i",casas,hotel);NLN;
        TAB;printf("Alquiler: %i",c.comprable.calle.alquiler[casas+hotel]);NLN;
    } else {
        TAB;printf("Precio terreno: %i",c.comprable.precio);NLN;
        TAB;printf("Precio casa: %i",c.comprable.calle.precio_casa);NLN;
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

static void negnew(u1 numero,char* nombre,u2 alquiler_base) {
    Casilla* c=casnew(numero,NEGOCIO,nombre);
    Comprable* cm=compnew(c,150);
    cm->negocio.alquiler_base=alquiler_base;
    cm->negocio.alquiler=alquiler_base*dado(1);
}

static void negprt(Casilla c) {
    printf("%02i. %s",c.numero,c.nombre);NLN;
    if(c.comprable.poseedor) {
        Jugador j=jugadores[c.comprable.poseedor];
        TAB;printf("Propietario: %s",j.nombre);NLN;
        TAB;printf("Alquiler: %i",c.comprable.negocio.alquiler);NLN;
    } else {
        TAB; printf("Precio: %i",c.comprable.precio);NLN;
    }
}

static void neginit() {
    negnew(12,"Companyia Electrica",10);
    negnew(27,"Aigues de Barcelona",4);
}

static void trenew(u1 numero,char* nombre) {
    Casilla* c=casnew(numero,TRENES,nombre);
    Comprable* cm=compnew(c,200);
    cm->tren.alquiler_base=25;
    cm->tren.estaciones=cm->tren.alquiler=0;
}

static void treprt(Casilla c) {
    printf("%02i. %s",c.numero,c.nombre);NLN;
    if(c.comprable.poseedor) {
        Jugador j=jugadores[c.comprable.poseedor];
        TAB;printf("Propietario: %s",j.nombre);NLN;
        TAB;printf("Alquiler: %i",c.comprable.tren.alquiler);NLN;
    } else {
        TAB; printf("Precio: %i",c.comprable.precio);NLN;
    }
}

static void treinit() {
    trenew(5,"Estacion de França");
    trenew(15,"Estacion de Sants");
    trenew(25,"Estacion de Passeig de Gracia");
    trenew(35,"Estacion del Clot");
}

static void cacprt(Casilla c) {
    printf("%02i. Caja de Comunidad",c.numero);NLN;
}

static void cacinit() {
    /* inicio de todas las casillas de caja de comunidad */
    u1 ccc[]={2,17,33};
    for(u1 k=0;k<3;k++) {
        casnew(ccc[k],COMUNIDAD,"Caja de Comunidad");
    }
}

static void sueprt(Casilla c) {
    printf("%02i. Suerte",c.numero);NLN;
}

static void sueinit() {
    /* inicio de todas las casillas de suerte */
    u1 sc[]={7,22,36};
    for(u1 k=0;k<3;k++) {
        casnew(sc[k],SUERTE,"Suerte");
    }
}

static void impnew(u1 numero,char* nombre,u2 impuesto) {
    Casilla* c=casnew(numero,IMPUESTO,nombre);
    c->impuesto.impuesto=impuesto;
}

static void impprt(Casilla c) {
    printf("%02i. Pago de %s",c.numero,c.nombre);NLN;
    TAB;printf("Tasa: %i",c.impuesto.impuesto);NLN;
}

static void impinit() {
    impnew(4,"Impuesto sobre ingresos",200);
    impnew(38,"Impuesto de lujo",100);
}

static void espprt(Casilla c) {
    printf("%02i. %s",c.numero,c.nombre);NLN;
    if(c.tipo==SALIDA) {
        TAB;printf("Premio: %i",c.salida.premio);NLN;
    } else if(c.tipo==IR_CARCEL) {
        TAB;printf("Multa: %i",c.carcel.sancion);NLN;
    }
}

static void espinit() {
    /* Todas las casillas especiales */
    Casilla* cs=casnew(0,SALIDA,"Salida");
    cs->salida.premio=200;
    casnew(20,PARKING,"Parking Gratuito");
    casnew(10,CARCEL,"Carcel");
    Casilla* ic=casnew(30,IR_CARCEL,"Condenado a la Carcel");
    ic->carcel.sancion=50;
}

void tabinit() {
    barinit();
    callinit();
    neginit();
    treinit();
    cacinit();
    sueinit();
    impinit();
    espinit();
}

void tabprt() {
    for(int k=0;k<TABSIZ;k++) {
        Casilla c=tablero[k];
        switch(c.tipo) {
            case SALIDA:
            case PARKING:
            case CARCEL:
            case IR_CARCEL:
                espprt(c);
                break;
            case CALLE:
                callprt(c);
                break;
            case NEGOCIO:
                negprt(c);
                break;
            case TRENES:
                treprt(c);
                break;
            case COMUNIDAD:
                cacprt(c);
                break;
            case SUERTE:
                sueprt(c);
                break;
            case IMPUESTO:
                impprt(c);
                break;
        }
    }
}

/* prueba */

int main() {
    tabinit();
    tabprt();
}



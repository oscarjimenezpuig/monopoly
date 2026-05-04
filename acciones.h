/* MONOPOLY 27/4/26 */

#include "cartas.h"

#define PVC 80 /* tanto por ciento del precio de venta de una casilla */

s2 pagar_alquiler(u1 jugador);
/* rutina donde se paga el alquiler de una poblacion en funcion de si es o no suya
 * numero positivo: representa el alquiler a pagar
 * -1 : No debe pagar alquiler
 */

s1 comprar(u1 jugador);
/* un jugador compra una casilla 
 * 1: Comprado
 * -1: No hay suficiente dinero
 * -2: No esta libre
 * -3: No es casilla comprable
 */

u1 puede_comprar_casa(u1 jugador,u1* casillas);
/* esta funcion dice si nos podemos comprar una casa en alguna de las casillas de nuestra propiedad
 * la salida es un vector donde se dan todas las casillas donde podemos comprar y el retorno es el 
 * numero de casillas que podemos comprar */

s1 comprar_casa(u1 jugador,u1 casilla);
/* se compra una casa en un terreno de nuestra posesion
 * 2: Compra hotel
 * 1: Compra casa
 * -1: No hay dinero suficiente
 * -2: Casilla donde no se puede comprar casa
 * -3: Sin barrios donde se puede comprar casa
 */

s2 precio_venta(u1 casilla);
/* se obtiene el precio de venta de una casilla */

u1 puede_vender(u1 jugador,u1* casillas,u2* venta);
/* esta funcion dice si un jugador puede vender casillas, da el numero de casillas asi como 
 * el precio de venta */

s1 vender(u1 jugador,u1 casilla);
/* un jugador pone a la venta la casilla
 * 1: Vendido
 * -1: No es tenido
 * -2: No es vendible
 */

s1 subastar(u1 subastador,u1* ofertas);
/* se realiza una subasta donde subastador es el jugador que subasta y por lo tanto no opta */
/* ofertas es un vector que contiene todas las ofertas de los jugadores excepto la del subastador */

void ir_carcel(u1 jugador);
/* el jugador va a la carcel se establece la condicion de condenado y se mueve a la casilla de carcel*/

s1 en_carcel(u1 jugador);
/* rutina que evalua si un jugador esta en la carcel
 * 1: Sale de la carcel porque tienes la carta de perdon 
 * 2: Sales de la carcel por un error judicial
 * 3: Sales de la carcel porque pagas la multa
 * 0: No sales de la carcel
 */

u1 extrae_comunidad(u1 jugador);
/* jugador extrae carta de comunidad */

u1 extrae_suerte(u1 jugador);
/* jugador extrae carta de suerte */

s1 no_arruinado(u1 jugador);
/* comprueba si un jugador esta arruinado y coloca la bandera de arruinado */
/* todas sus posesiones pasan al mercado libre y desaparecen casas y hoteles */
/* 1 : No esta arruinado
 * 0: Esta arruinado
 * -1: Ya estaba arruinado
 */

u1 es_ganador(u1 jugador);
/* esta funcion dice si el jugador es ganador */

u1 casilla_actual(u1 jugador);
/* se comprueba en que casilla esta el jugador
 * 3: Suerte
 * 2: Comunidad
 * 1: Puede comprar
 * 0: Nada especial
 * -1: Alquila
 * -2: Ir carcel
 *  -3: Impuesto
 */

u1 mover(u1 jugador);
/* funcion que mueve el jugador a lo largo del tablero, dice si gana premio
 * 1: Movimiento realizado
 * 2: Pasa por salida
 * 4: Saca doble
 */

void subasta_flag_on(u1 jugador);
/* se conecta la bandera de subasta */

void subasta_flag_off();
/* se desconecta la bandera de subasta */

s1 subasta_flag_get();
/* da el valor de la bandera de subasta */




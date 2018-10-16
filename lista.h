#ifndef LISTA_H
#define LISTA_H

#include <stdlib.h>
#include <stdbool.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/
typedef struct lista lista_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista
// Post: Devuelve una lista vacía. NULL en caso de error.
lista_t *lista_crear(void);

//Pre: La lista fue creada
//Post: Devuelve verdadero o falso, si la lista contiene elementos o no.
bool lista_esta_vacia(const lista_t *lista);

//Pre: La lista fue creada
//Post: Inserta un nuevo elemento en la primera posicion de la lista.
//		Devuelve verdadero si se agrego correctamente, false en caso de error. 
bool lista_insertar_primero(lista_t *lista, void *dato);

//Pre: La lista fue creada
//Post: Inserta un nuevo elemento en la ultima posicion de la lista.
//		Devuelve verdadero si se agrego correctamente, false en caso de error. 
bool lista_insertar_ultimo(lista_t *lista, void *dato);

//Pre: La lista fue creada
//Post: Se elimina el primer elemento de la lista y se devuelve el valor del elemento, si no esta vacia
void *lista_borrar_primero(lista_t *lista);

//Pre: La lista fue creada
//Post: Devuelve el valor del primer elemento de la lista, NULL si esta vacia
void *lista_ver_primero(const lista_t *lista);

//Pre: La lista fue creada
//Post: Devuelve el valor del ultimo elemento de la lista, NULL si esta vacia
void *lista_ver_ultimo(const lista_t* lista);

//Pre: La lista fue creada
//Post: Devuelve la cantidad de elementos que contiene la lista
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

typedef struct lista_iter lista_iter_t;

// Crea un iterador externo para recorrer la lista pasada por parámetro.
// Pre: La lista fue creada.
// Post: Se obtiene un iterador para recorrer la lista o NULL si hubo un error en la creación.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza un iterador a la siguiente posicion de la lista.
// Pre: Iterador fue creado
// Post: Devuelve verdadero si avanzó el iterador a la posicion siguiente, falso en caso contrario.
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve el elemento de la posicion actual del iterador
// Pre: Iterador fue creado
// Post: Devuelve el elemento de la posicion del iterador, NULL en caso de que la lista este vacia.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Verifica si el iterador apunta al final de la lista, no hay más elementos.
// Pre: Iterador fue creado
// Post: Devuelve verdadero si el iterador está al final de la lista, falso en caso contrario.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye al iterador
// Pre: Iterador fue creado
// Post: Se destruye al iterador
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un elemento en la lista
// Pre: Iterador fue creado
// Post: Inserta un elemento en la posición a la que estaba apuntando el iterador
// El iterador queda apuntando a la posicion del dato insertado.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Elimina el elemento de la posicion a la que apunta el iterador
// Pre: Iterador fue creado
// Post: El iterador elimina el elemento y devuelve el dato que contenía, pasa a puntar al elemento siguiente.
// En caso de que no se haya podido borrar la posicion (ej. lista vacía) devuelve NULL.
void *lista_iter_borrar(lista_iter_t *iter);

// Recorre la lista aplicando la función visitar a cada uno de los elementos.
// Pre: La lista fue creada
// Post: Se aplica visitar a cada uno de los miembros
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


#endif
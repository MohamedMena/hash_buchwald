#include "lista.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct nodo{
	void* dato;
	struct nodo* siguiente;
}nodo_t;

struct lista{
	nodo_t* primero;
	nodo_t* ultimo;
	size_t tamanio;
};

struct lista_iter{
	nodo_t* anterior;
	nodo_t* actual;
	lista_t* lista;
};

nodo_t* lista_crear_nodo(void* dato){
	nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
	if(!nuevo_nodo) return NULL;

	nuevo_nodo->dato = dato;
	nuevo_nodo->siguiente = NULL;

	return nuevo_nodo;
}

lista_t *lista_crear(){
	lista_t* nueva_lista = malloc(sizeof(lista_t));

	if(!nueva_lista) return NULL;

	nueva_lista->primero = NULL;
	nueva_lista->ultimo = NULL;
	nueva_lista->tamanio = 0;

	return nueva_lista;
}

bool lista_esta_vacia(const lista_t *lista){
	return lista->tamanio == 0 && !lista->primero && !lista->ultimo;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_t* nodo_a_insertar = lista_crear_nodo(dato);

	if(!nodo_a_insertar) return false;

	if(lista_esta_vacia(lista)){
		lista->ultimo = nodo_a_insertar;
	}else{
		nodo_a_insertar->siguiente = lista->primero;
	}
	lista->primero = nodo_a_insertar;
	lista->tamanio++;

	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
	nodo_t* nodo_a_insertar = lista_crear_nodo(dato);
	if(!nodo_a_insertar) return false;

	if(lista_esta_vacia(lista)){
		lista->primero = nodo_a_insertar;
	}else{
		lista->ultimo->siguiente = nodo_a_insertar;
	}

	lista->ultimo = nodo_a_insertar;
	lista->tamanio++;

	return true;
}

void *lista_borrar_primero(lista_t *lista){
	if(lista_esta_vacia(lista)) return NULL;

	void* dato = lista->primero->dato;
	nodo_t* nodo_a_eliminar = lista->primero;
	if(lista->primero == lista->ultimo){
		lista->primero = NULL;
		lista->ultimo = NULL;
	}else{
		lista->primero = nodo_a_eliminar->siguiente;
	}

	free(nodo_a_eliminar);
	lista->tamanio--;

	return dato;
}

void* lista_ver_primero(const lista_t *lista){
	if(lista_esta_vacia(lista)) return NULL;

	return lista->primero->dato;
}

void* lista_ver_ultimo(const lista_t* lista){
	if(lista_esta_vacia(lista)) return NULL;

	return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista){
	return lista->tamanio;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)){
	while(!lista_esta_vacia(lista)){
		void* dato_a_destruir = lista_borrar_primero(lista);
		if(destruir_dato != NULL) destruir_dato(dato_a_destruir);
	}
	free(lista);
}

lista_iter_t *lista_iter_crear(lista_t *lista){

	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if(!iter) return NULL;

	iter->lista = lista;
	iter->anterior = NULL;
	iter->actual = lista->primero;

	return iter;

}

bool lista_iter_avanzar(lista_iter_t *iter){
	if(!iter->actual) return false;

	iter->anterior = iter->actual;
	iter->actual = iter->actual->siguiente;

	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	if(!iter->actual) return NULL;

	return (iter->actual->dato);
}

bool lista_iter_al_final(const lista_iter_t *iter){
	return (!iter->actual);
}

void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){

	if(iter->actual == iter->lista->primero){
		if(lista_insertar_primero(iter->lista, dato)){
			iter->actual = iter->lista->primero;
			return true;
		}
	}else if (iter->actual == iter->lista->ultimo){
		if(lista_insertar_ultimo(iter->lista, dato)){
			iter->anterior = iter->actual;
			iter->actual = iter->lista->ultimo;
			return true;
		}
	}else{
		nodo_t* nodo_a_insertar = lista_crear_nodo(dato);
		if(nodo_a_insertar != NULL){
			if (!iter->actual) iter->lista->ultimo = nodo_a_insertar;
			nodo_a_insertar->siguiente = iter->actual;
			iter->anterior->siguiente = nodo_a_insertar;
			iter->actual = nodo_a_insertar;
			iter->lista->tamanio++;
			return true;
		}
	}
	return false;
}

void *lista_iter_borrar(lista_iter_t *iter){
	if(!iter->actual) return NULL;

	nodo_t* a_borrar = iter->actual;
	void* dato_borrado = a_borrar->dato;

	if(iter->actual == iter->lista->primero){
		dato_borrado = lista_borrar_primero(iter->lista);
		iter->actual = iter->lista->primero;
		return dato_borrado;
	}else if(iter->actual == iter->lista->ultimo){
		iter->lista->ultimo = iter->anterior;
	}else{
		iter->anterior->siguiente = a_borrar->siguiente;
	}

	iter->actual = a_borrar->siguiente;

	iter->lista->tamanio--;
	free(a_borrar);
	return dato_borrado;
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	nodo_t* actual = lista->primero;
	bool continuar_iterando = true;
	while(actual != NULL && continuar_iterando){
		continuar_iterando = visitar(actual->dato, extra);
		actual = actual->siguiente;
	}
}
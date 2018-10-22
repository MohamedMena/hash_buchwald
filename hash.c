#include "hash.h"
#include "lista.h"
#include <stdlib.h>
#include <string.h>

#define TAMANIO_INICIAL 20

/* DEFINICION ESTRUCTURAS */
struct hash{
	lista_t** tabla;
	size_t cantidad;
	size_t capacidad;
	hash_destruir_dato_t destruir_dato;
};

typedef struct nodo_hash{
	 const char* clave;
	 void* dato;
}nodo_hash_t;
 
 /* Definición del struct hash iterador.*/
struct hash_iter{
	 size_t pos_actual;
	 const hash_t* hash;
	 lista_iter_t* iter_actual;
 };

size_t hash_iter_buscar_siguiente(lista_t** tabla, size_t pos_inicio, size_t capacidad){
	size_t nueva_posicion = capacidad;
	bool encontro_siguiente = false;
	for (int i = pos_inicio; i < capacidad && !encontro_siguiente; ++i){
		encontro_siguiente = (tabla[i] != NULL);
		if(encontro_siguiente) nueva_posicion = i;
	}
	if(!encontro_siguiente && pos_inicio == 0) nueva_posicion = 0;

	return nueva_posicion;
}


char *duplicar_clave(const char *clave){
	size_t largo = strlen(clave) + 1;
	char *dup_clave = malloc(sizeof(char)*largo);
	if(!dup_clave) return NULL;

	strcpy(dup_clave, clave);
	return dup_clave;
}

nodo_hash_t* hash_nodo_crear(const char* clave, void* dato){
	nodo_hash_t* nuevo_nodo = malloc(sizeof(nodo_hash_t));
	if(!nuevo_nodo) return NULL;

	nuevo_nodo->clave = duplicar_clave(clave);
	nuevo_nodo->dato = dato;
	return nuevo_nodo;
}

//FUNCION DE HASHING
size_t hashing(const char* clave, size_t tamanio_tabla) {
	//unsigned char *p = clave;
	unsigned long number = 5381;
	int c;
	while ((c = *clave++)){
		number = ((number << 5) + number) + c;
	}
	return (number % tamanio_tabla);
}


hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash) return NULL;

	hash->tabla = malloc(sizeof(lista_t*)*TAMANIO_INICIAL);
	if(!hash->tabla){
		free(hash);
		return NULL;
	}
	hash->capacidad = TAMANIO_INICIAL;
	hash->cantidad = 0;
	hash->destruir_dato = destruir_dato;
	return hash;
}

hash_nodo_t* hash_buscar_nodo(const lista_t *lista_actual, const char *clave){

	lista_iter_t* iterador = lista_iter_crear(lista_actual);
	while(!lista_iter_al_final(lista_actual)){
		hash_nodo_t* nodo_actual = lista_iter_ver_actual(lista_actual);
		if(strcmp(nodo_actual->clave, clave) == 0){
			lista_iter_destruir(iterador);
			return nodo_actual;
		}
		lista_iter_avanzar(iterador);
	}
	lista_iter_destruir(iterador);
	return NULL;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){

	size_t posicion_hash = hashing(clave, hash->capacidad);

	if(hash->tabla[posicion_hash]){
		//BUSCAR Y REEMPLAZAR
		nodo_hash_t* nodo_a_actualizar = hash_buscar_nodo(hash->tabla[posicion_hash], clave);
		if(nodo_a_actualizar){
			nodo_a_actualizar->dato = dato;
			return true;
		}
	}else{
		hash->lista[posicion_hash] = lista_crear();

		nodo_hash_t* nuevo_nodo = hash_nodo_crear(clave, dato);
		if(!nuevo_nodo) return false;

		if(!lista_insertar_ultimo(hash->tabla[posicion_hash], nuevo_nodo)){
			free(nuevo_nodo->clave);
			free(nuevo_nodo);
			return false;
		}
	}
	hash->cantidad++;

	return true;
}

hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* iterador = malloc(sizeof(hash_iter_t));
	if(!iterador) return NULL;

	iterador->hash = hash;
	iterador->pos_actual = hash_iter_buscar_siguiente(hash->tabla, 0, hash->capacidad);
	iterador->iter_actual = NULL;
	if(iterador->hash->tabla[iterador->pos_actual] != NULL){
		iterador->iter_actual = lista_iter_crear(iterador->hash->tabla[iterador->pos_actual]);
	}
	return iterador;
}

bool hash_iter_avanzar(hash_iter_t *iter){
	bool avanzo = false;
	if(iter->iter_actual != NULL){
		avanzo = lista_iter_avanzar(iter->iter_actual);
		if(lista_iter_al_final(iter->iter_actual)){
			lista_iter_destruir(iter->iter_actual);
			iter->pos_actual = hash_iter_buscar_siguiente(iter->hash->tabla, iter->pos_actual+1, iter->hash->capacidad);
			if(iter->pos_actual == iter->hash->capacidad){
				iter->iter_actual = NULL;
				return false;
			}
			iter->iter_actual = lista_iter_crear(iter->hash->tabla[iter->pos_actual]);
		}
	}
	return avanzo;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if(!iter->iter_actual) return NULL;
	if(lista_iter_al_final(iter->iter_actual)) return NULL;

	nodo_hash_t* nodo_actual = lista_iter_ver_actual(iter->iter_actual);
	return nodo_actual->clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
	if(!iter->iter_actual) return true;

	if(lista_iter_al_final(iter->iter_actual) && iter->pos_actual == iter->hash->capacidad-1) return true;

	return false;
}

void hash_iter_destruir(hash_iter_t* iter){
	lista_iter_destruir(iter->iter_actual);
	free(iter);
}
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
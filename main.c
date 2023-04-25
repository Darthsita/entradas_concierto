#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Definimos la estructura Persona
typedef struct persona {
    char rut[12];
    char nombre[100];
    int entradas;
    struct persona* siguiente;
} Persona;
Persona* leerArchivoTxt(char* nombreArchivo) { //abrimos el archivo de txt y extraemos la data
    FILE* archivo = fopen(strcat(nombreArchivo, ".txt"), "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        exit(1);
    }
    Persona* primerNodo = NULL;
    Persona* ultimoNodo = NULL;
    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        Persona* nuevaPersona = (Persona*) malloc(sizeof(Persona));
        sscanf(linea, "%[^,],%[^,],%d,%[^,\n]", nuevaPersona->rut, nuevaPersona->nombre, &nuevaPersona->entradas);
        nuevaPersona->siguiente = NULL;
        if (primerNodo == NULL) {
            primerNodo = nuevaPersona;
            ultimoNodo = nuevaPersona;
        } else {
            ultimoNodo->siguiente = nuevaPersona;
            ultimoNodo = nuevaPersona;
        }
    }
    fclose(archivo);
    return primerNodo;
}
Persona* buscarPersona(Persona* lista, char* rut) { //buscamos a la persona por su rut para evitar que este 2 veces en la lista
    Persona* actual = lista;
    while (actual != NULL) {
        if (strcmp(actual->rut, rut) == 0) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}
Persona* sumarEntradas(Persona* listaPersonas) {
    Persona* listaNueva = NULL;
    // Recorremos la lista original
    while (listaPersonas != NULL) {
        char* rut = listaPersonas->rut;
        int entradas = listaPersonas->entradas;
        // Buscamos si ya existe una persona con el mismo RUT en la lista nueva
        Persona* personaExistente = buscarPersona(listaNueva, rut);
        if (personaExistente == NULL) {
            // Si no existe, creamos una nueva persona y la agregamos a la lista nueva
            Persona* nuevaPersona = (Persona*) malloc(sizeof(Persona));
            strcpy(nuevaPersona->rut, rut);
            strcpy(nuevaPersona->nombre, listaPersonas->nombre);
            nuevaPersona->entradas = entradas;
            nuevaPersona->siguiente = listaNueva;
            listaNueva = nuevaPersona;
        } else {
            // Si ya existe, simplemente sumamos las entradas
            personaExistente->entradas += entradas;
        }

        listaPersonas = listaPersonas->siguiente;
    }
    return listaNueva;
}
void asignarEntradasAsg(Persona* lista, char* datos1) {
    int asignadas = 0, pendientes = 0;
    char nombreArchivo[50];
    strcpy(nombreArchivo, datos1);
    strcat(nombreArchivo, ".asg");
    if (lista == NULL) {
        printf("Error: La lista de personas está vacía.\n");
        return;
    }
    for (Persona* p = lista->siguiente; p != NULL && asignadas < 50; p = p->siguiente) {
        int entradasAsignadas = 0;

        if (p->entradas <= 2) {
            // No hay penalización
            entradasAsignadas = p->entradas;
        } else if (p->entradas >= 3 && p->entradas <= 5) {
            // Penalización
            entradasAsignadas = 2;
        }

        if (asignadas + entradasAsignadas <= 50) {
            // Hay suficientes entradas disponibles
            p->entradas = entradasAsignadas;
            asignadas += entradasAsignadas;
        } else {
            // No hay suficientes entradas disponibles
            pendientes += p->entradas - entradasAsignadas;
        }
    }

    // Verificar que se hayan asignado entradas
    if (asignadas == 0) {
        printf("No se han realizado asignaciones.\n");
        return;
    }
    // Escribir archivo de asignaciones
    FILE* archivo = fopen(nombreArchivo, "w");
    if (archivo != NULL) {
        for (Persona* p = lista->siguiente; p != NULL && p->entradas > 0; p = p->siguiente) {
            fprintf(archivo, "%s %s %d\n", p->rut, p->nombre, p->entradas);
        }
        fprintf(archivo, "Asignadas: %d\n", asignadas);
        fprintf(archivo, "Pendientes: %d\n", pendientes);
        fclose(archivo);
    }
}
void asignarEntradasEsp(Persona* lista, char* datos2) {
    int asignadas = 0, pendientes = 0;
    char nombreArchivo[50];
    strcpy(nombreArchivo, datos2);
    strcat(nombreArchivo, ".esp");
    if (lista == NULL) {
        printf("Error: La lista de personas está vacía.\n");
        return;
    }
    for (Persona* p = lista->siguiente; p != NULL && asignadas < 50; p = p->siguiente) {
        int entradasAsignadas = 0;
        if (p->entradas <= 2) {
            // No hay penalización
            entradasAsignadas = p->entradas;
        } else if (p->entradas >= 3 && p->entradas <= 5) {
            // Penalización
            entradasAsignadas = 2;
        }

        if (asignadas + entradasAsignadas <= 50) {
            // Hay suficientes entradas disponibles
            p->entradas = entradasAsignadas;
            asignadas += entradasAsignadas;
        } else {
            // No hay suficientes entradas disponibles
            pendientes += p->entradas - entradasAsignadas;
        }
    }
    // Verificar que se hayan asignado entradas
    if (asignadas == 0) {
        printf("No se han realizado asignaciones.\n");
        return;
    }
    // Escribir archivo de asignaciones
    FILE* archivo = fopen(nombreArchivo, "w");
    if (archivo != NULL) {
        for (Persona* p = lista->siguiente; p != NULL && p->entradas > 0; p = p->siguiente) {
            fprintf(archivo, "%s %s %d\n", p->rut, p->nombre, p->entradas);
        }
        fprintf(archivo, "Asignadas: %d\n", asignadas);
        fprintf(archivo, "Pendientes: %d\n", pendientes);
        fclose(archivo);
    }
}
int main(void) { //ejecutamos las funciones en main
    char nombreArchivo[50], nombreArchivo2[50], nombreArchivoTxt[50], nombreArchivoTxt2[50];
    strcpy(nombreArchivo, "datos1");
    strcpy(nombreArchivo2, "datos2");
    strcpy(nombreArchivoTxt, "datos1");
    strcpy(nombreArchivoTxt2, "datos2");
    Persona* raizNodo = leerArchivoTxt(nombreArchivoTxt);
    raizNodo = sumarEntradas(raizNodo);
    asignarEntradasAsg(raizNodo, nombreArchivo);
    Persona* raizNodo2 = leerArchivoTxt(nombreArchivoTxt2);
    raizNodo2 = sumarEntradas(raizNodo2);
    asignarEntradasEsp(raizNodo2, nombreArchivo2);
    return 0;
}
//ByDarthsita

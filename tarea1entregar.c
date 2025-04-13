#include "tdas/list.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum {BAJA, MEDIA, ALTA} prioridad;

typedef struct {
  int id;
  char descripcion[256];
  time_t fecha_creacion;
  prioridad prioridad_ticket;
} ticket;

const char* prioridad_a_string(prioridad p) {
  switch(p) {
    case ALTA: return "ALTA";
    case MEDIA: return "MEDIA";
    case BAJA: return "BAJA";
    default: return "DESCONOCIDA";
  }
}

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("  Sistema de Gestion de Soporte Tecnico");
  puts("========================================");

  puts("1) Registrar Ticket");
  puts("2) Asignar prioridad a un Ticket");
  puts("3) Mostrar lista de Tickets");
  puts("4) Procesar siguiente Ticket");
  puts("5) Buscar Ticket por ID");
  puts("6) Salir");
}

void mostrar_ticket(ticket *t) {
  char buffer_fecha[30];
  struct tm *info_tiempo = localtime(&t->fecha_creacion);
  strftime(buffer_fecha, sizeof(buffer_fecha), "%Y-%m-%d %H:%M:%S", info_tiempo);

  printf("\nID: %d\n", t->id);
  printf("Descripcion: %s\n", t->descripcion);
  printf("Fecha: %s\n", buffer_fecha);
  printf("Prioridad: %s\n", prioridad_a_string(t->prioridad_ticket));
}

void registrar_ticket(List *baja) {
  printf("Registrar nuevo Ticket\n");
  ticket *nuevo_ticket = (ticket *)malloc(sizeof(ticket));
  if (nuevo_ticket == NULL) {
    printf("Error al asignar memoria para el nuevo ticket.\n");
    return;
  }
  
  printf("\nIngrese ID del ticket: ");
  scanf("%d", &nuevo_ticket->id);
  if (nuevo_ticket->id <= 0) {
    printf("ID invalido. Debe ser un numero positivo.\n");
    free(nuevo_ticket);
    return;
  }
  getchar(); // Limpiar buffer
  
  printf("Ingrese descripcion del ticket: ");
  fgets(nuevo_ticket->descripcion, sizeof(nuevo_ticket->descripcion), stdin);
  nuevo_ticket->descripcion[strcspn(nuevo_ticket->descripcion, "\n")] = '\0';
  
  time(&nuevo_ticket->fecha_creacion);
  nuevo_ticket->prioridad_ticket = BAJA;
  list_pushBack(baja, nuevo_ticket);
}


ticket* buscar_ticket_por_id(List *alta, List *media, List *baja, int id) {
  // Buscar en alta prioridad
  ticket *actual = list_first(alta);
  while(actual != NULL) {
    if(actual->id == id) return actual;
    actual = list_next(alta);
  }
  
  // Buscar en media prioridad
  actual = list_first(media);
  while(actual != NULL) {
    if(actual->id == id) return actual;
    actual = list_next(media);
  }
  
  // Buscar en baja prioridad
  actual = list_first(baja);
  while(actual != NULL) {
    if(actual->id == id) return actual;
    actual = list_next(baja);
  }

  return NULL; 
}

void asignar_prioridad(List *alta, List *media, List *baja) {
  int id;
  char opcion;
  
  printf("\nAsignar prioridad a Ticket\n");
  printf("Ingrese ID del ticket: ");
  scanf("%d", &id);
  
  ticket *buscado = buscar_ticket_por_id(alta, media, baja, id);
  if (buscado == NULL) {
    printf("\nTicket no encontrado.\n");
    return;
  }
  
  printf("\nTicket encontrado:");
  mostrar_ticket(buscado);
  
  printf("\nSeleccione nueva prioridad:\n");
  printf("1) ALTA\n");
  printf("2) MEDIA\n");
  printf("3) BAJA\n");
  printf("4) Cancelar\n");
  printf("Opcion: ");
  scanf(" %c", &opcion);
  
  // Eliminar de lista actual
  List *lista_actual = NULL;
  if(buscado->prioridad_ticket == ALTA) lista_actual = alta;
  else if(buscado->prioridad_ticket == MEDIA) lista_actual = media;
  else lista_actual = baja;
  
  ticket *actual = list_first(lista_actual);
  while(actual != NULL) {
    if(actual->id == id) {
      list_popCurrent(lista_actual);
      break;
    }
    actual = list_next(lista_actual);
  }
  
  // Asignar nueva prioridad
  switch(opcion) {
    case '1':
    buscado->prioridad_ticket = ALTA;
      list_pushBack(alta, buscado);
      break;
    case '2':
    buscado->prioridad_ticket = MEDIA;
      list_pushBack(media, buscado);
      break;
    case '3':
    buscado->prioridad_ticket = BAJA;
      list_pushBack(baja, buscado);
      break;
    case '4':
      list_pushBack(lista_actual, buscado);
      printf("\nOperacion cancelada.\n");
      return;
    default:
      list_pushBack(lista_actual, buscado);
      printf("\nOpcion no valida. No se realizaron cambios.\n");
      return;
  }
  
  printf("\nPrioridad actualizada con exito!\n");
}

void mostrar_tickets_por_prioridad(List *lista, const char *nombre_prioridad) {
  if(list_size(lista) == 0) {
    printf("\nNo hay tickets con prioridad %s.\n", nombre_prioridad);
    return;
  }
  
  printf("\n=== Tickets %s Prioridad ===\n", nombre_prioridad);
  ticket *actual = list_first(lista);
  while(actual != NULL) {
    mostrar_ticket(actual);
    actual = list_next(lista);
  }
}

void mostrar_todos_tickets_ordenados(List *alta, List *media, List *baja) {
  printf("\n=== LISTA DE TICKETS PENDIENTES ===\n");
  printf("=== (Ordenados por prioridad y hora de registro) ===\n");
  
  // Mostrar en orden de prioridad (ALTA -> MEDIA -> BAJA)
  mostrar_tickets_por_prioridad(alta, "ALTA");
  mostrar_tickets_por_prioridad(media, "MEDIA");
  mostrar_tickets_por_prioridad(baja, "BAJA");
}

void procesar_siguiente_ticket(List *alta, List *media, List *baja) {
  // Verificar si hay tickets usando list_size()
  if (list_size(alta) == 0 && list_size(media) == 0 && list_size(baja) == 0) {
      printf("\nNo hay tickets pendientes para procesar.\n");
      return;
  }

  ticket *ticket_a_procesar = NULL;
  const char *prioridad_str = NULL;

  // Seleccionar ticket según prioridad usando list_size()
  if (list_size(alta) > 0) {
      ticket_a_procesar = list_popFront(alta);
      prioridad_str = "ALTA";
  } 
  else if (list_size(media) > 0) {
      ticket_a_procesar = list_popFront(media);
      prioridad_str = "MEDIA";
  } 
  else {
      ticket_a_procesar = list_popFront(baja);
      prioridad_str = "BAJA";
  }

  // Mostrar información del ticket
  printf("\n=== TICKET SELECCIONADO PARA PROCESAR ===\n");
  printf("Prioridad: %s\n", prioridad_str);
  mostrar_ticket(ticket_a_procesar);
  
  // Liberar memoria
  free(ticket_a_procesar);
  printf("\nTicket procesado y eliminado del sistema.\n");
}

void buscar_ticket_mostrar(List *alta, List *media, List *baja) {
  int id;
  printf("\nBuscar Ticket por ID\n");
  printf("Ingrese ID del ticket: ");
  scanf("%d", &id);
  
  ticket *ticket = buscar_ticket_por_id(alta, media, baja, id);
  
  if(ticket == NULL) {
    printf("\nNo se encontro ticket con ID %d\n", id);
  } else {
    printf("\nTicket encontrado:\n");
    mostrar_ticket(ticket);
  }
}

int main() {
  char opcion;
  List *alta = list_create();
  List *media = list_create();
  List *baja = list_create(); 
  
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opcion: ");
    scanf(" %c", &opcion);
    getchar(); // Limpiar buffer

    switch (opcion) {
      case '1':
        registrar_ticket(baja);
        break;
      case '2':
        asignar_prioridad(alta, media, baja);
        break;
      case '3':
        mostrar_todos_tickets_ordenados(alta, media, baja);
        break;
      case '4':
        procesar_siguiente_ticket(alta, media, baja);
        break;
      case '5':
        buscar_ticket_mostrar(alta, media, baja);
        break;
      case '6':
        puts("\nSaliendo del sistema de gestion de soporte tecnico...");
        break;
      default:
        puts("\nOpcion no valida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '6');

  // Liberar recursos
  list_clean(alta);
  list_clean(media);
  list_clean(baja);
  
  return 0;
}
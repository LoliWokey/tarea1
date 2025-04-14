# tarea1
Programa Sistema de Gestion de Soporte Tecnico

# Descripcion
Este sistema permite gestionar tickets de soporte tecnico, facilitando el registro, priorizacion y seguimiento de solicitudes. Esta desarrollado en C y organiza los tickets por prioridad (Alta, Media, Baja) para garantizar una atencion eficiente.

# Requisitos previos
Compilador de C: GCC (Linux/WSL) o MinGW (Windows)

Bibliotecas: Requiere las bibliotecas list.h y extra.h (incluidas en el proyecto)

Editor recomendado: Visual Studio Code con extension C/C++

# Compilacion y ejecucion

Clonar repositorio:
bash
git clone https://github.com/tu-usuario/soporte-tecnico-c.git
cd soporte-tecnico-c

Compilar:

bash
gcc *.c -o soporte_tecnico

Ejecutar:
./soporte_tecnico

# Funcionalidades
Implementadas
Registro de tickets: ID unico, descripcion y fecha automatica

Gestion de prioridades: Asignacion dinamica (Alta/Media/Baja)

Visualizacion: Listado ordenado por prioridad y antiguedad

Procesamiento: Atencion FIFO segun prioridad

# Problemas conocidos
No persiste datos al cerrar el programa

Validacion limitada de entradas

Problemas de ordenamiento de tickets

# Mejoras planeadas
Exportar/importar tickets a CSV

Historial de tickets resueltos

# Ejemplo de uso
Registrar nuevo ticket:
Opcion: 1) Registrar Ticket
ID: 1001
Descripcion: "El servidor no responde"

Cambiar prioridad:
Opcion: 2) Asignar prioridad
ID: 1001
Nueva prioridad: 1) ALTA

Mostrar todos los tickets:
Opcion: 3) Mostrar lista
=== Tickets ALTA Prioridad ===
ID: 1001 | Desc: "El servidor no responde" | Fecha: 2023-11-20 14:30

Atender siguiente ticket:
Opcion: 4) Procesar ticket
=== TICKET SELECCIONADO ===
ID: 1001 | Prioridad: ALTA

buscar ticket por ID:
Opcion: 5) Buscar Ticket por ID
ID: 1001 | Desc: "El servidor no responde" | Fecha: 2023-11-20 14:30

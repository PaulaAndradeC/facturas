#include <stdio.h> 
#include "funciones.h"
#include <string.h> 

int menu () 
{
    int opcion; 
    printf("1. Crear factura\n"); 
    printf("2. Leer factura\n"); 
    printf("3. Editar factura\n"); 
    printf("4. Borrar factura\n"); 
    printf("5. Salir\n"); 
    printf(">> "); 
    scanf("%d", &opcion); 
    return opcion; 
}   

void savefactura (struct Factura *factura) 
{
    FILE *file= fopen("factura.dat", "ab+"); 
    if (file == NULL) 
    {
        printf("Error al abrir el archivo\n"); 
        return; 
    }else{
        fwrite(factura, sizeof(struct Factura), 1, file); 
        printf("factura guardada\n"); 

    } 
    fclose(file); 
} 

void leerCadena (char *cadena,int num)
{
    fflush(stdin); 
    fgets(cadena, num, stdin); 
    int len = strlen (cadena) -1; 
    cadena[len] = '\0'; 
}

void createFactura() {
    struct Factura factura;
    printf("Ingrese el nombre del cliente: ");
    leerCadena(factura.nombre, 20);

    do {
        printf("Ingrese la cedula del cliente: ");
        scanf("%d", &factura.cedula);
        if (factura.cedula < 0) {
            printf("La cedula no puede ser negativa. Intente de nuevo.\n");
        }
    } while (factura.cedula < 0);

    do {
        printf("Ingrese el numero de productos: ");
        scanf("%d", &factura.numProductos);
        if (factura.numProductos <= 0) {
            printf("El numero de productos debe ser mayor a 0. Intente de nuevo.\n");
        }
    } while (factura.numProductos <= 0);

    factura.total = 0;
    for (int i = 0; i < factura.numProductos; i++) {
        printf("Ingrese el nombre del producto: ");
        leerCadena(factura.productos[i].nombre, 50);

        do {
            printf("Ingrese la cantidad del producto: ");
            scanf("%d", &factura.productos[i].cantidad);
            if (factura.productos[i].cantidad <= 0) {
                printf("La cantidad debe ser mayor a 0. Intente de nuevo.\n");
            }
        } while (factura.productos[i].cantidad <= 0);

        do {
            printf("Ingrese el precio del producto: ");
            scanf("%f", &factura.productos[i].precio);
            if (factura.productos[i].precio <= 0) {
                printf("El precio debe ser mayor a 0. Intente de nuevo.\n");
            }
        } while (factura.productos[i].precio <= 0);

        factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
    }
    savefactura(&factura);
}


void readFactura() {
    FILE *file = fopen("factura.dat", "rb");
    struct Factura factura;

    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        printf("\nFactura:\n");
        printf("Cedula: %d\n", factura.cedula);
        printf("Nombre del cliente: %s\n", factura.nombre);
        printf("Productos:\n");
        printf("-------------------------------------------------\n");
        printf("| %-20s | %-10s | %-10s |\n", "Nombre", "Cantidad", "Precio Total");
        printf("-------------------------------------------------\n");

        for (int i = 0; i < factura.numProductos; i++) {
            float precioTotalProducto = factura.productos[i].cantidad * factura.productos[i].precio;
            printf("| %-20s | %-10d | %-10.2f |\n",
                   factura.productos[i].nombre,
                   factura.productos[i].cantidad,
                   precioTotalProducto);
        }

        printf("-------------------------------------------------\n");
        printf("Total de la factura: %.2f\n", factura.total);
    }

    fclose(file);
}

void editFactura() {
    FILE *file = fopen("factura.dat", "rb+");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    int cedula;
    struct Factura factura;
    int found = 0;

    printf("Ingrese la cédula de la factura a editar: ");
    scanf("%d", &cedula);

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedula) {
            found = 1;
            printf("Factura encontrada.\n");

            int opcion;
            do {
                printf("\n¿Que desea editar?\n");
                printf("1. Nombre del cliente\n");
                printf("2. Numero de productos\n");
                printf("3. Productos\n");
                printf("4. Editar todo\n");
                printf("5. Salir de edicion\n");
                printf(">> ");
                scanf("%d", &opcion);

                switch (opcion) {
                    case 1:
                        printf("Ingrese el nuevo nombre del cliente: ");
                        leerCadena(factura.nombre, 20);
                        break;

                    case 2:
                        do {
                            printf("Ingrese el nuevo numero de productos: ");
                            scanf("%d", &factura.numProductos);
                            if (factura.numProductos <= 0) {
                                printf("El numero de productos debe ser mayor a 0. Intente de nuevo.\n");
                            }
                        } while (factura.numProductos <= 0);

                        factura.total = 0; // Reiniciar total para recalcular con nuevos productos
                        for (int i = 0; i < factura.numProductos; i++) {
                            printf("Ingrese el nombre del producto: ");
                            leerCadena(factura.productos[i].nombre, 50);

                            do {
                                printf("Ingrese la cantidad del producto: ");
                                scanf("%d", &factura.productos[i].cantidad);
                                if (factura.productos[i].cantidad <= 0) {
                                    printf("La cantidad debe ser mayor a 0. Intente de nuevo.\n");
                                }
                            } while (factura.productos[i].cantidad <= 0);

                            do {
                                printf("Ingrese el precio del producto: ");
                                scanf("%f", &factura.productos[i].precio);
                                if (factura.productos[i].precio <= 0) {
                                    printf("El precio debe ser mayor a 0. Intente de nuevo.\n");
                                }
                            } while (factura.productos[i].precio <= 0);

                            factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
                        }
                        break;

                    case 3:
                        printf("Editando productos existentes:\n");
                        factura.total = 0; // Reiniciar total
                        for (int i = 0; i < factura.numProductos; i++) {
                            printf("\nProducto %d\n", i + 1);
                            printf("Nombre actual: %s\n", factura.productos[i].nombre);
                            printf("Ingrese el nuevo nombre del producto: ");
                            leerCadena(factura.productos[i].nombre, 50);

                            do {
                                printf("Cantidad actual: %d\n", factura.productos[i].cantidad);
                                printf("Ingrese la nueva cantidad: ");
                                scanf("%d", &factura.productos[i].cantidad);
                                if (factura.productos[i].cantidad <= 0) {
                                    printf("La cantidad debe ser mayor a 0. Intente de nuevo.\n");
                                }
                            } while (factura.productos[i].cantidad <= 0);

                            do {
                                printf("Precio actual: %.2f\n", factura.productos[i].precio);
                                printf("Ingrese el nuevo precio: ");
                                scanf("%f", &factura.productos[i].precio);
                                if (factura.productos[i].precio <= 0) {
                                    printf("El precio debe ser mayor a 0. Intente de nuevo.\n");
                                }
                            } while (factura.productos[i].precio <= 0);

                            factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
                        }
                        break;

                    case 4:
                        printf("Editando toda la factura:\n");
                        printf("Ingrese el nuevo nombre del cliente: ");
                        leerCadena(factura.nombre, 20);

                        do {
                            printf("Ingrese el nuevo numero de productos: ");
                            scanf("%d", &factura.numProductos);
                            if (factura.numProductos <= 0) {
                                printf("El numero de productos debe ser mayor a 0. Intente de nuevo.\n");
                            }
                        } while (factura.numProductos <= 0);

                        factura.total = 0;
                        for (int i = 0; i < factura.numProductos; i++) {
                            printf("Ingrese el nombre del producto: ");
                            leerCadena(factura.productos[i].nombre, 50);

                            do {
                                printf("Ingrese la cantidad del producto: ");
                                scanf("%d", &factura.productos[i].cantidad);
                                if (factura.productos[i].cantidad <= 0) {
                                    printf("La cantidad debe ser mayor a 0. Intente de nuevo.\n");
                                }
                            } while (factura.productos[i].cantidad <= 0);

                            do {
                                printf("Ingrese el precio del producto: ");
                                scanf("%f", &factura.productos[i].precio);
                                if (factura.productos[i].precio <= 0) {
                                    printf("El precio debe ser mayor a 0. Intente de nuevo.\n");
                                }
                            } while (factura.productos[i].precio <= 0);

                            factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
                        }
                        break;

                    case 5:
                        printf("Saliendo de edicion.\n");
                        break;

                    default:
                        printf("Opción no valida.\n");
                        break;
                }
            } while (opcion != 5);

            // Guardar los cambios realizados
            fseek(file, -(long)sizeof(struct Factura), SEEK_CUR); // Moverse al inicio de la factura
            fwrite(&factura, sizeof(struct Factura), 1, file);
            printf("Factura actualizada.\n");
            break;
        }
    }

    if (!found) {
        printf("Factura con cedula %d no encontrada.\n", cedula);
    }

    fclose(file);
}



void deleteFactura() {
    FILE *file = fopen("factura.dat", "rb+");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    int cedula;
    struct Factura factura;
    int found = 0;
    long posEscritura = 0;

    printf("Ingrese la cedula de la factura a eliminar: ");
    scanf("%d", &cedula);

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedula) {
            found = 1;
            printf("Factura con cedula %d eliminada.\n", cedula);
        } else {
            fseek(file, posEscritura, SEEK_SET); // Posición para sobrescribir
            fwrite(&factura, sizeof(struct Factura), 1, file);
            posEscritura += sizeof(struct Factura);
        }
    }

    if (!found) {
        printf("Factura con cedula %d no encontrada.\n", cedula);
    } else {
        // Truncar el archivo para eliminar los datos sobrantes
        ftruncate(fileno(file), posEscritura);
    }

    fclose(file);
}


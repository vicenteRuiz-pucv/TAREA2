## Código de Ejemplo (tarea2)
Para guardar cambios  debemos usar (en la carpeta raíz)
````
gcc tdas/*.c tarea1.c -Wno-unused-result -o tarea1
````

Y luego ejecutar:
````
./tarea1
````

## LO QUE FUNCIONA Y LO QUE NO

En este codigo todas las funcionalidades cumplen correctamente!

El unico caso que no podria funcionar sería si queremos buscar por director ingresando mas de uno! debido a que no se uso split string 
y solo se considero el caso en que buscamos de a un director!!

##EJEMPLO DE FUNCIONAMIENTO
========================================
     Stream Finder
========================================
1) Cargar Cátalogo
2) Buscar Por Género
3) Buscar por Director
4) Buscar por Década
5) Búsqueda Avanzada
6) Agregar a mi Watchlist
7) Eliminar de mi Watchlist
8) Mostrar mi Watchlist
9) Salir
Ingrese su opción: 1
Catalogo Cargado Exitosamente!
Presione una tecla para continuar...

========================================
     Stream Finder
========================================
1) Cargar Cátalogo
2) Buscar Por Género
3) Buscar por Director
4) Buscar por Década
5) Búsqueda Avanzada
6) Agregar a mi Watchlist
7) Eliminar de mi Watchlist
8) Mostrar mi Watchlist
9) Salir
Ingrese su opción: 2
Ingrese el genero que desea buscar : Action

Peliculas Con Género : Action
Titulo : Shichinin no samurai
ID: tt0047478 | Año : 1954 | Rating : 8.60
Director : Akira Kurosawa 
.....................................
Titulo : Star Wars: Episode V - The Empire Strikes Back
ID: tt0080684 | Año : 1980 | Rating : 8.70
Director : Irvin Kershner 
.....................................
//MUCHAS PELICULAS DESPUES!
Presione una tecla para continuar...


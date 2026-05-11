#include "tdas/list.h"
#include "tdas/extra.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//STRUCT DE PELICULAS
typedef struct {
  char id[100];
  char title[300];
  List *genres; //debe ser inicializada lista de strings
  int year;
  float rating;
  char director[300];
} pelicula;

//FUNCIONES DE COMPARACION
int is_equal_int(void* key1, void* key2)
{
  int *num1 = (int*) key1;
  int *num2 = (int*) key2;
  return *num1 == *num2;
}
int is_equal_str(void* key1, void* key2)
{
  return strcmp((char *)key1,(char *)key2) == 0;
}
/*
Lo que haremos sera crear un tipo de dato llamado pelicula el cual almacena todos los datos de cada pelicula, asi como sus generos..
Luego inicializaremos un mapa principal en donde estara todo el archivo.. ademas iremos llenando los mapas secundarios, de genero,
director y decada.. asignando como clave g/d/d y como valor simplemente una lista todo aquello que coincida con la clave...
*/


// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Stream Finder");
  puts("========================================");

  puts("1) Cargar Cátalogo");
  puts("2) Buscar Por Género");
  puts("3) Buscar por Director");
  puts("4) Buscar por Década");
  puts("5) Búsqueda Avanzada");
  puts("6) Agregar a mi Watchlist");
  puts("7) Eliminar de mi Watchlist");
  puts("8) Mostrar mi Watchlist");
  puts("9) Salir");
}

void cargar_catalogo(Map* principal,Map* generos,Map* directores,Map* decada)
{
  FILE *archivo = fopen("Top1500.csv","r");
  if(archivo == NULL)
  {
    perror(
      "Error al abrir el archivo");
      return;
  }
  char **campos;
  campos = leer_linea_csv(archivo,',');
  while((campos = leer_linea_csv(archivo,',')) != NULL)
  {
   //RESERVAR CADA PAIR DE EL MAPA PRINCIPAL
   pelicula *peli = (pelicula *)malloc(sizeof(pelicula));
   strcpy(peli->id,campos[1]);
   strcpy(peli->title,campos[5]);
   //peli->genres= list_create();
   peli->year= atoi(campos[10]); 
   peli->rating=  atof(campos[8]);
   strcpy(peli->director,campos[14]);
   //insertar en el mapa principal  
   map_insert(principal,peli->id,peli);//MAPA EN DONDE INSERTAR|ID COMO CLAVE| STRUCT COMO VALOR| 
   //LLENAR MAPA DIRECTORES
  //buscamos el director actual en el mapa de directores 
    MapPair *busqueda_director = map_search(directores,peli->director);
   //si no se encuentrra en el mapa de directores: creamos la lista
    if(busqueda_director == NULL)
   {
     List * lista_director = list_create();
     list_pushBack(lista_director,peli);
     map_insert(directores,peli->director,lista_director);
   }
   else
   {
     //hacemos el push de la pelicula completa por nombre del director
     list_pushBack((List *)busqueda_director->value,peli);
   }
   //LLENAR MAPA DECADA  
  int *calculoDecada = (int*) malloc(sizeof(int));
  *calculoDecada = (peli->year / 10) * 10;
    
  MapPair *busqueda_decada = map_search(decada,calculoDecada);
  if(busqueda_decada == NULL)
  {
    List * lista_decada = list_create();
    list_pushBack(lista_decada,peli);
    map_insert(decada,calculoDecada,lista_decada);
  }
  else
  {
    list_pushBack((List *)busqueda_decada->value,peli);
    free(calculoDecada);
  }    
   //LLENAR MAPA GENEROS
  peli->genres = split_string(campos[11],", ");
  char* genero = (char*) list_first(peli->genres);

  while(genero != NULL)
  {
    MapPair *busqueda_genero = map_search(generos,genero);
    if(busqueda_genero == NULL)
      {
        List * lista_genero = list_create();
        list_pushBack(lista_genero,peli);
        map_insert(generos,genero,lista_genero);
      }
      else
      {
        list_pushBack((List *)busqueda_genero->value,peli);
        //free(calculoDecada);
      }  
    genero= (char *) list_next(peli->genres);
  }
     
  }
  fclose(archivo);
}

void buscar_genero(Map* generos)
{
  //LEER LOS GENEROS INGRESADOS..
  char genero_buscado[100];

  printf("Ingrese el genero que desea buscar : ");
  scanf(" %99[^\n]",genero_buscado);
  printf("\n");
  MapPair * buscado = map_search(generos,genero_buscado);
  
  if(buscado == NULL)
  {
    printf("No se encontrar Peliculas con ese Genero\n");
    return;
  }

  List* lista_peliculas = (List *)buscado->value;
  pelicula *peliActual = (pelicula *)list_first(lista_peliculas);

  printf("Peliculas Con Género : %s\n",genero_buscado);
  while(peliActual != NULL)
    {
      printf("Titulo : %s\n",peliActual->title);
      printf("ID: %s | Año : %d | Rating : %.2f\n",peliActual->id,peliActual->year,peliActual->rating);
      printf("Director : %s \n",peliActual->director);
      printf(".....................................\n");
      peliActual = (pelicula *)list_next(lista_peliculas);
    } 
}

void buscar_director(Map* director)
{
  //LEER EL DIRECTOR INGRESADO..
  char director_buscado[300];

  printf("Ingrese el/la director/a que desea buscar : ");
  scanf(" %99[^\n]",director_buscado);
  printf("\n");
  MapPair * buscado = map_search(director,director_buscado);

  if(buscado == NULL)
  {
    printf("No se encontraron peliculas de ese director\n");
    return;
  }

  List* lista_peliculas = (List *)buscado->value;
  pelicula *peliActual = (pelicula *)list_first(lista_peliculas);

  printf("Peliculas Del Director : %s\n",director_buscado);
  while(peliActual != NULL)
    {
      printf("Titulo : %s\n",peliActual->title);
      printf("ID: %s | Año : %d | Rating : %.2f\n",peliActual->id,peliActual->year,peliActual->rating);
      printf("Director : %s \n",peliActual->director);
      printf("------------------------------------------\n");
      peliActual = (pelicula *)list_next(lista_peliculas);
    } 
}

void buscar_decada(Map * MapaDecadas)
{
  //LEER LA DECADA INGRESADA..
  int decada;

  printf("Ingrese la decada por la cual desea buscar : ");
  scanf("%d",&decada);
  printf("\n");
  MapPair * buscado = map_search(MapaDecadas,&decada);

  if(buscado == NULL)
  {
    printf("No se encontraron peliculas de esa decada\n");
    return;
  }

  List* lista_peliculas = (List *)buscado->value;
  pelicula *peliActual = (pelicula *)list_first(lista_peliculas);

  printf("Peliculas De la Decada : %d\n",decada);
  while(peliActual != NULL)
    {
      printf("Titulo : %s\n",peliActual->title);
      printf("ID: %s | Año : %d | Rating : %.2f\n",peliActual->id,peliActual->year,peliActual->rating);
      printf("Director : %s \n",peliActual->director);
      printf("------------------------------------------\n");
      peliActual = (pelicula *)list_next(lista_peliculas);
    } 
}

void busqueda_avanzada(Map* generos)
{
  char genero_buscado[100];
  int decada;

  printf("Ingrese el genero que desea buscar : ");
  scanf(" %99[^\n]",genero_buscado);
  printf("\n");
  printf("Ingrese la decada por la cual desea buscar : ");
  scanf("%d",&decada);


  MapPair * buscado = map_search(generos,genero_buscado);

  if(buscado == NULL)
  {
    printf("No se encontrar Peliculas con ese Genero\n");
    return;
  }

  List* lista_peliculas = (List *)buscado->value;
  pelicula *peliActual = (pelicula *)list_first(lista_peliculas);


  int encontradas = 0;
  printf("Peliculas Con Género : %s / Año : %d\n",genero_buscado,decada);

  while(peliActual != NULL)
  {
    int decadaActual = (peliActual->year / 10 ) * 10;
    if(decadaActual == decada)
    {
      printf("Titulo : %s\n",peliActual->title);
      printf("ID: %s | Año : %d | Rating : %.2f\n",peliActual->id,peliActual->year,peliActual->rating);
      printf("Director : %s \n",peliActual->director);
      printf(".....................................\n");
      encontradas++;
    }
    peliActual = (pelicula *)list_next(lista_peliculas);
  }
  if(encontradas == 0)
  {
    printf("No se encontraron peliculas del año %d con genero %s\n",decada,genero_buscado);
  }
  
}

void agregar(Map * mapa, List * lista)
{
  //SOLICITAR ID
  char idBuscado[100];
  printf("Ingrese el id que desea agregar : ");
  scanf(" %99[^\n]",idBuscado);
  printf("\n");
  MapPair* buscado= map_search(mapa,idBuscado);
  if(buscado == NULL)
  {
    printf("No se encontro una pelicula con id: %s\n",idBuscado);
  }
  else
  {
  //si se encontro un id lo agregamos a la lista!
  pelicula * encontrada = (pelicula *)buscado->value;
  list_pushBack(lista,encontrada);
  printf("%s se agrego con exito!\n",encontrada->title);
  }
}

void eliminar(List* watchlist)
{
  char idBuscado[100];
  printf("Ingrese el id que desea eliminar : ");
  scanf(" %99[^\n]",idBuscado);
  printf("\n");

  pelicula *current = (pelicula *)list_first(watchlist);
  int eliminada = 0;
  while(current != NULL)
    {
      if(strcmp(idBuscado,current->id) == 0)
      {
        list_popCurrent(watchlist);
        printf("La Pelicula %s ha sido eliminada\n",current->title);
        eliminada = 1;
        break;
      }
      current= (pelicula *) list_next(watchlist);
    }
  if(eliminada == 0)
  {
    printf("No se encontro la pelicula con id : %s\n",idBuscado);
  }
}

void mostrar(List* watchlist)
{
  pelicula* current = (pelicula *) list_first(watchlist);
  if(current == NULL)
  {
    printf("La watchlist esta vacía!\n");
    return;
  }
  printf("Tu Watchlist: \n");
  while(current != NULL)
    {
      printf("Titulo : %s\n",current->title);
      printf("ID: %s | Año : %d | Rating : %.2f\n",current->id,current->year,current->rating);
      printf("Director : %s \n",current->director);
      printf(".....................................\n");
      current = (pelicula *)list_next(watchlist);
      
    }
}
int main() {
  char opcion;
  //List *categorias = list_create(); // Lista para almacenar categorías
  Map *mapaPrincipal = map_create(is_equal_str);
  Map *mapaGenero= map_create(is_equal_str);
  Map *mapaDirectores= map_create(is_equal_str);
  Map *mapaDecada= map_create(is_equal_int);
  List *watchlist = list_create();
  int cargado = 0;
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el
                           // newline anterior

    switch (opcion) {
    case '1':
      //CARGAR CATALOGO
      if(cargado == 0)
      {
      cargar_catalogo(mapaPrincipal,mapaGenero,mapaDirectores,mapaDecada);
      printf("Catalogo Cargado Exitosamente!\n");
      cargado = 1;
      }
      else
      {
        printf("El catalogo ya fue cargado!\n");
      }
      break;
      case '2':
      //BUSCAR POR GENERO
      buscar_genero(mapaGenero);
      break;
    case '3':
      buscar_director(mapaDirectores);
      break;
    case '4':
      buscar_decada(mapaDecada);
      break;
    case '5':
      busqueda_avanzada(mapaGenero);
      break;
    case '6':
      //AGREGAR A LA WATCHLIST
      agregar(mapaPrincipal,watchlist);
      break;
    case '7':
      eliminar(watchlist);
      break;
    case '8':
      mostrar(watchlist);
      break;
    case '9':
      puts("Saliendo del sistema...");
      break;
      default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '9');

  // Liberar recursos, si es necesario
  //list_clean(categorias);

  return 0;
}

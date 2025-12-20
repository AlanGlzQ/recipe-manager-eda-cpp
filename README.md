# Recetario EDA – C++ Recipe Manager

Proyecto final de la materia de **Estructuras de Datos (EDA)**.  
Es una aplicación de consola en **C++** que gestiona un recetario usando **listas ligadas genéricas** implementadas desde cero (simple y doblemente ligadas), junto con **ordenamiento por merge sort** y **persistencia en archivo**.

La idea principal es demostrar el uso de estructuras de datos propias aplicadas a un caso “real”: administrar recetas e ingredientes desde un menú interactivo.

---

## Funcionalidad

La aplicación permite:

- **Gestionar recetas**:
  - Agregar nuevas recetas.
  - Editar nombre, categoría, porciones, etc.
  - Eliminar una receta.
  - Eliminar todas las recetas (borrado masivo).

- **Gestionar ingredientes** de cada receta:
  - Agregar ingredientes con nombre, cantidad y unidad.
  - Mantener los ingredientes **ordenados alfabéticamente** mediante inserción ordenada.
  - Modificar la cantidad o datos de un ingrediente.
  - Eliminar ingredientes individuales.

- **Búsqueda y consulta**:
  - Buscar receta por nombre.
  - Listar todas las recetas.
  - Mostrar detalle de una receta (ingredientes y datos asociados).

- **Ordenamiento** de recetas:
  - Ordenar recetas por nombre usando **merge sort sobre una lista doblemente ligada**.
  - El ordenamiento se hace manipulando **nodos**, no solo índices.

- **Persistencia**:
  - Guardar todo el recetario en un archivo de texto.
  - Cargar las recetas desde archivo al iniciar el programa.

---

## Estructuras de datos y diseño

Este proyecto implementa estructuras propias en lugar de usar contenedores de la STL para la parte principal del modelo.

### `DoublyLinkedList<T>`

Lista genérica **doblemente ligada**, con:

- Nodos `DNode<T>` (punteros a `prev` y `next`).
- Inserción al inicio / final.
- Búsqueda basada en predicados (`findIndexIf`).
- Eliminación basada en predicados (`removeFirstIf`).
- **Merge sort recursivo**:
  - Se ordena la lista usando un comparador genérico (`std::function<bool(const T&, const T&)>`).
  - El algoritmo trabaja directamente sobre la cadena de nodos (reacomoda enlaces).

Usada para manejar colecciones de recetas (`RecipeList`, por ejemplo).

### `SinglyLinkedList<T>`

Lista genérica **simplemente ligada**, con:

- Nodos simples (solo puntero a `next`).
- Inserción ordenada con un comparador (manteniendo la lista ordenada de forma natural).
- `removeFirstIf`, `forEach`, acceso por índice (solo para operaciones específicas).

Usada, por ejemplo, para la lista de **ingredientes** de cada receta.

### Clases de dominio

- `Recipe`  
  Nombre, categoría, porciones, etc., y lista de ingredientes asociada.
- `Ingredient`  
  Nombre, cantidad, unidad de medida.
- `RecipeList`  
  Encapsula el manejo de recetas usando `DoublyLinkedList<Recipe>`.
- `Storage`  
  Se encarga de guardar y cargar el recetario a/desde archivo.
- `Menu`  
  Implementa el menú de consola (alta, baja, cambios, consultas).

---

## Estructura del proyecto

```text
include/
  DoublyLinkedList.h
  SinglyLinkedList.h
  Recipe.h
  Ingredient.h
  RecipeList.h
  Storage.h
  Menu.h
  ...

src/
  DoublyLinkedList.cpp
  SinglyLinkedList.cpp
  Recipe.cpp
  Ingredient.cpp
  RecipeList.cpp
  Storage.cpp
  Menu.cpp
  main.cpp

data/
  recetas_ejemplo.txt   # archivo con recetas de muestra

docs/
  PROYECTO_FINAL_EDA_AGQ.pdf  # documento del proyecto (reporte, diagramas, etc.)

README.md
LICENSE
.gitignore
```

##  Compilación y ejecución
- Requisitos

  - Compilador C++17 (g++ o similar).
  - Sistema operativo tipo Unix (Linux/macOS) o Windows con MinGW / WSL.
    
Compilar rápido (un solo comando)

Si no tienes Makefile, puedes compilar algo así:
```text
g++ -std=c++17 src/*.cpp -I include -o recetario
```
Y luego ejecutar:
```text
./recetario    # Linux / macOS
recetario.exe  # Windows
```
Si usas un IDE (Code::Blocks, CLion, VS, etc.), solo asegúrate de:
- Añadir include/ al include path del proyecto.
- Añadir todos los .cpp de src/ al build.



##  Formato de archivo 
El módulo Storage se encarga de:
- Guardar las recetas e ingredientes en un archivo de texto (por ejemplo data/recetas.txt).
- Cargar las recetas al iniciar el programa, reconstruyendo las listas.
El formato es sencillo y legible, pensado para:
- Ser fácil de depurar.
- Mantener estabilidad entre ejecuciones.
Puedes incluir un archivo de ejemplo (data/recetas_ejemplo.txt) con algunas recetas precargadas.



## Objetivo académico
Este proyecto está enfocado en:

- Aplicar estructuras de datos (listas ligadas y merge sort) a un problema concreto.
- Usar plantillas (templates) para crear estructuras genéricas.
- Organizar el código en capas:
  - Estructuras de datos.
  - Lógica de negocio (recetas, ingredientes).
  - Interfaz de usuario por consola.
  - Persistencia en archivo.
- Documentar el sistema con:  
  - Diagramas de clases.
  - Diagramas de secuencia.
  - Casos de uso / pruebas.
 

 
## Archivos ignorados

En este repositorio se excluyen los siguientes elementos (ver .gitignore):
- Archivos intermedios de compilación:
  - *.o, *.obj, *.exe, etc.
- Directorios generados por el IDE:
  - .vscode/, .idea/, etc.
- Carpetas de build:
  - build/, cmake-build-*, out/, etc.

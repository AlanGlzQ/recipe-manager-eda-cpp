# Recetario EDA – C++ Recipe Manager

![CI](https://github.com/AlanGlzQ/recipe-manager-eda-cpp/actions/workflows/ci.yml/badge.svg)

Proyecto final de la materia de **Estructuras de Datos (EDA)**.  
Es una aplicación de consola en **C++** que gestiona un recetario usando **listas ligadas genéricas** implementadas desde cero (simple y doblemente ligadas), junto con **ordenamiento por merge sort** y **persistencia en archivo**.

La idea principal es demostrar el uso de estructuras de datos propias aplicadas a un caso "real": administrar recetas e ingredientes desde un menú interactivo.

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
- **Merge sort recursivo** con comparador genérico (`std::function<bool(const T&, const T&)>`), trabajando directamente sobre los nodos.

Usada para manejar colecciones de recetas (`RecipeList`).

### `SinglyLinkedList<T>`

Lista genérica **simplemente ligada**, con:

- Nodos simples (solo puntero a `next`).
- Inserción ordenada con comparador (mantiene la lista ordenada de forma natural).
- Copy constructor y copy assignment propios para evitar shallow copy.
- `removeFirstIf`, `forEach`, acceso por índice.

Usada para la lista de **ingredientes** de cada receta.

### Clases de dominio

- `Recipe` — nombre, categoría, tiempo de preparación, procedimiento y lista de ingredientes.
- `Ingredient` — nombre, cantidad y unidad de medida.
- `RecipeList` — encapsula el manejo de recetas usando `DoublyLinkedList<Recipe>`.
- `Storage` — guarda y carga el recetario desde archivo de texto.
- `Menu` — implementa el menú de consola (alta, baja, cambios, consultas).

---

## Estructura del proyecto

```text
include/
  doubly_linked_list.hpp
  singly_linked_list.hpp
  recipe.hpp
  ingredient.hpp
  recipe_list.hpp
  storage.hpp
  menu.hpp
  category.hpp
  constants.hpp

src/
  main.cpp
  menu.cpp
  recipe.cpp
  storage.cpp

tests/
  test_doubly_linked_list.cpp   # 18 tests
  test_singly_linked_list.cpp   # 14 tests
  test_recipe_list.cpp          # 22 tests
  test_storage.cpp              # 12 tests

data/
  recetas.txt

docs/
  DOCUMENTACION_RECETARIO_AGQ.pdf

CMakeLists.txt
.github/workflows/ci.yml
```

---

## Compilación y ejecución

### Requisitos

- CMake 3.14 o superior
- Compilador C++17 (MSVC, GCC o Clang)
- Conexión a internet la primera vez (CMake descarga Google Test automáticamente)

### Compilar y ejecutar

```bash
cmake -B build
cmake --build build
```

```bash
./build/Debug/recetario      # Windows
./build/recetario            # Linux / macOS
```

---

## Tests

El proyecto usa **Google Test** con **66 tests** que cubren las cuatro capas del sistema:

| Suite | Tests | Qué cubre |
|---|---|---|
| `DoublyLinkedList` | 18 | pushBack, sort (merge sort), find, remove, edge cases |
| `SinglyLinkedList` | 14 | insertOrdered, remove, forEach, copy |
| `Recipe` + `RecipeList` | 22 | CRUD de recetas e ingredientes, ordenamiento, búsqueda |
| `StorageTest` | 12 | save, load, round-trip completo, manejo de errores I/O |

### Correr los tests localmente

```bash
cmake -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Los tests también corren automáticamente en **GitHub Actions** con cada push (ver badge arriba).

---

## Ejemplo de uso (salida de consola)

```text
=== RECETARIO EDA ===
1) Agregar receta
2) Listar recetas
3) Buscar receta por nombre
4) Guardar en archivo
5) Salir
Opción: 1

Nombre de la receta: Spaghetti Boloñesa
Tiempo de preparación (min): 30
...

Receta agregada correctamente.
```

---

## Formato de archivo

El módulo `Storage` guarda las recetas en texto plano (`data/recetas.txt`), con un formato sencillo y legible:

```text
<número de recetas>
<nombre>
<autor>
<categoría>
<tiempo de preparación>
<procedimiento>
<número de ingredientes>
<nombre>|<cantidad>|<unidad>
...
```

Este formato es fácil de depurar y estable entre ejecuciones.

---

## Objetivo académico

- Aplicar estructuras de datos (listas ligadas y merge sort) a un problema concreto.
- Usar plantillas (templates) para crear estructuras genéricas reutilizables.
- Organizar el código en capas: estructuras de datos, lógica de negocio, UI y persistencia.
- Escribir tests unitarios e integración con Google Test.
- Configurar CI/CD con GitHub Actions.

---

## Documentación

El reporte completo del proyecto (requerimientos, diagramas, pruebas, etc.) está disponible en:

[📄 DOCUMENTACION_RECETARIO_AGQ.pdf](docs/DOCUMENTACION_RECETARIO_AGQ.pdf)

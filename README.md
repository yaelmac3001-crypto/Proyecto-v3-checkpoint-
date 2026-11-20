Proyecto – Sistema de Alumnos v3 (checkpoint)

Versión 3 del Sistema de Alumnos. (captura de alumnos, grupos, carreras y maestros, y reportes en pantalla) 
y le agrega persistencia en archivos y un reporte de texto.

Qué se agregó en esta versión
Opción del menú	Función
5. Guardar datos	Escribe alumnos, grupos y maestros en datos.txt
6. Cargar datos	Lee datos.txt y reconstruye todo en memoria (pide confirmación si ya hay datos)
7. Generar reporte a archivo	Crea reporte.txt con los alumnos ordenados por promedio (mayor a menor)
8. Salir	Pregunta si se quieren guardar los datos antes de salir

Las opciones 1 a 4 son las de  v2 y funcionan igual.

Cómo funciona
Carga automática: al iniciar, el programa intenta leer datos.txt.
Primer uso: si el archivo no existe, avisa y arranca con datos vacíos (no falla).
Archivo dañado: se lee primero a copias temporales; si algo está mal, no se carga nada y se conservan los datos que ya había en memoria.
Validaciones al cargar: calificaciones entre 0 y 100 y que el grupo de cada alumno exista.



Pruebas realizadas
Primer uso sin datos.txt: inicia vacío sin errores.
Capturar datos, guardar, cerrar y volver a abrir: se recuperan alumnos, grupos y maestros.
Opción 4 (reportes de  v2) después de cargar desde archivo: mismos resultados.
Archivo vacío o con datos inválidos: avisa que está dañado y no pierde lo que hay en memoria.
Reporte con empates en promedio: se mantiene el orden de captura.

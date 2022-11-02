a) En caso de solo ejecutar el programa .exe sin compilar.
- ejecutar el .exe para ver el programa.

b) En caso de compilar y ejecutar el programa en windows.
- Solo ejecutar el archivo que dice compilador.cmd, esto abrirá una (terminal), esperas a que se cierre el cmd (este proceso compila el programa).
- Una vez cerrada el cmd, ejecutar el main.exe que se acaba de generar.

c) En caso de compilar el y ejecutar el programa en linux.
- Abrir el terminal, ir a la dirección del proyecto y ejecutar este comando para compilar el main.cpp "g++ -o main main.cpp -I gladinclude  -lglfw -lGL"
- Una vez compilado el programa correr el ejectable generado.
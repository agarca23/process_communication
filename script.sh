#!/bin/bash


numArg=$#
if [ $numArg -ne 0 ]; then
	echo "No debe insertar ningún argumento"
else
		opc=1
		min=0
		max=0
	while [ $opc -ne 4 ]
	do
		echo "1.) Mostrar código"
		echo "2.) Compilar el código"
		echo "3.) Ejecutar el código compilado"
		echo "4.) Salir"
		read opc
		case $opc in
			1) less programa.c;;
			2) gcc programa.c -o programa;;
			3)  echo "introduzca un valor minimo"
				read min
				echo "Introduzca un valor máximo"
				read max
				./programa $min $max;;
			4) echo "Saliendo...";;
			*) echo "Opción erronea";;
		esac
	done
fi

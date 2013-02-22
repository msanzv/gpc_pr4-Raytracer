#include "Almacenes.h"

int main()
{
	Animal **pa;
	pa= new Animal*;
	GuarderiaAnimal* pets = new GuarderiaAnimal;
	pets->meter(new Animal);
	pets->meter(new Perro);
	pets->meter(new Perro("Satan"));
	pets->meter(new Gato);
	pets->meter(new Gato("Micifu"));
	pets->listar();
	pets->animal(2,pa);
	(*pa)->quiensoy();
	getchar();
}
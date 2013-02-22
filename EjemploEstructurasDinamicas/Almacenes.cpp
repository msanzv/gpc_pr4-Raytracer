#include "Almacenes.h"
#include <iostream>

Animal::Animal()
{
	nombre="Nolose";
}
Animal::Animal(char* minombre)
{
	nombre= minombre;
}
void Animal::quiensoy()
{
	cout<<"Soy un Animal de la especie "<<especie<<" y me llamo "<< nombre << "\n";
}
Perro::Perro()
{
}
Perro::Perro(char* minombre):Animal(minombre)
{
	//nombre= minombre;
}
void Perro::quiensoy()
{
	cout<<"Soy un Perro de la especie "<<especie<<" y me llamo "<< nombre << "\n";
}
Gato::Gato()
{
}
Gato::Gato(char* minombre)
{
	nombre= minombre;
}
void Gato::quiensoy()
{
	cout<<"Soy un Gato de la especie "<<especie<<" y me llamo "<< nombre << "\n";
}
GuarderiaAnimal::GuarderiaAnimal(){
	cuantos=0;
}
void GuarderiaAnimal::meter(Animal* otro)
{
	mascota[cuantos++]=otro; //Se almacena la referencia al objeto. Polimorfismo
	copia.push_back(*otro);  //Se almacena el objeto (siempre de clase Animal). No hay polimorfismo
}
void GuarderiaAnimal::animal(int i, Animal ** pa)
{
	*pa= mascota[i];
}
void GuarderiaAnimal::listar()
{
	for(int i=0; i<cuantos; i++){
		mascota[i]->quiensoy();
		copia[i].quiensoy();
	};
}
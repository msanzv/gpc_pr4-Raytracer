#include <vector>
using namespace std;

class Animal
{
public:
	static const int especie= 0;
	char* nombre;
	Animal();
	Animal(char* minombre);
	virtual void quiensoy();
};

class Perro: public Animal
{
public:
	static const int especie= 1;
	Perro();
	Perro(char* minombre);
	void quiensoy();
};
class Gato: public Animal
{
public:
	static const int especie= 2;
	Gato();
	Gato(char* minombre);
	void quiensoy();
};
class GuarderiaAnimal
{
public:
	Animal* mascota[10];
	vector<Animal> copia;
	int cuantos;
	GuarderiaAnimal();
	void meter(Animal* unAnimal);
	void animal(int i, Animal** );
	void listar();
};
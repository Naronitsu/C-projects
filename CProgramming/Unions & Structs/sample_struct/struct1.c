#include <stdio.h> 
#include <string.h> 
 
struct Persons {
   char  name[50];
   char  surname[50];
   char  address[100];
   int   age;
}; 
//how to use typedef to rename a type
typedef struct Pet{
	int petId;
	char petName[50];
} Pet;
 
int main(void) {
	struct Pet mypet;
	Pet yourPet;
	struct Persons p1;
	
	strcpy(p1.name, "John");
	strcpy(p1.surname, "Said");
	strcpy(p1.address, "Flat 33, Binja Buqana, Mtarfa.");
	p1.age = 36;

	printf("%s %s is %d yrs old.\nAddress is: %s\n", 
		p1.name, p1.surname, p1.age, p1.address);
    
    return 0; 
} 

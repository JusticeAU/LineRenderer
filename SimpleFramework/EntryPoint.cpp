#include <iostream>
#include "ApplicationHarness.h"

//Include the necessary header file here for your game type
#include "CollisionFramework.h"
//#include "ParticleApp.h"


int main()
{
	ApplicationHarness program(new CollisionFramework());	//Create a new instance of your application type to pass into the harness here.

	program.Run();

	return 0;
}
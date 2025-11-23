#include <iostream>

void gun(double)
{
	std::cout<<"double\n";
}

void gun(int)
{
	std::cout<<"int\n";
}

void fun()
{
	gun(42);
}

int main(int argc, char* argv[])
{
	fun();
	return 0;
}

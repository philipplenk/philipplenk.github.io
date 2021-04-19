#include <iostream>

void gun(double)
{
	std::cout<<"double\n";
}

void fun()
{
	gun(42);
}

void gun(int)
{
	std::cout<<"int\n";
}

int main(int argc, char* argv[])
{
	fun();
	return 0;
}

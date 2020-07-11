/*
 * from demo-iopiread.cpp
 *
 *  Modified on: December 2018
 *
 *      compile with "g++ demo-iopiread.cpp ABE_IoPi.cpp  -o demo-iopiread"
 *      run with "./demo-iopiread"
 */

#include <stdio.h>
#include <stdexcept>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "ABE_IoPi.h"

using namespace std;

uint16_t value;

void clearscreen()
{
	printf("\033[2J\033[1;1H");
}

uint16_t bitArrayToInt32(bool arr[], int count)
{
	uint16_t ret = 0;
	int tmp;
	for (int i = 0; i < count; i++) {
		tmp = arr[i];
		ret |= tmp << (count - i - 1);
	}
	return ret;
}

bool bits[10] = { false };
bool res_smiley[4] = { false };

uint16_t ReadInputs(uint16_t& last_iter, ABElectronics_CPP_Libraries::IoPi & bus1)
{
	uint16_t res;
	
	if (bus1.read_pin(16))
		system("shutdown now");

	printf("last_iter %d\n", last_iter);
	printf("pin4 : %d\n",bus1.read_pin(4));
	if (last_iter != bus1.read_pin(4))
	{

		printf("APRES READINPUTS\n");
		last_iter = !last_iter;
		for(int i = 0; i < 4; ++i)
			res_smiley[i] = false;
		res_smiley[3] = last_iter;
		if (bus1.read_pin(1))
			res_smiley[0] = true;
		else if (bus1.read_pin(2))
			res_smiley[1] = true;
		else if (bus1.read_pin(3))
			res_smiley[2] = true;
	}
	for (int i = 5; i < 15; ++i) // 10bits_int
		bits[i - 5] = (bool)bus1.read_pin(i);
	res = bitArrayToInt32(bits, 10);
	fstream output("test.bin", ios::out);
	for (int i = 0; i < 4; i++)
	{
		printf("%d\n", res_smiley[i]);
		output << res_smiley[i];
	}
	for (int i = 0; i < 10; i++)
	{
		printf("bits : %d\n", bits[i]);
		output << bits[i];
	}
	output.close();
	return res;
}

int main(int argc, char **argv)
{
	setvbuf(stdout, NULL, _IONBF, 0); // needed to print to the command line
	uint16_t last_iter = 0;
	using namespace ABElectronics_CPP_Libraries;
	uint16_t res;
	try
	{
		IoPi bus1(0x20);

		// initialise one of the io pi buses on i2c address  default address for bus 1

		bus1.set_port_direction(0, 0xFF); // set bank 0 to be inputs
		bus1.set_port_direction(1, 0xFF); // set bank 1 to be inputs
		bus1.set_port_pullups(0, 0xFF);   // enable internal pullups for bank 0
		bus1.invert_port(0, 0xFF);		  // invert output so bank will read as 0
		bus1.set_port_pullups(1, 0xFF);   // enable internal pullups for bank 0
		bus1.invert_port(1, 0xFF);		  // invert output so bank will read as 0

		while (1)
		{
			clearscreen();
			value = ReadInputs(last_iter, bus1);
			printf("VALUE : %d\n", value);
			usleep(2000000); // sleep 0.2 seconds
		}
	}
	catch (exception &e)
	{
		cout << e.what();
	}

	return (0);
}

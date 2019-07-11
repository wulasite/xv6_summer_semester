#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int main(int argc, char const *argv[])
{
	printf(1, "--------------------io type test start!-----------------------------\n" );
	
	int start = uptime();

	// code
	int end = uptime();
	int pass = end-start ;

	printf(1, "--------------------io type test end!-----------------------------\n---------------pass %dms--------------", pass*10);

	exit();
}

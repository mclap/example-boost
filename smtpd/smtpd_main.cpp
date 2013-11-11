#include "smtpd_server.h"

int main(int argc, char *argv[])
{
	try
	{
		smtp::server srv;

		srv.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return 0;
}

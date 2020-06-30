/**
 *  @example serial_port_read_write.cpp
 */
#include <libserial/SerialPort.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
constexpr const char* const SERIAL_PORT_1 = "/dev/CONEXANT0" ;
/**
 * @brief This example demonstrates multiple methods to read and write
 *        serial stream data.
 */
int main()
{
	using namespace LibSerial ;
	SerialPort serial_port_1 ;
	try
	{
		serial_port_1.Open(SERIAL_PORT_1) ;
	}
	catch (const OpenFailed&)
	{
		std::cerr << "The serial ports did not open correctly." << std::endl ;
		return EXIT_FAILURE ;
	}
	serial_port_1.SetBaudRate(BaudRate::BAUD_2400) ;
	serial_port_1.SetCharacterSize(CharacterSize::CHAR_SIZE_8) ;
	serial_port_1.SetFlowControl(FlowControl::FLOW_CONTROL_NONE) ;
	serial_port_1.SetParity(Parity::PARITY_NONE) ;
	serial_port_1.SetStopBits(StopBits::STOP_BITS_1) ;

	std::vector<std::string> commands = { "AT+GMR\r", "ATE1\r", "AT+VCID=1\r"};
	std::string read_string_1 ;
	size_t timeout_milliseconds = 500 ;
	for(std::string command : commands)
	{
		serial_port_1.Write(command) ;
		serial_port_1.DrainWriteBuffer() ;
		try
		{
			serial_port_1.Read(read_string_1, 255, timeout_milliseconds) ;
		}
		catch (const ReadTimeout&)
		{
			std::cerr << "The Read() call has timed out." << std::endl ;
		}
//		std::cout  << command  << std::endl ;
		std::cout << read_string_1 << std::endl ;
	}

	std::string user_input ;
	user_input.clear() ;
	timeout_milliseconds=500;
	std::cout << timeout_milliseconds   <<std::endl;
        while(true){
                read_string_1 ="";
                try
                {
                        serial_port_1.Read(read_string_1, 255, timeout_milliseconds) ;
                }
                catch(const ReadTimeout&)
                {
                }
                if(read_string_1.size()>1 && read_string_1.find("RING") ==std::string::npos )
                {
                        std::cout <<"READ:  "<< read_string_1  << std::endl;

                }
        }
	serial_port_1.Close() ;
	std::cout << "The example program successfully completed!" << std::endl ;
	return EXIT_SUCCESS ;
}

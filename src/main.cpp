/**
 *  @example serial_port_read_write.cpp
 */
#include <libserial/SerialPort.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <unistd.h>
constexpr const char* const SERIAL_PORT_1 = "/dev/CONEXANT0" ;
/**
 * @brief This example demonstrates multiple methods to read and write
 *        serial stream data.
 */
 #include "UDPClient.h"
 
struct Value{
    std::string name;
    std::string value;
};
void fixline(std::string &token);
void logCall(std::vector<Value> cid);
int notifyNetwork(std::string msg);

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
	size_t timeout_milliseconds = 50 ;
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

		std::cout << read_string_1 << std::endl ;
	}

	std::string user_input ;
	user_input.clear() ;
	timeout_milliseconds= 1000;

        while(true){
                read_string_1 ="";
                try
                {
                    serial_port_1.Read(read_string_1, 255, timeout_milliseconds) ;
                }
                catch(const ReadTimeout&)
                {

                }           
                catch(const NotOpen){
                    std::cout <<  "EXITING..."  <<std::endl;
                    return EXIT_FAILURE;
                }
                if(read_string_1.size()>1 && read_string_1.find("RING") == std::string::npos ){
                    std::vector<Value> fields;
                    std::stringstream cid_ss(read_string_1);
                    std::string line("");
                    
                    while(std::getline(cid_ss, line, '\n')){
                        Value val;
                        fixline(line);
                        if(line.size() == 1 ){
                            continue;
                        }
                        std::stringstream line_ss(line);
                        int i=0;
                        std::string t;
                        while(std::getline(line_ss,t,'=')){      
                            if(i%2 < 1){
                                val.name=t;
                            }else{
                                val.value=t;
                            }
                            ++i;
                        }
                        fields.push_back(val);
                    }
                    logCall(fields);
                }
                
        }
	serial_port_1.Close() ;
	std::cout << "The example program successfully completed!" << std::endl ;
	return EXIT_SUCCESS ;
}
void fixline(std::string &line){
    std::size_t len = line.size();
    int sep = line.find(" = ");
    if(sep!=line.npos && sep > 3)
    line.replace(sep ,3, "=");
    
    sep = line.find('\r');
    if(sep!=line.npos && sep > 1)
    line.replace(sep ,1, "");

    sep = line.find('\n');
    if(sep!=line.npos && sep > 1)
    line.replace(sep ,1, "");
}
void logCall(std::vector<Value> cid){
    std::fstream file;
    file.open("/tmp/call.log", std::ios_base::app);
//    for(Value val : cid){
//    file<<val.name;
//     if(val.name != "NMBR"){
//        file<<",";
//    }
//    }
//    file <<std::endl;
    for(Value val : cid){
    file<<val.value;
    if(val.name != "NMBR"){
        file<<",";
        if(val.name == "NAME"){
           notifyNetwork(val.value);
        }

    }
    }
    file <<std::endl;
    file.close();
}
int notifyNetwork(std::string msg){
    msg=msg+'\n';
    udp_client_server::udp_client client("192.168.0.198",8000);
    std::cout << msg<<"->"<< client.get_addr()<<":"<<client.get_port()  <<std::endl;
    return client.send(msg.c_str(),msg.size());
    
}
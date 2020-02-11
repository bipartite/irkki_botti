#include "parser.hh"

#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cctype>


int main(void)
{
  //Otetaan ensin selville serveri, portti, kanava ja botin nick
 //	std::list<string> channelList; 
	std::string server = "irc.cs.hut.fi"; //defaulttina irc.cs.hut.fi
	std::string port = "6667"; //defaulttina 6667
	std::string nick = "JTTBot"; //defaulttina JTTBot
	std::string tmp;

	std::cout << "Enter irc-server: (irc.cs.hut.fi by default)" << std::endl;
	std::getline(std::cin, tmp);
	if (tmp != "")
		server = tmp;
	std::cout << "Enter port: (6667 by default)" << std::endl;
	std::getline(std::cin, tmp);
	if (tmp != "")
		port = tmp;
	std::cout << "Enter a nick for the bot: (JTTBot by default)" << std::endl;
	std::getline(std::cin, tmp);
	if (tmp != "")
		nick = tmp;

	Parser parser(server, nick);

	boost::asio::io_service io_service;
	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(server, port);	//kysely annetusta palvelimesta tekee taulukon niistä luultavasti
	boost::asio::ip::tcp::resolver::iterator iter=resolver.resolve(query);		//iteraattori
	boost::asio::ip::tcp::resolver::iterator end;		//päätepiste
	boost::asio::ip::tcp::socket socket(io_service);
	boost::system::error_code error = boost::asio::error::host_not_found;		//testi yhdistääkö servuun?

	while(error && iter != end)
	{
		std::cout << "connecting.."<< std::endl;
		socket.close();
		socket.connect(*iter++, error);		//käydään kaikki mahd. ip osoitteet läpi
	}
	if(error){
		throw boost::system::system_error(error);
	} else {
		std::cout << "connected" << std::endl;
	}
	
	boost::asio::streambuf asioOutput, asioInput;		//lähetyksessä ja vastaanotossa pitää käyttää ilmeisesti asion streambuf nimistä otusta johon tungetaan lähetettävät stringit tähän tapaan
	std::string message, output;

	std::istream asioInput_stream(&asioInput);
	std::ostream asioOutput_stream(&asioOutput);
	
	asioOutput_stream << "USER JTTBot jttbot jttbot :Olen botti\r\n";
	asioOutput_stream << "NICK " + nick + "\r\n";


	asioOutput_stream << parser.joinAllChannels();
	boost::asio::write(socket, asioOutput);

//jää kuuntelemaan serverin syötteitä
	while(1)
	{
	//tyhjennä streami ja message
		message = ""; output = "";
		asioOutput_stream.clear();
	//	boost::asio::read_until(socket, asioInput, "\r\n", error);
		if(error == boost::asio::error::eof)
		{
	    		boost::asio::ip::tcp::resolver::query query1(server, port);	//luultavasti tarttee jos yhteys menee kokonaan poikki
			boost::asio::ip::tcp::resolver::iterator iter=resolver.resolve(query1);
			boost::asio::ip::tcp::resolver::iterator end;
			boost::system::error_code err = boost::asio::error::host_not_found;
			std::cout << "reconnecting..." << std::endl;
			while(err && iter != end)
			{
				socket.close();
				socket.connect(*iter++, err);		//käydään kaikki mahd. ip osoitteet läp
			}
			if(err){
				throw boost::system::system_error(error);
			} else {
				std::cout << "connected" << std::endl;
		      
			}
				asioOutput_stream << "USER JTTBot jttbot jttbot :Olen botti\r\n";	
				asioOutput_stream << "NICK " + nick + "\r\n";
				asioOutput_stream << parser.joinAllChannels();
				boost::asio::write(socket, asioOutput);
				asioInput_stream.clear();
				asioOutput_stream.clear();
			}
		boost::asio::read_until(socket, asioInput, "\r\n", error);
		std::getline(asioInput_stream, message);
		std::cout << message << std::endl; //syöttää parserille tekstisyötteen kutsuu parseria messagella
//		std::string output = parser.parse(message);
		output = parser.parse(message);
		std::cout << output;	//output sisältää jo rivinvaihdon	
		asioOutput_stream << output;
		boost::asio::write(socket, asioOutput);
	}

	socket.close();
	return 0;
}


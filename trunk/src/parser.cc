#include "parser.hh"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;

Parser::Parser() :  server("irc.cs.hut.fi"), botName("JTTBot") {}
Parser::Parser(string server, string botName) : server(server), botName(botName) {}


string Parser::parse(string str) {
  
  std::istringstream iss(str);
  
  // Tehdään syötteenä saadusta tekstijonosta vektori, jolloin sen käsittely on helppoa
  std::vector<string> strVec;
  string tmp;
  while (iss >> tmp) {
    strVec.push_back(tmp);
  }
    
  // Liian lyhyitä syötteitä ei käsitellä
  if (strVec.size() < 1)
    return "";


  /** serverin PING-komentoon vastaaminen on niin triviaalia, ettei sitä kannata kierrättää commandHandlerin kautta
      serveriltä tulee siis syöte "PING :<server>" johon vastataan "PONG <server>". **/
  if (strVec[0] == "PING") {
    return "PONG " + server + "\r\n";
  } 
  
  /** Erotellaan syötteestä ilmoitukset, kun henkilö liittyy kanavalle, ja kutsutaan handlerin joinEvent-funktiota.
      Tässä syötteestä parsitun kanavan nimen edessä on kaksoispiste, mikä pitää saada pois ts. syöte on muotoa
      ":<nick>!<host> JOIN :<kanava>" **/
  if (strVec[1] == "JOIN") {
    channel = simplify(strVec[2]);
    host = parseHost(strVec[0]);
    nick = parseNick(strVec[0]);
    return handler.joinEvent(channel, nick, host);
  }

  /** Mikäli botille annetaan jollain kanavalla komento, on tuleva syöte muotoa 
      ":<nick>!<host> PRIVMSG <kanava> :<botin nick> <komento>". Syöte parsitaan ja kutsutaan asianmukaisia commandHandlerin
      funktioita. **/
  if (strVec.size() > 4 && strVec[3] == ":" + botName) {
    // Parsitaan syötteestä kanava, nick, host ja komento
    channel = strVec[2];
    host = parseHost(strVec[0]);
    nick = parseNick(strVec[0]);
    command = strVec[4];
    
    // tarkistetaan aluksi onko kyseessä quote-toimintoihin liittyvä komento      
    // addQuote-komennon jälkeen tuleva tektijono kasataan taas string-muuttujaksi ennen commandHandlerin kutsumista
    if (command == "addQuote") {
      string quote;
      for (unsigned int i = 5; i < strVec.size(); i++)
	quote += strVec.at(i) + " ";
      return handler.addQuote(channel, quote);
    }
      
    if (command == "getQuote")
      return handler.getQuote(channel);
    
    if (command == "removeLastQuote")
      return handler.removeLastQuote(channel, host);
    
    // Parametritön loadQuotes-komento käsitellään tässä
    if (command == "loadQuotes" && strVec.size() == 5)
      return handler.setUMode(channel, command, nick, host, "");

    // ja loadAuth-komento tässä
    if (command == "loadAuth" && strVec.size() == 5)
      return handler.loadAuth(channel, host);

    // Muissa komennoissa on myös kohde, ja niitä varten on setUMode-funktio commandHandlerissa.
    if (strVec.size() != 6) {
      return "PRIVMSG " + channel + " :Give commands in the following form:  <bot's nick> <command> <target>\r\n";      
    }
    
    target = strVec[5];

    return handler.setUMode(channel, command, nick, host, target);
  }
  
  /** Jos syötteenä tulee pelkkä botin nick, osoitetaan elonmerkkejä **/
  if (strVec.size() == 4 && strVec[3] == ":" + botName) 
    return "PRIVMSG " + strVec[2] + " :Hi " + parseNick(strVec[0]) + "\r\n";

  // Muuten palautetaan tyhjä string.
  return "";
}



// main kutsuu tätä funktiota, mikäli botti yhdistää uudestaan serverille.
string Parser::joinAllChannels() {
  return handler.joinAllChannels();
}



// poistaa ikävät kaksoispisteet string-muuttujien edestÃ¤
string Parser::simplify(string str) {
  return str.substr(1, str.size());
}



// funktio parsii nickin nick-hostin ketjusta
string Parser::parseNick(string str) {
  // etsitÃ¤Ã¤n :Esim!eaesim@palvelin.fi -muotoisesta hostista nick
  size_t end = str.find_first_of('!') - 1;
  return str.substr(1, end);
}



// toimii samoin kuin parseNick, mutta parsii hostin
string Parser::parseHost(string str) {
  size_t begin = str.find_first_of('!') + 1;
  return str.substr(begin, str.size());
}

#include "quotes.hh"

#include <cstdlib>
#include <fstream>
#include <iostream>

using std::string;



Quotes::Quotes() : lastQuote("") {
  loadQuotes();
}



bool Quotes::contains(std::list<string> list, string str) {
  std::list<string>::iterator iter = list.begin();
  while (iter != list.end()) {
    if (*iter == str)
      return true;
    iter++;
  }
  return false;
}



bool Quotes::addQuote(string quote) {
  quotes.push_back(quote);
  lastQuote = quote;
  return saveQuotes();
}



bool Quotes::removeLastQuote() {
  if (lastQuote == "") 
    return false;
  quotes.remove(lastQuote);
  std::cout << "Removed quote: " << lastQuote << std::endl;
  lastQuote = "";
  return saveQuotes();
}



string Quotes::getQuote() {
  if (quotes.size() == 0)
    return "No quotes found";
  
  unsigned int randomInteger = rand() % quotes.size();
  std::list<string>::iterator iter = quotes.begin();
  // std::listin iteraattori on bidirectional, joten valitettavasti helpompi iter += randomInteger ei toimi.
  for (unsigned int i = 0; i < randomInteger; i++)
    iter++;
  lastQuote = *iter;
  return *iter;
}



bool Quotes::saveQuotes() {
  // Järjestetään quotet selkeämmän tiedoston aikaansaamiseksi
  quotes.sort();
  quotes.unique();

  std::ofstream out("./quotes.txt");
  std::list<string>::iterator iter;

  try {
    out << "BEGIN\n";
    iter = quotes.begin();
    while (iter != quotes.end()) {
      out << *iter << "\n";
      iter++;
    }

    out << "END\n";
  } catch (std::ofstream::failure e) {
    out.close();
    std::cout << "Exception in Quotes::saveQuotes funktion" << std::endl;
    return false;
  }
  
  out.close();

  std::cout << "Quotes saved" << std::endl;

  return true;
}



bool Quotes::loadQuotes() {  
  // Tyhjennetään aluksi lista, mikäli funktiota kutsutaan ajon aikana
  quotes.clear();
  lastQuote == "";

  std::ifstream in;
  in.open("quotes.txt");
  try {
    
    // Tarkistetaan onko quotes.txt-tiedostoa.
    if (!in.is_open()) {
      in.close();
      std::cout << "quotes.txt not found." << std::endl;
      return false;
    }
    
    string quote;
  
    getline(in, quote);
    if ( quote != "BEGIN") {
      std::cout << "Invalid first line: " << quote << "\"" << std::endl;;
      return false;
    }
    
    while (getline(in, quote), quote!="END")
      quotes.push_back(quote);
  } catch (std::ifstream::failure e) {
    in.close();
    std::cout << "Exception in Quotes::loadQuotes() function" << std::endl;
    return false;
  }
  
  in.close();

  std::cout << "Quotes loaded" << std::endl;

  return true;
}



bool Quotes::loadQuotes(string fname) {
  // jos fname == "", kutsutaan loadQuotes() funktiota
  if (fname == "")
    return loadQuotes();

  std::cout << "loadQuotes(string) called" << std::endl;

  lastQuote == "";

  std::ifstream in;
  in.open(fname.c_str());

  std::list<string> tmpQuotes;

  try {
    if (!in.is_open()) {
      in.close();
      std::cout << fname << " not found." << std::endl;
      return false;
    }
    
    string quote;
    getline(in, quote);
    if ( quote != "BEGIN") {
      std::cout << "Invalid first line: " << quote << "\"" << std::endl;;
      return false;
    }

    while (getline(in, quote), quote != "END")
      tmpQuotes.push_back(quote);
  } catch (std::ifstream::failure  e) {
    in.close();
    std::cout << "Exception in Quotes loadQuotes(string) function" << std::endl;
    return false;
  }
  in.close();

  quotes.merge(tmpQuotes);
  

  std::cout << "Quotes loaded from file " << fname << std::endl;

  saveQuotes();

  return true;

}

#ifndef QUOTES_HH
#define QUOTES_HH

#include <list>
#include <string>

using std::string;

class Quotes
{
public:
  Quotes();
  
  // Lis‰‰ quoten quotes-listaan
  bool addQuote(string);

  /** Funktio poistaa viimeisimp‰n‰ k‰sitellyn quoten listasta. Viimeisin quote
      on joko saatu juuri getQuotella tai juuri lis‰tty quote. Funktio v‰hent‰‰
      manuaalisen quote-listan muokkauksen tarvetta. **/
  bool removeLastQuote();

  // Hakee listasta satunnaisen quoten
  string getQuote();
  
  /** Yhdist‰‰ toisen quotes-tiedoston nykyiseen. Tiedoston pit‰‰ olla oikeassa formaatissa,
      eli alkaa BEGIN ja loppua END-riviin **/
  bool loadQuotes(string);

private:
  bool contains(std::list<string>, string);
  
  // Funktiot tallentavat ja hakevat tiedoja quotes.txt-tiedostosta.
  bool saveQuotes();
  bool loadQuotes();

  // Viimeisin quote
  string lastQuote;

  std::list<string> quotes;
};

#endif

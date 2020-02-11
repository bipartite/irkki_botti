#ifndef AUTHORITY_HH
#define AUTHORITY_HH

#include <list>
#include <string>

using std::string;

class Authority
{
public:
  Authority();
  
  /** Seuraavat funktiot palauttavat vain boolean-arvon, joten syötteen muodostus jää commandHandlerin tehtäväksi.
      Virhetilanteissa kannattaa tarkastella couttiin tulleita syötteitä. **/
  bool addUser(string);
  bool addAutoOp(string, string);
  bool addAutoVoice(string, string);
  bool removeUser(string);
  bool removeAutoOp(string, string);
  bool removeAutoVoice(string, string);
  bool isUser(string);
  
  /** Palauttaa int-arvon, josta ilmenee, saako käyttäjä kanavalle
      liittyessään opit tai voicen.
      Paluuarvot: 2 op, 1 voice, 0 ei mitään. */
  int getAutoStatus(string, string);

  /** lataa authorities.txt:n uudestaan **/
  bool loadAuth();
  
private:
  bool contains(std::list<string>, string);
  
  /** save() ja load() funktiot kirjoittavat ja hakevat informaatiota authorities.txt -tiedostosta, jonne on
      tallennettu tiedot botin käyttäjistä. **/
  bool save();
  bool load();

  std::list<string> users;
  std::list<string> autoOp;
  std::list<string> autoVoice;
};

#endif

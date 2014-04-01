/*
Copyright (c) 2017 - Brice Rouanet
Copyright (c) 2013 - Philippe Laulheret, Second Story 

This code is licensed under MIT. 
For more information visit :
https://github.com/tmator/GalileoOSCsample
https://github.com/secondstory/LYT
*/

#include <EthernetUdp.h>
#include <OSCBundle.h>
#include <OSCMessage.h>

//port d'écoute en OSC
int UDPPort=8888;
EthernetUDP UDPOsc;

//les trois sorties ou l'on connecte la led RGB
int redPin = 11;
int greenPin = 10;
int bluePin = 9;

void setup() {

  //on ecoute en UDP
  UDPOsc.begin(UDPPort);
  UDPOsc.listen();
  //lors de la lecture d'un paquet la bouclo principale se bloque donc on exécute le tout dans un thread
  pthread_t oscThread;
  pthread_create(&oscThread,NULL,osc_loop,0);

  //on active les pin de la led en sortie
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

}

int hex2dec(byte c) { 
  // converts one HEX character into a number
  if (c >= '0' && c <= '9') {
    return c - '0';
  } 
  else if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  }
  else if (c >= 'a' && c <= 'f') {
    return c - 32 - 'A' + 10;
  }

}

void setColor(int red, int green, int blue)
{
  //envoie les valeurs a la led
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void setColorHex(char *col)
{
  //transforme la couleur html en r,g et b
  int r = 255- (hex2dec(col[2]) + hex2dec(col[1]) * 16);
  int g = 255- (hex2dec(col[4]) + hex2dec(col[3]) * 16);
  int b = 255- (hex2dec(col[6]) + hex2dec(col[5]) * 16); 
  setColor(r,g,b);
} 

int getNextMessage(OSCBundle &b)
{
  //récupère le message OSC
  int bytes = UDPOsc.parsePacket();
  if (bytes)
  {
    unsigned char *buffer = new unsigned char[bytes];
    bytes = UDPOsc.read(buffer,bytes);
 

    if (bytes >0)
    {
      if (buffer[0] == '#') b.fill(buffer,bytes);
      else
      {
        OSCMessage *m = new OSCMessage();

        m->fill(buffer,bytes);
        if (!m->hasError()) b.add(*m);
        else {
          delete m;
          delete[] buffer;
          return 0;
        }
      }
    }
  
    delete[] buffer;
    delay(10);
    if (!b.hasError())
    {
      return 1;
    }
    return 0;

  }
  return 0;
}

void *osc_loop(void *dummy)
{

  //l'endroit ou tout se passe :)
  while (true) {
    OSCBundle b;
    OSCMessage *m;
    
    while (getNextMessage(b))
    {
      //si on gette un message osc
      if (m = b.getOSCMessage(0))
      {

        //si c'est bien un string (on envoie un string coté processing)
        if(m->isString(0))
        {
          //on récupère le string et on envoie la couleur à la led
          int sLength=m->getDataLength(0);
          char color[sLength];
          m->getString(0,color,sLength); 
          setColorHex(color); 
        }
      }
     b.empty();
    }
  }

  return 0;
}

void loop()
{
  //rien
}

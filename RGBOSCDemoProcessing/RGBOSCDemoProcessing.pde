/*

Copyright (c) 20114 - Brice Rouanet

This code is licensed under MIT. 
For more information visit  : https://github.com/tmator/GalileoOSCsample

*/

//import de la lib pour faire de l'OSC
import oscP5.*;
import netP5.*;
 

OscP5 oscP5;
NetAddress galileo;

PImage myImage;
PFont myFont;

color theColor;$
String colorStr="toto";
 
void setup()
{
  oscP5 = new OscP5(this,12000);
  //la carte galiléo ecoute sur le port 8888
  galileo = new NetAddress("10.8.1.85", 8888);

  size(900, 900);
  background(0);
   
  //on charge l'image de laquelle on souhaite avoir les couleurs
  myImage = loadImage("rgb_circles.png");
   
  //on charge la police pour afficher la couleur au format HTML
  myFont = createFont("Helvetica", 24);
  textFont(myFont);
  textSize(14);
   

}
 
void mousePressed() {

  //on crée le message ledData
  OscMessage ledData = new OscMessage("/");
  //on ajoute la couleur au mesage
  ledData.add(colorStr);
  //on l'envoie
  oscP5.send(ledData, galileo);
  
} 
 
void draw()
{
  
  background(0);
  //on affiche l'image
  image(myImage,0,0);
  //on appele la fonction pour récupérer la couleur
  pickColor();
}
 
 void pickColor() {
   
  //récupère la couleur la ou se trouve la souris
  theColor = get(mouseX, mouseY);
  
  //on définie la couleur du texte avec la couleur choisie
  fill(theColor);
  
  //on crée le string de la couleur au format html et on l'affiche
  colorStr = "#"+hex(theColor, 6);
  text ("#" + hex(theColor, 6), 800, 160);
};

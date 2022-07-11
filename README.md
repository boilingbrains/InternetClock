# __Horloge connectée qui affiche l'heure🕞, la température🌡️, l'humidité💧 et le prix du Bitcoin 💰__

## Introduction:
Ce projet est un exemple de ce qu'on peut réaliser avec une carte NodeMCU ESP8266. On utilise le protocol NTP pour récuperer l'heure de Paris... qui est ensuite affichée sur **une matrice LED (MAX7219)**. **La photorésistances(GL5539 30K-90K)** va évaluer le niveau de luminosité ambient pour permettre d'ajuster celles des LEDs de la matrice. Le capteurs **DHT11**  permet d'avoir la température & l'humidité. Ainsi, on peut montrer que la carte peut collecter des données venant de capteur éléctronique comme pour l'Arduino. Enfin, à l'aide de l'API de CoinDesk, on récupère le prix du BTC en dollars. 
## Montage:

🔗 NodeMCU : https://amzn.to/3ACJpLL 

🔗 DHT11: https://amzn.to/3NWhrxA

🔗 matrice LED (MAX7219):https://amzn.to/3Oc3YSt

🔗 photorésistances(GL5539 30K-90K):https://amzn.to/3NUbGAh

[![image](#center)](D:\Projects\InternetClock\Montage.PNG)

## Liens utiles:

🔗 https://randomnerdtutorials.com/installing-esp8266-nodemcu-arduino-ide-2-0/

🔗 https://microcontrollerslab.com/max7219-led-dot-matrix-display-esp8266-nodemcu-tutorial/ 

🔗 https://www.ntppool.org/zone 

🔗 https://api.coindesk.com/v1/bpi/currentprice.json 




## Structure du projet
```
📦Project
│   📜README.md
│   🖼️Montage.png
└───📜InternetClock.ino

```
## Explications:

<span style="display:block;text-align:center">

[![](#center)]()

</span>
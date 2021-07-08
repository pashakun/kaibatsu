# KAIBATSU
Using Arduino-type microcontroller with an atmospheric pressure sensor and a Bluetoosh Low Energy module to track elevation above sea level.

## Abstract
Got the idea for this when the family decided to move to the shore. This is Japan, so tsunamis are a thing. Figured it would be nice to know your elevation above sea level at all times. Sure, there is an app for that, but I wanted something that gives an ambient awareness at all times. After a while you generate a mental map of local topography.

## Hardware

* TinyDuino 7 Segment Display
![alt text](https://github.com/TinyCircuits/TinyCircuits-TinyShield-7Segment-ASD2421/raw/master/images/ASD2421-R-N-Tutorial-Image-01.png)
* TinyDuino Barometric Pressure Sensor 
![alt text](https://cdn.shopify.com/s/files/1/1125/2198/products/ASD2511-R-P-Product-Picture-01_1800x1800.png?v=1577128324)
* TinyDuino BLE Module
![alt text](https://github.com/TinyCircuits/TinyCircuits-TinyShield-BLE-ASD2116/raw/master/images/ASD2116-R-Tutorial-Image-01.png)

## Software

* Arduino IDE
* Pythonista

## Process

* Barometric pressure sensors only show relative altitude. To know your absolute elevation, you need current atmospheric pressure at sea level (P0)
* One way to handle it is to instantiate the sensor at a known altitude with the number hard-coded in
* The problem is atmospheric pressure can change A LOT during the day and the altitude can drift by up to 70 meters
* My solution is to get the most current atmospheric reading from Japan Meteorological Agency website that provides on the hour updates. The site lists elevation of its barometric sensor, which can be used to calculate P0
* Use Pythonista iPhone app to scrape the data with BeautifulSoup and pipe it to the TinyDuino via BLE
![alt text](https://photos.app.goo.gl/B6tj4sBPkknxrN278)

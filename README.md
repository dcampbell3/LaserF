# LaserF
As a response to a lack of accessible and affordable soccer training products, our team designed a soccer goal frame with a laser grid that can calculate and display soccer ball coordinates on a user interface. Our design included 27 lasers with 27 corresponding photoresistors that detect laser occlusions. A weighted average of occluded points using the time of occlusion as weights was performed on each kick to calculate the center position of the ball. A velocity calculation was performed by finding the diameter of the circular cross-section of the ball that occludes a laser. This diameter is then divided by the time of occlusion to obtain velocity. This information would be displayed on the User Interface where position is given relative to the bottom right corner. 

This product was developed during the Fall 2022 semester as part of ME 491: Product Design at Duke University. 

## Undergraduate Team: 
###
Danny Campbell (daniel.campbell@duke.edu)
###
Lelia Jennings (lelia.jennings@duke.edu)
### 
Jake Mann (jake.mann@duke.edu)
### 
Matthew Bloom (matthew.bloom@duke.edu)

## Technical Mentors: 
###
Professor Rebecca Simmons
###
Professor Tyler Bletsch 

# Implementation Guide
Below details the function of all provided code and how to replecate our team's results. 

## 2X2 Laser Prototype
This code was intended to be used with an Arduino Mega. This is an initial test of feasibility on a 1/2 scale goal with a 2X2 grid of lasers. Upon detecting a kick, samples of current occlusions and current time are taken until no more occlusions are detected. The ball's center position and velocity is then calculated.

## 3X3 Laser Prototype 
This code was intended to be used with an Arduino Mega. This expands on our initial test by adding an additional laser in each dimension. Upon detecting a kick, samples of current occlusions and current time are taken until no more occlusions are detected. The four points that bound the center of the ball are found, then the ball's center position and velocity is calculated.

## Final Goal Code
This code was intended to be used with an Arduino Mega. This is the final code deployed for use on the finished product. Upon detecting a kick, samples of current occlusions and current time are taken until no more occlusions are detected. Next, the two or three lasers in each dimension that are occluded and found. Then, four points that bound the center of the ball are found. Finally, the ball's center position and velocity is calculated. 

## Laser Diagnostic
This code was intended to be used with an Arduino Mega. This code was used to tune the trimpots used as variable resistors in a voltage divider circuit to obtain a desired voltage across the photoresistors.

## Goal UI 
This python code is intended to be run in parallel with Final Goal Code after all Serial.print()s have been removed except for the final one. The coordinates displayed are in inches with respect to the bottom left corner.



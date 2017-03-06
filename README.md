# EmbeddedSystemsF16
Final project for my embedded systems class that I took on Fall 2016

This is the final project for ECE362 in Purdue. a digital implementation of the popular Musical Chairs
The project 'Game of chairs' is a multiplayer game in which each player has a color on the circle.
Each player can move his/her color one space at a time using the controller. 
Two players can never be in the same spor at the same time, not jump over other player.
The objective of the game is to be aligned with one of the active 'chairs' (central jumbo LEDs) when the timer stops.
Every time a player looses it is eliminated. when only one player remains in the game, that player is declared 'The new king'.

The inicializations of the microcontroller were implemented by Alejandro Tejada
The PCB for the project was done by Rodolfo Leiva
The software was developed by Mateo Vasquez and me.

__________________________________________________________________________________________________________________

The central part from the software is the Master array. This array represent every spot for each player in the game.
This methos was implemented in order to control the LEDs in the game. The information was being passed using a SPI interface.
The SPI was converted to parallel connections using three ATF22V10C PLDs in cascode configuration.
The game included functions to select between two or three players and also to have a practice round.

The final video can be seen at:

https://www.youtube.com/watch?v=EN_Q6ers-bw&t=91s

This project was presented on the Spark challenge at Purdue Universty on December 9th 2016.

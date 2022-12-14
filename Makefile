# 
# Small makefile for Saisie
#

CC=g++
saisieinteractive: saisieinteractive.o  
	$(CC) -o saisieinteractive saisieinteractive.o -lGL -lGLEW -lGLU -lglut  -lm
all: saisieinteractive

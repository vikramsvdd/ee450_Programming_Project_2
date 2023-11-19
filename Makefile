# -*- Makefile -*-

#target: dependencies
	#action

all: servermain serverA serverB serverC

servermain: servermain.cpp
	g++ -o servermain servermain.cpp 


serverA: serverA.cpp
	g++ -o serverA serverA.cpp 
	
serverB: serverB.cpp
	g++ -o serverB serverB.cpp 
	
serverC: serverC.cpp
	g++ -o serverC serverC.cpp 	






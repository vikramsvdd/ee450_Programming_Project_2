1. Vikram Vasudevan

2. 6201-1130-74

3. Platform: Ubuntu 20.04.6

4. In this assignment, I have implemented a simple main server to backend server interaction, where upon an user's request, the main server requests for the student list associated with a department and the backend servers satisfy that request, else the main server prompts the user to enter a relevant department name

5.servermain.cpp : It is the code that drives the main server and implements the main server functionality.

serverA.cpp: It is the code that drives the serverA where the server extracts data from dataA.txt and then provides relevant information to the main server upon its request.

serverB.cpp: It is the code that drives the serverB where the server extracts data from dataB.txt and then provides relevant information to the main server upon its request.

serverC.cpp:It is the code that drives the serverC where the server extracts data from dataC.txt and then provides relevant information to the main server upon its request.

Makefile: a file that compiles the  server files into their respective object files and converts them further into an executable file for each cpp file.

6. the message exchange format is as follows: (Here Sa stands for serverA, and M for main server) The same is the process for serverB and C too!
M:(gets the list from all the servers)
M: asks the user for a relevant dept id 
(if dept name found)
M: dept found in server A (say as an example)
M: sends the request to server A
Sa: received request from main server corresponding to <dept name>
Sa: Sent the relevant info
M: received (displays the info too)
M: prompts the user for a new query


(if dept name not found)
M: dept name not found
M: prompts the user for a new query



7. My system can handle atmost 15,000 characters. Also, I have designed the system to be case-sensitive wrt the handling of the department name (say if in the dataA.txt, there is a department ECE, but if the client inputs ece or eCE, it will fail to recognise that!). Also mainly, the main server gets the department list only once i.e after the startup of the backend servers and its own startup, thus to restart the system you must start the backend servers first and then the main server.

8. References: Beej's Network Programming

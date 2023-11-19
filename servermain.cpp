#include <iostream>  // All the essential libraries
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include<map>
#include <vector>
#include<sstream>
#include<set>
#include<algorithm>
using namespace std;

// referenced Beej's Socket Programming and ChatGPT 

int main() {
    
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);  // UDP used Datagram unlike TCP
 struct sockaddr_in serverAAddr, serverBAddr, serverCAddr;
    socklen_t serverAAddrLen = sizeof(serverAAddr);     // socket references for backend servers A, B and C
    socklen_t serverBAddrLen = sizeof(serverBAddr);
    socklen_t serverCAddrLen = sizeof(serverCAddr);

    bzero((char*)&serverAAddr, sizeof(serverAAddr));
    bzero((char*)&serverBAddr, sizeof(serverBAddr));
    bzero((char*)&serverCAddr, sizeof(serverCAddr));

    serverAAddr.sin_family = AF_INET;  // IPv4 format
    serverBAddr.sin_family = AF_INET;
    serverCAddr.sin_family = AF_INET;

    serverAAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // loopback address
    serverBAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverCAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    serverAAddr.sin_port = htons(30074);   // port no of server A
    serverBAddr.sin_port = htons(31074);   // port no of server B
    serverCAddr.sin_port = htons(32074);   // port no of server C
    struct sockaddr_in serverAddr;
    socklen_t serverAddrLen = sizeof(serverAddr);
	
	
  // Main Server Socket initialization and binding
    bzero((char*)&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(33074);  // port no
    bind(udpSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	cout<<"Main Server is up and running"<<endl;
	// get the department lists from backend servers A, B and  C
	
	
	char duffer[15000];
	string serverADepartments;  // string which stores server A's list
    string serverBDepartments;   //  ''  '' B
    string serverCDepartments;   // "" "" C
	sendto(udpSocket, "deptlist",sizeof("deptlist"), 0, (struct sockaddr*)&serverAAddr, serverAAddrLen);       // send the datagram conisiting of the string "deptlist" to the desired UDP server
    recvfrom(udpSocket, duffer, sizeof(duffer), 0, (struct sockaddr*)&serverAAddr, &serverAAddrLen);     // receive data from a backend server and store it in a buffer
    serverADepartments=duffer;

       
		
		cout <<" Main Server has received the department list from Backend server A using UDP over port 33074"<<endl;
        cout<<" "<<serverADepartments<<endl; 
		 
		 // The above procedure is repeated for server B and C respectively
		sendto(udpSocket, "deptlist",sizeof("deptlist"), 0, (struct sockaddr*)&serverBAddr, serverBAddrLen);
    recvfrom(udpSocket, duffer, sizeof(duffer), 0, (struct sockaddr*)&serverBAddr, &serverBAddrLen);
serverBDepartments = duffer;
		cout << " Main Server has received the department list from Backend server B using UDP over port 33074 "<<endl;
        cout<<" "<<serverBDepartments<<endl; 
		
		sendto(udpSocket, "deptlist",sizeof("deptlist"), 0, (struct sockaddr*)&serverCAddr, serverCAddrLen);
    recvfrom(udpSocket, duffer, sizeof(duffer), 0, (struct sockaddr*)&serverCAddr, &serverCAddrLen);
    serverCDepartments= duffer;
		cout << "Main Server has received the department list from Backend server C using UDP over port 33074 "<<endl;
        cout<<" "<<serverCDepartments<<endl; 
		
		// Finished getting the department lists
        cout << endl;



    while (true) { // Entering the chatting loop
        char buffer[15000];
        bzero(buffer, sizeof(buffer));
		    
        // Prompting the user to enter a department name
        cout << "Enter a Department Name: ";
        string departmentName;  // stores the dept name
        getline(cin, departmentName);

       

//  checking to see whether the user's query is a relevant dept available in any of the backend servers A,B or C, if found, gets the no of unique students under the queried dept, else prompts invalid dept name.

		if(serverADepartments.find(departmentName) != string :: npos){  
		    cout<<departmentName+" shows up in Server A"<<endl;
			cout<<"The Main Server has sent request for "+departmentName+" to server A using UDP over port 33074"<<endl;
        sendto(udpSocket, departmentName.c_str(), departmentName.length(), 0, (struct sockaddr*)&serverAAddr, serverAAddrLen);

        // Receive the  response from the backend server
        recvfrom(udpSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAAddr, &serverAAddrLen);
		// Display the response to the user
		cout<<"The Main Server has received the searching result(s) of "+departmentName+" from Backend Server A"<<endl;
        cout << " "<< buffer << endl;}
		
    
	else if(serverBDepartments.find(departmentName) != string :: npos){
		cout<<departmentName+" shows up in Server B"<<endl;
			cout<<"The Main Server has sent request for "+departmentName+" to server B using UDP over port 33074"<<endl;
        sendto(udpSocket, departmentName.c_str(), departmentName.length(), 0, (struct sockaddr*)&serverBAddr, serverBAddrLen);
         recvfrom(udpSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverBAddr, &serverBAddrLen);
         cout<<"The Main Server has received the searching result(s) of "+departmentName+" from Backend Server B"<<endl;
		 cout <<" "<< buffer << endl;
	}
	
	else if(serverCDepartments.find(departmentName) != string :: npos){
		cout<<departmentName+" shows up in Server C"<<endl;
			cout<<"The Main Server has sent request for "+departmentName+" to server C using UDP over port 33074"<<endl;
        sendto(udpSocket, departmentName.c_str(), departmentName.length(), 0, (struct sockaddr*)&serverCAddr, serverCAddrLen);
         recvfrom(udpSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverCAddr, &serverCAddrLen);
                   cout<<"The Main Server has received the searching result(s) of "+departmentName+" from Backend Server C"<<endl;

		  cout << " " << buffer << endl;
	}
	
	else{
		cout<<departmentName+" does not show up in Backend Servers"<<endl;
	}
	
	
	// Start all over again!!
	cout<<"===== Start a new query ====="<<endl;
	}
  //  close(udpSocket);
    return 0;
}


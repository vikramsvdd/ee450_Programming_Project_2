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
using namespace std;


// Exactly Same Implementation as serverA.cpp thus, for comments refer serverA.cpp
int main() {
    
    map<string, vector<string>> departmentToStudents;
    ifstream file("dataB.txt");  // read dataB.txt 

    if (!file.is_open()) {
        cerr << "Error opening file: dataB.txt" << endl;
        return 1;
    }

    string line;
    string currentDepartment;

    while (getline(file, line)) {
        if (!line.empty()) {
            if (isalpha(line[0])) {  
                currentDepartment = line;
            } else {
                // Split the line into individual student IDs using semicolon as a delimiter
                istringstream iss(line);
                string studentId;
                while (getline(iss, studentId, ';')) {
                    
                    departmentToStudents[currentDepartment].push_back(studentId);
                }
            }
        }
    }

    // Now, the departmentToStudents map contains the mapping of department names to student IDs

   

    file.close();

    
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serverAddr, mainServerAddr;
    socklen_t serverAddrLen = sizeof(serverAddr);
    socklen_t mainServerAddrLen = sizeof(mainServerAddr);
	
  bzero((char*)&mainServerAddr, sizeof(mainServerAddr));
    bzero((char*)&serverAddr, sizeof(serverAddr));
	mainServerAddr.sin_family=AF_INET;
    serverAddr.sin_family = AF_INET;
	mainServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	mainServerAddr.sin_port=htons(33074);
    serverAddr.sin_port = htons(31074);  
   bind(udpSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	cout<<" Server B is up and running using UDP on port 31074"<<endl;

    /*for (const auto& entry : departmentToStudents) {
        cout << "Department: " << entry.first << "\n";
        cout << "Student IDs: ";
        for (const string& studentID : entry.second) {
            cout << studentID << " ";
        }
        cout << "\n";
    }*/
	string departmentListMessage="";
	char duffer[15000];
	recvfrom(udpSocket, duffer, sizeof(duffer), 0, (struct sockaddr*)&mainServerAddr, &mainServerAddrLen);
       if(strcmp(duffer,"deptlist")==0){
        departmentListMessage += "ServerB : ";
    for (const auto& entry : departmentToStudents) {
        departmentListMessage += entry.first + ", ";
    }
	   sendto(udpSocket, departmentListMessage.c_str(), departmentListMessage.length(), 0, (struct sockaddr*)&mainServerAddr, mainServerAddrLen);}
       cout<<"Server B has sent a department list to Main Server"<<endl;

    while (true) {
        char buffer[15000];
        bzero(buffer, sizeof(buffer));
		

        recvfrom(udpSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&mainServerAddr, &mainServerAddrLen);
        cout<<"Server B has received a request for ";
		cout<<buffer<<endl;

      

          
        
            // Process the query and find the count of unique student IDs
            string departmentName = buffer;  // Extract the department name from the query

            if (departmentToStudents.find(departmentName) != departmentToStudents.end()) {
            // The department exists, calculate the count of unique students
            set<string> uniqueStudents;

            for (const string& studentId : departmentToStudents[departmentName]) {
                uniqueStudents.insert(studentId);
            }

            int uniqueStudentCount = uniqueStudents.size();
			string deptid="Student IDs: ";     // string to store the dept ids
            for(const std::string& value: departmentToStudents[departmentName]){
				deptid+=value+" ,";
			}

            // Create a response message
            string response = "Server B found  " + to_string(uniqueStudentCount) + " distinct student(s) for "+departmentName+" "+deptid;
            // Send the response back to the main server
			cout<<response<<endl;
            sendto(udpSocket, response.c_str(), response.length(), 0, (struct sockaddr*)&mainServerAddr, mainServerAddrLen);
			cout<<"Server B has sent the searching result to Main Server"<<endl;
			}
        
    }

    //pclose(udpSocket);
    return 0;
}


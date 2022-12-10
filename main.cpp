//=================================================================================
// Name         : Aditya Pandhare
// Description  : Starter Code for Flight-Ticket Management System using HASH TABLE in C++
//=================================================================================
#include<iostream>
#include<cstdlib>
#include<sstream>
#include<fstream>
#include<string>
#include<time.h>

#include "flightticket.h"
#include "flighthashtable.h"

//==========================================================
void listCommands()
{
    cout << "\n----------------------------------" << endl;
    cout << "import <path>          :Import flight-tickets from a CSV file" << endl
         << "export <path>          :Export flight-tickets to a CSV file" << endl
         << "count_collisions       :Print number of collisions" << endl
         << "add                    :Add a new flight-ticket" << endl
         << "delete <key>           :Delete a flight-ticket" << endl
         << "find <key>             :Find a flight-ticket's details" << endl
         << "allinday <date>        :Display all flight-tickets in a day" << endl
         << "printASC <key>         :Print flight-tickets in ascending order" << endl
         << "help                   :Display the available commands" << endl
         << "exit                   :Exit the program" << endl;
}
//==========================================================
//This functions checks if the flight number is an integer. If not, the function will not continue.
bool isNumber(string flightNo) {
    for(int i = 0; i < flightNo.length(); i++) {
        if(isdigit(flightNo[i]) == false) {return false; }
    }
    return true;
}
//==========================================================
int main(void)
{
    FlightHASHTABLE FHT(100003);
    listCommands();
    string user_input;
    string command;
    string parameter1, parameter2;
    
    string company, flightno, country_origin, country_destination, stopover, price, time_depart, time_arrive, date;
        
    do {
        do{
            cout << endl;
            cout << ">";
            getline(cin, user_input);
                    
            stringstream sstr(user_input);
            getline(sstr, command, ' ');    //Parses userinput into command.
            getline(sstr, parameter1, ','); //Parses userinput into parameter1 (file path or key - companyName)
            getline(sstr, parameter2);      //Parses userinput into another parameter2 (key - flightNumber)
            
            if(isNumber(parameter2) == false) {
                cout << "Flight Number must be an integer!" << endl;
            }
        }while(isNumber(parameter2) == false);
        
        
        //Conditional statements, if the user types a specific command, it should call the necessary methods. If they don't enter the correct command, then it is invalid.
        if(command == "import") {
            cout << FHT.importCSV(parameter1) << " ticket(s) imported." << endl;; }
                 
        else if(command == "export") {
            cout << FHT.exportCSV(parameter1) << " contact(s) exported." << endl; }
                 
        else if(command == "count_collisions" or command == "c") {
            cout << "Total Number of Collisions is " <<  FHT.count_collisions() << endl;
        }
        
        else if(command == "add" or command == "a") {
            fflush(stdin);
            //Print statements to ask the user to input the details of the contact they wish to add.
            cout << "Please enter the details of the flight-ticket: " << endl;
            cout << "Company Name: ";                   getline(cin, company);
            cout << "Flight Number: ";                  getline(cin, flightno);
            while(isNumber(flightno) == false) {
                cout << "Invalid! Flight Number must be an integer!" << endl;
                cout << "Flight Number: ";              getline(cin, flightno);
            }
            cout << "Country Of Origin: ";              getline(cin, country_origin);
            cout << "Country Of Destination: ";         getline(cin, country_destination);
            cout << "Stopover: ";                       getline(cin, stopover);
            cout << "Price: ";                          getline(cin, price);
            cout << "Time Of Departure: ";              getline(cin, time_depart);
            cout << "Time Of Arrival: ";                getline(cin, time_arrive);
            cout << "Date: ";                           getline(cin, date);

            //Generates the Flight Ticket Data and adds it to the Hash Table
            Flight_Ticket *data = new Flight_Ticket(company, stoi(flightno), country_origin, country_destination, stopover, price, time_depart, time_arrive, date);
            
            FHT.add(data);
            cout << "Flight Ticket Added!" << endl;
            fflush(stdin);
            }
                
                 
        else if(command == "delete" or command == "d") {
            FHT.removeRecord(parameter1, stoi(parameter2)); }
                 
        else if(command == "find" or command == "f") {
            //Timer variables: Beginning of the timer and end of the timer and the time itself
            clock_t begin, end;
            double time;
            
            //Start the clock and then call the find function. Once it ends, end the clock
            begin = clock();
            FHT.find(parameter1, stoi(parameter2));
            end = clock();
            
            //Take the time as the end - begining time
            time = ((double)end - begin)/CLOCKS_PER_SEC; //CLOCKS_PER_SEC = 1,000,000
            cout << "Time Taken: " << time << " seconds" << endl;
        }
        
        else if(command == "allinday" or command == "all") {
            FHT.allinday(parameter1);
        }
                
        else if(command == "printASC" or command == "pa") {
            if(isNumber(parameter2) == false) {
                cout << "Invalid! Flight Number must be an integer!" << endl;
            }
            
            else
                FHT.printASC(parameter1, stoi(parameter2));
        }
                 
        else if(command == "help") {listCommands(); }
                 
        else if(command == "exit")  {break; }
             
        else
            cout << "Invalid Input!" << endl;
            
        fflush(stdin);
    }
    while(true);

    return 0;
}



/*
 Functions that WORK:
 - Import
 - Export
 - Add
 - Find
 - Help, Exit
 - Delete
 - printASC
 
 
 Functions that don't WORK:
 - AllinDay
 - Collisions
 
 */

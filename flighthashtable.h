#ifndef FlightHASHTABLE_H
#define FlightHASHTABLE_H

#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include "flightticket.h"
#include "myList.h"

using namespace std;

class HashNode
{
    private:
        string key;                 //key = companyName+','+flightNumber or date
        Flight_Ticket value;        //value = flight ticket data
    
    public:
        HashNode() : key(""), value() {}
    
        HashNode(string key, Flight_Ticket value) {
            this->key = key;
            this->value = value;
        }
    
        string getKey() {return key;}
        Flight_Ticket getValue() {return value;}
        friend class FlightHASHTABLE;
};

//=============================================================================
class FlightHASHTABLE
{
    private:
        MyList<HashNode> *buckets;          //List of Buckets
        MyList<HashNode> *date_buckets;     //List of Buckets for Dates Hash Table (for AllinDay function)
        int size;                           //Current Size of HashTable
        int capacity;                       //Total Capacity of HashTable
        int probes;                         //Number of Collisions avoided (aka. no. of collisions)
    
    public:
        FlightHASHTABLE(int capacity);      //Constructor
        ~FlightHASHTABLE();                 //Destructor
        long hashCode(string key);          //Polynomial + MAD Hash Function
        int importCSV(string path);         //Loads all flight-tickets from a CSV file to the HashTable
        int exportCSV(string path);   //Exports all flight-tickets from HashTable to a CSV file in ASC order
        int count_collisions();             //Returns the number of collisions in the HashTable
        void add(Flight_Ticket* data);       //Adds a new flight ticket to the HashTable
        void removeRecord(string companyName, int flightNumber);  //Delete a record with key from the hashtable
        void find(string companyName, int flightNumber);           //Find and Display records with same key entered
        void allinday(string date);         //Find and Display records with same day entered
        void printASC(string companyName, int flightNumber);       //Display flights in ASC order
};

#endif

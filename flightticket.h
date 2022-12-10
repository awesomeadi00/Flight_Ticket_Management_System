#ifndef FLIGHT_TICKET_H
#define FLIGHT_TICKET_H

#include<iostream>
using namespace std;

class Flight_Ticket
{
    private:
        string companyName;
        int flightNumber;
        string country_of_origin;
        string country_of_destination;
        string stopOver;
        string price;
        string time_of_departure;
        string time_of_arrival;
        string date;

    public:
        Flight_Ticket();
        Flight_Ticket(string CompanyName, int FlightNumber, string Country_of_origin, string Country_of_Destination, string StopOver, string Price, string Time_of_Departure, string Time_of_Arrival, string Date);
    
    friend class FlightHASHTABLE;
    friend class HashNode;
};

#endif

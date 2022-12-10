#include "flightticket.h"

//Default Constructor for Flight Ticket:
Flight_Ticket::Flight_Ticket() {
    companyName = "-";
    flightNumber = 0;
    country_of_origin = "-";
    country_of_destination = "-";
    stopOver = "-";
    price = "-";
    time_of_departure = "-";
    time_of_arrival = "-";
    date = "-";
}


//Non-Default Constructor for initializing all details on a Flight Ticket
Flight_Ticket::Flight_Ticket(string CompanyName, int FlightNumber, string Country_of_origin, string Country_of_Destination, string StopOver, string Price, string Time_of_Departure, string Time_of_Arrival, string Date) {
    
    companyName = CompanyName;
    flightNumber = FlightNumber;
    country_of_origin = Country_of_origin;
    country_of_destination = Country_of_Destination;
    stopOver = StopOver;
    price = Price;
    time_of_departure = Time_of_Departure;
    time_of_arrival = Time_of_Arrival;
    date = Date;
}

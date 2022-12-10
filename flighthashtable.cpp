#include "flighthashtable.h"

//========================================================================
//Constructor:
FlightHASHTABLE::FlightHASHTABLE(int capacity) {
    buckets = new MyList<HashNode>[capacity];       //Array of lists/buckets of key: companyName+','+flightNumber
    date_buckets = new MyList<HashNode>[capacity];  //Array of lists/buckets of key: date

    this->capacity = capacity;
    this->size = 0;
    this->probes = 0;
}

//========================================================================
//Destructor:
FlightHASHTABLE::~FlightHASHTABLE() {
    while(!buckets->empty())
        buckets->pop_front();
    
    while(!date_buckets->empty())
        date_buckets->pop_front();
}

//========================================================================
//Hash Function: Polynomial Hash Code and MAD Compression Function
long FlightHASHTABLE::hashCode(string key) {
    unsigned long code(0);
    int c(3);      //c = 3 is optimal
    
    for(int i = 0; i < key.length(); i++) {
       code += (unsigned long)key[i] * pow(c, i);
    }
    
    return code % capacity;
}

//========================================================================
//ImportCSV: Loads all flight-tickets from a CSV file to the HashTable
int FlightHASHTABLE::importCSV(string path) {
    
    //Open an ifstream and if it fails then return -1.
    ifstream inFile(path);
    if(inFile.fail()) {
        cout << "File " << path << " could not be opened." << endl;
        return -1;
    }
        
    //While it's not the end of the file, it will read every attribute and store it into the flight_ticket array. Then initialize the flight_ticket through the constructor.
    int tickets(0);
    string line, data[9], country_origin, country_destination;
    char ch;
    getline(inFile, line); //Skip the first line
    
    while(!inFile.eof()) {
        
        getline(inFile, data[0], ','); //companyName
        getline(inFile, data[1], ','); //flightNumber
        
        //Parsing for data[2]: Country of Origin
        inFile >> ch;
        if(isalpha(ch)) {
            data[2] += ch;
            
            getline(inFile, country_origin, ',');
            data[2] += country_origin;
        }
        
        else
            getline(inFile, data[2], '"');

            
        //Parsing for data[3]: Country of Destination
        inFile >> ch;
        if(ch == ',') {
            inFile >> ch;
            if(isalpha(ch)) {
                data[3] += ch;
                
                getline(inFile, country_origin, ',');
                data[3] += country_origin;
            }
            
            else {
                getline(inFile, data[3], '"');
            }
        }
        
        else if(isalpha(ch)) {
            data[3] += ch;
            
            getline(inFile, country_destination, ',');
            data[3] += country_destination;
        }
        
        else {
            getline(inFile, data[3], '"');
            inFile >> ch;
        }
        
        
        getline(inFile, data[4], ','); //stopover
        getline(inFile, data[5], ','); //price
        getline(inFile, data[6], ','); //time_of_departure
        getline(inFile, data[7], ','); //time of arrival
        getline(inFile, data[8]);      //date

        Flight_Ticket *flight_ticket = new Flight_Ticket(data[0], stoi(data[1]), data[2], data[3], data[4], data[5], data[6], data[7], data[8]);
        add(flight_ticket);
        tickets++;
        
        data[2] = "";
        data[3] = "";
    }
    
    inFile.close();
    return tickets;
}

//========================================================================
//ExportCSV: Exports all flight-tickets from HashTable to a CSV file in ASC order
int FlightHASHTABLE::exportCSV(string path) {
    
    //Open of ofstream and if it fails then return -1
    ofstream outFile(path);
    if(outFile.fail()) {
        cout << "File could not be created." << endl;
        return -1;
    }
    
    //Checks if the Hash Table is empty, if so, export 0 Tickets
    if(size == 0) {
        cout << "No Tickets! Record is empty!" << endl;
        return 0;
    }
    
    int tickets(0);
    //For every cell in the Hash Table, if the bucket is not empty, for every HashNode in the bucket
    for(int i = 0; i < capacity; i++) {
        if(!buckets[i].empty()) {
            for(MyList<HashNode>::iterator it = buckets[i].begin(); it != buckets[i].end(); ++it) {
                HashNode data = *it;
                outFile << data.getValue().companyName << ","
                        << data.getValue().flightNumber << ","
                        << data.getValue().country_of_origin << ","
                        << data.getValue().country_of_destination << ","
                        << data.getValue().stopOver << ","
                        << data.getValue().price << ","
                        << data.getValue().time_of_departure << ","
                        << data.getValue().time_of_arrival << ","
                        << data.getValue().date << endl;
                tickets++;
            }
        }
    }
    
    return tickets;
}

//========================================================================
//No. Of Collisions: Returns the number of collisions in the HashTable
int FlightHASHTABLE::count_collisions() {
    return probes;
}

//========================================================================
//Add: Adds a new flight ticket to the HashTable
void FlightHASHTABLE::add(Flight_Ticket *data) {

    //Key Fixing:
    //=======================================================================================
    //Table 1: This takes the Uppercased CompanyName and for each letter, Lowercases them.
    string LowerCase, UpperCase = data->companyName;
    for(int i = 0; i < UpperCase.length(); i++) {
        LowerCase += tolower(UpperCase[i]);
    }
    
    string key = LowerCase + "," + to_string(data->flightNumber); //Create Key: companyName+','+flightNumber
    unsigned long index = hashCode(key); //Assign index of key through the hashCode() function.
    HashNode new_ticket(key, *data); //Create the new HashNode for main table
    //=======================================================================================
    //Table 2: This takes the Uppercased Date and for each letter, Lowercases them
    string LowerDate, UpperDate = data->date;
    
    //This takes the UpperCased Date passed from the user and for each letter, LowerCases them.
    for(int i = 0; i < UpperDate.length(); i++) {
        if(isalpha(UpperDate[i])) {
            LowerDate += tolower(UpperDate[i]);
        }
        
        else if(isdigit(UpperDate[i])) {
            LowerDate += UpperDate[i];
        }
    }
    
    string key2 = LowerDate; //Create Key: date
    unsigned long index2 = hashCode(key2); //Assign index of key2 through the hashCode() function.
    HashNode date_ticket(key2, *data); //Create the new HashNode for date table
    //=======================================================================================
    
    
    //Insertion:
    //=======================================================================================
    //Insertion for Hash Table 1: MAIN TABLE
    MyList<HashNode>::iterator it = buckets[index].begin();
    //We will first cycle through the bucket to check if the element already exists. If so, we do nothing
    while(true) {
        HashNode temp = *it;
        if(temp.value.companyName == data->companyName and
            temp.value.flightNumber == data->flightNumber and
            temp.value.country_of_origin == data->country_of_origin and
            temp.value.country_of_destination == data->country_of_destination and
            temp.value.stopOver == data->stopOver and
            temp.value.price == data->price and
            temp.value.time_of_departure == data->time_of_departure and
            temp.value.time_of_arrival == data->time_of_arrival and
            temp.value.date == data->date) {
            break;}

        
        //MAIN TABLE: Sorting the buckets during Insertion for MAIN TABLE: Comparing Country of Origin of new_ticket and entry ticket
        else if(data->country_of_origin < temp.value.country_of_origin) {
            buckets[index].addBefore(it.getIt(), new_ticket);
            break;
        }
        
        //MAIN TABLE: If it reaches to the end of the bucket, it means it's not in the hash table, hence pushback.
        else if(it == buckets[index].end()) {
            buckets[index].push_back(new_ticket);
            break;
        }
    
        //Iterate to the next HashNode in the bucket (list). Each time it iterates, it means it avoids a collision, hence we probe it.
        ++it;
        probes++;
    }
    //=======================================================================================
    //Insertion for Hash Table 2: DATE TABLE
    MyList<HashNode>::iterator ptr = date_buckets[index2].begin();
    //We will first cycle through the bucket to check if the element already exists. If so, we do nothing
    while(true) {
        HashNode temp2 = *ptr;
        if(temp2.value.companyName == data->companyName and
            temp2.value.flightNumber == data->flightNumber and
            temp2.value.country_of_origin == data->country_of_origin and
            temp2.value.country_of_destination == data->country_of_destination and
            temp2.value.stopOver == data->stopOver and
            temp2.value.price == data->price and
            temp2.value.time_of_departure == data->time_of_departure and
            temp2.value.time_of_arrival == data->time_of_arrival and
            temp2.value.date == data->date) {
            break;}

        
        //DATE TABLE: Sorting the buckets during Insertion for DATE TABLE: Comparing Departure Time of new_ticket and entry ticket
        else if(data->time_of_departure < temp2.value.time_of_departure) {
            date_buckets[index2].addBefore(ptr.getIt(), date_ticket);
            break;
        }
        
        //MAIN TABLE: If it reaches to the end of the bucket, it means it's not in the hash table, hence pushback.
        else if(ptr == date_buckets[index2].end()) {
            date_buckets[index2].push_back(date_ticket);
            break;
        }
    
        //Iterate to the next HashNode in the bucket (list)
        ++ptr;
    }
    //=======================================================================================
    
    size++;
}

//========================================================================
//Remove Record: Delete a record with key from the hashtable
void FlightHASHTABLE::removeRecord(string companyName, int flightNumber) {
 
    string LowerCase, UpperCase = companyName;
    
    //This takes the UpperCased CompanyName and for each letter, LowerCases them.
    for(int i = 0; i < UpperCase.length(); i++) {
        LowerCase += tolower(UpperCase[i]);
    }
    
    string key = LowerCase + "," + to_string(flightNumber); //Create Key
    unsigned long index = hashCode(key); //Assign index of key through the hashCode() function.
   
    int count(0), number(1);
    MyList<HashNode>::iterator it;
    
    //Will check how many records in the bucket have the same key
    for(it = buckets[index].begin(); it != buckets[index].end(); ++it) {
        HashNode temp = *it;
        if(temp.key == key) {count++; }
    }
    
    //If there are no records, it will print no records!
    if(count == 0) {
        cout << "There are no records of this flight!" << endl;
        return;
    }
    
    //Otherwise, it will print all of the records shown:
    cout << count << " record(s) found: " << endl;
    for(it = buckets[index].begin(); it != buckets[index].end(); ++it) {
        HashNode temp = *it;
        if(temp.key == key) {
            cout << number << ". " << temp.value.companyName << ", "
                 << temp.value.flightNumber << ", "
                 << temp.value.country_of_origin << ", "
                 << temp.value.country_of_destination << ", "
                 << temp.value.stopOver << ", "
                 << temp.value.price << ", "
                 << temp.value.time_of_departure << ", "
                 << temp.value.time_of_arrival << ", "
                 << temp.value.date << endl;
            number++;
        }
    }
    
    //Asks the user which record they wish to delete from this list
    int choice;
    cout << "Please select the record you want to delete: ";
    cin >> choice;
    while(choice <= 0 or choice > count) {
        cout << "Invalid Input! Please select a valid Record to delete: ";
        cin >> choice;
    }
    
    count = 1; //Reset count to search search the bucket for deletion.
    
    //Search through the bucket, if the restarted count == the choice of the user, then it will erase it.
    for(it = buckets[index].begin(); it != buckets[index].end(); ++it) {
        if(count == choice) {
            buckets[index].erase(it.getIt());
            size--;
            break;
        }
        
        else {count++; }
    }
    
    cout << "The flight-ticket record has been successfully deleted" << endl;
}

//========================================================================
//Find: Find and Display records with same key entered
void FlightHASHTABLE::find(string companyName, int flightNumber) {
    
    string LowerCase, UpperCase = companyName;
    
    //This takes the UpperCased CompanyName and for each letter, LowerCases them.
    for(int i = 0; i < UpperCase.length(); i++) {
        LowerCase += tolower(UpperCase[i]);
    }
    
    string key = LowerCase + "," + to_string(flightNumber); //Create Key
    unsigned long index = hashCode(key); //Assign index of key through the hashCode() function.
        
    int count(0), number(1);
    MyList<HashNode>::iterator it;
    
    //Will check how many records in the bucket have the same key
    for(it = buckets[index].begin(); it != buckets[index].end(); ++it) {
        HashNode temp = *it;
        if(temp.key == key) {count++; }
    }
    
    //If there are no records, it will print no records!
    if(count == 0) {
        cout << "There are no records!" << endl;
        return;
    }
    
    //Otherwise, it will print all of the records shown:
    cout << count << " record(s) found: " << endl;
    for(it = buckets[index].begin(); it != buckets[index].end(); ++it) {
        HashNode temp = *it;
        if(temp.key == key) {
            cout << number << ". " << "Company Name: " << temp.value.companyName << endl;
            cout << "Flight Number: " << temp.value.flightNumber << endl;
            cout << "Country of Origin: " << temp.value.country_of_origin << endl;
            cout << "Country of Destination: " << temp.value.country_of_destination << endl;
            cout << "Stopover: " << temp.value.stopOver << endl;
            cout << "Price: " << temp.value.price << endl;
            cout << "Time of Departure: " << temp.value.time_of_departure << endl;
            cout << "Time of Arrival: " << temp.value.time_of_arrival << endl;
            cout << "Date: " << temp.value.date << endl;
            cout << endl;
            number++;
        }
    }
}

//========================================================================
//All-In-Day: Find and Display records with same day entered
void FlightHASHTABLE::allinday(string date) {
    
    string user_UpperDate = date, user_LowerDate = "";
    string LowerDate, UpperDate;
    int number(1), count(0);
    
    //This takes the UpperCased Date passed from the user and for each letter, LowerCases them.
    for(int i = 0; i < user_UpperDate.length(); i++) {
        if(isalpha(user_UpperDate[i])) {
            user_LowerDate += tolower(user_UpperDate[i]);
        }
        
        else if(isdigit(user_UpperDate[i])) {
            user_LowerDate += user_UpperDate[i];
        }
    }
        
    string key = user_LowerDate; //Create Key: date
    unsigned long index = hashCode(key); //Assign index of key2 through the hashCode() function.
    
    
    //This searches through the date_buckets Hash Table and checks how many dates are within this bucket.
    MyList<HashNode>::iterator it;
    for(it = date_buckets[index].begin(); it != date_buckets[index].end(); ++it) {
        HashNode temp = *it;
        if(temp.key == key) {count++; }
    }
    
    //If there are no records, it will print no records!
    if(count == 0) {
        cout << "There are no records!" << endl;
        return;
    }
    
    //Otherwise, it will print all of the records shown:
    cout << count << " record(s) found: " << endl;
    for(it = date_buckets[index].begin(); it != date_buckets[index].end(); ++it) {
        HashNode temp = *it;
        if(temp.key == key) {
            cout << number << ". " << temp.value.companyName << ", "
                 << temp.value.flightNumber << ", "
                 << temp.value.country_of_origin << ", "
                 << temp.value.country_of_destination << ", "
                 << temp.value.stopOver << ", "
                 << temp.value.price << ", "
                 << temp.value.time_of_departure << ", "
                 << temp.value.time_of_arrival << ", "
                 << temp.value.date << endl;
            number++;
        }
    }
    
    //If number if still '1', it means that no flight tickets have been found, hence print No flights and return.
    if(number == 1) {
        cout << "No Flights on this Date!" << endl;
        return;
    }
}

//========================================================================
//PrintASC: Display all flights in ASC order
void FlightHASHTABLE::printASC(string companyName, int flightNumber) {

    string LowerCase, UpperCase = companyName;
    
    //This takes the UpperCased CompanyName and for each letter, LowerCases them.
    for(int i = 0; i < UpperCase.length(); i++) {
        LowerCase += tolower(UpperCase[i]);
    }
    
    string key = LowerCase + "," + to_string(flightNumber); //Create Key
    unsigned long index = hashCode(key); //Assign index of key through the hashCode() function.
    
    int count(0), number(1);
    MyList<HashNode>::iterator it;
    
    //Will check how many records in the bucket have the same key
    for(it = buckets[index].begin(); it != buckets[index].end(); ++it) {
        HashNode temp = *it;
        if(temp.key == key) {count++; }
    }
    
    //If there are no records, it will print no records!
    if(count == 0) {
        cout << "There are no records!" << endl;
        return;
    }
    
    //This will cycle through the bucket and if they key is the same, it will print them out.
    for(it = buckets[index].begin(); it != buckets[index].end(); ++it) {
        HashNode temp = *it;
        if(temp.key == key) {
            cout << number << ". " << temp.value.companyName << ", "
                 << temp.value.flightNumber << ", "
                 << temp.value.country_of_origin << ", "
                 << temp.value.country_of_destination << ", "
                 << temp.value.stopOver << ", "
                 << temp.value.price << ", "
                 << temp.value.time_of_departure << ", "
                 << temp.value.time_of_arrival << ", "
                 << temp.value.date << endl;
            number++;
        }
    }
}


//==================================================HASHCODES====================================================

/*
==================================================
 1: Summation Code + Division Compression
 
 unsigned long code = 0;
 for(int i = 0; i < key.length(); i++) {
    code += (unsigned long)key[i];
 }
 
 return code % capacity;
 
==================================================
 2. Summation Code + MAD Compression
 
 unsigned long code = 0;
 int a, b, p;
 
 p = 101203; //prime number > capacity, for now we have randomly chosen this number
 a = 50; //randomly chosen a (any random below p)
 b = 12; //randomly chosen b (any random below p)
 
 for(int i = 0; i < key.length(); i++) {
    code += (unsigned long)key[i];
 }
 
 return ((a*code + b) % p) % capacity;

==================================================
 3. Polynomial Code + Division Compression
 
 unsigned long code(0);
 int c(3);      //c = 3 is optimal
 
 for(int i = 0; i < key.length(); i++) {
    code += (unsigned long)key[i] * pow(c, i);
 }
 
 return code % capacity;
 
==================================================
 4. Polynomial Code + MAD Compression
 
 unsigned long code(0);
 int c(3);      //c = 3 is optimal
 
 int a, b, p;
 p = 101203; //prime number > capacity, for now we have randomly chosen this number
 a = 50; //randomly chosen a (any random below p)
 b = 12; //randomly chosen b (any random below p)
 
 for(int i = 0; i < key.length(); i++) {
    code += (unsigned long)key[i] * pow(c, i);
 }
 
 return ((a*code + b) % p) % capacity;

==================================================
 5. Cyclic Code + Division Compression

 unsigned long code = 0;
 for(int i = 0; i < key.length(); i++){
     code = (code << 5) | (code >> 27);
     code += (unsigned long) key[i];
 }
 return code % capacity;

 
 ==================================================
 6. Cyclic Code + MAD Compression
 
 unsigned long code = 0;
 
 int a, b, p;
 p = 101203; //prime number > capacity, for now we have randomly chosen this number
 a = 50; //randomly chosen a (any random below p)
 b = 12; //randomly chosen b (any random below p)
 
 
 for(int i = 0; i < key.length(); i++){
     code = (code << 5) | (code >> 8);
     code += (unsigned long) key[i];
 }
 
 return ((a*code + b) % p) % capacity;
 */


#include "database.h"
#include <iostream>
#include <fstream>
using namespace std;

void create_index(ifstream& file, Database& database)
{
	int counter = 0;
	while(true)
	{
	    const auto offset = file.tellg();
	    
	    Person person;
	    file.read(reinterpret_cast<char *>(&person), sizeof(Person));
	    if(!file)
	        break;
	    database.insert_record(person.bdate, offset);
	    ++counter;
	    if (counter == 10)
	    {
	        file.ignore(46);
	        counter = 0;
	    }
	}
}

int main()
{
    int counter = 0;
    ifstream file;

    file.open("../large.bin", ios::in | ios::out | ios::binary);
    if (file.fail())
    {
        cout << "ERROR: Cannot open the file..." << endl;
        return -1;
    }

    Database db(Database::Write);
    create_index(file, db);

    file.close();
    return 0;

}
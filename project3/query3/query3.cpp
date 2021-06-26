#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "query3.h"
#include "database.h"

using namespace std;

// function to check if age is less than 21 
bool is_age(DATE &date)
{
    time_t tt; 
    struct tm * ti; 
    time (&tt); 
    ti = localtime(&tt); 

    int current_year = ti->tm_year+1900;
    if ((current_year - date.year) < 21)
        return true;
    else
        return false;
}

int main()
{
    ifstream infile;

    infile.open("../large.bin", ios::in | ios::out | ios::binary);
    if (infile.fail())
    {
        cout << "ERROR: Cannot open the file..." << endl;
        return -1;
    }

    // read offset
    int people_below_21 = 0;
    int counter = 0;
    Database db(Database::Read);
    auto key = db.first();
    while (key)
    {
        if (is_age(*key))
        {
            const auto offsets = db.fetch_record(*key);
            for (const auto offset : offsets)
            {
                if(offset == 0)
                    continue;

                cout<<"\nLocation on disk :" << offset;
                infile.seekg(offset);

                Person person;
                infile.read(reinterpret_cast<char *>(&person), sizeof(Person));

                cout<<"\nName :"<<person.Fname <<" "<<person.Lname;
                cout<<"\nSSN  :"<<person.ssn;
                cout<<endl;

                ++people_below_21;
            }
        }

        key = db.next();
    }

    cout<<"\nTotal users under the age of 21 :"<<people_below_21<<endl;

    infile.close();
    return 0;
 }
  

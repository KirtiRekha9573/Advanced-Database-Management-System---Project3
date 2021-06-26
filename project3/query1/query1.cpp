#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

#define BLOCK_SIZE      10
#define FILE_SIZE       4096

// Structure definitions
struct DATE
{
    int32_t day;
    int32_t month;
    int32_t year;
};

#pragma pack(push, 1)
struct Person
{
    char Fname[20];
    char Lname[20];
    char job[70];
    char company[40];
    char address[80];
    char phone[26];
    DATE bdate;
    char ssn[12];
    char username[25];
    char email[50];
    char url[50];
};
#pragma pack(pop)

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

    int people_below_21 = 0;
    int counter = 0;
    int next_block = 0;

    infile.open("../large.bin", ios::in | ios::out | ios::binary);
    if (infile.fail())
    {
        cout << "ERROR: Cannot open the file..." << endl;
        return -1;
    }

    // Calculate bytes to move to next block
    next_block = (FILE_SIZE - (sizeof(Person)*BLOCK_SIZE));

    // Create person object
    Person person;
    while(true)
    {
        // Read record
        infile.read(reinterpret_cast<char *>(&person), sizeof(Person));

        if(!infile)
            break;  

       if(is_age(person.bdate))
       {
            cout<<"\nName :"<<person.Fname <<" "<<person.Lname;
            cout<<"\nSSN  :"<<person.ssn;
            cout<<endl;
            people_below_21++;
       } 
        ++counter;
        if (counter == 10)
        {
            infile.ignore(next_block);
            counter = 0;
        }
    }

    cout<<"\nTotal users under the age of 21 :"<<people_below_21<<endl;
    infile.close();
 	return 0;
 }
  

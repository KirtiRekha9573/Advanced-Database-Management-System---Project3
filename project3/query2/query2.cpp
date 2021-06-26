#include <iostream>
#include <fstream>
#include <gdbm.h>
#include <string.h>

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

int main()
{

    ifstream infile;
    int next_block ;

    infile.open("../large.bin", ios::in | ios::out | ios::binary);
    if (infile.fail())
    {
        cout << "ERROR: Cannot open the file..." << endl;
        return -1;
    }

    // Open gdbm database
    GDBM_FILE dbfile;
    dbfile = gdbm_open("person.gdb", 4096, GDBM_WRCREAT | GDBM_NOLOCK , 0664, NULL);
    if(dbfile == NULL)
    {
        cout<<"\nERROR: Could not open the DB file.";
        return 1;
    }

    int dbRet;
    datum key, val;
    int success_count = 0;
    int fail_count = 0;
    int counter = 0;  
    
    next_block = (FILE_SIZE - (sizeof(Person)*BLOCK_SIZE));

    Person person;
    
    // Read bin file block by block
    while(true)
    {
        // read record
        infile.read(reinterpret_cast<char *>(&person), sizeof(Person));

        if(!infile)
           break;  
        //write to gdbm
        key.dptr = person.ssn;
        key.dsize = strlen(person.ssn)+1;
        val.dptr = person.Fname;
        val.dsize = strlen(person.Fname)+1;
        
        dbRet = gdbm_store(dbfile, key, val, GDBM_INSERT);
        switch(dbRet)
        {
            case 0:
                //cout<<"\nSuccess insert Key "<<(char *)key.dptr<<" => "<<(char *)val.dptr<<"\n";
                success_count++;
                break;
            case 1: 
            {
                cout<<"\nDuplicate key"<<(char *)key.dptr<<" => "<<(char *)val.dptr<<"\n"; 
                fail_count++;
                break;
            }
            default:
                cout<<"\nERROR: Could not insert item "<<(char *)key.dptr<<" , "<<(char *)val.dptr<<"\n";
                return 1;
        }
       ++counter;
       if (counter == 10)
       {
           infile.ignore(next_block);
           counter = 0;
       }
    }

    //cout<<"\nTotal successful key insert ="<<success_count<<endl;
    cout<<"Total duplicate key count ="<<fail_count<<endl;
    
    gdbm_close(dbfile);    
    infile.close();
    return 0;
}


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
	file.clear();
}

void reorder_data_file(std::ifstream& file, std::ofstream& out_file, Database& database)
{
	int counter = 0;
	auto key = database.first();
	while (key)
	{
		const auto offsets = database.fetch_record(*key);
		std::vector<int> new_offsets;
		for (const auto offset : offsets)
		{
			file.seekg(offset);

			Person person;
			file.read(reinterpret_cast<char *>(&person), sizeof(Person));

			new_offsets.push_back(out_file.tellp());
			out_file.write(reinterpret_cast<char *>(&person), sizeof(Person));
			++counter;
			if(counter == 10)
			{
				// add disk spacer.
				std::fill_n(std::ostreambuf_iterator<char>(out_file), 46, '\0');
				counter = 0;
			}
		}
		database.replace(*key, new_offsets);

		key = database.next();
	}
}

int main()
{
    int counter = 0;
    ifstream file;

    file.open("../large.bin", ios::binary);
    if (file.fail())
    {
        cout<<"ERROR: Cannot open the file..."<<endl;
        return -1;
    }

    Database db(Database::Write);
    create_index(file, db);

    ofstream new_file("clustered.bin", ios::binary);
    if(!new_file.is_open())
    {
    	cout<<"failed to create new file"<<endl;
    	return 1;
    }

    reorder_data_file(file, new_file, db);

    file.close();
    return 0;
}
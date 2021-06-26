#include <iostream>
#include <gdbm.h>
#include <vector>
#include <optional>
#include <cstring>
#include "database.h"

namespace
{
	datum to_datum(DATE& date)
	{
		datum value;
		value.dptr = reinterpret_cast<char *>(&date);
		value.dsize = sizeof(date);

		return value;
	}

	datum to_datum(std::vector<int>& offsets)
	{
		datum value;
		value.dptr = reinterpret_cast<char *>(offsets.data());
		value.dsize = offsets.size() * sizeof(int);
		return value;
	}

	DATE to_date(const datum& value)
	{
		DATE date;
		std::memcpy(&date, value.dptr, value.dsize);
		return date;
	}

	std::vector<int> to_int_vector(const datum& value)
	{
		std::vector<int> offsets;
		offsets.resize(value.dsize/sizeof(int));
		std::memcpy(offsets.data(), value.dptr, value.dsize);
		free(value.dptr);
		return offsets;
	}
}

Database::Database(const Mode mode)
{
	dbfile = gdbm_open("index.gdb", 4096, GDBM_WRCREAT | GDBM_NOLOCK | GDBM_SYNC, 0664, NULL);
	if(dbfile == NULL)
	{
		throw std::runtime_error("Error opening db file");
	}
}

void Database:: insert_record(DATE date, const int offset)
{
	auto key = to_datum(date);		//convert date member struct to datum
	
	std::vector<int> values;
	if (static_cast<bool>(gdbm_exists(dbfile, key)))
	{
		const auto result = gdbm_fetch(dbfile, key);
		values = to_int_vector(result);
		//cout << "duplicate for " << date << "offsets : " << values.size() + 1 << std::endl;
	}

	values.push_back(offset);
	auto value = to_datum(values);

	gdbm_store(dbfile, key, value, GDBM_REPLACE);
}

std::vector<int> Database::fetch_record(DATE date)
{
	auto key = to_datum(date);

	const auto result = gdbm_fetch(dbfile, key);
	return to_int_vector(result);
}


std::optional<DATE> Database::first()
{
	datum key = gdbm_firstkey(dbfile);
	if(key.dptr) {
		currentKey = key;
		return to_date(key);
	}

	return std::nullopt;
}

std::optional<DATE> Database::next()
{
	datum key = gdbm_nextkey(dbfile, *currentKey);
	if(key.dptr) {
		free(currentKey->dptr);
		currentKey = key;
		return to_date(key);
	}

	//std::cout << std::boolalpha << currentKey.has_value() << std::endl;
	gdbm_count_t count;
	gdbm_count(dbfile, &count);
	//std::cout << " no next key, key count: " << count << std::endl;

	return std::nullopt;
}

Database::~Database()
{
	if(dbfile)
		gdbm_close(dbfile);
}

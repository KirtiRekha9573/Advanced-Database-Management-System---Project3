#include <vector>
#include <gdbm.h>
#include "query4.h"
#include <optional>

class Database
{
	public:
	enum Mode: int
	{
		Read = GDBM_READER,
		Write = GDBM_WRCREAT
	};

	Database(const Mode mode);
	~Database();
	std::vector<int> get_record(DATE date);
	void insert_record(DATE date, const int offset);
	std::vector<int> fetch_record(DATE date);
	void replace(DATE date, std::vector<int> offsets);
	std::optional<DATE> first();
	std::optional<DATE> next();

	private:
	std::optional<datum> currentKey;
	GDBM_FILE dbfile;
};

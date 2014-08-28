# include <algorithm>
# include <iostream>

# if _WIN32 || _WIN64
#  include <sstream>
#  include <Windows.h>
# endif // !WIN32 || _WIN64

#include "tracer.h"

namespace nq
{
	Tracer::Entry::Entry()
		:file_(nullptr),
		line_(0),
		size_(0)
	{}
	Tracer::Entry::Entry(const char *file, int line, int size)
		: file_(file),
		line_(line),
		size_(size)
	{}

	void Tracer::Add(void* p, const char* file, int line, int size)
	{
		/* If lock_ is positive (so we are calling the map_ new),
		** return to avoid infinite recursion */
		if (lock_ > 0)
			return;
		/* Locking before inserting into the map */
		Tracer::Lock l(*this);
		map_[p] = Entry(file, line, size);
	}

	void Tracer::Remove(void* p)
	{
		/* If lock_ is positive (so we are calling the map_ new),
		** return to avoid infinite recursion */
		if (lock_ > 0)
			return;
		/* Locking before removing from the map */
		Tracer::Lock l(*this);
		auto it = map_.find(p);
		if (it != map_.end())
			map_.erase(p);
	}

# ifdef _WIN32 || _WIN64
	/* Specialized for windows */
	void Tracer::Dump()
	{
		std::for_each(begin(map_), end(map_),
			[](const std::pair<void *, Entry>& p)
		{
			std::stringstream ss;
			ss << "Memory leak found!\n"
				<< "From the ptr: " << "0x" << std::hex << p.first << "\n"
				<< "In file(line): " << std::dec << p.second.file() << "(" << p.second.line() << ")\n"
				<< "Of size : " << p.second.size() << " bytes\n"
				<< "------------------------------\n"
				<< std::endl;
			OutputDebugString(ss.str().c_str());
		});
	}
# else // _WIN32 || _WIN64
	void Tracer::Dump()
	{
		std::for_each(begin(map_), end(map_),
			[](const std::pair<void *, Entry>& p)
		{
			std::cout << "Memory leak found!\n"
				<< "From the ptr: " << "0x" << std::hex << p.first << "\n"
				<< "In file(line): " << std::dec << p.second.file() << "(" << p.second.line() << ")\n"
				<< "Of size : " << p.second.size() << " bytes\n"
				<< "------------------------------\n"
				<< std::endl;
		});
	}
# endif // !_WIN32 || _WIN64
}
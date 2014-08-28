#ifndef TRACER_H_
# define TRACER_H_

# include <map>

/* The Tracer allocate 32 bytes of memory at each call to new (that's huge) */

namespace nq
{
	/* Tracer is a singleton logging every pass by new the user done 
	allows to locate the leaks (by file, line ...) if the user entered it */
	class Tracer
	{
	private:
		/* Entry store the datas linked to the allocated ptr */
		class Entry
		{
		public:
			explicit Entry();
			explicit Entry(const char *file, int line, int size);

			const char* file() const { return file_; }
			const int line() const { return line_; }
			const int size() const { return size_; }

		private:
			const char* file_;
			int line_;
			int size_;
		};

		/* Lock call the subclass (Tracer) lock()/unlock() methods
		** uppon construction and destruction to avoid infinite recursion */
		class Lock
		{
		public:
			explicit Lock(Tracer& t)
				: t_(t)
			{ t_.lock(); }

			~Lock()
			{ t_.unlock(); }
			Tracer& t_;
		};
	public:
		explicit Tracer()
			: lock_(0),
			map_()
		{ Ready = true; }

		~Tracer()
		{
			Dump();
			Ready = false;
		}

		/* Add to the map the ptr p his logged data when he's allocated */
		void Add(void* p, const char* file, int line, int size);

		/* Remove from the map the ptr p */
		void Remove(void* p);

		/* Pretty print the current new leaking */
		void Dump();
	private:
		void lock()
		{ lock_++; }
		void unlock()
		{ lock_--; }

	public:
		static bool Ready;
	private:
		/* Tracer datas */
		int lock_;
		std::map<void*, Entry> map_;
	};
}

#endif // !TRACER_H_
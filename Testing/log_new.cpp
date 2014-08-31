# include <new>
# include <cstdlib>
# include "tracer.h"

extern nq::Tracer NewTrace;
bool nq::Tracer::Ready = false;

/* Change the operator new so it calls nq::allocator but it dont have the type ....*/

/* Standard operator new */
void *operator new(size_t size)
{
	void *p = std::malloc(size);
	if (p == nullptr)
		throw std::bad_alloc();
	if (nq::Tracer::Ready)
		NewTrace.Add(p, "unknown", 0, size);

	return p;
}

/* Standard operator delete */
void operator delete(void *p)
{
	if (nq::Tracer::Ready)
		NewTrace.Remove(p);
	std::free(p);
}

/* Logged operator new */
void *operator new(size_t size, const char *file, int line)
{
	void *p = std::malloc(size);
	if (p == nullptr)
		throw std::bad_alloc();

	if (nq::Tracer::Ready)
		NewTrace.Add(p, file, line, size);
	return p;
}

/* Logged operator delete */
void operator delete (void *p, const char *file, int line)
{
	if (nq::Tracer::Ready)
		NewTrace.Remove(p);
	free(p);
}

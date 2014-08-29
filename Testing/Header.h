#include <cstddef>

namespace test{
	template<class T, class X = UnknownDomain, class F = DefaultAlloc> class test_allocator
	{
	public:
		typedef T * pointer;
		typedef T const * const_pointer;
		typedef T & reference;
		typedef T const & const_reference;
		typedef T value_type;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

	public:
		template<class U> struct rebind
		{
			typedef test_allocator<U, X, F> other;
		};

		pointer address(reference r) const
		{
			return &r;
		}

		const_pointer address(const_reference s) const
		{
			return &s;
		}

		explicit test_allocator()
		{
		}

		template<class U> test_allocator(test_allocator<U, X> const & r)
		{
		}

		template<class U> test_allocator & operator=(test_allocator<U, X, F> const & r)
		{
			return *this;
		}

		void deallocate(pointer p, size_type n)
		{
			::operator delete(p);
		}

		pointer allocate(size_type n)
		{
			T * p = static_cast<T*>(::operator new(n * sizeof(T)));
			return p;
		}

		void construct(pointer p, T const & t)
		{
			new(p)T(t);
		}

		void destroy(pointer p)
		{
			p->~T();
		}

		size_type max_size() const
		{
			return size_type(-1) / sizeof(T);
		}
	};
	template<class T, class U, class X> inline bool operator==(test_allocator<T, X> const & a1, test_allocator<U, X> const & a2)
	{
		return true;
	}

	template<class T, class U, class X> inline bool operator!=(test_allocator<T, X> const & a1, test_allocator<U, X> const & a2)
	{
		return false;
	}
	/*
	template<> class test_allocator<void, void> : public test_allocator_base
	{
	public:
		typedef void * pointer;
		typedef void const * const_pointer;
		typedef void value_type;

		template<class U> struct rebind
		{
			typedef test_allocator<U, void> other;
		};

		explicit test_allocator(int id = 0) : test_allocator_base(id)
		{
		}

		template<class U, class X> test_allocator(test_allocator<U, X> const & r) : test_allocator_base(r)
		{}

		template<class U, class X> test_allocator & operator=(test_allocator<U, X> const & r)
		{
			test_allocator_base::operator=(r);
			return *this;
		}
	};*/
}
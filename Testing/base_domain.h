#ifndef BASE_DOMAIN_H_
# define BASE_DOMAIN_H_

# include <cassert>
# include <cstddef>

# include "env_maccro.h"

/*
** Domains log memory allocation through different cases so we can know
** what kind of part of the app take more memory or allocate the most.
** They are singletons inheriting from BaseDomain (which is an interface).
** Domains are double linked lists of Headers.
** Headers are placed (by the allocator!) before all the allocated memory
** so we can recover them with trivial pointer arithmetic.
** Placing them there avoid useless extra allocation for every logging.
*/

class BaseDomain
{
private:
	// PAUL(29/09/2014) destructor only have to know about Header Structure.
	friend void operator delete(void *usr_ptr) noexcept;
	
	class Header
	{
	private:
		Header *prev_;
		Header *next_;
		/* We use a size_t instead of others int (uint**_t, etc..) to get
		* a 32 and 64 bits adaptability */
		const size_t size_;

		/* The padding is here for allignement */
		const size_t padding_;
	public:
		Header(size_t size, size_t padding = 0,
                Header *prev = nullptr, Header *next = nullptr)
			: prev_(prev),
			next_(next),
			size_(size),
			padding_(padding)
		{}
		void add(Header *next);
		void remove();

		/* remove_begin()/remove_end() return a Header* so the Domain can put
         * his begin_/end_ pointers up-to-date */
		Header* remove_begin();
		Header* remove_end();

		const size_t size() const { return size_; }

		/* printer for the debug, will probably change */
		void print() const;
	};

private:
    class SubHeader : public Header
    {
    private:
        const char *file_;
        size_t line_;
        BaseDomain *dom_;
        size_t nothing_;
    public:
        SubHeader(size_t size, const char *file, size_t line, BaseDomain *dom)
            : Header(size, 1, nullptr, nullptr),
            file_(file),
            line_(line),
            dom_(dom),
            nothing_(0)
        {}

        const char *get_file() const { return file_; }
        size_t get_line() const { return line_; }
		BaseDomain *get_domain() const { return dom_; }
    };
private:
	size_t count_; // The number of non freed allocation in the domain
public:
	size_t get_count() const { return count_; }

private:
	Header *begin_;
	Header *end_;
public:
	enum HSENUM { header_size = sizeof(Header),
        sub_header_size = sizeof(SubHeader)};

	/* Add the Header constructed with size at the ptr location to the
     * current Domain's list */

	void add(void *internal_ptr, size_t size);

    //TEST
    void add(void* internal_ptr, std::size_t size,
        const char *file, size_t line, BaseDomain *dom);
    //!TEST

	/* Remove from the current Domain's list the Header associated with
     * the allocated ptr send */
	void remove(void *internal_ptr);

    //TEST
    virtual void virtual_remove(void *internal_ptr) = 0;
    //!TEST

protected:
	/* BaseDomain is an interface  it's constructor can't be called */
	BaseDomain()
		: count_(0),
		begin_(nullptr),
		end_(nullptr)
	{}
private:
	BaseDomain(const BaseDomain&) {}
	BaseDomain& operator= (const BaseDomain&) { return *this; };

	/* return a string of the domain name for the printer */
	virtual const char* domain_name() const
    { assert(!"How did you got here!?"); return "never_reached"; };

public:
    /* printer for the debug, will probably to change */
	void print() const;

# ifdef NQ_ENV_32
		static_assert(sizeof(Header) == 16,
                "Header don't take 16 bytes in 32 bits");
# else // NQ_ENV_32
		static_assert(sizeof(Header) == 32,
                "Header don't take 32 bytes in 64 bits");
# endif // !NQ_ENV_32
};

/* Generic declaration of a Domain to avoid copy paste at every
 * new domain creation */
# define NQ_DOMAIN(new_domain)	      \
class new_domain : public BaseDomain  \
{                                     \
public:                               \
	static new_domain& getInstance()  \
	{                                 \
		static new_domain instance;   \
		return instance;              \
	}                                 \
    virtual void virtual_remove(void *internal_ptr) override\
    {\
        this->remove(internal_ptr);\
    }\
private:                              \
	virtual const char* domain_name() const { return #new_domain; } \
};

#endif // !BASE_DOMAIN_H_

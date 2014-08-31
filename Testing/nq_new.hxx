/* TODO include the news log_new here make some fusuin things */
/* Now handling line file but really those maccros -.- */


template <typename Domain, class AllocStrat>
void *operator new(size_t size, Domain)
{
	std::cout << "What you gonna do guyz" << std::endl;
	return nq::allocator < int, Domain, AllocStrat > {}.allocate(size);
}

template <typename Domain>
void *operator new(size_t size, Domain, const char* file, int line)
{
	//nq::malloc(size, domain, 0, nullptr);
	std::cout << "all your bases are belong to us" << std::endl;
	void *p = std::malloc(size);
	if (p == nullptr)
		throw std::bad_alloc();
	return p;
}

template <typename Domain, class AllocStrat>
void *operator new(size_t size, Domain, AllocStrat)
{
	std::cout << "What you gonna do guyz" << std::endl;
	return nq::allocator < int, Domain, AllocStrat > {}.allocate(size);
}

template <typename Domain>
void *operator new(size_t size, Domain)
{
	//nq::malloc(size, domain, 0, nullptr);
	std::cout << "all your bases are belong to us" << std::endl;
	void *p = std::malloc(size);
	if (p == nullptr)
		throw std::bad_alloc();
	return p;
}

template <typename Domain>
void operator delete(void* p, Domain)
{
	std::cout << "mb they dont :D" << std::endl;
	free(p);
}

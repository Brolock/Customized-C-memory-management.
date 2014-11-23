# Novaquark Memlib usage


## Headers

**Environmental headers**

```
<nq_memlib/domains.h>

<nq_memlib/alloc_strat.h>

<nq_memlib/env_maccro.h>
```

**Memory headers**

```
<nq_memlib/nq_new.h>

<nq_memlib/nq_shared.h>

<nq_memlib/nq_unique.h>
```


***Containers headers***

```
<nq_memlib/nq_vector.h>

<nq_memlib/nq_list.h>

<nq_memlib/nq_forward_list.h>

<nq_memlib/nq_map.h>

<nq_memlib/nq_multimap.h>

<nq_memlib/nq_unordered_map.h>

<nq_memlib/nq_unordered_multimap.h>

<nq_memlib/nq_set.h>

<nq_memlib/nq_multiset.h>

<nq_memlib/nq_unordered_set.h>

<nq_memlib/nq_unordered_multiset.h>

<nq_memlib/nq_deque.h>
```

**Memlib specific allocations methods (only for very specific needs)**

```
<nq_memlib/nq_deleter.h>

<nq_memlib/nq_allocator.h>
```

## Adding a new Domain
1. Open the file `memlib/domains/domains_decl.h`
2. Add your Domain with the macro `NQ_USR_DOMAIN(MyDomainName)`
3. Note that domains will be printed in the same order as they are in domains_decl file

## Namespace

The entire memlib is in the header `nq`

## How do I Print ?

```
nq::log::print(ostream file, const char* message = default) (log in the file :  file)
nq::log::print_file(const char* filename, const char* message = default) (open a file named filename and log on it)

eg: nq::log::print_file("Myfile.txt", "my message message");

nq::log::dump(const char* filename, const char* message = default) (log ONLY if something is allocated, to use at the end of the program to recover leaks).
```

## Containers

`nq::container<Type, Domain = UnknownDomain, AllocStrat = DefaultAlloc, Other_Args...>`

Where Domain is the reason why you allocate (for logs) and AllocStrat is the way you'll allocate your memory (malloc by default)

## Memory Handlers

### NEW

**STANDARD OPERATOR** `new` **IS NOT OVERRIDED**

**POINTERS ALLOCATED WITH** `new` **SHOULD BE DELETED WITH** `delete`

`NQ_NEW(Domain) type()`

macro that calls an overload of new specified to be logged in Domain

`NQ_DELETE(ptr)`

macro to call the delete of a `NQ_NEW`'d pointer


`NQ_NEW_ARRAY(Domain, type, size)`

macro that tries at it's best to imitate the new[] behaviour.

`NQ_DELETE_ARRAY(ptr)`

macro to call the delete of a `NQ_NEW_ARRAY`'ed pointer


new_array and delete_array are working as expected but have not the exact stl behaviour (prefere them nq::vector or std::array)

### unique_ptr

`nq::unique_ptr<T>(new T())`

`nq::unique_ptr<T, Domain>(NQ_NEW(Domain) T())`

`nq::unique_ptr<T, Domain> unique = nq::make_unique<T, Domain>(T_args...)`

or `auto unique = nq::make_unique<T, Domain>(T_args...)`


When not specifying a Domain as a second argument (or if the second argument is not a Domain) nq::unique_ptr behave exactly like std::unique_ptr (so needs a `new`'d pointer)


*Extra functions added with the lib:*


`unique.new_reset(args_of_newed_object)`

`unique.make_reset(args_of_newed_object)`


Can be used to avoid writing reset(NQ_NEW...), and so avoid to write new in the code.

### shared_ptr

`nq::shared_ptr<T>(NQ_NEW(T())`

`nq::shared_ptr<T>(new T(), nq::new_deleter<T>())`

but if you have a non logged shared with a pointer allocated with `new` prefer:

`std::shared_ptr<T>(new T())`


`nq::shared_ptr<T> shared = nq::make_shared<T, Domain, AllocStrat>(T_args)`

or `auto shared = nq::make_shared<T, Domain, AllocStrat>(T_args)`


*Extra functions added with the lib:*

`shared.new_reset(args_of_newed_object)` (standard reset but without writing NQ_NEW in code)

`shared.make_reset(args_of_newed_object)` (reset as if make_shared was used for the new object)


`auto = nq::new_shared<T, Domain, AllocStrat>(T_args)` (behave as the standard constructor (2 allocations), but avoid writing NQ_NEW)

#  Novaquark memory library Install

This is the logged customable memory library used at Novaquark

** memlib is a submodule, do not delete it's content **

** For the Windows users create a directory Memlib in Saved (Saved/Memlib), the dump write on it **

**In file `memlib/domains/log_path.h` set the static string path to where you'll will print your logged files (For windows `PATH_TO_SQUARION/Saved/Memlib`)**
**REMOVE THE .sample AT THE END OF LOG_PATH.H**


## Requirements 

cmake >= 2.6

### Unix specific
GCC >= 4.8.2.

###Windows specific

Visual Studio 12 2013 x64

## Install

###Under Unix

run `sudo ./install.sh`
The lib and include will be installed in `/usr/local/lib` and `/usr/local/include`
(They are also present in the current directory)

###Under Windows
run `.install.bat`
The lib and include directories are installed in the current directory

## Include to project

### Options

#### Code options

  * WITH_NQ_MEMLOG (to activate logging)
  * WITH_NQ_LOGTIME (to add time of log (only works with memlog on))
  * WITH_NQ_MEMOFF (desactivate the entire library)

#### Cmake options (correspond to code ones):

* Lib Specific options *
  * COMPILE_WITH_LOG  *suffixe* : **_l**
  * LOG_WITH_TIME  *suffixe* : **_lt**
  * COMPILE_WITH_MEM_OFF  *suffixe* : **_off**


* Cmake modes (suffix added after lib optins ones) *
  * CMAKE_BUILD_TYPE=RELEASE *no suffixe*
  * CMAKE_BUILD_TYPE=DEBUG *suffixe* : **_d**
  * CMAKE_BUILD_TYPE=RELWITHDEBINFO *suffixe* : **_rd**
  * CMAKE_BUILD_TYPE=MINSIZEREL *suffixe* : **_rm**

**eg**: `-DCMAKE_BUILD_TYPE=DEBUG -DCOMPILE_WITH_LOG -DLOG_WITH_TIME  =>  nq_memlib_lt_d`

	`-DCMAKE_BUILD_TYPE=RELEASE -DCOMPILE_WITH_MEM_OFF  => nq_memlib_off`


### Add to project
#### For Unix
* include the `/usr/local/include` and `PATH_TO_PROJECT/memlib/domains` directories to the project
* link the static library`
	* `nq_memlib(_options_mode).a`

####For Windows
* include the `PATH_TO_PROJECT/memlib/include` and `PATH_TO_PROJECT/memlib/domains` directories to the project
* link the static library
	* `nq_memlib(_options_mode).lib`


## IntelliSense does not recognize the headers?

1. Go to the `Solution Explorer` on the right of VS
2. Right click on the project name (`Squarion` in our case)
3. Click on `Properties` (Alt + Enter)
4. Go to `Configuration Properties` tab and `NMake`
5. In the `IntelliSense` part on `Include Search Path` click on `Edit`
6. On the right corner of the window there's a little file icon New Line (Ctrl + insert)
7. Add the path to include and domains directories (should be ..\..\ThirdParty\memlib\include ...)
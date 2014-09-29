#ifndef ENV_MACCRO_H_
# define ENV_MACCRO_H_

# if _WIN32 || _WIN64
#  define NQ_WIN_
# endif // _WIN32 || _WIN64

# if __GNUC__
#  define NQ_GNU_
# endif // __GNUC__

/* Check windows */
# if _WIN32 || _WIN64
#  if _WIN64
#   define NQ_ENV_64
#  else // _WIN64
#   define NQ_ENV_32
#  endif // _WIN64
# endif // !_WIN32 || _WIN64

/* Check GCC */
# if __GNUC__
#  if __x86_64__ || __ppc64__
#   define NQ_ENV_64
#  else // __x86_64__ || __ppc64__
#   define NQ_ENV_32
#  endif // !__x86_64__ || __ppc64__
# endif // !__GNUC__

# if _WIN32 || _WIN64 || __GNUC__
/* Compiler recognized */
# else
#  error "Compiler not recognized (Try to go with visual x32 or x64 or GCC)"
# endif // !_WIN32 || _WIN64 || __GNUC__



/* Check specific compiler syntax */

/* Exception handling */
# ifdef NQ_GNU_
# endif // !NQ_GNU_

# ifdef NQ_WIN_
#  define noexcept
# endif // !NQ_WIN_

#endif // !ENV_MACCRO_H_

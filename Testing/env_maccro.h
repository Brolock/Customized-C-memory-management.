#ifndef ENV_MACCRO_H_
# define ENV_MACCRO_H_

/* NQ_WIN_ to check if we are on a windows compiler */
# if _WIN32 || _WIN64
#  define NQ_WIN_
# endif // _WIN32 || _WIN64

/* NQ_GNU_ to check if we are on a gnu compiler */
# if __GNUC__
#  define NQ_GNU_
# endif // __GNUC__

/* NQ_ENV_64 if compiling on 64 bits, NQ_ENV_32 if compiling on a 32 */
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

/* Under Windows the keyword noexcept doesn't exist so we replace it with an
 * empty macro */
# ifdef NQ_WIN_
#  define noexcept
# endif // !NQ_WIN_

#endif // !ENV_MACCRO_H_

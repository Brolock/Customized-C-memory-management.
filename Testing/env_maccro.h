#ifndef ENV_MACCRO_H_
# define ENV_MACCRO_H_

/* Check windows */
# if _WIN32 || _WIN64
#  if _WIN64
#   define ENVIRONMENT64
#  else // _WIN64
#   define ENVIRONMENT32
#  endif // _WIN64
# endif // !_WIN32 || _WIN64

/* Check GCC */
# if __GNUC__
#  if __x86_64__ || __ppc64__
#   define ENVIRONMENT64
#  else // __x86_64__ || __ppc64__
#   define ENVIRONMENT32
#  endif // !__x86_64__ || __ppc64__
# endif // !__GNUC__

# if _WIN32 || _WIN64 || __GNUC__
/* Compiler recognized */
# else
#  error "Compiler not recognized (Try to go with visual x32 or x64 or GCC)"
# endif // !_WIN32 || _WIN64 || __GNUC__

#endif // !ENV_MACCRO_H_

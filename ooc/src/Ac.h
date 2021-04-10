#ifndef AC_H
#define AC_H

#define AC_HVAR(C) __attribute__((cleanup(C##_dealloc))) C
#define AC_ALLOC(C, V)    \
    AC_HVAR(C) *V = NULL; \
    C##_alloc(&V);

#endif /* ifndef AC_H */
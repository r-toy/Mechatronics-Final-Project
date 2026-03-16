#ifndef MYMACROS_H
#define MYMACROS_H

#define ABS(A) ((A < 0) ? -A : A)
#define SIGN(A) ((A < 0) ? 0 : 1)
#define SIGNNUM(A,B) ((B < 0) ? -A : ((B > 0) ? A : 0))
#define GETSIGN(A) (A ? 1 : -1)
#define LOWERBOUND(A,B) ((ABS(A) < B) ? 0 : A)
#define UPPERBOUND(A,B) ((ABS(A) > B) ? (SIGNNUM(B,A)) : A)

inline long pos_mod(long i, long n) {
    return (i % n + n) % n;
}

#endif
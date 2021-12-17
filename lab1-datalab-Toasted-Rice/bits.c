/*
 * CS:APP Data Lab
 *
 * James Vogenthaler (javo9205)
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:

  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.


  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function.
     The max operator count is checked by dlc. Note that '=' is not
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 *
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* We do not support C11 <threads.h>.  */
/*
 * bitOr - x|y using only ~ and &
 *   Example: bitOr(6, 5) = 7
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitOr(int x, int y) {   //DeMorgan's law
    return ~(~x & ~y);      //~~(x | y) => ~(~x & ~y)
}
/*
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int evenBits(void) {  
    int even = 0x55 << 8 | 0x55;  // H2B(0x55) = 0101 0101
    even = even << 16 | even;
    return even;
}
/*
 * minusOne - return a value of -1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
    return ~0;
}
/*
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
    int even = 0x55 << 8 | 0x55;
    even = even << 16 | even;
    return !((x & even) ^ even);
}
/*
 * anyOddBit - return 1 if any odd-numbered bit in word set to 1
 *   Examples anyOddBit(0x5) = 0, anyOddBit(0x7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyOddBit(int x) {  
    int odd = 0xAA << 8 | 0xAA;  // H2B(0xAA) = 1010 1010
    odd = odd << 16 | odd;
    return !!(x & odd);
}
/*
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
    int nShift = n << 3;                              // The length of a byte is 8 bits.
    int mShift = m << 3;                              // Shifting by 3 is equivalent to multiplying by 2^3 = 8.
    int nMask = 0xFF << nShift;                       // Move masks to correct byte.
    int mMask = 0xFF << mShift;                       //     
    int byte1 = (x >> mShift << nShift) & nMask;      // Shift right by "x" bytes and left by "y" bytes.
    int byte2 = (x >> nShift << mShift) & mMask;      // This will position bytes correctly to be extracted.
    return (x & ~(nMask | mMask)) | (byte1 | byte2);
}
/*
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1,
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 38
 */
int addOK(int x, int y) {
    int sum = x+y;
    int check1 = x ^ sum;                    // check1 tests if x and the sum have DIFFERENT signs
    int check2 = y ^ sum;                    // check2 tests if y and the sum have DIFFERENT signs
    int overflow = (check1 & check2) >> 31;  // If both are true, overflow has occured.
    return 0x1 & ~overflow;                  // Result is stored in the sign bit.             
}
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
    int mask = !x << 31 >> 31;         // If (x = 0) mask = 0xFFFFFFFF; else mask = 0x0;
    return (~mask & y) | (mask & z);   // Invert mask for true statement
}
/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {              // H2B(ASCII Digit Range)   =   H2B(0x30 to 0x3F)   =   0011 0000 to 0011 1001
    int check1 = !((x+0x06) & ~0x3F);  // check1 tests if x is <= 0x39  (HINT: 0x39 + 0x06 = 0x3F; H2B(0x3F) = 0011 1111)
    int check2 =  ((x+0x10) &  0x40);  // check2 tests if x is >= 0x30  (HINT: 0x30 + 0x10 = 0x40; H2B(0x40) = 0100 0000)
    return check1 & (check2 >> 6);     
}
/*
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
    int shift = n << 3;
    int mask = 0xFF << shift;
    int byte = c << shift;
    return (x & ~mask) | byte;
}
/*
 * rotateRight - Rotate x to the right by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateRight(0x87654321,4) = 0x18765432
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 25
 *   Rating: 3
 */
int rotateRight(int x, int n) {
    int compliment = (0x1F ^ n) + 1;   // Get complimentary pair of n (i.e. 32-n). This is a similar operation to ~n+1 but for only 6 bits
    int snip = x << compliment;        // Rotate the "n" LSBs by shifting them all the way to the left.
    int mask = ~(~0 << compliment);    // Prepare mask to get the (32-n) remaining bits
    return ((x >> n) & mask) | snip;   // Shift the remaining (32-n) from MSB to LSB and clip them with a mask. Return rotation.
}
/*
 * satAdd - adds two numbers but when positive overflow occurs, returns
 *          maximum possible value (Tmax), and when negative overflow occurs,
 *          it returns minimum negative value (Tmin)
 *   Examples: satAdd(0x40000000,0x40000000) = 0x7fffffff
 *             satAdd(0x80000000,0xffffffff) = 0x80000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 4
 */
int satAdd(int x, int y) {
    int sum = x+y;
    int check1 = x ^ sum;                    // check1 tests if x and the sum have DIFFERENT signs
    int check2 = y ^ sum;                    // check2 tests if y and the sum have DIFFERENT signs
    int overflow = (check1 & check2) >> 31;  // If both are true, overflow has occured.
    
    int signMask = x >> 31;
    int Tmin = 0x1 << 31;
    int Tminmax = Tmin ^ ~signMask;
    
    return (~overflow & sum) | (overflow & Tminmax);
}
/*
 * Extra credit
 */
/*
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
    unsigned  absMask = 0x7FFFFFFF;
    unsigned  expMask = 0x7F800000;
    unsigned  negativeInfinity = 0xFF800000;
    
    if (uf == negativeInfinity) return expMask;
    if ((uf & expMask) == expMask) return uf;
    return uf & absMask;
}
/*
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int float_f2i(unsigned uf) {
    unsigned sign = uf & 0x80000000;
    unsigned exp = ((uf & 0x3F800000) >> 23) + 1;       // Extract exp except MSB bit.  (HINT: exp = E - 128 + 1)
    unsigned num = (uf & 0x007FFFFF) | 0x00800000;      // Actual number is fractional component plus extra 1 bit
    unsigned shift = exp + 0x9;                         // Will only use 5 LSBs. (6th bit will be truncated)
                                                        // Shift amount   =   exp + 9 - 32   =   exp - 23
    
    if (uf == 0xCF000000) return 1 << 31;               // Special case for Tmin = -2^(158-127) = -2^31
    if (uf &  0x40000000)                               // Only consider cases where exp > zero (at least 128 before bias)
    {
        if ((exp + 1) & 0xE0) return 0x80000000u;       // If exp + 1 >= 32, return 0x80000000u
        else if (shift & 0xE0)                          // If exp + 9 >= 32, shift to the left, else shift to the right
             num = num << (shift & 0x1F);               // shift amount   =   exp + 9 - 32     =   exp - 23
        else num = num >> (24 + ~exp);                  // shift amount   =   23 + (1 - exp)   =   24 + ~exp
    }
    else if ((uf & 0x3F800000) == 0x3F800000) num = 1;  // Return 1 for exponent = 127 (0). Return zero for all others
    else return 0u;                                     // Return zero for any exp <= zero (exponent's MSB = 0)
    
    if (sign) num = ~num + 1;
    return num;
}
/*
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */

unsigned float_half(unsigned uf) {
    unsigned signMask = 0x80000000;  // First bit
    unsigned fracMask = 0x007FFFFF;  // Last 23 bits
    unsigned expMask  = 0x7F800000;  // 2-9th bits
    
    unsigned sign = uf & signMask;
    unsigned frac = uf & fracMask;
    unsigned exp =  uf & expMask;
    unsigned mantissa = exp | frac;           // Only applicable if exp is 0 or 1.
    unsigned truncated = mantissa & 0x1;      // If mantissa is shifted, LSB is truncated.
    
    if (exp ^ expMask)                        // If exp isn't all ones, then exp ^ expMask != 0
    {
        if (exp & 0x7F000000)                 // Check if exp > 1
            exp = (exp + expMask) & expMask;  // If so, subtact one from exponent (exp + 255 - 256 = exp - 1)
        else if (mantissa)                    // Otherwise we will divide by two by shifting the mantissa
        {                                     // (Here we are treating the first bit of the exp as part of the mantissa)
            exp = 0;                          // No exponent if mantissa is used
            frac = mantissa >> 1;             // Shift mantissa to divide by 2
            frac = frac + (frac & truncated); // If a 1 was truncated and the number is odd, round up.
        }
        else return sign;                     // If there is no exponent and mantissa, just return the sign bit
    }
    return sign | exp | frac;                 // Otherwise return full float
    
}

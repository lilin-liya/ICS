/* 
 * CS:APP Data Lab 
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

#include "btest.h"
#include <limits.h>

/*
 * Instructions to Students:
 *
 * STEP 1: Fill in the following struct with your identifying info.
 */
team_struct team =
{
   /* Team name: Replace with either:
      Your login ID if working as a one person team
      or, ID1+ID2 where ID1 is the login ID of the first team member
      and ID2 is the login ID of the second team member */
    "5130379091", 
   /* Student name 1: Replace with the full name of first team member */
   "LiLin",
   /* Login ID 1: Replace with the login ID of first team member */
   "5130379091",

   /* The following should only be changed if there are two team members */
   /* Student name 2: Full name of the second team member */
   "",
   /* Login ID 2: Login ID of the second team member */
   ""
};

#if 0
/*
 * STEP 2: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

CODING RULES:
 
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


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.
#endif

/*
 * STEP 3: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the btest test harness to check that your solutions produce 
 *      the correct answers. Watch out for corner cases around Tmin and Tmax.
 */
/* 
 * abs - absolute value of x (except returns TMin for TMin)
 *   Example: abs(-1) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
/*1.*/int abs(int x) {

  	int temp=x>>31;//temp为x右移31位得到的x的符号位
  	int result=0;
   	x=x^temp; //若x为正,temp为0,x^0不变;若x为负,temp全为1,x^temp相当于x取反
  	result=x+(~temp)+1;/*x+(~temp)+1相当于x-temp,x为正，temp为0,结果不变;x为负，temp为-1*/
  	return result;
}
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
/*2.*/int bitAnd(int x, int y) {
  	int result=~(~x|~y); //x,y都取反，再进行或运算，最后结果再取反
  	return result;
}
/* 
 * bitMask - Generate a mask consisting of all 1's 
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
/*3.*/int bitMask(int highbit, int lowbit) {
  	int high=((~0)<<highbit)<<1; //将~0的前highbit+1位置为0，其余为1 
	int low=((~0)<<lowbit);  //将~0的前lowbit位置为0，其余为1
	int temp_result=(high^low);//异或运算生成结果(当highbit>lowbit时成立)
	int result = temp_result&low;//当highbit<lowbit时，需要再与low&，生成全0    
	return result;  
}
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 2
 */
/*4.*/int bitXor(int x, int y) {
  	int result=~(x&y)&~(~x&~y);//x^y相当于x|y-x&y
  	return result;
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
/*5.*/int conditional(int x, int y, int z) {
/*若x为0,应返回z的值,mask为1,(mask<<31)>>31取得mask的符号位，并将mask变为全1,此时~mask为全0,~mask&y为0,mask&z为z,最终返回z的值;
若x不为0,应返回y的值,mask为0,(mask<<31)>>31取得mask的符号位，并将mask变为全0,此时~mask为全1,~mask&y为y,mask&z为0,最终返回y的值*/
  	int mask=!x;
  	mask=(mask<<31)>>31;  
  	return (mask&z)|(~mask&y);  
}
/* 
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 2
 */
/*6.*/int evenBits(void) {
  	int result=(0x55<<24)+(0x55<<16)+(0x55<<8)+0x55;//0x55=0101 0101,通过移位可得最终结果
  	return result;
}
/* 
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
/*7.*/int isEqual(int x, int y) {
  	int result=!(x^y);/*当x=y时,x^y=0,result=1;
				当x!=y时,x^y!=0,result=0;*/
  	return result;
}
/* 
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
/*8.*/int isLess(int x, int y) {
	int x_s=(x>>31)&0x1;//只获得x的符号位，其余都置为0  
	int y_s=(y>>31)&0x1;//只获得y的符号位，其余都置为0
/*x<0<y时，(x_s&~y_s)为1,结果必为1；
x>0>y时，(x_s&~y_s)为0,(!(~x_s&y_s))为0,结果必为0；
x,y同号或x,y相等时，(!!((x+~y+1)>>31))对x,y作减法，并得到其符号位，判断是否为0或1；
*/
	return (x_s&(~y_s))|((!!((x+(~y)+1)>>31))&(!((~x_s)&y_s)));
}
/* 
 * isNegative - return 1 if x < 0, return 0 otherwise 
 *   Example: isNegative(-1) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3
 */
/*9.*/int isNegative(int x) {
	int result=!!(x>>31);//x>>31得到x的符号位,只有当x>=0时，其为0；!!(x>>31)返回1或0
  	return result;
}
/* 
 * isNonZero - Check whether x is nonzero using
 *              the legal operators except !
 *   Examples: isNonZero(3) = 1, isNonZero(0) = 0
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4 
 */
/*10.*/int isNonZero(int x) {
//若x=0,x|(~x+1)=x|(-x)=0,>>31后也为0,&1后为0
//若x!=0,x|(~x+1)=x|(-x),>>31后为全1,&1后必为1
	int result = ((x|(~x+1))>>31)&1;  
  	return  result;
}
/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 60
 *   Rating: 4
 */
/*11.*/int isPower2(int x) {
	int temp=!((x>>31)&0x1);//temp获得x的符号位，或者为1或者为0
/*一般情况下,x为正,!!x=1,temp=1,!(x&(x+(~1)+1))相当于x&(x-1)判断一般情况下某个数是否为2的幂,若是,返回1,否则返回0,&之后返回结果;
若x为负,此时temp为0,&之后的结果必为0;
若x=0，此时!!x=0,&之后的结果必为0.*/
    	return (!!x)&(temp)&!(x&(x+(~1)+1));
}
/* 
 * leastBitPos - return a mask that marks the position of the
 *               least significant 1 bit. If x == 0, return 0
 *   Example: leastBitPos(96) = 0x20
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 4 
 */
/*12.*/int leastBitPos(int x) {
 	int result=(~x+1)&x;//-x&x取为1的最低位
  	return result;
}
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
/*13.*/int logicalNeg(int x) {
/*若x=0,(~x)&(~(~x+1)))生成全1,>>31得到全1，&0x1后生成1;
若x!=0,(~x)&(~(~x+1)))生成的数的符号位必为0,>>31得到全0,&0x1后生成0*/
  	int result=(((~x)&(~(~x+1)))>>31)&0x1; 
  	return result;
}
/* 
 * reverseBytes - reverse the bytes of x
 *   Example: reverseBytes(0x01020304) = 0x04030201
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 25
 *   Rating: 3
 */
/*14.*/int reverseBytes(int x) {
/*记x=0x12345678;
(x>>24)获得x1,2位的两个byte,&0xff使除这两个byte外的六个都变为0,生成0x00000012的形式;
(x>>16)&0xff)<<8获得x3,4位的两个byte,&0xff使除这两个byte外的六个都变为0,生成0x00003400的形式;
((x>>8)&0xff)<<16获得x5,6位的两个byte,&0xff使除这两个byte外的六个都变为0,生成0x00560000的形式;
(x&0xff)<<24获得x7,8位的两个byte,&0xff使除这两个byte外的六个都变为0,生成0x78000000的形式;
四者进行或运算生成最终结果*/
  	int result=((x>>24)&0xff) | (((x>>16)&0xff)<<8) |   
         	(((x>>8)&0xff)<<16) | ((x&0xff)<<24);  
  	return result;
}			
/* 
 * sum3 - x+y+z using only a single '+'
 *   Example: sum3(3, 4, 5) = 12
 *   Legal ops: ! ~ & ^ | << >>
 *   Max ops: 16
 *   Rating: 3
 */
/* A helper routine to perform the addition.  Don't change this code */
/*15.*/static int sum(int x, int y) {
  	return x+y;
}
int sum3(int x, int y, int z) {
	int word1=0;
	int word2=0;
  /**************************************************************
     Fill in code below that computes values for word1 and word2
     without using any '+' operations 
  ***************************************************************/
//三数相加，可先根据0^0=0,1^0=1,0^1=1,1^1=0的原则直接相加,在考虑进位产生的数，二者相加可得最终结果
	word1=x^y^z;//x^y^z直接相加，根据0^0=0,1^0=1,0^1=1,1^1=0的原则,不考虑进位  
  	word2=((x&y)|(y&z)|(x&z))<<1;/*word2则是进位生成的数，两者相加得到最终结果*/
/*根据规律推断可得,x,y,z三个数字的某一位至少有两个为1时,在生成的进位的数中的相应位置的左边一位就为1,
用式子来描述就是((a&b)|(b&c)|(a&c))<<1,其中(a&b)|(b&c)|(a&c)判断在某一位是否至少有两个为1,<<1即是将结果左移一位得到进位的数.*/  
  /**************************************************************
     Don't change anything below here
  ***************************************************************/
  	return sum(word1,word2);
}
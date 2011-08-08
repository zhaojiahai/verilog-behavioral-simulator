// Verilog Behavioral Simulator
// Copyright (C) 1995,2001,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// testbv.cc

#include <string>
#include <iostream>
#include "common/bvector.h"

using namespace std;

bit_vector Zero(LO);
bit_vector One(HI);

void
test_sqrebracket(bit_vector &, bit_vector &, bit_vector &c,
		bit_vector &, bit_vector &)
	{
	c(0) = LO;
	c(1) = HI;
	c(2) = Z;
	c(3) = DC;
	c(4) = LO;
	cout << "c=" << c << " (==0xz10)"
		<< endl;
	}

void
test_equality(bit_vector &, bit_vector &, bit_vector &c,
		bit_vector &d, bit_vector &)
	{
	bit_vector test1(string("1101"));
	if (test1 == d)
		cout << "test1 == d (ok!)" << endl;
	else
		cout << "test1 == d (not!)" << endl;
	if (test1 != d)
		cout << "test1 != d (not!)" << endl;
	else
		cout << "test1 != d (ok!)" << endl;
	if (test1 == c)
		cout << "test1 == c (not!)" << endl;
	else
		cout << "test1 == c (ok!)" << endl;
	if (test1 != c)
		cout << "test1 != c (ok!)" << endl;
	else
		cout << "test1 != c (not!)" << endl;
	}

void
test_notop(bit_vector &, bit_vector &b, bit_vector &,
		bit_vector &, bit_vector &)
	{
	if (!b)
		cout << "!b (not!)" << endl;
	else
		cout << "!b (ok!)" << endl;
	if (!(!b))
		cout << "!(!b) (ok!)" << endl;
	else
		cout << "!(!b) (not!)" << endl;
	}

void
test_equal(bit_vector &, bit_vector &, bit_vector &c,
		bit_vector &d, bit_vector &)
	{
	bit_vector test1(0,2), test2(2,1);
	test1 = d;
	test2 = c;
	cout << "test1(0,2=d)=" << test1 << " (=101)"
		<< " test2(2,1=c)=" << test2 << " (=10)"
		<< endl;
	}

void
test_plus(bit_vector &, bit_vector &, bit_vector &,
		bit_vector &, bit_vector &e)
	{
	bit_vector test1(3,0), test2(4,0);

	//test1 = e + One;
	binary_add(test1, e, One);
	//test2 = e + e;
	binary_add(test2, e, e);
	cout << "test1(e+1)=" << test1 << " (=1110)"
		<< " test2(e+e)=" << test2 << " (=11010)"
		<< endl;
	}

void
test_minus(bit_vector &, bit_vector &, bit_vector &,
		bit_vector &, bit_vector &e)
	{
	bit_vector test1(3,0), test2(4,0);

	//test1 = e - One;
	binary_sub(test1, e, One);
	//test2 = e - e;
	binary_sub(test2, e, e);
	cout << "test1(e-1)=" << test1 << " (=1100)"
		<< " test2(e-e)=" << test2 << " (=00000)"
		<< endl;
	}

void
test_shift(bit_vector &, bit_vector &, bit_vector &,
		bit_vector &, bit_vector &e)
	{
	bit_vector four(4UL), two(2UL);
	bit_vector test(7,0), test1(7,0), test2(3,0);

	test = e;
	//test1 = test << four;
	binary_lshf(test1, test, four);
	//test2 = e << two;
	binary_lshf(test2, e, two);
	cout << "test1(e<<4)=" << test1 << " (=11010000)"
		<< " test2(e<<2)=" << test2 << " (=0100)"
		<< endl;

	test = test1;
	//test1 = test >> four;
	binary_rshf(test1, test, four);
	//test2 = e >> two;
	binary_rshf(test2, e, two);
	cout << "test1(e>>4)=" << test1 << " (=00001101)"
		<< " test2(e>>2)=" << test2 << " (=0011)"
		<< endl;
	}

void
test_logic(bit_vector &, bit_vector &, bit_vector &,
		bit_vector &, bit_vector &e)
	{
	bit_vector test1(3,0), test2(string("10110"));

	//test1 = test2 | e;
	logic_or(test1, test2, e);
	cout << "test1(test2|e)=" << test1 << " (=1111)" << endl;
	//test1 = test2 & e;
	logic_and(test1, test2, e);
	cout << "test1(test2&e)=" << test1 << " (=0100)" << endl;
	//test1 = test2 ^ e;
	logic_xor(test1, test2, e);
	cout << "test1(test2^e)=" << test1 << " (=1011)" << endl;
	}

void
test_range(bit_vector &, bit_vector &, bit_vector &,
		bit_vector &d, bit_vector &e)
	{
	bit_vector test1(7,0);

	test1(6,5) = d(1,0);
	test1(4,1) = e;
	cout << "test1(6,5/4,1=d1,0/e)=" << test1 << " (=x101101x)"
		<< endl;
	}

void
test_conv(bit_vector &, bit_vector &, bit_vector &,
		bit_vector &, bit_vector &)
	{
	bit_vector test1(string("4567"), bit_vector::BASE8),
		test2(string("6789"), bit_vector::BASE10),
		test3(string("9abc"), bit_vector::BASE16);

	cout << "test1(8,4567)=" << test1 << endl
		<< "test2(d,6789)=" << test2 << endl
		<< "test3(h,9abc)=" << test3 << endl;
	}

void
test_comp(bit_vector &, bit_vector &, bit_vector &,
		bit_vector &, bit_vector &)
	{
	bit_vector test1(15UL), test2(25UL), test3(15UL), test4("e7", bit_vector::BASE16, 8, true);

	if (test1 < test2)
		cout << test1 << " < " << test2 << " ok!" << endl;
	else
		cout << test1 << " < " << test2 << " not!" << endl;
	if (test1 <= test3)
		cout << test1 << " <= " << test3 << " ok!" << endl;
	else
		cout << test1 << " <= " << test3 << " not!" << endl;
	if (test1 < test4)
		cout << test1 << " < " << test4 << " not!" << endl;
	else
		cout << test1 << " < " << test4 << " ok!" << endl;
	if (test4 < test2)
		cout << test4 << " < " << test2 << " ok!" << endl;
	else
		cout << test4 << " < " << test2 << " not!" << endl;
	}

void
test_div(bit_vector &, bit_vector &, bit_vector &,
		bit_vector &, bit_vector &)
	{
	bit_vector test(1586UL), test1(1586UL), test2(25UL), test3(63UL);
	//test = test1 / test2;
	binary_div(test, test1, test2);
	cout << "test1/test2=" << test;
	if (test == test3)
		cout << " ok!" << endl;
	else
		cout << " not!" << endl;
	}

void
test_mod(bit_vector &, bit_vector &, bit_vector &,
		bit_vector &, bit_vector &)
	{
	bit_vector test(1586UL), test1(1586UL), test2(25UL), test3(11UL);
	//test = test1 % test2;
	binary_mod(test, test1, test2);
	cout << "test1%test2=" << test;
	if (test == test3)
		cout << " ok!" << endl;
	else
		cout << " not!" << endl;
	}

void
printout(bit_vector &a, bit_vector &b, bit_vector &c,
		bit_vector &d, bit_vector &e)
	{
	cout << "a=" << a
		<< " b=" << b
		<< " c=" << c
		<< " d=" << d
		<< " e=" << e
		<< endl;
	}

int
main()
	{
	bit_vector a;
	bit_vector b(HI);
	bit_vector c(4,0);
	bit_vector d(string("1101"));
	bit_vector e(d);
	bit_vector f(115UL);

	printout(a,b,c,d,e);
	test_sqrebracket(a,b,c,d,e);
	printout(a,b,c,d,e);
	test_equality(a,b,c,d,e);
	printout(a,b,c,d,e);
	test_notop(a,b,c,d,e);
	printout(a,b,c,d,e);
	test_equal(a,b,c,d,e);
	printout(a,b,c,d,e);
	d = b;
	test_plus(a,b,c,d,e);
	printout(a,b,c,d,e);
	d = c;
	test_minus(a,b,c,d,e);
	printout(a,b,c,d,e);
	test_shift(a,b,c,d,e);
	printout(a,b,c,d,e);
	test_logic(a,b,c,d,e);
	printout(a,b,c,d,e);
	test_range(a,b,c,d,e);
	printout(a,b,c,d,e);
	test_conv(a,b,c,d,e);
	test_comp(a,b,c,d,e);
	test_div(a,b,c,d,e);
	test_mod(a,b,c,d,e);

	bit_vector res(d);
	//res = ~d;
	unary_inv(res, d);
	cout << "d inverted = " << res << " (=xx01)" << endl;
	//res = e + d;
	binary_add(e, res, d);
	cout << "e(res+d)=" << e << " (=xxxx)" << endl;
	printout(a,b,c,d,e);

	bit_vector tmp(string("101101101011"));
	string tmp1 = tmp.to_string(bit_vector::BASE8);
	string tmp2 = tmp.to_string(bit_vector::BASE10);
	string tmp3 = tmp.to_string(bit_vector::BASE16);
	cout << "101101101011: " << tmp << " "
		<< tmp1 << " "
		<< tmp2 << " "
		<< tmp3 << " "
		<< endl;

	cout << "f(115) = " << f << " (=1110011)"
		<< endl;

	return 0;
	}

/*
 * Test system tasks and functions.
 *  dependencies:
 *	initial procedural block
 *	always procedural block
 *	sequential block
 *	system tasks ($dump*)
 */

module test(a, b, c);
	reg a, b, c;
	inout a, b, c;

	always @(posedge a)
		b = ~b;

	always @(posedge b)
		c = ~c;

	always @(posedge c)
		a = ~a;
endmodule

module main;
	wire a, b, c;

	test t1(a, b, c);

	initial
		begin
		$dumpfile("myfile.vcd");
		a = 0;
		b = 0;
		c = 0;
		$dumpvars(0, main);
		$monitor("%d %d %d", a, b, c);
		$dumpon;
		#1 a = 1;
		#1 a = 0;
		#1 a = 1;
		$dumpoff;
		#1 a = 0;
		#1 a = 1;
		#1 a = 0;
		#1 a = 1;
		#1 a = 0;
		#1 a = 1;
		#1 a = 0;
		#1 a = 1;
		$dumpon;
		#1 a = 0;
		#1 a = 1;
		$dumpall;
		end
endmodule

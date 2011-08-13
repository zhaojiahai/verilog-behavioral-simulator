/*
 * Test named arguments to module instantiation.
 */

module a_mod (a, b, c);

	input a;
	output b;
	inout c;
	reg b, c;

	always @(a or c)
		begin
		b = a ^ c;
		end

endmodule

module test;

	reg x, y;
	wire z;

	a_mod a( .c(x), .a(y), .b(z) );

	initial
		begin
		$monitor("%0b ^ %0b = %0b", x, y, z);
		x = 0;
		y = 0;
		#1 x = 1;
		#1 y = 1;
		#1 x = 0;
		#1 $finish;
		end

endmodule

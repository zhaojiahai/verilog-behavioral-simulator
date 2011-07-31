/*
 * Test named arguments to module instantiation.
 * This is not currently supported, but vbs should not crash.
 */

module a_mod (a, b, c);

	input a;
	output b;
	inout c;
	reg b, c;

	always @(a or c)
		begin
		b = a + c;
		end

endmodule

module test;

	a_mod a( .c(x), .a(y), .b(z) );

	initial
		begin
		#10 $finish;
		end

endmodule

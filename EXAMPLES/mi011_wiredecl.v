/*
 * Test wire declaration.
 *  dependencies:
 *	write system task
 *	initial procedural block
 *	system task enable
 */

module main;

	wire this_is_a_variable, this_is_another_variable;
	wire [1:4] this_is_a_range_variable;
	wire 	a,
		b,
		c;
	wire [7:0] d = 'b1001,
		   e = 6,
		   f = 8'hd;

	initial
		begin
		#1 ;
		$write("d = %b (00001001)\n", d);
		$write("e = %d (6)\n", e);
		$write("f = %h (0d)\n", f);
		end

endmodule

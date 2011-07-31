/*
 * Test system tasks and functions.
 *  dependencies:
 *	initial procedural block
 *	system tasks
 */

module main;

	reg a;

	initial
		begin
		a = 1;
		$display("Hello world!");
		$display("a = %d (1), %d (2)", a, 2);
		end

endmodule

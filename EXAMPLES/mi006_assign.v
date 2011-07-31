/*
 * Test continuous assignments.
 *  dependencies:
 *	initial procedural block
 *	sequential block
 *	system task/function
 *	add/ternary expression
 *	delay
 */

module main;

	reg [7:0] a, b, c;
	reg d;
	integer e, f, g;

	assign a = b + c;

	assign d = 1'b1;

	assign e = b < 2 ? f : g;

	initial
		begin
		$monitor("%d:  a = %d d = %b e = %d", $stime, a, d, e);
		b = 0;
		c = 0;
		f = 3;
		g = 4;
		#1 b = 1;
		#1 f = 5;
		#1 c = 1;
		#1 b = 2;
		#1 g = 6;
		#1 c = 2;
		#1 $finish;
		end

endmodule

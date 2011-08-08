/*
 * Test integer type comparison with negative constant.
 *   The test of 'pipe <= -1' should return false.  Thus,
 *   after 2000 clocks, $finish is called and nothing
 *   happens.  If the test fails, we have an infinite
 *   loop.
 */

module main ;

	reg [4:0] a;
	reg clk;
	integer pipe;

	initial
		begin
		a = 0;
		clk = 0;
		pipe = -24;
		$display("15 < pipe: %0b (0)", 15 < pipe);
		#2000 ;
		$display("done at %t (2000)", $stime);
		$finish;
		end

	always #20 clk = !clk;

	always @(posedge clk)
		for (pipe = 2; pipe <= -1; pipe = pipe + 1)
			a <= a + 1;

endmodule

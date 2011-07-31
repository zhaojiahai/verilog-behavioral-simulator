/*
 * Complex test.
 *  dependencies:
 *	register declaration
 *	initial procedural block
 *	module instantiation
 *	system tasks
 */

module divide_test ;
	reg [31:0] dvdend, dvsor;
	reg clk, reset;
	wire [31:0] quotient;
	wire [63:0] remainder;
	wire finished;

	divider div(clk, quotient, remainder, finished, dvdend, dvsor, reset);

	initial
		begin
		clk = 0;
		dvdend = 7;
		dvsor = 2;
		reset = 0;
		#1 reset = 1;
		#3 reset = 0;	// Must set reset for one clock cycle.
		end

	always #1 clk = ~clk;

	always @(posedge finished)
		begin
		$write("Finished at %d:  %d / %d = %d(%d,%d)\n",
			$stime, dvdend, dvsor, quotient,
			remainder[31:0], remainder[63:32]);
		$finish;
		end

endmodule

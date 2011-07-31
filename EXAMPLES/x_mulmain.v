/*
 * Complex test.
 *  dependencies:
 *	register declaration
 *	initial procedural block
 *	module instantiation
 *	system tasks
 */

module multiple_test ;
	reg [31:0] mplier, mcand;
	reg clk, reset;
	wire [63:0] product;
	wire finished;

	multiply m(clk, product, finished, mcand, mplier, reset);

	initial
		begin
		clk = 0;
		mcand = 14;
		mplier = 13;
		reset = 0;
		#1 reset = 1;
		#4 reset = 0;	// Must set reset for one clock cycle.
		end

	always #1 clk = !clk;

	always @(posedge finished)
		begin
		$write("Finished at %d:  %d * %d = %d\n",
			$stime, mcand, mplier, product);
		$finish;
		end
endmodule

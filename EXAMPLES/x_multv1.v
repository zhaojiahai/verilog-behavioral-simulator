/*
 * Multiply module
 *  dependencies:
 *	register declaration
 *	io declaration
 *	always procedural block
 *	system tasks
 *	ifelse statement
 */

module multiply (clk, prod, fin, mcand, mplier, reset);
	input clk, reset;
	output [63:0] prod;
	output fin;
	input [31:0] mcand, mplier;

	reg [63:0] prod, mcand_copy;
	reg [31:0] mplier_copy;
	reg [7:0] working;
	reg fin;

	always @(posedge clk)
		begin
		if (reset == 1)
			begin
			prod = 0;
			mcand_copy = 0;
			mplier_copy = 0;
			fin = 0;
			working = 1;	// Start work on next clk!
			end
		else
			begin
			if (working == 1)
				begin
				// Make copies of the data.
				mcand_copy = mcand;
				mplier_copy = mplier;
				working = working + 1;
				end
			else if (working > 1)
				begin
				// Basic grade school multiplying.
				if (mplier_copy[0:0] == 1)
					prod = prod + mcand_copy;
				mcand_copy = mcand_copy << 1;
				mplier_copy = mplier_copy >> 1;	// Next bit.

				// When are we finished?
				if (working > 32)
					fin = 1;
				working = working + 1;
				end
			end
		end

endmodule

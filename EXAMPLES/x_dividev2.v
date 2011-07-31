/*
 * Divide module
 *  dependencies:
 *	register declaration
 *	io declaration
 *	always procedural block
 *	system tasks
 *	ifelse statement
 */

module divider (clk, quot, rem, fin, dvdend, dvsor, reset);
	output [31:0] quot;
	output [63:0] rem;
	output fin;
	input [63:0] dvdend;
	input [31:0] dvsor;
	input clk, reset;

	reg [31:0] quot;
	reg [63:0] rem;
	reg fin;
	reg [5:0] rep;

	always @(posedge clk)
		begin
		if (reset == 1)
			begin
			fin = 0;
			rep = 0;
			end
		else
			begin
			if (rep == 0)
				begin
				// Make copies of the data.
				rem = dvdend;
				rep = rep + 1;
				end
			else if (rep >= 1)
				begin
				rem = rem << 1;
				rem[63:32] = rem[63:32] - dvsor;
				quot = quot << 1;
				if (rem[63:63] == 1)
					rem[63:32] = rem[63:32] + dvsor;
				else
					quot[0:0] = 1;
				// When are we finished?
				if (rep == 32)
					fin = 1;
				rep = rep + 1;
				end
			end
		end

endmodule

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
	reg [63:0] rem, dvsor_copy;
	reg fin;
	reg [5:0] rep;

	always @(posedge clk)
		begin
		if (reset == 1)
			begin
			quot = 0;
			dvsor_copy = 0;
			fin = 0;
			rep = 0;
			end
		else
			begin
			if (rep == 0)
				begin
				// Make copies of the data.
				rem = dvdend;
				dvsor_copy[63:32] = dvsor;
				rep = rep + 1;
				end
			else if (rep >= 1)
				begin
				rem = rem - dvsor_copy;
				quot = quot << 1;
				if (rem[63:63] == 1)
					rem = dvsor_copy + rem;
				else
					quot[0:0] = 1;
				dvsor_copy = dvsor_copy >> 1;
				// When are we finished?
				if (rep == 33)
					fin = 1;
				rep = rep + 1;
				end
			end
		end

endmodule

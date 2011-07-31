/* Missing qoute. */
module main;
	reg bit;
	reg [0:3] bitvector;

	initial
		begin
		$monitor("%d %d\n", bit, bitvector);
		bit = 0;
		bitvector = 15;
		#10 bit = 1b1;
		#15 bitvector = 4'o61;
		#100 bit = 1'b0;
		#5000 bitvector = 4'hc;
		end
endmodule

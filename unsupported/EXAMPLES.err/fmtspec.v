/* Testing $write format string. */
module main;
	reg bit;
	reg [0:3] bitvector;

	initial
		begin
		bit = 0;
		$write("%12 %d %d\n", $time, bit, bitvector);
		#10 bitvector = 15;
		$write("%d %d %d\n", $time, bit, bitvector);
		end
endmodule

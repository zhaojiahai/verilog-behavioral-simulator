/* Testing undefined symbol. */
module main;
	reg bit;
	reg [0:3] bitvector;

	initial
		begin
		bit = 0;
		$write("%d %d %d\n", $time, bit, bitvector);
		#10 bitvector = 15;
		$write("%d %d %d\n", $time, bit, xxx);
		end
endmodule

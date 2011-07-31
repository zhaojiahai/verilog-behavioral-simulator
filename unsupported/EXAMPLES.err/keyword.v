/* Testing keyword used as symbol. */
module main;
	reg abc;
	reg [0:3] name;

	initial
		begin
		$write("Should never get here.\n");
		end
endmodule

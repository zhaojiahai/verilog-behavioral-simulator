/* Test range selection of registers. */
module main;
	reg [7:0] a;

	initial
		begin
		a[3:4] = 8'b00000010;
		end

endmodule

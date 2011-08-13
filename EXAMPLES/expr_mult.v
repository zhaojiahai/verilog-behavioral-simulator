/*
 * Test multiply expression.
 *  dependencies:
 *	register declaration
 *	initial procedural block
 *	system tasks
 *	blocking assignment
 */

module main ;

	integer ip;
	reg [71:0] a, b;
	reg [143:0] rp;

	initial
		begin
		a = 4 * 5;
		b = 0 * a;
		$write("0x%0h 0x%0h (0x14 0x0)\n", a, b);
		ip = 3 * -6;
		a = -2 * ip;
		b = -3 * -5;
		$write("%0d %0d %0d (-18 36 15)\n", ip, a, b);

		// Trick to get large number into register.
		a = 64'h8000_0000_0000_0000; a = a << 4;
		b = 5;
		$write("0x%0h (0x8_0000_0000_0000_0000)\n", a);
		rp = a * b;
		$write("0x%0h (0x28_0000_0000_0000_0000)\n", rp);

		$finish;
		end

endmodule

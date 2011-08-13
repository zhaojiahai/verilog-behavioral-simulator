/*
 * Test divide expression.
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
		a = 20 / 5;
		b = 0 / a;
		$write("0x%0h 0x%0h (0x4 0x0)\n", a, b);
		ip = 21 / -3;
		a = -35 / ip;
		b = 65 / 10;
		$write("%0d %0d %0d (-7 5 6)\n", ip, a, b);

		// Trick to get large number into register.
		a = 64'h8000_0000_0000_0000; a = a << 4;
		b = 2;
		$write("0x%0h (0x8_0000_0000_0000_0000)\n", a);
		rp = a / b;
		$write("0x%0h (0x4_0000_0000_0000_0000)\n", rp);

		$finish;
		end

endmodule

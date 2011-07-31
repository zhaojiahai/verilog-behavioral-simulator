module tst;

	reg [3:0] a, b;
	reg c;
	reg [7:0] d;

	initial
		begin
		a = 7;
		b = 3;
		c = 0;
		d = 4'b1001;
		$write("%b (00001001)\n", d);
		$write("%b %b %b (0111 0011 0)\n", a, b, c);
		a[3:2] = 0;
		b[2:1] = 2;
		c = ~c;
		$write("%b %b %b (0011 0101 1)\n", a, b, c);
		{a, c, b} = 9'b1101_1_0111;
		$write("%b %b %b (1101 0111 1)\n", a, b, c);
		{a, c, b} = 9'b1101_0_1110;
		$write("%b %b %b (1101 1110 0)\n", a, b, c);
		end

endmodule

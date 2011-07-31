/* Missing a keyword. */
module main;
	reg a;
	reg [3:1] b;

	initial
		begin
		a = 0;
		b = 0;
		#10 $finish;
		end


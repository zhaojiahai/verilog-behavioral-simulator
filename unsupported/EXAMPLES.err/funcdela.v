/* Test function with delay in statement. */
module main;
	reg [7:0] a, b, c;

function [7:0] testfunc;
	input [7:0] z;
	begin
	b = z + 6;
	testfunc = 8;
	#6 $write("testfunc %d: %d %d %d (4 10 x)\n", $time, a, b, c);
	end
endfunction

	initial
		begin
		$write("%d: %d %d %d (x x x)\n", $time, a, b, c);
		#1 a = 4;
		$write("%d: %d %d %d (4 x x)\n", $time, a, b, c);
		#1 b = 6;
		$write("%d: %d %d %d (4 6 x)\n", $time, a, b, c);
		#1 c = testfunc(a);
		$write("%d: %d %d %d (4 10 8)\n", $time, a, b, c);
		end
endmodule

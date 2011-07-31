/* Testing multiple default case items. */
module main;
	reg [0:3] a,b,c;

	initial
		begin
		a = 0;  /* initialize variables */
		b = 5;
		c = 10;
		$write("time %d: a=%d,b=%d,c=%d\n",$time,a,b,c);
		case (1)
		    a == b: $write("time %d: Passed a false test!\n",$time);
		    default : $write("time %d: default\n",$time);
		    a == 0: a = b;
		    a == b: $write("time %d: a=%d,b=%d,c=%d !!\n",$time,a,b,c);
		    default : $write("time %d: default\n",$time);
		endcase
		if (a == b) $write("time %d: a=%d,b=%d,c=%d !!\n",$time,a,b,c);

		#2 $finish;
		end

endmodule

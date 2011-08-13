module main ;

	integer i;

	initial
		begin
		i = -24;
		if (i < -1)
			$display("i < -1: ok!\n");
		else
			$display("i < -1: not!\n");
		$finish;
		end

endmodule

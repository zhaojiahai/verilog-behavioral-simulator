/*
 * Test delayed loop.
 *  dependencies:
 *	register declaration
 *	initial procedural block
 *	system tasks
 *	for statement
 */

module main ;

	reg [3:0] i;

	initial
		begin
		i = 0;
		#1 $write("%d:  Starting loop.\n", $stime);
		for (i = 0; i < 5; i = i + 1)
			begin
			$write("%d:  First line in loop.\n", $stime);
			#1 $write("%d:  Second line in loop.\n", $stime);
			$write("%d:  Third line in loop.\n", $stime);
			end
		#1 $write("%d:  Ending loop.\n", $stime);
		$finish;
		end

endmodule

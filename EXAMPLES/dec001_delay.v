/*
 * Test delay statements.
 *  dependencies:
 *	register declaration
 *	initial procedural block
 *	system tasks
 */

module main ;

	initial
		begin
		#1 $write("current time: %d\n", $stime);
		#5 $write("current time: %d\n", $stime);
		#10 $write("current time: %d\n", $stime);
		#15 $write("current time: %d\n", $stime);
		$finish;
		end

endmodule

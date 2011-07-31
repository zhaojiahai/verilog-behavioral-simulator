
// test case to show vector ordering bugs.
// I believe the register directions below should work, but they don't.
// Must change to [4:0] for it to work.
// Brendan Simon (brendan.simon@bigpond.com).

module  test;

reg [4:0] foo40;	// works great.

reg	[0:4] foo04;	// only works for time=0;
//reg	[4:0] foo04;

reg	[5:1] foo51;	// never works.
//reg	[4:0] foo51;

reg	[1:5] foo15;	// never works.
//reg	[4:0] foo15;

initial begin
	#100; $finish;
end

initial begin
	foo40 = 0;
	foo04 = 0;
	foo51 = 0;
	foo15 = 0;
end

always #10 begin
	foo40 <= foo40 + 1;
	foo04 <= foo04 + 1;
	foo51 <= foo51 + 1;
	foo15 <= foo15 + 1;
end

always @(foo40) begin
	$write("foo40=%08d\n", foo40);
end

always @(foo04) begin
	$write("               foo04=%08d\n", foo04);
end

always @(foo51) begin
	$write("                              foo51=%08d\n", foo51);
end

always @(foo15) begin
	$write("                                             foo15=%08d\n", foo15);
end

endmodule


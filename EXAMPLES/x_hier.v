/*
 * Test hierarchal reference.
 *  dependencies:
 *	initial procedural block
 *	system tasks
 *	module instantiation
 *	module definition
 */

module main;
    reg [3:0] a, b;
    wire [0:3] x, y;

    mod1 m1(x, a);
    mod2 m2(y, b);

    initial
      begin
	a = 9;
	b = 5;
	#1 ;
	$write("m1.z = %d (9)\n", m1.z);
	$write("m1.m2.e = %d (9)\n", m1.m2.e);
	$write("m2.q = %d (5)\n", m2.q);
      end
endmodule

module mod1(z, d);
    output [0:3] z;
    input [3:0] d;
    wire [0:3] z;
    mod2 m2(z, d);
endmodule

module mod2(q, e);
    output [0:3] q;
    input [3:0] e;
    wire [0:3] q;
    assign q = e;
endmodule

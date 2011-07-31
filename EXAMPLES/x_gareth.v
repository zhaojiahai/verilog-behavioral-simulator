/************************************************************/
/*                                                          */
/* Test bench for VBS.  GNU public license blah....         */
/*                                                          */
/************************************************************/
/*                                                          */
/*   Module:   verilog test bench                           */
/*                                                          */
/*   Description:  This module implements a simple test     */
/*             bench with an RTL description and a          */
/*             Behavioural description.  It monitors the    */
/*             changes using the $monitor PLI command.      */
/*                                                          */
/*   Filename: GARETH_TEST.v                                */
/*                                                          */
/*   Author:   Gareth Baron                                 */
/*                                                          */
/*   Tools:    LINUX Suse 2.7, VBS 1.3.5                    */
/*                                                          */
/*                                                          */
/*                                                          */
/*                                                          */
/************************************************************/
/*  Revision:   0.0       09/06/99                          */
/*              Original entry        -  GB                 */
/*                                                          */
/*                                                          */
/************************************************************/

module mux (sel, a, b, f, clock);   
  input sel, a, b;  
  output f;  
  output clock;

reg f;

reg clock;

parameter xyz = 10'h3;

always @(sel or a or b)
begin
  if(sel == 0) f = a; 
  else  f = b; 
end

endmodule


module reg_dff (a, b, clock);
input a;
output b;
input clock;

reg b;

always @(posedge clock)
begin
	b <= a;
end

endmodule


/*******************/
/* End of RTL Code */
/*******************/

/************************************/
/* Start of Behavioural Simulations */
/************************************/

module main ;

// define interconnections
wire sel, a, b, f, clock;
wire finished;
wire dout;


	// instantiate designs
	mux muxa(sel, a, b, f,clock);
	reg_dff dff_a (a,dout,clock);


/******************************/
/* start simulation bench off */
/******************************/
initial 
begin
	$write("clock\tsel\ta\tb\tf\tdout \n");
  	finished = 0;
  	sel = 0; a = 0; b = 0 ; 
  	#10 a = 1;
  	#10 sel = 1;
  	#10 b = 1;
  	#20 a = 0;
  	#10 a = 1;
  	#40 a = 0;

  	#40 finished = 1;
end

// here we have a free running clock
initial
begin
	#1;
	clock = 0;
	forever begin
		#2 clock = ~clock;
	end
end

/*************************************/
/* monitor the signals as a waveform */
/*************************************/

initial 
begin
	forever begin
		
		if(finished == 1) $finish;
		else  begin
//			$write ("%b\t%b\t%b \t%b\t%b\t%b\n", clock, sel,a,b,f,dout);
			$monitor("%b\t%b\t%b \t%b\t%b\t%b", clock, sel,a,b,f,dout);				//#1;   BTW, #0 time increments will lock the simulator - There needs to be an explicit check for this !
		end 
		#1;
	end	
end

// Commands that need to be added for the simulator to be commercial/industrial grade are;
//	`include "<filename>"	-- so that RTL and behavioural models can be seperate
//	`timescale 1ns / 100ps
//	$dumpvars();		//dump all variables to SDF format ?
//	$display();		//equivalent to printf PLI call
//	
//	I tend to seperate my test bench from my RTL code and to do this I usually have a `include at the start of
//	my test bench.  This allows me to compile one testbench file, which then will automatically include the
//	RTL and the testbench.
//
//
//	Things to do;
//		- test tasks and functions in a Behavioural environment
//		- test events
//		


endmodule


/************************/
/* End behavioural code */
/************************/

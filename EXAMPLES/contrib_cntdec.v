/*
From: Hans Akerlund <Hans.Akerlund@uab.ericsson.se>
Subject: Re: Verilog Code for Counters and Decoders
Date: Tue, 14 Oct 1997 17:15:04 +0200
*/

/* 7-bit clock */
/***************/
module counter(clk,reset,count);

  input clk,reset;
  output [6:0] count;
  reg [6:0] count;

  always @(posedge clk or negedge reset)
    begin
    if (reset == 0) counter <= 0;
    else count = count + 1;
    end

endmodule

/* 7/128 asynch decoder */
/************************/
module decoder(count,select);
input [6:0] count;
output [127:0] select;
reg [127:0] select;

always @(count)
  begin
  for (i=0;i<128;i=i+1)
    begin
    if (count == i) select[i] <= 1'b0;
    else select[i] <=1'b1;
    end
  end

endmodule

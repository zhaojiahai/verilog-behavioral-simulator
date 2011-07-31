/*
From: "shplatt" <bubba@bob.com>
Subject: Re: Need VHDL CODE
Date: 26 Jun 1997 06:20:51 GMT

Djavad Amiri <amiri@ascinc.com> wrote in article
<33B131C5.7E36@ascinc.com>...
> Hello
>   I am looking for a VHDL or Verilog source code for a divider. 
>   Please If you have someone send this to me.
> 
>  Thanks in advance.
> 

This is a very crude Verilog implementation I put together while I was
figuring out how to do it.  It has two modules - usubtract (unsigned
subtract) and divide (4-bit divider).  I'm sure there is some way to put
this in a 'for' loop construct to build the mux'es and adders.  I just
didn't have the time to figure it out.

Question to anyone:  Is there a Verilog syntax for an unsigned subtract? 
Couldn't seem to find it.  (Could just be blind --- or stupid.)  Anyway,
that's the reason for 'usubtract'

Be gentle.

--shplatt
*/

//*********
// divide
//*********

module divide ( inta,		// input	- 4 bit numerator
		intb,		// input	- 4 bit denominator
		result		// output	- 4 bit result
	      );

input	[3:0]	inta;
input	[3:0]	intb;
output	[3:0]	result;

reg	[3:0]	result;
reg	[3:0]	temp1;
reg	[3:0]	temp2;
reg	[3:0]	temp3;
wire	[3:0]	subresult1;
wire	[3:0]	subresult2;
wire	[3:0]	subresult3;

usubtract usubtract1 (.inta(inta), .intb(intb << 3), .result(subresult1));
usubtract usubtract2 (.inta(temp1), .intb(intb << 2), .result(subresult2));
usubtract usubtract3 (.inta(temp2), .intb(intb << 1), .result(subresult3));

always@(inta or intb or subresult1)
begin
  if (((intb << 3) <= inta) & (intb[3:1] == 3'b000))
    begin
      //#10 temp1 = inta - (intb << 3);	// This is where I attempted an
      temp1 = subresult1;			// unsigned subtract.
      result[3] = 1'b1;
    end
  else
    begin
      temp1 = inta;
      result[3] = 1'b0;
    end
end

always@(temp1 or intb or subresult2)
begin
  if (((intb << 2) <= temp1) & (intb[3:2] == 2'b00))
    begin
      //#10 temp2 = temp1 - (intb << 2);
      temp2 = subresult2;
      result[2] = 1'b1;
    end
  else
    begin
      temp2 = temp1;
      result[2] = 1'b0;
    end
end

always@(temp2 or intb or subresult3)
begin
  if (((intb << 1) <= temp2) & (intb[3] == 1'b0))
    begin
      //#10 temp3 = temp2 - (intb << 1);
      temp3 = subresult3;
      result[1] = 1'b1;
    end
  else
    begin
      temp3 = temp2;
      result[1] = 1'b0;
    end
end

always@(temp3 or intb)
begin
  if (intb  <= temp3)
    result[0] = 1'b1;
  else
    result[0] = 1'b0;
end
endmodule


//************
// usubtract 
//************

module usubtract ( inta,	// input	- 4 bit value
		   intb,	// input	- 4 bit value
		   result	// input	- 4 bit result
	         );

input	[3:0]	inta;
input	[3:0]	intb;
output	[3:0]	result;

reg	[3:0]	result;

always@(inta or intb)
begin
  if (intb >= inta)
    result = 4'b0000;
  else
    result = inta - intb;
end
endmodule

/*
 * Sample Verilog code from:
 *	rivo@pacbell.net
 *
 * I'm a newbie with verilog and wanted to know why the two if
 * statement in this code below containing 'or' and '||' are
 * incorrect. (it produced an error through Synplicity)
 */

module COUNTER (Clock, Reset, Enable, Load, Mode, Data, Q);
  input Clock ;
  input Reset ;
  input Enable;
  input Load  ;
  input Mode  ;
  input  [7:0] Data ;
  output [7:0] Q;

  reg [7:0] Q;

  always @ (posedge Clock or negedge Reset)
  begin
    if (!Reset)
      Q <= 0;
    else
      if (!Enable)
        if (!Load)
          Q <= Data;
        else if (!Mode && (Q[3:0] != 15)) ||
              (Mode && (Q[3:0] != 9))
          Q[3:0] <= Q[3:0] + 1;
        else
        begin
          Q[3:0] <= 0;
          if (!Mode && (Q[7:4] != 15)) or
             (Mode && (Q[7:4] != 9))
            Q[7:4] <= Q[7:4] + 1;
          else
            Q[7:4] <= 0;
        end
  end

endmodule

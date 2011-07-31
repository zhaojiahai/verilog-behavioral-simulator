module main ;
    reg clk ;
    always #10 clk = ~clk ;

    initial begin
        clk = 1 ;

        @(posedge clk) $write("time=%d, clk=%d\n", $stime, clk );

        repeat(32) begin
            @(posedge clk) $write("time=%d, clk=%d\n", $stime, clk );
        end

        $finish ;
    end
endmodule

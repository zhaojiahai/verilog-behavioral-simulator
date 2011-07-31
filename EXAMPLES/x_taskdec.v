module test;

task prin;
inout [3:0] a;
begin
        #10;
        $write("%d: parameter = %d\n", $stime, a);
end
endtask

initial
begin
        prin(4'h1);
        prin(4'h2);
end

endmodule

module delay_in_task;
test t1();
endmodule

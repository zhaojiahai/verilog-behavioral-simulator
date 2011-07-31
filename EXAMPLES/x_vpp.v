/*
 * Test the preprocessor support.
 *  dependencies:
 *  initial procedural block
 *  sequence block
 *  register declaration
 *  $write system procedure
 */

`let a = ABS(-1)
`let b = ROUND(2.49)
`let c = FLOOR(3.84)
`let d = MIN(0,5)
`let REG_TMP_LS = d * ((a + b) / c - (d - c))
`let REG_TMP_MS = LOG2(128)
`let FORMAT_STR = "%b %b %b %b %b %b %b %b\n"

module main;
	reg [`REG_TMP_MS:`REG_TMP_LS] tmp;

	initial
		begin
		tmp = 8'b10011010;

		$write("`FORMAT_STR"
`let reg_tmp_size = CEIL(7.54321)
`for (i = 0; i < reg_tmp_size; i++)
			, tmp[`i]
`endfor
			);
		end

endmodule

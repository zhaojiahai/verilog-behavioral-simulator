#! /bin/sh

PATH=/bin:/usr/bin

if [ "$1"X = "-v"X ]
then
	verbose=1
	shift
else
	verbose=0
fi

if [ "$1"X = "-t"X ]
then
	test_file_only=1
	shift
	test_file=$1
	shift
else
	test_file_only=0
fi

vbs=$1		# Command name.
ivldir=$2	# Top directory of IVL tests.

if [ "$vbs"X = X -o "$ivldir"X = X ]
then
	echo "Syntax: $0 exec_name ivl_dir"
	exit 0
fi

if [ ! -x $vbs ]
then
	echo "$vbs is not a valid executable file."
	exit 1
fi

if [ ! -f $ivldir/regress.list -o ! -d $ivldir/ivltests ]
then
	echo "'$ivldir' does not contain the IVL test suite."
	exit 1
fi

# Create a link to the testsuite directory in the current directory.
rm -f regress.list
rm -f ivltests
rm -f contrib
rm -f gold
ln -s $ivldir/regress.list .
ln -s $ivldir/ivltests .
ln -s $ivldir/contrib .
ln -s $ivldir/gold .

bad_exit() {
	echo The following command line caused a fatal exit, quitting shell script.
	echo $@
	exit -1
}

run_test () {
	vfn=$1/$2
	arg="-q $3"
	logf=$4

	[ $verbose -eq 1 ] && echo Running: $vbs $arg ${vfn}.v

	rm -f tmp.log
	rm -f core
	$vbs $arg ${vfn}.v > tmp.log
	retval=$?

	# Bus error, segmentation fault
	[ $retval -eq 138 ] && bad_exit $vbs $arg ${vfn}.v
	[ $retval -eq 139 ] && bad_exit $vbs $arg ${vfn}.v

	# Problem during compilation.
	[ $retval -ne 0 ] && return $retval

	if [ "$3"X = "-q -c"X ]; then
		# Compiled ok, asked not to simulate.
		return 0
	else
		# Compiled/simulated ok, check result.
		grep -q FAILED tmp.log
		retval=$?
		[ $retval -eq 0 ] && return -1
		grep -q PASSED tmp.log
		retval=$?
		[ $retval -eq 0 ] && return 0
		if [ -f gold/${logf} ]; then
			diff -q -w tmp.log gold/${logf} > /dev/null
			retval=$?
			[ $retval -ne 0 ] && return -2
			return 0
		fi
		return 1
	fi
}

rm -f unsupported.log
rm -f wrong_pass.log
rm -f failed.log
rm -f parser_problem.log
rm -f invalid_verilog.log
rm -f other_error.log
rm -f logdiff.log

# Need a sub-shell.
true && (

total=0
tested=0
skipped=0
passed=0
unsupported=0
wrong_pass=0;
failed=0
parser_problem=0
invalid_verilog=0
other_error=0
logdiff=0

echo "Executing tests, please wait..."

while read first second third mixed; do

	# We ignore comments, blank lines and CN/EF/NI test cases.
	[ "$first"X = X ] && continue
	case "$first" in
		\#*)
			continue
			;;
	esac

	type=`echo $second | sed -e "s/[,].*//"`
	case "$type" in
		CN|EF|NI)
			continue
			;;
	esac

	# Make sure file and directory exists.
	fn=$first

	if [ $test_file_only -eq 1 ]; then
		[ "$fn"X != "$test_file"X ] && continue
	fi

	testdir=$third
	[ "$testdir"X = X ] && continue
	[ ! -f $testdir/${fn}.v ] && continue

	# Count number of test cases in the file.
	total=$(expr $total + 1)

	# We ignore the parameter options.
	options=`echo $second | sed -e "s/^[^,]*[,]//"`
	[ "$options"X = "$type"X ] && options=
	if [ "$options"X = "-S"X ]; then
		case "$fn" in
			pr519|pr685|basicreg|multireg|basicexpr[234]|shiftl)
				echo "$fn.v -- synthesis test case, skipped"
				skipped=$(expr $skipped + 1)
				continue
				;;
		esac
	fi

	# FIXME: improve performance.
	case "$fn" in
		multiply_large|no_if_statement)
			echo "$fn.v -- takes too long, skipped"
			skipped=$(expr $skipped + 1)
			continue
			;;
	esac

	# Extract optional log file for comparison.
	mixed=`echo $mixed | sed -e "s/[#].*//"`
	echo ${mixed} | grep -q "diff="
	diff_retval=$?
	echo ${mixed} | grep -q "gold="
	gold_retval=$?
	logf=
	[ $diff_retval -eq 0 ] && logf=`basename $mixed`
	[ $gold_retval -eq 0 ] && logf=`echo $mixed | sed -e "s/gold=//"`
	logf_ext=`echo $logf | sed -e "s/^[^.]*[.]//"`

	# Process each file according to test case type.
	tested=$(expr $tested + 1)
	if [ "$type"X = "CO"X ]; then
		run_test $testdir $fn "-q -c" $logf
	else
		run_test $testdir $fn "-q" $logf
	fi
	retval=$?

	if [ $retval -eq 16 ]; then
		unsupported=$(expr $unsupported + 1)
		cat tmp.log >> unsupported.log
	elif [ $retval -eq 33 ]; then
		unsupported=$(expr $unsupported + 1)
		cat tmp.log >> unsupported.log
	else
		case "$type" in
			normal|CO)
				# Delete object file (if exists).
				rm -f ${fn}.vo

				if [ $retval -eq 0 ]; then
					passed=$(expr $passed + 1)
				elif [ $retval -eq 255 ]; then
					failed=$(expr $failed + 1)
					echo "$0: vbs failed to simulate/compile $testdir/${fn}.v!" >> failed.log
					cat tmp.log >> failed.log
				elif [ $retval -eq 254 ]; then
					logdiff=$(expr $logdiff + 1)
					cat tmp.log >> logdiff.log
				else
					case "$retval" in
						1[12])
							parser_problem=$(expr $parser_problem + 1)
							cat tmp.log >> parser_problem.log
							;;
						9|1[045]|1[7-9]|2[0-9]|3[0-2]|3[4-9]|4[01])
							invalid_verilog=$(expr $invalid_verilog + 1)
							cat tmp.log >> invalid_verilog.log
							;;
						*)
							other_error=$(expr $other_error + 1)
							cat tmp.log >> other_error.log
							;;
					esac
				fi
				;;
			CE)
				if [ $retval -eq 255 ]; then
					passed=$(expr $passed + 1)
				elif [ $retval -ne 0 ]; then
					passed=$(expr $passed + 1)
				else
					wrong_pass=$(expr $wrong_pass + 1)
					echo "$0: vbs passed $testdir/${fn}.v unexpectedly!" >> wrong_pass.log
					cat tmp.log >> wrong_pass.log
				fi
				;;
		esac
	fi

done

echo "Total:                $total"
echo "Tested:               $tested"
echo "Skipped:              $skipped"
echo "Passed:               $passed"
echo "Unsupported:          $unsupported"
echo "Unexpected pass:      $wrong_pass"
echo "Failed:               $failed"
echo "Parser errors:        $parser_problem"
echo "Invalid Verilog code: $invalid_verilog"
echo "Other errors:         $other_error"
echo "Logs differ:          $logdiff"

) < regress.list

Echo The following files were generated:
if [ -f unsupported.log ]; then
	echo "unsupported.log."
fi
if [ -f wrong_pass.log ]; then
	echo "wrong_pass.log."
fi
if [ -f failed.log ]; then
	echo "failed.log."
fi
if [ -f parser_problem.log ]; then
	echo "parser_problem.log."
fi
if [ -f invalid_verilog.log ]; then
	echo "invalid_verilog.log."
fi
if [ -f other_error.log ]; then
	echo "other_error.log."
fi
if [ -f logdiff.log ]; then
	echo "logdiff.log."
fi

rm -f regress.list
rm -f ivltests
rm -f contrib
rm -f gold
rm -f tmp.log
rm -f *.vcd
rm -f *.dump

exit 0

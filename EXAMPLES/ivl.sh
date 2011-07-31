#! /bin/sh

PATH=/bin:/usr/bin

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

run_test () {
	vfn=$1/$2
	arg="-q $3"
	logf=$4

	rm -f tmp.log
	rm -f core
	$vbs $arg ${vfn}.v > tmp.log
	retval=$?
	if [ -f core ]; then
		echo "'$vbs $arg ${vfn}.v' caused a core dump."
		exit -1
	fi

	# Problem during compilation.
	[ $retval -eq 16 ] && return 200
	[ $retval -eq 32 ] && return 200
	[ $retval -ne 0 ] && return 100

	if [ "$3"X = "-q -c"X ]; then
		# Compiled ok, asked not to simulate.
		return 0
	else
		# Compiled/simulated ok, check result.
		grep -q FAILED tmp.log
		retval=$?
		[ $retval -eq 0 ] && return 2
		grep -q PASSED tmp.log
		retval=$?
		[ $retval -eq 0 ] && return 0
		if [ -f gold/${logf} ]; then
			diff -q -w -B -b tmp.log gold/${logf} > /dev/null
			retval=$?
			[ $retval -ne 0 ] && return 3
			return 0
		fi
		return 1
	fi
}

rm -f failed.log
rm -f unsupported.log
rm -f inconclusive.log

# Need a sub-shell.
true && (

tested=0
passed=0
failed=0
skipped=0
unsupported=0
inconclusive=0
echo "Executing tests, please wait..."

while read first second third mixed; do

	# Make sure file and directory exists.
	fn=$first
	type=`echo $second | sed -e "s/[,].*//"`

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

	case "$fn" in
		multiply_large)
			echo "$fn.v -- takes too long, skipped"
			skipped=$(expr $skipped + 1)
			continue
			;;
		pr594|pr596|const[234]|idiv3|disp_dec2)
			echo "$fn.v -- unsupported '-' arithmetic, skipped"
			skipped=$(expr $skipped + 1)
			continue
			;;
		pr224|pr622)
			echo "$fn.v -- macro needed, skipped"
			skipped=$(expr $skipped + 1)
			continue
			;;
		time4|pr524|pr569|disp_parm|lh_varindx[45]|port-test3|blankport|memport_bs|format|test_width|test_extended)
			CI_msg="NOTE: $fn.v -- unsupported (conformance)"
			type=CI
			;;
		land4|pr547)
			CI_msg="NOTE: $fn.v -- log file differs"
			type=CI
			;;
		port-test[567]|delay|signed8|pr693)
			CI_msg="NOTE: $fn.v -- unsupported (new feature)"
			type=CI
			;;
		escape*|mangle|mangle_1|dotinid|big_int)
			CI_msg="NOTE: $fn.v -- unsupported (unable to parse)"
			type=CI
			;;
		scope1|inertdly|pic|vcd-dup|sp2|basiclatch)
			CI_msg="NOTE: $fn.v -- implicit variable declaration"
			type=CI
			;;
		shellho1|param_select2|memidx|dcomp1|assign_nb2)
			CI_msg="NOTE: $fn.v -- use before declaration"
			type=CI
			;;
		mhead_task|hello1|monitor|string10|pr307a|pr673)
			CI_msg="NOTE: $fn.v -- stupid behavior"
			type=CI
			;;
		talu|event_list3|mem1|param_string|string9|bitsel|prng|pr273|pr492|pr590|pr690|pr704)
			CI_msg="NOTE: $fn.v -- format string problem"
			type=CI
			;;
		timescale1|inout|function4|task-scope|hierspace|nblkpush|basicexpr)
			CI_msg="NOTE: $fn.v -- non-conforming code"
			type=CI
			;;
		pr243|pr528*|pr532*|pr632)
			CI_msg="NOTE: $fn.v -- incorrect log file"
			type=CI
			;;
		\#*|"")
			continue
			;;
		*)
			;;
	esac
	testdir=$third
	[ "$testdir"X = X ] && continue
	[ ! -f $testdir/${fn}.v ] && continue

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

	if [ $retval -eq 200 ]; then
		unsupported=$(expr $unsupported + 1)
		cat tmp.log >> unsupported.log
	else
		case "$type" in
			normal|CO)
				# Delete object file (if exists).
				rm -f ${fn}.vo

				if [ $retval -eq 100 ]; then
					failed=$(expr $failed + 1)
					cat tmp.log >> failed.log
				elif [ $retval -eq 0 ]; then
					passed=$(expr $passed + 1)
				elif [ $retval -eq 1 ]; then
					inconclusive=$(expr $inconclusive + 1)
					echo "$0: inconclusive test case '$fn':" >> inconclusive.log
					cat tmp.log >> inconclusive.log
				else
					failed=$(expr $failed + 1)
					echo "$0: failure detected for '$fn':" >> failed.log
					cat tmp.log >> failed.log
				fi
				;;
			CE)
				if [ $retval -eq 100 ]; then
					passed=$(expr $passed + 1)
				else
					failed=$(expr $failed + 1)
					echo "$0: failure detected for '$fn':" >> failed.log
					cat tmp.log >> failed.log
				fi
				;;
			CI)
				inconclusive=$(expr $inconclusive + 1)
				echo "$0: inconclusive test case '$fn':" >> inconclusive.log
				cat tmp.log >> inconclusive.log
				echo $CI_msg >> inconclusive.log
				;;
		esac
	fi

done

total=$(expr $tested + $skipped)
echo "Total cases:             $total"
echo "Tested cases:            $tested"
echo "Skipped test cases:      $skipped"
echo "Passed test cases:       $passed"
echo "Failed test cases:       $failed"
echo "Unsupported test cases:  $unsupported"
echo "Inconclusive test cases: $inconclusive"

) < regress.list

if [ -f failed.log ]; then
	echo "Failed test cases are logged to failed.log."
fi
if [ -f unsupported.log ]; then
	echo "Unsupported test cases are logged to unsupported.log."
fi
if [ -f inconclusive.log ]; then
	echo "Inconclusive test cases are logged to inconclusive.log."
fi

rm -f regress.list
rm -f ivltests
rm -f contrib
rm -f gold
rm -f tmp.log
rm -f *.vcd
rm -f *.dump

exit 0
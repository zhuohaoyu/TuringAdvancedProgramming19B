Module:TestMe.c
 + Function:Test7_func1
 + Function:llvm.dbg.declare
   - Empty Function
 + Function:read1
   - Empty Function
 + Function:read2
   - Empty Function
 + Function:is_valid
   - Empty Function
 + Function:foo
   - Empty Function
 + Function:bar
   - Empty Function
 + Function:Test7_func2
 + Function:Test7_func3
 + Function:Test7_func4
 + Function:Test7_func5
 + Function:Test7_func6
 + Function:Test7_func7
 + Function:Test7_func8
 + Function:Test7_func9
 + Function:Test7_func10
 + Function:Test7Frequent Itemsets:
	"bar(i32)", "i32 = is_valid(i32)", 
	"bar(i32)", "i32 = read2(...)", 
	"foo(i32)", "i32 = is_valid(i32)", 
	"foo(i32)", "i32 = read2(...)", 
	"i32 = is_valid(i32)", "i32 = read2(...)", 
	"bar(i32)", "i32 = is_valid(i32)", "i32 = read2(...)", 
	"foo(i32)", "i32 = is_valid(i32)", "i32 = read2(...)", 
Infrequent Itemsets:
	"bar(i32)", "foo(i32)", "i32 = is_valid(i32)", 
_func11
 + Function:Test7_func12

PARs:
	Rule: { i32 = is_valid(i32) } -> { bar(i32) }
	confidence rate = 1.000000e+00

	Rule: { bar(i32) } -> { i32 = is_valid(i32) }
	confidence rate = 1.000000e+00

	Rule: { i32 = read2(...) } -> { bar(i32) }
	confidence rate = 1.000000e+00

	Rule: { bar(i32) } -> { i32 = read2(...) }
	confidence rate = 1.000000e+00

	Rule: { i32 = is_valid(i32) } -> { foo(i32) }
	confidence rate = 1.000000e+00

	Rule: { foo(i32) } -> { i32 = is_valid(i32) }
	confidence rate = 1.000000e+00

	Rule: { i32 = read2(...) } -> { foo(i32) }
	confidence rate = 1.000000e+00

	Rule: { foo(i32) } -> { i32 = read2(...) }
	confidence rate = 1.000000e+00

	Rule: { i32 = read2(...) } -> { i32 = is_valid(i32) }
	confidence rate = 1.000000e+00

	Rule: { i32 = is_valid(i32) } -> { i32 = read2(...) }
	confidence rate = 1.000000e+00

	Rule: { i32 = read2(...) i32 = is_valid(i32) } -> { bar(i32) }
	confidence rate = 1.000000e+00

	Rule: { bar(i32) i32 = read2(...) } -> { i32 = is_valid(i32) }
	confidence rate = 1.000000e+00

	Rule: { bar(i32) i32 = is_valid(i32) } -> { i32 = read2(...) }
	confidence rate = 1.000000e+00

	Rule: { i32 = read2(...) i32 = is_valid(i32) } -> { foo(i32) }
	confidence rate = 1.000000e+00

	Rule: { foo(i32) i32 = read2(...) } -> { i32 = is_valid(i32) }
	confidence rate = 1.000000e+00

	Rule: { foo(i32) i32 = is_valid(i32) } -> { i32 = read2(...) }
	confidence rate = 1.000000e+00

NARs:
	Rule: { bar(i32) i32 = is_valid(i32) } -> { foo(i32) }
	confidence rate = 9.166667e-01

Error #1: 
  %6 = call i32 @is_valid(i32 %5), !dbg !19, TestMe.c:140
  call void @bar(i32 %12), !dbg !29, *Linker Error*:144
with the(se) following UNEXPECTED instruction(s): 
  call void @foo(i32 %13), !dbg !31, *Linker Error*:145


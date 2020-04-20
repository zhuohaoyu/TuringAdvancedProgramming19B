extern int xfunc(int);
extern int yfunc();
extern int test(int, int);
extern int foo(int);
extern int bar(int);

int TestMe(int param1, short param2) {
	int local1, local2, ret;
	if (param1 > 10) {
		local1 = param1 * param2;
		switch (local1) {
			case 3:
				local2 = xfunc(param2);
				break;
			case 5:
				local2 = yfunc();
				break;
			default:
				local2 = local1 % 7;
		}
	} else if (param2 > 0) {
		local2 = (param2 << 3) / 11;
	}
	if (test(local1, local2)) {
		ret = foo(local1);
	} else {
		ret = bar(local2);
	}
	 return ret;
}

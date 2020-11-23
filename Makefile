.tmp:
	mkdir -p .tmp

conf_r.h: .tmp
	gcc -o .tmp/solve_conf solve_conf.c
	.tmp/solve_conf > conf_r.h

diff_r.h: .tmp
	gcc -o .tmp/solve_diff solve_diff.c
	.tmp/solve_diff > diff_r.h

backward: .tmp conf_r.h diff_r.h
	gcc -o .tmp/backward backward.c
	.tmp/backward

forward: .tmp
	gcc -o .tmp/forward forward.c
	.tmp/forward

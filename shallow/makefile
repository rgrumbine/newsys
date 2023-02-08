all : global shallow hello 

global : global.C
	mpic++ -Wall -O2 -I /Users/rmg3/usrlocal/mmablib/include $< -o $(@) 

shallow : shallow.C
	mpic++ -Wall -O2  -DSCALE=8 -I /Users/rmg3/usrlocal/mmablib/include shallow.C -o shallow 

hello : hello.C
	mpic++ hello.C -o hello 


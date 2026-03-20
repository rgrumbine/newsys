all : global shallow hello 

global : global.C
	mpic++ -Wall -O2 -I $(HOME)/rgops/mmablib/include $< -o $(@) 

shallow : shallow.C
	mpic++ -Wall -O2 -DSCALE=8 -I $(HOME)/rgops/mmablib/include $< -o $(@)

hello : hello.C
	mpic++ hello.C -o hello 


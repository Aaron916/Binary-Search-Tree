###############################################################
# Program:
#     Assignment 09, Binary Search Tree
#     Brother JonesL, CS235
# Author:
#     Adam Goff, Aaron Rook, Martin Melerio, Tanner Stratford, Allan Marina
# Summary:
#     Our Binary Search Tree implemenation.
# Time:
#     12+ hours combined for the core part of the assignment
###############################################################

##############################################################
# The main rule
##############################################################
a.out: assignment09.o
	g++ -o a.out assignment09.o -g
	tar -cf assignment09.tar *.h *.cpp makefile

##############################################################
# The individual components
#      assignment09.o     : the driver program
##############################################################
assignment09.o: bst.h assignment09.cpp
	g++ -c assignment09.cpp -g

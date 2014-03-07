#####################################################################
##                              TAPSsc                             ##
##                                                                 ##
##                     TAPS slowcontrol software                   ##
##                                                                 ##
##                                                                 ##
##                        (C) 2013-2014 by                         ##
##      Dominik Werthmueller <dominik.werthmueller@unibas.ch>      ##
##              Thomas Strub <thomas.strub@unibas.ch>              ##
##                                                                 ##
#####################################################################

all: uml/class_hierarchy_1.pdf uml/class_hierarchy_2.pdf

clean:
	rm -f uml/class_hierarchy_1.pdf uml/class_hierarchy_2.pdf

uml/class_hierarchy_1.pdf: uml/class_hierarchy_1.uxf
	umlet -action=convert -format=eps -filename=uml/class_hierarchy_1.uxf
	epstopdf uml/class_hierarchy_1.eps
	rm uml/class_hierarchy_1.eps

uml/class_hierarchy_2.pdf: uml/class_hierarchy_2.uxf
	umlet -action=convert -format=eps -filename=uml/class_hierarchy_2.uxf
	epstopdf uml/class_hierarchy_2.eps
	rm uml/class_hierarchy_2.eps


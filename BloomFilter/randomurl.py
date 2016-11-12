#!/usr/bin/env python
# -*- coding: utf-8 -*-

import random
import sys

str = 'abcde/fghijk/lmnopqr/stuvwx/yzABCDEFGHIJK/LMNOPQRSTUV/WXYZ&/?#1234567890%.=-_';
strlen = len(str)
syslen = int(sys.argv[1])
fp = open('data.txt', 'w')
for i in range(syslen):
	mystr = ''
	urllen = random.randint(10, 128)
	for j in range(urllen):
		i = random.randint(0,strlen - 1)
		mystr = mystr + str[i]
#		mystr = mystr + str[random.randint(0, syslen - 1)];
	mystr = mystr + '\r\n'
	fp.write(mystr);
fp.close()

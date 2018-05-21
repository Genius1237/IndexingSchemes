import math
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as mt
import matplotlib.patches as mp

def f1():
	df = pd.read_csv('data2', header=None)

	plt.figure(1,figsize=(12,8))
	ax = plt.gca()
	#ax.xaxis.set_major_locator(mt.FixedLocator([1, 2, 3, 4, 5, 6, 7, 8]))
	ax.set_title('Blocks Accesses vs No of Records \n MultiLevel Indexing')
	ax.set_xscale('log')
	ax.set_yscale('log')
	ax.set_xlabel('Number of records')
	ax.set_ylabel('Number of blocks acceses')

	x = df.iloc[:6,1]#.apply(math.log10)
	y0 = df.iloc[:6,2]
	y1 = df.iloc[6:12,2]
	y2 = df.iloc[12:18,2]
	y3 = df.iloc[18:24,2]
	y4 = df.iloc[24:30,2]
	y5=df.iloc[30:36,2]

	ax.plot(x,y0,markersize=10,marker='.',linewidth=1,color='k')
	ax.plot(x,y1,markersize=10,marker='.',linewidth=1,color='r')
	ax.plot(x,y2,markersize=10,marker='.',linewidth=1,color='b')
	ax.plot(x,y3,markersize=10,marker='.',linewidth=1,color='g')
	ax.plot(x,y4,markersize=10,marker='.',linewidth=1,color='c')
	ax.plot(x,y5,markersize=10,marker='.',linewidth=1,color='y')
	ax.legend(handles=[	#mp.Patch(label='Without Indexing', color='k'), mp.Patch(label='With Primary Index', color='r')])
						mp.Patch(label='0 level(s) of index', color='k'),mp.Patch(label='1 level(s) of index', color='r'),
						mp.Patch(label='2 level(s) of index', color='b'),mp.Patch(label='3 level(s) of index', color='g'),
						mp.Patch(label='4 level(s) of index', color='c'),mp.Patch(label='5 level(s) of index', color='y')])

	#plt.show()
	plt.savefig('fig.png')

def f2():
	df = pd.read_csv('data3', header=None)

	plt.figure(1,figsize=(12,8))
	ax = plt.gca()
	ax.set_title('Blocks required vs No of Records \n Secondary Index')
	#ax.set_title('No of Block Accesses vs No of Records \n Secondary Index')
	ax.set_xscale('log')
	#ax.set_yscale('log')
	ax.set_xlabel('Number of records')
	ax.set_ylabel('Number of blocks required')
	#ax.set_ylabel('Number of blocks accesses')

	x = df.iloc[:6,0]#.apply(math.log10)
	y0 = df.iloc[:6,2]
	y1 = df.iloc[6:12,2]
	#y2 = df.iloc[12:18,2]
	#y3 = df.iloc[18:24,2]
	#y4 = df.iloc[24:30,2]
	#y5=df.iloc[30:36,2]

	ax.plot(x,y0,markersize=10,marker='.',linewidth=1,color='k')
	ax.plot(x,y1,markersize=10,marker='.',linewidth=1,color='r')
	#ax.plot(x,y2,markersize=10,marker='.',linewidth=1,color='b')
	#ax.plot(x,y3,markersize=10,marker='.',linewidth=1,color='g')
	#ax.plot(x,y4,markersize=10,marker='.',linewidth=1,color='c')
	#ax.plot(x,y5,markersize=10,marker='.',linewidth=1,color='y')
	ax.legend(handles=[	mp.Patch(label='Without Indexing', color='k'), mp.Patch(label='With Secondary Index', color='r')])
						#mp.Patch(label='0 level(s) of index', color='k'),mp.Patch(label='1 level(s) of index', color='r'),
						#mp.Patch(label='2 level(s) of index', color='b'),mp.Patch(label='3 level(s) of index', color='g'),
						#mp.Patch(label='4 level(s) of index', color='c'),mp.Patch(label='5 level(s) of index', color='y')])

	#plt.show()
	plt.savefig('fig.png')

if __name__=="__main__":
	f1()
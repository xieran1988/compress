import marshal

m = marshal.load(open('sumup.m', 'rb'))
totsiz = m['totsiz']
print m['totsiz']
del m['totsiz']
for i in m:
	if i[1] == 80:
		print i, '%.2f'%(m[i]*1.0/totsiz)

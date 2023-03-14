import re

mylist = [{'one': 'hi', 'two':'hello'}, {'three': 'yes', 'four':'bye'}]
d = ['one', 'two', 'three']
r = re.compile(r'(\{[^{}]+\})')
if(r.match, d): # Read Note
    print("True")
else:
    print("False")  
f = open('CK.txt')

countLine = 0
count = 0
first = ''
second = ''
wordList = []
"""
a = ord('a')
b = ord('b')
print a - 96
print b
"""
for line in f:
    if countLine==0:
        countLine+=1
        continue
    else:
        for word in line:
            wordList.append(word)

    print len(wordList)-1
    wordList = []

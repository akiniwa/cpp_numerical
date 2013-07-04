f = open('UBA.txt')

countLine = 0
count = 0
first = ''
second = ''
for line in f:
    for s in line.split(' '):
        if countLine==0:
            pass
            #print s
        elif count==0:
            first = s
            count = 1
        elif count==1:
            second = s
            count = 0
    if countLine!=0:
        for 
        print first[0]
        print second
    countLine+=1

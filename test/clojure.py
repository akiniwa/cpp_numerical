#f = open('input.txt')
f = open('tes.text')

left = []
countLine = 0
countWord = 0
total = []
maxWord = 0
for line in f:
    if countLine==0:
        countLine+=1
        continue
    else:
        for w in line:
            if w=='('or w=='{'or w=='[':
                left.append(w)
                total.append(countWord)
                countWord=0
            elif w==')' or w=='}' or w==']':
                if len(left)!=0:
                    if left[-1]=='('and w!=')': 
                        total.append(countWord)
                        countWord = 0
                    elif left[-1]=='['and w!=']': 
                        total.append(countWord)
                        countWord = 0
                    elif left[-1]=='{'and w!='}': 
                        total.append(countWord)
                        countWord = 0
                    else:
                        total.append(countWord)
                        left.pop()
            elif w!=' ':
                countWord+=1
           
        for t in total:
            if maxWord <= t:
                maxWord = t
        print maxWord
        total = []
        countWord = 0
        maxWord=0

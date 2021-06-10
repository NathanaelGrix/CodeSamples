#question 1
def missing_number(numbers):
    tempsum = 0
    for x in numbers:
        tempsum += x
        #print(x)
    return 5050 - tempsum

#Q1 test
missing = []
for i in range(1,101):
    missing.append(i)
missing[23] = 0
print("missing number is: ",missing_number(missing))


#question 2
def find_duplicate(numbers):
    seen = []
    for x in numbers:
        if x in seen:
            return x
        seen.append(x)
    return False

#Q2 test
duplicate = [1,2,3,4,5,3]
print("duplicate number is: ",find_duplicate(duplicate))

#question 3
def smallest_largest(numbers):
    smallest = numbers[0]
    largest = numbers[0]
    for x in numbers[1:]:
        if x > largest:
            largest = x
        elif x < smallest:
            smallest = x
    return (smallest,largest)

#Q3 test
min_max = [100, 20, 22, 10005, 1, 43, -1, 0]
print("smallest and largest numbers are: ",smallest_largest(min_max))


#question 4
def sum_pairs(numbers,target):
    pairs = []
    for i,x in enumerate(numbers):
        diff = target-x
        for y in numbers[i+1:]:
            if (y == diff):
                pairs.append((x,y))
    return pairs

#Q4 test
target_pairs = [6, 10, 2, 11, 1, 0, 4, 12, 5, 10, 6]
target_num = 12
print("these pairs sum to ", target_num, ": \n", sum_pairs(target_pairs,target_num))


#question 5
def find_duplicate(numbers):
    seen = []
    duplicates = []
    for x in numbers:
        if x in seen and x not in duplicates:
            duplicates.append(x)
        seen.append(x)
    return duplicates

#Q5 test
duplicate = [1,2,3,4,5,3,3,10,3,22,22,1,3,6]
print("duplicate numbers are: ",find_duplicate(duplicate))

#question 6
def my_quicksort(numbers,start=0,end=0,is_starting=True):

    #set ending point if this is the first one
    if(is_starting):
        end=len(numbers)-1
    print("current numbers : ", numbers[start:end+1])

    #if there is more than one number then reorder and recurse
    if end-start > 1:

        #reorder
        compare = numbers[end]
        x = start
        y = end-1
        xturn = True
        while x<=y:
            if numbers[x]<=compare:
                x+=1
            elif numbers[y]>compare:
                y-=1
            else:
                numbers[x],numbers[y] = numbers[y],numbers[x]
                print("current numbers : ", numbers[start:end+1])
            xturn = not xturn
        #print("swapping ", numbers[end], " and ", numbers[y+1])
        numbers[end],numbers[y+1]=numbers[y+1],numbers[end]
        #print("x and y: ",x)
        print("numbers are ", numbers, "\n")

        #recurse
        #print("start and x would be : ", start, ", ", x-1)
        my_quicksort(numbers,start,x-1,False)
        #print(numbers, (end//2)+1, end, False)
        my_quicksort(numbers,x,end,False)


#Q6 test
sortnum = [8,16,1,6,9,2,3,15,14,4,10,12,11,5,7]
my_quicksort(sortnum)
print(sortnum)

#question 7
def remove_duplicates(numbers):
    gaps = 0
    i = 0
    for i,x in enumerate(numbers):
        numbers[i-gaps] = numbers[i]
        for j,y in enumerate(numbers[0:i-gaps]):
            if x == y:
                gaps += 1
    for k in range(-gaps,0):
        numbers[k]=0

#Q7 test
duplicate_remove = [1,1,2,3,1,5,4,3,6,9,1000,23,5]
print("duplicated array: ",duplicate_remove)
remove_duplicates(duplicate_remove)
print("duplicated array: ",duplicate_remove)

#question 8
def reverse_array(numbers):
    for i in range(0,len(numbers)//2):
        numbers[i],numbers[-(i+1)]=numbers[-(i+1)],numbers[i]

reverse = [1,2,3,5,6,4,7,8,9,10]
print("reverse array: ",reverse)
reverse_array(reverse)
print("reverse array: ",reverse)

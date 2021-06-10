import math

def createDiagonalSystem(a,b,c,n):
    matrix = [[0 for x in range(n)] for y in range(n)]
    for x in range(n):
        matrix[x][x] = b
        if x>0:
            matrix[x-1][x] = a
        if x<n-1:
            matrix[x+1][x] = c
    return matrix
def solver(A,b,n):
    solution = [0 for x in range(n)]
    for x in range(1,n):
        A[x][x] = A[x][x] - (A[x-2][x-1]/A[x-1][x-1])*A[x][x-1]
        b[x] = b[x] - (A[x-2][x-1]/A[x-1][x-1])*b[x-1]
    solution[n-1] = b[n-1]/A[n-1][n-1]
    for x in range(n-2,-1,-1):
        solution[x] = 1/A[x][x]*(b[x]-A[x+1][x]*solution[x+1])
    return solution

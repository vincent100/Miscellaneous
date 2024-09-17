# This is a project to simulate the spread of an infectious disease in a network (graph).
# Some fields include: connections, infection rate (per connection), death rate, etc...
# Started off deterministic, then some params were (impossible?) to figure out with stats

# Vincent Bucourt

import random

numpeople = int(input("How many people?\n"))
grid = []
gridfull = []
gridconnection = []

for i in range(numpeople):
    toadd = [0]*numpeople
    grid.append(toadd)
    toadd = [0]*numpeople
    gridfull.append(toadd)
    toadd = [0]*numpeople
    gridconnection.append(toadd)
    
for i in range(numpeople):
    for d in range(i, numpeople-1):
        print("Is person", str(i+1), "close to person", str(d+2), "?")
        answer = input()
        if answer == "yes" or answer == "Yes":
            grid[i][d+1] = 1
            gridfull[i][d+1] = 1
            gridfull[d+1][i] = 1
            strength = int(input("What is the percentage of chance that it spreads in one day?\n"))
            gridconnection[i][d+1] = strength
            gridconnection[d+1][i] = strength

infectedfirst = int(input("Who starts off as infected?\n"))
days = int(input("For how long?\n"))
infectedlen = int(input("How long should someone stay infected?\n"))
cooldown = int(input("How long after an infection can someone get reinfected?\n"))
mortality = float(input("What is the mortality rate? (<5% is a lot)\n"))
timessimulate = int(input("How many times should it be simulated?\n"))
numsimulationsdwindled = 0
numsimulationsnotdwindled = 0
numdeath = 0
mortalityperday = -100*(((-mortality)/100+1)**(1/days)-1)

# Part of the code tofor uninfecting people

def uninfect():
    global numpeople, infectedlist, wheninfected
    for i in range(numpeople):
        if infectedlist[i] == 1 and wheninfected[i]-1 == l:
            infectedlist[i] = 0
            wheninfected[i] = 0

# Part of the code for infecting people

def infect():
    global numpeople, infectedlist, checkinfected, lastinfected, l, cooldown, infectedpercent, gridconnection, testpercent
    for i in range(numpeople):

        # Part to check if somone will get infected

        if infectedlist[i] == 0 and (checkinfected[i] == False or lastinfected[i] == l-cooldown+1): # or (i == firstinfected-1 and i > infectedlen-1)):
            for d in range(numpeople):
                if gridfull[i][d] == 1 and infectedlist[d] == 1:
                    infectedpercent[i] *= 1-gridconnection[i][d]/100
            infectedpercent[i] = 1-infectedpercent[i]
            testpercent = random.uniform(0,1)
            if testpercent <= infectedpercent[i]:
                infectedlist[i] = 1
                wheninfected[i] = l+infectedlen+1
                lastinfected[i] = l+infectedlen+1
                checkinfected[i] = True
        
        #Part to check if someone will die

        if infectedlist[i] == 1:
            checking = random.uniform(0,1)
            if checking < (mortalityperday/100):
                infectedlist[i] = "Dead"

# Prints what there is to print at the end of a day
 
def print_day():
    global numpeople, infectedlist
    toprint = ""
    for i in range(numpeople):
        toprint += str(infectedlist[i])
        toprint += " "
    print(toprint)

# Defines the variables needed before every simulation

def define_variables():
    global wheninfected, infectedlist, infectedpercent, infectedlist, lastinfected, checkinfected, wheninfectedday
    wheninfected = [0]*numpeople
    wheninfected[infectedfirst-1] = infectedlen
    infectedlist = [0]*numpeople
    infectedpercent = [1]*numpeople
    infectedlist[infectedfirst-1] = 1
    lastinfected = [0]*numpeople
    lastinfected[infectedfirst-1] = infectedlen
    checkinfected = [False]*numpeople
    checkinfected[infectedfirst-1] = True
    wheninfectedday = [False]*numpeople
    wheninfectedday[infectedfirst-1] = -1

# Checks the characteristics of a day

def checks():
    global numsimulationsdwindled, numsimulationsnotdwindled, numdeath
    if 1 not in infectedlist:
        numsimulationsdwindled += 1
    else:
        numsimulationsnotdwindled += 1
    if "Dead" in infectedlist:
        numdeath += 1

# Main part of the program:

for s in range(timessimulate):
    print(f"\nSimulation {s+1}: \n")
    
    define_variables()
    
    for l in range(days):
        infectedpercent = [1]*numpeople
        uninfect()
        infect()
        print_day()
        
    checks()
    
print(f"\nNumber of simulations where no one was infected after {days} days: {numsimulationsdwindled}")
print(f"Number of simulations where people were infected after {days} days: {numsimulationsnotdwindled}")
print(f"Number of simulations where people died: {numdeath}")

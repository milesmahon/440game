import subprocess

#print (subprocess.check_output(["java AtroposGame 7 './bwebPlayer' './bwebPlayer6' > log.txt"], shell=True))

numTimes = 30

#init
one = 0
two = 0

for i in range(numTimes):
    subprocess.check_output(["java AtroposGame 7 './bwebPlayerAB' > log.txt"], shell=True, stderr=subprocess.STDOUT)
    #print(subprocess.check_output(["grep 'has won' log.txt"], shell=True))
    out = subprocess.check_output(["grep 'has won' log.txt"], shell=True)
    if ("Script" in str(out)):
        one+= 1
    elif ("Script Two" in str(out)):
        two+=1
    elif ("Script" in str(out)):
   	    one+=1
    elif("Default" in str(out)):
   	    two+=1
    else:
        print ("default won")
        two+=1

    # print("Game " + str(i) + " Completed")

print("eval2 wins with prob: " + str(one/(one+two)))

import subprocess

#print (subprocess.check_output(["java AtroposGame 7 './bwebPlayer' './bwebPlayer6' > log.txt"], shell=True))

numTimes = 50

#init
one = 0
two = 0

for i in range(numTimes):
    subprocess.check_output(["java AtroposGame 7 './bwebPlayerE5' > log.txt"], shell=True, stderr=subprocess.STDOUT)

    #print(subprocess.check_output(["grep 'has won' log.txt"], shell=True))
    out = subprocess.check_output(["grep 'has won' log.txt"], shell=True)
    if ("Script One" in str(out)):
        one+= 1
    elif ("Script Two" in str(out)):
        two+=1
    elif ("Script" in str(out)):
   	    one+=1
    elif("Default" in str(out)):
   	    two+=1
    else:
        print ("who?")

    print("Game " + str(i + 1) + " Completed. One: " + str(one) + " Two: " + str(two))


print("One wins with prob: " + str(one/(one+two)))


import subprocess

#print (subprocess.check_output(["java AtroposGame 7 './bwebPlayer' './bwebPlayer6' > log.txt"], shell=True))

numTimes = 50

#init
one = 0
two = 0

for i in range(numTimes):
<<<<<<< HEAD
    subprocess.check_output(["java AtroposGame 7 './bwebPlayer' './bwebPlayer6' > log.txt"], shell=True, stderr=subprocess.STDOUT)
=======
    subprocess.check_output(["java AtroposGame 7 './bwebPlayerE4D7' > log.txt"], shell=True, stderr=subprocess.STDOUT)
>>>>>>> e0eb5e4349687ff66140b3cab9986336c4a03dcf
    #print(subprocess.check_output(["grep 'has won' log.txt"], shell=True))
    out = subprocess.check_output(["grep 'has won' log.txt"], shell=True)
    if ("Script One" in str(out)):
        one+= 1
    elif ("Script Two" in str(out)):
        two+=1
    # elif ("Script" in str(out)):
 #   	    one+=1
    # elif("Default" in str(out)):
 #   	    two+=1
    else:
        print ("who?")

<<<<<<< HEAD


    print("Game " + str(i) + " Completed")

print("at depth 5, eval4 beats eval2 with prob: " + str(one/(one+two)))
=======
    print("Game " + str(i) + " Completed")

print("One wins with prob: " + str(one/(one+two)))
>>>>>>> e0eb5e4349687ff66140b3cab9986336c4a03dcf

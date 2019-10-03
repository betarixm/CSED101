import subprocess
import random
from math import ceil
from math import floor

subprocess.call('gcc assn1_1.c -lm -o assn1_1.out', shell=True)


def getRes(targetList):

    target = str()
    for i in targetList:
        target = f"{target}{str(i)}\n"
    target = target.encode()
    p = subprocess.Popen(["./assn1_1.out"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    return str(p.communicate(target)[0], 'utf-8')


def resolve_1_1(res):
    bills = list()

    for i in res.split(":"):
        j = i.replace(" ", "")
        if "Enter" in i or "20$" in j:
            continue
        bills.append(int(j.split("\n")[0]))

    return bills

def check_1_1(amount, rate, r):
    res = 2000 * r[0] +  1000 * r[1] + 500 * r[2] + 100 * r[3] + 50 * r[4] + 10 * r[5] + 1 * r[6]
    ans = ceil(floor(amount/rate*pow(10,3))/10)
    if ans == res:
        return True
    else:
        return False


print("ASSN1_1 Auto Check")
num = int(input("Num. of test case: "))
score = 0
error = str()

for i in range (0, num):
    if True:
        print(f"{round(i/num*100, 2)}%...")

    amount = random.randrange(10000, 999999)
    rate = random.randrange(1, amount)
    # print(f"[+] Amount: {amount} Rate: {rate}")
    exList = resolve_1_1(getRes([amount, rate]))
    isRight = check_1_1(amount, rate, exList)

    if isRight:
        score = score + 1
        # print("[+] Correct!")
    else:
        print("[-] Something wrong...")
        print("    Expected: " + str(ceil(floor((amount/rate*pow(10,3))/10))))
        print("    Result  : " + str(exList))
        print("            : " + str((20 * exList[0] +  10 * exList[1] + 5 * exList[2] + 1 * exList[3] + 0.5 * exList[4] + 0.1 * exList[5] + 0.01 * exList[6])))

        error = error + "[-] Case Amount: "+str(amount)+" Rate: " + str(rate) +"\n    Expected: " + str(ceil(floor((amount/rate*pow(10,3))/10))) + "\n    Result  : " + str(exList) +"\n            : " + str((20 * exList[0] +  10 * exList[1] + 5 * exList[2] + 1 * exList[3] + 0.5 * exList[4] + 0.1 * exList[5] + 0.01 * exList[6])) + "\n"

print("[+] Score: " + str(score/num*100))
print(f"    Correct: {score} Wrong: {num-score} Total: {num}")
print(error)




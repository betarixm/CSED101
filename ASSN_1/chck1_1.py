import subprocess

subprocess.call('gcc assn1_1.c -o assn1_1.out', shell=True)

process=subprocess.Popen(["./assn1_1.out"], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
res=process.communicate(b'2')
print(res[0])

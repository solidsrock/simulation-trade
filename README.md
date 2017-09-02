# simulation-trade
this task is to understand the concurrency issues in multi process trade in Operating System.
Assume there is a bank, which has a number of accounts.Here is a test, which randomly transfer a random money from one random
account to another random account.

In Linux environment, use C language to simulate the mentioned scenario.
Say, there are 100 accounts.
Each account has an initial money 1000.
Obviously the total money of all accounts in the bank should be 100,000.
Generate 100 threads/processes.
Let 100 threads/processes run together.
In any thread/process # i, transfer a random money, from account # i to another random
account # j.Every transfer loops for ever, unless you press Ctrl-C to stop the whole program.

The random money transferred each time should be between 0 – 1000.
In each transfer, if the account has not enough money left, use printf() to output a message:
 i has not enough money, and loop again.

In each transfer, after account # i subtracts the transferred money, use printf() to output the
message:  i transfers how much amount, how much left now.

In each transfer, after account # j adds the transferred money, use printf() to output the
message: j receives how much money, how much left now.

After each transfer, use a function TotalMoney() to sum up all accounts to get the total
money in the bank. Then use printf() to output the message: Current total money in bank is how
much.

In order to prevent the program and output to flush too fast, you need use sleep() to sleep
for a random 0 - 10 of milliseconds after every transfer. 

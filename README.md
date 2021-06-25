# Summer of Bitcoin - Task 1

#### Time Complexity of Proposed Solution
For number of transactions to be *n*
- Creating set for transactions = O(nlogn)
- For getting valid transaction and appending it in block = O(n<sup>2</sup>)
- Overall time complexity of solution = O(n<sup>2</sup>)

#### Program Information
- **Language:** C++
- **Code file:** code.cpp
- **Output file:** block.txt
- **Dataset:** mempool.csv

#### Results
1. Total number of transactions read: 5214
2. Number of tx in final block 3174
3. Total fee in final block : 5696031
4. Total weight of final block: 3999936.00000
5. Percentage of weight: 99.99840 %
6. Time taken by program is : 1.53348 sec

![preview](https://github.com/pradyuman-verma/BitcoinSummerOfCode/blob/main/output.jpg?raw=true)
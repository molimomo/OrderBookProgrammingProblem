# OrderBookProgrammingProblem
Programming Project from RGM

1. How to compile this program?
    - g++ pricer.cpp -o pricer

2. How to execute this program?
    - ./pricer target_size (e.g. ./pricer 200)

3. Questions Answering
    3.1 How did you choose your implementation language?
        - I choose C++, since I am more familiar with C++. Furthermore STL in C++ has a lot of effective data structures can be utilized.

    3.2 What is the time complexity for processing an Add Order message?
        - Every Add Order information will be inserted into two tables.
            First: An unordered_map  "orderRec" to record order ID and its transaction (type, price, count). O(1)
            Second: An map "ask/bid PriceRec" to record price and it count. O(log n), n: size of PriceRec table.
        - If the # of shares is meet or exceed the target size of shares, we wil travere ask/bid PriceRec to calculate income/expence. O(n), n: size of PriceRec table. 

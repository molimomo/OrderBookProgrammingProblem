# OrderBookProgrammingProblem
Programming Project from RGM


Author: Mao-Lin Li 
Date: 08/19/2015
E-Mail: molimomo0@gmail.com

1. How to compile this program?
    - g++ pricer.cpp -o pricer

2. How to execute this program?
    - ./pricer target_size (e.g. ./pricer 200)

3. Question Answerings

    * How did you choose your implementation language?

        - I choose C++, since I am more familiar with C++. Furthermore STL in C++ has a lot of effective data structures can be utilized.

    * What is the time complexity for processing an Add Order message?

        - Every Add Order information will be inserted into two tables.

            First: An unordered_map (hash table) "orderRec" to record order ID and its transaction (type, price, count). O(1)

            Second: An map (binary tree) "ask/bid PriceRec" to record price and it count. O(log n), n: size of PriceRec table.

        - If the # of shares is meet or exceed the target size of shares, we wil travere ask/bid PriceRec to calculate income/expence. O(n), n: size of PriceRec table. 

        - The average time complexity should be: O(n)

    * What is the time complexity for processing an Reduce Order message?

        - Every Reduce Order information will be used in two tables.

            First: Find this order ID's transaction information (type, price, count) from "orderRec" (hash table). O(1)

            Second: Use price found from first step to further update the information in ask/bid PriceRec (binary tree). O(log n), n: size of PriceRec table.

        - If the # of shares is meet or exceed the target size of shares, we wil travere ask/bid PriceRec to calculate income/expence. O(n), n: size of PriceRec table.

        - The average time complexity should be: O(n)

    * If your implementation were put into production and found to be too slow, what ideas would you try out to improve its performance? (Other than reimplementing it in a different language such as C or C++.)

    	- Currently, I read input data from file line by line, I think file I/O will be one of performance bottlenect in this version. If we have enough memory to store these input, the performance should be improved.

    	- There are some functions in my program. E.g. AddOrder, ReduceOrder, printResult, etc. Currently, I just call them within nultiple function call. However, if we can utilized the feature of multithread, each function can be performed within different threads. I think the performance can be also improved.


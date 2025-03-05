# **Grace Hash Join Implementation**

## **Overview**
This project implements the **Grace Hash Join** algorithm in C++. Grace Hash Join is a database join algorithm optimized for handling large datasets that do not fit into memory. The implementation efficiently partitions and probes relations stored on disk, simulating the data flow of a database system.

## **Key Features**
- **Efficient Partitioning**: Utilizes a hash-based partitioning strategy to split large datasets into manageable chunks.
- **Optimized Probing Phase**: Builds an in-memory hash table to quickly find matching records across partitions.
- **Disk and Memory Simulation**: Models the behavior of a database by simulating page-based memory and disk storage.
- **Scalable Design**: Ensures that partitions fit into memory, avoiding the need for recursive partitioning.

## **Implementation Details**
The project consists of the following key components:

- **Partitioning Phase**: 
  - Reads input relations from disk.
  - Hashes records using a partitioning function to distribute them into different partitions.
  - Writes partitioned data back to disk.
  
- **Probing Phase**:
  - Loads one partition into an in-memory hash table.
  - Scans the corresponding partition from the other relation and finds matching records.
  - Outputs joined records to disk.

- **Data Storage Simulation**:
  - **Disk**: Simulates a paged storage system, where records are stored in disk pages.
  - **Memory**: Implements an in-memory buffer that holds a limited number of pages at a time.
  - **Pages**: Emulates database pages to manage records efficiently.

## **Setup and Execution**
### **1. Compile the Project**
Run the following command to compile the project:
```sh
make
```

### **2. Run the Program**
Execute the join algorithm on two datasets:
```sh
./GHJ left_rel.txt right_rel.txt
```

### **3. Clean Up Build Files**
```sh
make clean
```

## **Example Output**
After executing the join operation, the output will list the joined records:
```
Size of GHJ result: 1 pages
Page 0 with disk id = 6
Record with key=1 and data=1l
Record with key=1 and data=1r
...
```

## **Performance Considerations**
- Uses **hash-based partitioning** to reduce memory usage.
- Ensures that **partitions fit into memory**, eliminating recursive partitioning.
- Efficiently utilizes **in-memory hash tables** to speed up the probing phase.

## **Technologies Used**
- **C++** (Standard Library, Data Structures)
- **Makefile** (Build Automation)

## **Potential Enhancements**
- Implementing **parallel processing** to speed up partitioning and probing.
- Extending the algorithm to handle **recursive partitioning** for extremely large datasets.
- Optimizing memory management to improve efficiency.

## **Acknowledgments**
This project showcases database algorithm implementation and efficient disk-based data processing, demonstrating core concepts of **database management systems** and **efficient query execution**.


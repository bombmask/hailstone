from multiprocessing import Pool
from os import cpu_count
import random
import collections

def hailstone(n):
    values = []
    cVal = n
    
    while cVal != 1:
        cVal =  3 * cVal + 1 if cVal & 1 else cVal // 2

        values.append(cVal)

    return values

def eval_hailstone(n):
    data=hailstone(n)
    #print("Hailstone_{} Complete:Length {}".format(n, data[0]))
    return {"n": n, "length":len(data), "list":data}

def pretty_print(data_type, doList=300):
    out_string = ""
    #print Level One
    out_string += "N: " + str(data_type["n"]) + "\n"

    #Print Level Two
    out_string += "\t" + "Length: " + str(data_type["length"]) + "\n"

    #Print Level Three
    if doList > 0:
        size = 0

        for place in data_type['list']:
            out_string += "\t" * 2 + str(place) + "\n"
            size += 1
            if size > doList and doList is not -1:
                break

    return out_string


if __name__ == '__main__':
    # print(eval_hailstone(27))
    # exit()
    # Create pool
    process_pool = Pool()
    print("Executing on {} CPU(s)".format(cpu_count()))

    BlockSize = 100000 # Chunk Size
    BlockAmount = 5 #BlockSize * BlockAmount Is upper limit

    print("Testing to {}!".format(str(BlockAmount*BlockSize)))

    total_largest = None

    for BAI in range(BlockAmount):
        # Create new Items list
        cBLOCK_start = BlockSize * BAI + 1
        cBLOCK_end = BlockSize * (BAI + 1)


        items = range( cBLOCK_start, cBLOCK_end)
        print("Create New Items List {}".format(items))

        # Map Process Pool to Items
        print("Mapping...")
        eval_list = process_pool.map(eval_hailstone, items)
        print("Done...")

        # Find New Largest
        current_largest = eval_list[0]
        for item in eval_list:
            # Only assign on larger number found.
            if current_largest["length"] < item["length"]:
                current_largest = item

        if total_largest:
            if total_largest["length"] < current_largest["length"]:
                total_largest = current_largest

        else: #Should only execute once.
            total_largest = current_largest

        #Print Current Largest
        print(pretty_print(total_largest, doList=False))

    #End For loop
    process_pool.close()


    #print(pretty_print(total_largest, doList=False))

from os import system
from time import sleep

def main(filename, file):
    with open(filename, "r") as f:
        current = f.read()
        if file != current:
            return [ 1, current ]
        return [ 0, file ]

if __name__ == "__main__":
    
    filename = "createdb.cpp"
    
    with open(filename, "r") as f:
        file = f.read()
    
    while True:
        [ is_not_equal, file ] = main(filename, file)
        if is_not_equal:
            system("clear")
            system(f"g++ {filename} -o {filename.split(".")[0]}.out -lsqlite3 && ./{filename.split(".")[0]}.out")
            
        sleep(1)

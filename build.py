import os
from datetime import datetime

def build():
    os.system("make all")
    return 0

def clean():
    os.system("make clean")
    return 0

def versioning():
    build = input("Enter Build number: ")
    versioning = input("Enter versioning: ")
    current_datetime = datetime.now()
    format_string = "%H:%M:%S %m:%d:%Y"
    date =  current_datetime.strftime(format_string)
    with open("src/version.h","w") as f:
        f.write(f"#define BUILD \"{str(build)}\"\n")
        f.write(f"#define DATE \"{str(date)}\"\n")
        f.write(f"#define VERSION \"{str(versioning)} {str(build)} {str(date)}\"\n")
        print("Versioning updated successfully.")

x = str(input("Enter build, clean, or versioning. "))
match x:
    case "build":
        build()
    case "clean":
        clean()
    case "versioning":
        versioning()
    case _:
        print("Invalid input.")
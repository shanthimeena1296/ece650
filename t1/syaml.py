#!/usr/bin/env python3

#
# Skeleton for ECE650 Test 1
# DO NOT DISTRIBUTE
#
# Complete the two provided functions. Implement as much functionality as you
# can. Make sure to not change the interface.
#
# More details are in the question on LEARN.
#
# You are allowed to use a Python IDE and Python Documentation available at
# https://docs.python.org/3/
#
# You are very likely to need to add extra functions and/or classes to organize
# your code. We will mark your code for functionality only. Using extra
# functions is not required, but is highly advised
#

def is_string_valid(line):
    line = line.strip()
    if len(line) == 0:
        raise Exception("Invalid string")

    for c in line:
        if not (('a' <= c and c <= 'z') or
            ('A' <= c and c <= 'Z') or
            ('0' <= c and c <= '9') or
            (c == ' ')):
            raise Exception("Invalid String.")
    return True

def show_list(data):
    if not isinstance(data, list):
        raise Exception("Invalid value passed")

    for element in data:
        if isinstance(element, str) and is_string_valid(element):
            pass # Valid
        if isinstance(element, dict):
            keys = element.keys()
            for key in keys:
                if ((not isinstance(key, str)) or (not isinstance(element[key], str))):
                    raise Exception("Nested dictionary can have string key and values only")
                if not (is_string_valid(key) and is_string_valid(element[key])):
                    raise Exception("Invalid string provided")

    print("---")
    for element in data:
        if isinstance(element, str):
            print("- " + element)
        if isinstance(element, dict):
            keys = element.keys()
            first_element_done = False
            for key in keys:
                if first_element_done:
                    print("  " + key + ": " + element[key])
                else:
                    first_element_done = True
                    print("- " + key + ": " + element[key])
    print("...")

def show_dict(data):
    if not isinstance(data, dict):
        raise Exception("Invalid value passed")
    
    for key in data.keys():
        if isinstance(key, str) and isinstance(data[key], str) and is_string_valid(data[key]) and is_string_valid(key):
            pass
        elif isinstance(key, str) and isinstance(data[key], list) and is_string_valid(key):
            for ele in data[key]:
                if not (isinstance(ele, str) and is_string_valid(ele)):
                    raise Exception("Invalid value passed")
        else:
            raise Exception("Invalid value passed")

    print("---")

    for key in data.keys():
        if isinstance(key, str) and isinstance(data[key], str):
            print(key + ": " + data[key])
        elif isinstance(key, str) and isinstance(data[key], list):
            if len(data[key]) > 0:
                print(key + ":")
            for ele in data[key]:
                print("  - "+ ele)
        else:
            raise Exception("Invalid value passed")
    print("...")

def print_syaml(data):
    """Print data in SYAML format. Raise an exception if this is not possible"""

    if isinstance(data, list):
        show_list(data)
    elif isinstance(data, dict):
        show_dict(data)
    else:
        raise Exception("Wrong type of data: " + str(data.__class__))


def parse_syaml(lines):
    """Parse SYAML document. See LEARN for details."""
    if not isinstance(lines, list):
        raise Exception("Expecting a list of strings")
    if not (len(lines) >= 2 and lines[0] == "---\n" and lines[-1] == "...\n"):
        raise Exception("Begin or end document is missing")

    for line in lines:
        if line[-1] != '\n':
            raise Exception('New line character missing')
    
    lines = [line.strip() for line in lines]

    if len(lines) == 2:
        return []

    if lines[1][0] == '-': # List
        result_obj = []

        for line in lines[1:-1]:
            if line.find(':') != -1: # its dictionary
                if line[:2] == "- ":
                    idx = line.find(":")
                    key = line[2:idx]
                    value = line[idx+2:]
                    if is_string_valid(key) and is_string_valid(value):
                        result_obj.append({key: value})
                    else:
                        raise Exception("Invalid string passed")
                else:
                    idx = line.find(":")
                    key = line[:idx]
                    value = line[idx+2:]
                    if is_string_valid(key) and is_string_valid(value) and line[idx+1] == " ":
                        result_obj[-1][key] = value
                    else:
                        raise Exception("Invalid value passed")

            elif line[:2]== "- " and is_string_valid(line[2:]): # String
                result_obj.append(line[2:])
            else:
                raise Exception("Invalid input passed.")

    elif lines[1].find(':') != -1: # Dictionary
        result_obj = {}
        key_till_now = None

        for line in lines[1:-1]:
            if line.find(':') != -1: # Key is present
                key = line[:line.find(":")]
                value = line[line.find(":")+2:]
                if not (is_string_valid(key)):
                    raise Exception("Invalid entry given")

                if len(value) >= 1 and is_string_valid(value) and line[line.find(":")+1] == " ":  # Value is string
                    if key in result_obj.keys():
                        raise Exception("Duplicate keys found.")
                    result_obj[key] = value
                elif len(value) == 0: # Value should be present in next line as string
                    key_till_now = key
                    result_obj[key] = []
                else:
                    raise Exception("Invalid input provided")

            elif line.find('-') != -1 and line[line.find('-') + 1] == " " and key_till_now != None and is_string_valid(line[2:]): # Value is present as list
                result_obj[key].append(line[2:])
            else:
                raise Exception("Invalid input provided")

    else:
        raise Exception("Invalid input passed.")

    return result_obj


# This is a simple test function provided from your convenience
if __name__ == "__main__":

    d1 = ["one", "two", "three", "four", "five"]
    print_syaml(d1)
    d2 = {"one":"un", "two":"duo", "three":"trios"}
    print_syaml(d2)
    d3 = ["one", "two", {"thre fd": "duo", "four": "sdfdf"}]
    print_syaml(d3)
    d4 = {"one":["two", "ds"], "two":"three", "three":["one", "un"]}
    print_syaml(d4)
    d6 = ["one", "two", "three"]
    print_syaml(d6)
    d7 = [{'one': 'hi', 'two':'hello'}, {'one': 'yes', 'two':'bye'}]
    print_syaml(d7)
    d8 = {'one':['un','two','trios'], 'two':['hana','thur','sath']}
    print_syaml(d8)
    d9 = {"one":"un", "two":"duos", "three":"trios"}
    print_syaml(d9)
    
    d1 = parse_syaml(["---\n",
                    "- one\n",
                    "...\n"])
    print(d1)
    d1 = parse_syaml(["---\n",
                    "- one\n",
                    "- two\n",
                    "...\n"])
    print(d1)
    d1 = parse_syaml(["---\n",
                    "- one : 1\n",
                    "two: 2\n",
                    "...\n"])
    print(d1)
    d1 = parse_syaml(["---\n",
                    "one: un\n",
                    "af: fb\n",
                    "...\n"])
    print(d1)
    d1 = parse_syaml(["---\n",
                    "a:\n", 
                    "- b\n",
                    "- d\n",
                    "af: fb\n",
                    "...\n"])
    print(d1)
    d1 = parse_syaml(["---\n",
                    "a:\n", 
                    "- twof\n",
                    "two: duos\n",
                    "...\n"])
    print(d1)
    d1 = parse_syaml(["---\n",
                
                    "...\n"])
    print(d1)
    #wrong input
    # d1 = parse_syaml([""])
    # print(d1)

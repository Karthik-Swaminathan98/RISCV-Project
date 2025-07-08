import sys
import re
import os
from collections import defaultdict, deque

def parse_map_file(filename):
    """Parse .map file to extract function sizes."""
    functions = {}
    current_func = None

    with open(filename, 'r') as f:
        for line in f:
            # Match section like .text.func_name
            sec_match = re.match(r'\s*\.text\.([^\s]+)', line)
            if sec_match:
                current_func = sec_match.group(1)
                continue

            # Match address and size
            size_match = re.match(r'\s*0x([0-9a-fA-F]+)\s+0x([0-9a-fA-F]+)', line)
            if size_match and current_func:
                addr = int(size_match.group(1), 16)
                size = int(size_match.group(2), 16)
                functions[current_func] = {'address': addr, 'size': size}
                current_func = None

    return functions

def parse_objdump_file(filename):
    """Parse objdump disassembly to extract function call graph."""
    call_graph = defaultdict(set)
    current_func = None

    with open(filename, 'r') as f:
        for line in f:
            # Match function header: 00002f3c <func_name>:
            header = re.match(r'^\s*[0-9a-fA-F]+ <([\w$@.]+)>:', line)
            if header:
                current_func = header.group(1)
                continue

            if current_func:
                # Match jal/j/c.j/call to <callee>
                call = re.search(r'\b(jal|j|c\.j|call)\b.*<([\w$@.]+)>', line)
                if call:
                    callee = call.group(2)
                    call_graph[current_func].add(callee)

    return call_graph

def compute_total_size(functions, call_graph, root_func):
    visited = set()
    total_size = 0
    queue = deque([root_func])
    breakdown = []

    while queue:
        func = queue.popleft()
        if func in visited:
            continue
        visited.add(func)

        size = functions.get(func, {}).get('size', 0)
        total_size += size
        breakdown.append((func, size))

        for callee in call_graph.get(func, []):
            if callee not in visited:
                queue.append(callee)

    return total_size, breakdown

def main():
    if len(sys.argv) < 4:
        print("Usage: python function_size.py <map_file> <objdump_file> <function_name1> [function_name2 ...]")
        sys.exit(1)

    map_file = sys.argv[1]
    objdump_file = sys.argv[2]
    root_funcs = sys.argv[3:]

    if not os.path.isfile(map_file) or not os.path.isfile(objdump_file):
        print("[!] Invalid file path(s)")
        sys.exit(1)

    print(f"[*] Analyzing file: {map_file}\n")

    functions = parse_map_file(map_file)
    call_graph = parse_objdump_file(objdump_file)

    for root_func in root_funcs:
        print(f"[*] Function: {root_func}")
        if root_func not in functions:
            print("    [!] Not found in map file.\n")
            continue

        total_size, breakdown = compute_total_size(functions, call_graph, root_func)

        print(f"    Own size: {functions[root_func]['size']} bytes")
        print(f"    Total size (with dependencies): {total_size} bytes")
        print("    [*] Breakdown:")
        for name, size in sorted(breakdown, key=lambda x: -x[1]):
            if size == 0:
                continue
            print(f"        {name:<35} {size:>6} bytes")
        print("-" * 60)

if __name__ == "__main__":
    main()

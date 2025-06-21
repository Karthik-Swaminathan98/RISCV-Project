import sys
import re
import os
from collections import defaultdict, deque

def parse_map_file(filename):
    """Parse .map file to extract function sizes and call graph."""
    functions = {}
    call_graph = defaultdict(set)
    current_func = None

    with open(filename, 'r') as f:
        lines = f.readlines()

    # Pass 1: extract function sizes
    for line in lines:
        sec_match = re.match(r'\s*\.text\.([^\s]+)', line)
        if sec_match:
            current_func = sec_match.group(1)
            continue

        size_match = re.match(r'\s*0x([0-9a-fA-F]+)\s+0x([0-9a-fA-F]+)', line)
        if size_match and current_func:
            addr = int(size_match.group(1), 16)
            size = int(size_match.group(2), 16)
            functions[current_func] = {'address': addr, 'size': size}
            current_func = None

    # Pass 2: extract call references like:
    # libdsp.a(riscv_dsp_cfft_q15.o) (riscv_dsp_cfft_rd2_q15)
    for line in lines:
        match = re.search(r'\(([\w$@.]+)\)\s*$', line)
        if match:
            callee = match.group(1)
            # Try to extract calling .o from left-hand side of the line
            caller_match = re.match(r'.*\(([\w$@.]+)\.o\).*?\({}'.format(re.escape(callee)), line)
            if caller_match:
                caller_o = caller_match.group(1)
                if caller_o != callee:
                    call_graph[caller_o].add(callee)

    return functions, call_graph

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
        print("Usage: python function_size.py <map_file> <dummy_objdump> <function_name1> [function_name2 ...]")
        sys.exit(1)

    map_file = sys.argv[1]
    # We no longer need dummy_objdump, but keep to maintain your args signature
    root_funcs = sys.argv[3:]

    if not os.path.isfile(map_file):
        print("[!] Invalid map file path")
        sys.exit(1)

    print(f"[*] Analyzing file: {map_file}\n")

    functions, call_graph = parse_map_file(map_file)

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
            print(f"        {name:<30} {size:>6} bytes")
        print("-" * 60)

if __name__ == "__main__":
    main()

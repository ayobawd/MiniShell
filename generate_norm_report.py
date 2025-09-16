#!/usr/bin/env python3
import json
import re
import sys

def parse_norm_output(content):
    lines = content.strip().split('\n')
    findings = []
    current_file = None
    
    for line in lines:
        # Match file status lines like "file.c: Error!"
        file_match = re.match(r'^(.+\.c|.+\.h): (OK!|Error!)$', line)
        if file_match:
            current_file = file_match.group(1)
            continue
            
        # Match error lines like "Error: RULE_NAME       (line:   X, col:   Y):	Description"
        error_match = re.match(r'^Error: (\w+)\s+\(line:\s*(\d+), col:\s*(\d+)\):\s*(.+)$', line)
        if error_match and current_file:
            rule = error_match.group(1)
            line_num = int(error_match.group(2))
            col_num = int(error_match.group(3))
            description = error_match.group(4)
            
            # Categorize based on file path - include files in parsing directory
            if (current_file.startswith('src/parsing/') or 
                current_file in ['cmds.c', 'env.c', 'pipes.c', 'utils.c', 'parser.c', 'redirect.c', 
                               'spaces_parsing.c', 'dollar.c', 'utils2.c', 'signals.c']):
                category = 'A'  # parsing-origin
            else:
                category = 'B'  # non-parsing
                
            findings.append({
                'path': current_file,
                'line': line_num,
                'rule': rule,
                'category': category,
                'description': description
            })
    
    return findings

def generate_reports(findings):
    # JSON report
    with open('reports/norm_report.json', 'w') as f:
        json.dump(findings, f, indent=2)
    
    # Text report
    category_a = [f for f in findings if f['category'] == 'A']
    category_b = [f for f in findings if f['category'] == 'B']
    
    with open('reports/norm_report.txt', 'w') as f:
        f.write("42 Minishell Norm Compliance Report\n")
        f.write("="*50 + "\n\n")
        
        f.write("SUMMARY:\n")
        f.write(f"Category A (parsing-origin): {len(category_a)} errors (IGNORED by policy)\n")
        f.write(f"Category B (non-parsing): {len(category_b)} errors (REQUIRE FIXES)\n\n")
        
        if category_a:
            f.write("CATEGORY A - PARSING ORIGIN (IGNORED):\n")
            f.write("-" * 40 + "\n")
            for finding in category_a:
                f.write(f"{finding['path']}:{finding['line']} - {finding['rule']}: {finding['description']}\n")
            f.write("\n")
        
        if category_b:
            f.write("CATEGORY B - NON-PARSING (REQUIRE FIXES):\n")
            f.write("-" * 40 + "\n")
            for finding in category_b:
                f.write(f"{finding['path']}:{finding['line']} - {finding['rule']}: {finding['description']}\n")
            f.write("\n")
        
        # Group by file for easier analysis
        files_b = {}
        for finding in category_b:
            if finding['path'] not in files_b:
                files_b[finding['path']] = []
            files_b[finding['path']].append(finding)
        
        f.write("FILES REQUIRING FIXES:\n")
        f.write("-" * 25 + "\n")
        for file_path, file_findings in files_b.items():
            f.write(f"{file_path}: {len(file_findings)} errors\n")

if __name__ == "__main__":
    with open('current_norm_output.txt', 'r') as f:
        content = f.read()
    
    findings = parse_norm_output(content)
    generate_reports(findings)
    print(f"Generated reports with {len(findings)} total findings")
    
    category_a = [f for f in findings if f['category'] == 'A']
    category_b = [f for f in findings if f['category'] == 'B']
    print(f"Category A (parsing): {len(category_a)}")
    print(f"Category B (non-parsing): {len(category_b)}")
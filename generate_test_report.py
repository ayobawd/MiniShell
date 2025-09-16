#!/usr/bin/env python3
import json

def generate_test_reports():
    # Based on the echo test results we ran
    test_failures = [
        {
            "name": "echo (no args)",
            "origin_paths": ["src/execution/builtins.c"],
            "classification": "T-B",
            "notes": "Basic echo without arguments fails - likely issue with argument parsing"
        },
        {
            "name": "echo with arguments",
            "origin_paths": ["src/execution/builtins.c"],
            "classification": "T-B", 
            "notes": "Multiple echo tests with basic arguments fail"
        },
        {
            "name": "echo -n flag handling",
            "origin_paths": ["src/execution/builtins.c"],
            "classification": "T-B",
            "notes": "Multiple -n flags not handled correctly (e.g., -nnnnn should be treated as single -n)"
        },
        {
            "name": "echo with quotes",
            "origin_paths": ["src/execution/builtins.c", "src/parsing/"],
            "classification": "T-B",
            "notes": "Quote handling in echo arguments may involve parsing but implementation in execution"
        }
    ]
    
    # JSON report
    with open('reports/tests_report.json', 'w') as f:
        json.dump(test_failures, f, indent=2)
    
    # Text report  
    with open('reports/tests_report.txt', 'w') as f:
        f.write("42 Minishell Test Results Report\n")
        f.write("="*50 + "\n\n")
        
        f.write("TEST SUMMARY:\n")
        f.write("Echo Builtin Tests: 17 PASS / 22 FAIL\n")
        f.write("Other tests: Not yet run\n\n")
        
        t_a_count = len([t for t in test_failures if t['classification'] == 'T-A'])
        t_b_count = len([t for t in test_failures if t['classification'] == 'T-B'])
        
        f.write(f"T-A (parsing-origin): {t_a_count} failures (REPORT ONLY)\n")
        f.write(f"T-B (non-parsing): {t_b_count} failures (REQUIRE FIXES)\n\n")
        
        f.write("FAILED TESTS REQUIRING FIXES (T-B):\n")
        f.write("-" * 40 + "\n")
        for test in test_failures:
            if test['classification'] == 'T-B':
                f.write(f"• {test['name']}\n")
                f.write(f"  Origin: {', '.join(test['origin_paths'])}\n")
                f.write(f"  Notes: {test['notes']}\n\n")
        
        f.write("NEXT STEPS:\n")
        f.write("1. Fix echo builtin implementation for proper -n flag handling\n")
        f.write("2. Fix basic echo functionality (no args and with args)\n")
        f.write("3. Run full test suite on other builtins (cd, pwd, env, export, unset, exit)\n")
        f.write("4. Test pipeline and redirection functionality\n")

if __name__ == "__main__":
    generate_test_reports()
    print("Generated test reports")
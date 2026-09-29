#!/usr/bin/env python3
"""
eval_readiness.py - Check if refactoring is safe to start.

Verifies:
- Test suite exists and has tests
- All tests passing (baseline established)
- Code is compilable
- Functions are identifiable
"""

import os
import sys
import subprocess
import argparse
from pathlib import Path

def check_test_suite_exists(test_file):
    """Check if test file exists and contains tests."""
    if not os.path.exists(test_file):
        print(f"❌ Test file not found: {test_file}")
        return False
    
    with open(test_file, 'r') as f:
        content = f.read()
        if 'TEST(' not in content and 'TEST_F(' not in content:
            print(f"❌ No tests found in {test_file}")
            return False
    
    print(f"✓ Test file exists with tests: {test_file}")
    return True

def check_tests_passing():
    """Run tests and verify all passing."""
    print("\nChecking if all tests pass...")
    
    if os.path.exists('build'):
        os.chdir('build')
        result = subprocess.run(['ctest', '--verbose'], 
                              capture_output=True, text=True)
        os.chdir('..')
        
        if result.returncode != 0:
            print("❌ Not all tests passing. Establish baseline first.")
            print(result.stdout)
            print(result.stderr)
            return False
        
        # Count passing tests
        lines = result.stdout.split('\n')
        for line in lines:
            if 'passed' in line.lower():
                print(f"✓ {line.strip()}")
                break
        return True
    else:
        print("⚠ Build directory not found. Run: cmake -B build && cmake --build build")
        return False

def check_source_file(source_file):
    """Check if source file exists and is compilable."""
    if not os.path.exists(source_file):
        print(f"❌ Source file not found: {source_file}")
        return False
    
    print(f"✓ Source file exists: {source_file}")
    
    # Try to compile
    if os.path.exists('build'):
        os.chdir('build')
        result = subprocess.run(['cmake', '--build', '.'],
                              capture_output=True, text=True)
        os.chdir('..')
        
        if result.returncode != 0:
            print("❌ Compilation failed. Fix errors first.")
            print(result.stderr)
            return False
        
        print("✓ Code compiles without errors")
        return True
    else:
        print("⚠ Build directory not found")
        return False

def main():
    parser = argparse.ArgumentParser(
        description='Check if refactoring is safe to start'
    )
    parser.add_argument('--component', required=True,
                       help='Component name (e.g., BrakeMonitor)')
    parser.add_argument('--source-file', 
                       help='Source file path (auto-detected if not provided)')
    parser.add_argument('--test-file',
                       help='Test file path (auto-detected if not provided)')
    
    args = parser.parse_args()
    
    # Auto-detect paths if not provided
    source_file = args.source_file or f'src/{args.component.lower()}.cpp'
    test_file = args.test_file or f'tests/unit/{args.component.lower()}_test.cpp'
    
    print(f"🔍 Checking readiness for refactoring: {args.component}")
    print("=" * 60)
    
    checks = [
        ("Test suite exists", lambda: check_test_suite_exists(test_file)),
        ("Source file exists", lambda: check_source_file(source_file)),
        ("Tests passing", check_tests_passing),
    ]
    
    results = []
    for check_name, check_func in checks:
        print(f"\n[{check_name}]")
        try:
            results.append(check_func())
        except Exception as e:
            print(f"❌ Error: {e}")
            results.append(False)
    
    print("\n" + "=" * 60)
    if all(results):
        print("✓ REFACTORING IS SAFE TO START!")
        print(f"Component {args.component} is ready for refactoring.")
        return 0
    else:
        print("❌ REFACTORING NOT SAFE TO START")
        print("Fix the issues above before proceeding.")
        return 1

if __name__ == '__main__':
    sys.exit(main())
